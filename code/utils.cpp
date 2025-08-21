#include "utils.h"

Vec3::Vec3()
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vec3::Vec3(const Vec3& V)
{
	X = V.X;
	Y = V.Y;
	Z = V.Z;
}

Vec3::Vec3(real32 ParamX, real32 ParamY, real32 ParamZ)
{
	X = ParamX;
	Y = ParamY;
	Z = ParamZ;
}