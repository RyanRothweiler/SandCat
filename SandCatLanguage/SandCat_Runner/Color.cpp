#ifndef COLOR_CPP
#define COLOR_CPP


struct color
{
	real32 R;
	real32 G;
	real32 B;
	real32 A;
};

const color COLOR_WHITE = color{1.0f, 1.0f, 1.0f, 1.0f};
const color COLOR_BLACK = color{0.0f, 0.0f, 0.0f, 1.0f};
const color COLOR_GREY = color{0.5f, 0.5f, 0.5f, 1.0f};
const color COLOR_RED = color{1.0f, 0, 0, 1.0f};
const color COLOR_ORANGE = color{1.0f, 0.5f, 0, 1.0f};
const color COLOR_BLUE = color{0, 0, 1.0f, 1.0f};
const color COLOR_GREEN = color{0, 1.0f, 0, 1.0f};
const color COLOR_PINK = color{1.0, 1.0f, 0.6f, 1.0f};
const color COLOR_YELLOW = color{1.0f, 1.0f, 0.0, 1.0f};
const color COLOR_LIGHT_BLUE = color{0.0, 1.0f, 1.0f, 1.0f};
const color COLOR_PURPLE = color{0.5f, 0.0f, 0.5f, 1.0f};
const color COLOR_TEAL = color{0.0f, 0.5f, 0.5f, 1.0f};

const color EditorColor_Blue = color{0.04f, 0.46f, 0.88f, 1.0f};
const color EditorColor_LightBlue = color{0.33f, 0.69f, 0.72f, 1.0f};
const color EditorColor_Pink = color{0.91f, 0.17f, 0.5f, 1.0f};
const color EditorColor_Purple = color{0.55f, 0.3f, 0.9f, 1.0f};
const color EditorColor_MainDark = color{0.07f, 0.07f, 0.10f, 1.0f};
const color EditorColor_SecondaryDark = color{0.22f, 0.22f, 0.29f, 1.0f};

#endif