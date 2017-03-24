struct rect
{
	vector2 TopLeft;
	vector2 BottomRight;
};

bool32
RectContains(rect Rect, vector2 Pos)
{
	if (Rect.TopLeft.X < Pos.X &&
	        Rect.TopLeft.Y < Pos.Y &&
	        Rect.BottomRight.X > Pos.X &&
	        Rect.BottomRight.Y > Pos.Y)
	{
		return (true);
	}

	return (false);
}

real64
RectWidth(rect Rect)
{
	return (Abs(Rect.TopLeft.X - Rect.BottomRight.X));
}

real64
RectHeight(rect Rect)
{
	return (Abs(Rect.TopLeft.Y - Rect.BottomRight.Y));
}

rect
GetOutlineRect(rect Rect, real64 OutlineSize)
{
	rect OutlineRect = {};
	OutlineRect.TopLeft = Rect.TopLeft + vector2{ -OutlineSize, -OutlineSize};
	OutlineRect.BottomRight = Rect.BottomRight + vector2{OutlineSize, OutlineSize};
	return (OutlineRect);
}

vector2
RectCenter(rect Rect)
{
	vector2 Center = {};
	Center.X = (Rect.BottomRight.X + Rect.TopLeft.X) / 2.0f;
	Center.Y = (Rect.BottomRight.Y + Rect.TopLeft.Y) / 2.0f;
	return (Center);
}

void
RectSetHeight(rect* Rect, real64 Height)
{
	real64 Width = RectWidth(*Rect);
	Rect->BottomRight = Rect->TopLeft + vector2{Width, Height};
}