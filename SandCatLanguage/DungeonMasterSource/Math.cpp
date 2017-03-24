#ifndef MATH_ARCADE
#define MATH_ARCADE

#include "Vector2.h"
#include <math.h>

real64
Lerp(real64 Start, real64 End, real64 Percentage)
{
	return (Start + (Percentage * (End - Start)));
}

/*
 SQUARE
*/
int64
Square(int64 input)
{
	return (input * input);
}

real64
Square(real64 input)
{
	return (input * input);
}


/*
 CLAMP VALUE
*/
real64
ClampValue(real64 Bottom, real64 Top, real64 Value)
{
	if (Value <= Bottom)
	{
		return (Bottom);
	}
	if (Value >= Top)
	{
		return (Top);
	}
	return (Value);
}

vector2
ClampValue(real64 Bottom, real64 Top, vector2 Value)
{
	vector2 Output;
	Output.X = ClampValue(Bottom, Top, Value.X);
	Output.Y = ClampValue(Bottom, Top, Value.Y);
	return (Output);
}

int64
ClampValue(int64 Bottom, int64 Top, int64 Value)
{
	if (Value <= Bottom)
	{
		return (Bottom);
	}
	if (Value >= Top)
	{
		return (Top);
	}
	return (Value);
}

uint64
ClampValue(uint64 Bottom, uint64 Top, uint64 Value)
{
	if (Value <= Bottom)
	{
		return (Bottom);
	}
	if (Value >= Top)
	{
		return (Top);
	}
	return (Value);
}

// ----- -----


/*
 ABSOLUTE VALUE
*/
real64
Abs(real64 A)
{
	if (A >= 0)
	{
		return (A);
	}
	else
	{
		return (A * -1);
	}
}

int64
Abs(int64 A)
{
	if (A >= 0)
	{
		return (A);
	}
	else
	{
		return (A * -1);
	}
}

uint64
Abs(uint64 A)
{
	if (A >= 0)
	{
		return (A);
	}
	else
	{
		return (A * -1);
	}
}
// ----- -----


int32
Abs(int32 A)
{
	if (A >= 0)
	{
		return (A);
	}
	else
	{
		return (A * -1);
	}
}
// ----- -----

/*
 MAX
*/
real64
Max(real64 A, real64 B)
{
	if (A > B)
	{
		return (A);
	}
	else
	{
		return (B);
	}
}

uint64
Max(uint64 A, uint64 B)
{
	if (A > B)
	{
		return (A);
	}
	else
	{
		return (B);
	}
}

int64
Max(int64 A, int64 B)
{
	if (A > B)
	{
		return (A);
	}
	else
	{
		return (B);
	}
}
// ----- -----


/*
 MIN
*/
real64
Min(real64 A, real64 B)
{
	if (A < B)
	{
		return (A);
	}
	else
	{
		return (B);
	}
}

uint64
Min(uint64 A, uint64 B)
{
	if (A < B)
	{
		return (A);
	}
	else
	{
		return (B);
	}
}
// ----- -----

real64
SquareRoot(real64 num)
{
	real64 i = 0;
	real64 x1, x2;

	while ( (i * i) <= num )
	{
		i += 0.1f;
	}
	x1 = i;
	for (int j = 0; j < 10; j++)
	{
		x2 = num;
		x2 /= x1;
		x2 += x1;
		x2 /= 2;
		x1 = x2;
	}
	return x2;
}

real64
DotProduct(vector2 A, vector2 B)
{
	real64 Result;
	Result = (A.X * B.X) + (A.Y * B.Y);
	return (Result);
}

real64
Cos(real64 Input)
{
	return (cos(Input));
}

real64
ArcCos(real64 Input)
{
	return (acos(Input));
}

real64
Sin(real64 Input)
{
	return (sin(Input));
}

const real64 PI = 3.14159f;

uint64
Pow(uint64 Num, uint32 Expo)
{
	if (Expo == 0)
	{
		return (1);
	}
	else
	{
		return (Num * Pow(Num, Expo - 1));
	}
}

#endif