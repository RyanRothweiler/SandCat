#ifdef USING_OPENGL

#include <gl/gl.h>

static real64 DisplayPPI = 1.0f;

struct loaded_image
{
	string FilePath;
	string FileName;

	// Bound between 0.0 and 1.0 (0.0 = top/left, 1.0 = bottom/right).
	// This is the offset which controls where this images center is logically in the world.
	vector2 CenterOffset;

	// NOTE this is old and isn't used but removing it will chnage the file size of things
	vector2 FILLER;

	uint32 Width;
	uint32 Height;

	uint32 GLID;

	// If this image was loaded successfully
	bool32 Valid;
};

struct gl_square
{
	color Color;
	vector2 TopLeft;
	vector2 TopRight;
	vector2 BottomLeft;
	vector2 BottomRight;
};

struct gl_texture
{
	loaded_image *Image;

	vector2 Center;
	vector2 Scale;
	real64 RadiansAngle;
	color Color;
};

struct gl_color_points
{
	// Points are done in a triangle strip
	vector2* Points;
	int32 PointsCount;
	color Color;
};

struct gl_line
{
	vector2 Start;
	vector2 End;
	real32 Width;
	color Color;
};

struct gl_square_outline
{
	gl_line LeftLine;
	gl_line RightLine;
	gl_line BottomLine;
	gl_line TopLine;
};


void
RenderLine(gl_line *Line)
{
	glColor4f((GLfloat)Line->Color.R, (GLfloat)Line->Color.G, (GLfloat)Line->Color.B, (GLfloat)Line->Color.A);
	glBegin(GL_LINES);
	{
		// NOTE line width isn't working
		// glLineWidth(10.0f);

		glVertex2d(Line->Start.X, Line->Start.Y);
		glVertex2d(Line->End.X, Line->End.Y);
	}
	glEnd();
}

void
RenderTexture(gl_texture *TextureRendering)
{
	glPushMatrix();

	vector2 Center = TextureRendering->Center;
	vector2 Scale = TextureRendering->Scale * (DisplayPPI / 2.0f);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, TextureRendering->Image->GLID);
	glBegin(GL_QUADS);
	{
		glColor4f((GLfloat)TextureRendering->Color.R, (GLfloat)TextureRendering->Color.G,
		          (GLfloat)TextureRendering->Color.B, (GLfloat)TextureRendering->Color.A);

		real64 Radians = TextureRendering->RadiansAngle;

		vector2 RotatedPoint = {};
		vector2 OrigPoint = {};

		OrigPoint = {Center.X - Scale.X, Center.Y - Scale.Y};
		RotatedPoint = Vector2RotatePoint(OrigPoint, Center, Radians);
		glTexCoord2f(0, 1);
		glVertex2f((GLfloat)RotatedPoint.X, (GLfloat)RotatedPoint.Y);

		OrigPoint = {Center.X + Scale.X, Center.Y - Scale.Y};
		RotatedPoint = Vector2RotatePoint(OrigPoint, Center, Radians);
		glTexCoord2f(1, 1);
		glVertex2f((GLfloat)RotatedPoint.X, (GLfloat)RotatedPoint.Y);

		OrigPoint = {Center.X + Scale.X, Center.Y + Scale.Y};
		RotatedPoint = Vector2RotatePoint(OrigPoint, Center, Radians);
		glTexCoord2f(1, 0);
		glVertex2f((GLfloat)RotatedPoint.X, (GLfloat)RotatedPoint.Y);

		OrigPoint = {Center.X - Scale.X, Center.Y + Scale.Y};
		RotatedPoint = Vector2RotatePoint(OrigPoint, Center, Radians);
		glTexCoord2f(0, 0);
		glVertex2f((GLfloat)RotatedPoint.X, (GLfloat)RotatedPoint.Y);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void
RenderColorPoints(gl_color_points *Points)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glColor4f((GLfloat)Points->Color.R, (GLfloat)Points->Color.G, (GLfloat)Points->Color.B, (GLfloat)Points->Color.A);

		for (int32 PointIndex = 0; PointIndex < Points->PointsCount; PointIndex++)
		{
			glVertex2d(Points->Points[PointIndex].X, Points->Points[PointIndex].Y);
		}
	}
	glEnd();
}

