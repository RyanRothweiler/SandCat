#ifndef VECTOR2_H
#define VECTOR2_H

#include "String.cpp"

struct vector2
{
	real64 X;
	real64 Y;

	operator string ()
	{
		string FinalString = {};

		string XString = (int64)X;
		string YString = (int64)Y;

		FinalString = "(" + XString + "," + YString + ")";

		return (FinalString);
	}
};

#endif