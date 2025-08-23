#include "utils.h"
#include <math.h>

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

real32 Vec3::Length() const
{
	return (real32)sqrt((X * X) + (Y * Y) + (Z * Z));
}

Vec3& Vec3::Normalize()
{
	float _Length = Length();
	if (_Length > 0)
	{
		float _InvLength = 1 / _Length;
		X *= _InvLength;
		Y *= _InvLength;
		Z *= _InvLength;
	}

	return *this;
}

real32 Vec3::Dot(const Vec3& V) const
{
	return (X * V.X) + (Y * V.Y) + (Z * V.Z);
}

Vec3 Vec3::Cross(const Vec3& V) const
{
	return Vec3(
			       (Y * V.Z) - (Z * V.Y), 
				   (Z * V.X) - (X * V.Z), 
				   (X * V.Y) - (Y * V.X)
			   );
}

Vec3 Vec3::operator+(const Vec3& V) const
{
	return Vec3(X + V.X, Y + V.Y, Z + V.Z);
}

Vec3 Vec3::operator-(const Vec3& V) const
{
	return Vec3(X - V.X, Y - V.Y, Z - V.Z);
}

Vec3 Vec3::operator*(const Vec3& V) const
{
	return Vec3(X * V.X, Y * V.Y, Z * V.Z);
}

Vec3 Vec3::operator/(const Vec3& V) const
{
	return Vec3(X / V.X, Y / V.Y, Z / V.Z);
}



MatrixS4::MatrixS4()
{
}

const real32* MatrixS4::operator[](uint8 i) const
{
	return M[i];
}

MatrixS4 MatrixS4::operator*(const MatrixS4& Ma) const
{
	MatrixS4 _R;

	for (uint8 i = 0; i < 4; i++)
	{
		for (uint8 j = 0; j < 4; j++)
		{
			_R[i][j] = M[i][0] * Ma[0][j] + 
					   M[i][1] * Ma[1][j] + 
					   M[i][2] * Ma[2][j] + 
					   M[i][3] * Ma[3][j] ;
		}
	}

	return _R;
}