void
RenderSquare(gl_square *Square)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	{
		glColor4f((GLfloat)Square->Color.R, (GLfloat)Square->Color.G, (GLfloat)Square->Color.B, (GLfloat)Square->Color.A);
		// NOTE the order of this can't be changed. Though I can't find any documentation on why or what the correct order is, but this works.
		glVertex2d(Square->TopRight.X, Square->TopRight.Y);
		glVertex2d(Square->TopLeft.X, Square->TopLeft.Y);
		glVertex2d(Square->BottomLeft.X, Square->BottomLeft.Y);
		glVertex2d(Square->BottomRight.X, Square->BottomRight.Y);
	}
	glEnd();
}

void
GLMakeTexture(loaded_image *Image, uint32 *BitmapPixels)
{
	glGenTextures(1, (GLuint *)&Image->GLID);
	glBindTexture(GL_TEXTURE_2D, Image->GLID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             Image->Width, Image->Height,
	             0, GL_RGBA, GL_UNSIGNED_BYTE, BitmapPixels);
}






// NOTE below is the platform independent stuff. Above is all platform dependent










enum render_entity_type
{
	RenderEntity_Line,
	RenderEntity_Texture,
	RenderEntity_Square,
	RenderEntity_ColorPoints,
};

struct render_entity
{
	render_entity_type Type;

	// This is the same as center offset in loaded_image, but it's controlled in game logic and usualy changes over time.
	// Simply added to CenterOffset when sorting (not bounded after addition)
	// Used to make vertical
	vector2 GameManualOffset;

	gl_line Line;
	gl_texture Texture;
	gl_square Square;
	gl_color_points ColorPoints;
};

int32 RenderLayer_MaxEntities = 1000;
struct render_layer
{
	render_entity Entities[1000];
	uint64 EntitiesCount;
};

enum render_layer_id
{
	RenderLayer_One,
	RenderLayer_Two,
	RenderLayer_Three,

	RenderLayer_Count,
};

render_entity*
GetNextEntity(render_layer* Layer, render_entity_type Type)
{
	render_entity* NextEntity = {};

	Layer->Entities[Layer->EntitiesCount] = {};
	Layer->Entities[Layer->EntitiesCount].Type = Type;
	NextEntity = &Layer->Entities[Layer->EntitiesCount];

	Layer->EntitiesCount++;
	Assert(Layer->EntitiesCount < RenderLayer_MaxEntities);

	return (NextEntity);
}

void
PushRenderSquare(render_layer* RenderLayer, gl_square Square)
{
	gl_square *SquareLinkData = &GetNextEntity(RenderLayer, RenderEntity_Square)->Square;
	SquareLinkData->TopLeft = Square.TopLeft;
	SquareLinkData->TopRight = Square.TopRight;
	SquareLinkData->BottomLeft = Square.BottomLeft;
	SquareLinkData->BottomRight = Square.BottomRight;
	SquareLinkData->Color = Square.Color;
}


gl_square
MakeGLRectangle(rect Rect, color Color)
{
	gl_square Result = {};

	Result.Color = Color;

	Result.TopLeft = Rect.TopLeft;
	Result.TopRight = vector2{Rect.BottomRight.X, Rect.TopLeft.Y};
	Result.BottomRight = Rect.BottomRight;
	Result.BottomLeft = vector2{Rect.TopLeft.X, Rect.BottomRight.Y};

	return (Result);
}

gl_square
MakeGLSquare(vector2 Pos, int32 SideLength, color Color)
{
	gl_square Result = {};

	Result.Color = Color;

	int32 HalfSide = SideLength / 2;
	Result.TopLeft = vector2{Pos.X - HalfSide, Pos.Y - HalfSide};
	Result.TopRight = vector2{Pos.X + HalfSide, Pos.Y - HalfSide};
	Result.BottomLeft = vector2{Pos.X - HalfSide, Pos.Y + HalfSide};
	Result.BottomRight = vector2{Pos.X + HalfSide, Pos.Y + HalfSide};

	return (Result);
}

#endif