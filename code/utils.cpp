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

Vec3 Vec3::operator*(const MatrixS4& M) const
{
	Vec3 _V;
	_V.X = (X * M[0][0]) + (Y * M[1][0]) + (Z * M[2][0]) + M[3][0];
	_V.Y = (X * M[0][1]) + (Y * M[1][1]) + (Z * M[2][1]) + M[3][1];
	_V.Z = (X * M[0][2]) + (Y * M[1][2]) + (Z * M[2][2]) + M[3][2];
	return _V;
}

Vec3 Vec3::operator/(const Vec3& V) const
{
	return Vec3(X / V.X, Y / V.Y, Z / V.Z);
}

Vec3& Vec3::Scale(const MatrixS4& M)
{
	//Matrix

	return *this;
}





MatrixS4::MatrixS4()
{
}

MatrixS4::MatrixS4(real32 N)
{
	M[0][0] = N; M[0][1] = 0; M[0][2] = 0; M[0][3] = 0;
	M[1][0] = 0; M[1][1] = N; M[1][2] = 0; M[1][3] = 0;
	M[2][0] = 0; M[2][1] = 0; M[2][2] = N; M[2][3] = 0;
	M[3][0] = 0; M[3][1] = 0; M[3][2] = 0; M[3][3] = 1;
}

MatrixS4::MatrixS4(const MatrixS4& Ma)
{
	for (uint8 i = 0; i < 4; i++)
	{
		for (uint8 j = 0; j < 4; j++)
		{
			M[i][j] = Ma[i][j];
		}
	}
}

const real32* MatrixS4::operator[](uint8 i) const
{
	return M[i];
}

real32* MatrixS4::operator[](uint8 i)
{
	return M[i];
}

//real32* MatrixS4::operator*=(real32 N)
//{
//	
//}

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

/*MatrixS4 MatrixS4::operator=(real32* Ma) const
{
	MatrixS4 _R;

	for (uint8 i = 0; i < 4; i++)
	{
		for (uint8 j = 0; j < 4; j++)
		{
			_R[i][j] = Ma[i][j];
		}
	}

	return _R;
}*/

MatrixS4& MatrixS4::Translate(const Vec3& V)
{
	M[0][3] += V.X;
	M[1][3] += V.Y;
	M[2][3] += V.Z;

	return *this;
}

MatrixS4& MatrixS4::RotateAroundX(real32 Amount)
{
	M[1][1] *= (real32) cos(Amount);
	M[1][2] *= (real32) sin(Amount);
	M[2][1] *= (real32)-sin(Amount);
	M[2][2] *= (real32) cos(Amount);
	
	return *this;
}

MatrixS4& MatrixS4::RotateAroundY(real32 Amount)
{
	M[0][0] *= (real32) cos(Amount);
	M[0][2] *= (real32)-sin(Amount);
	M[2][0] *= (real32) sin(Amount);
	M[2][2] *= (real32) cos(Amount);

	return *this;
}

MatrixS4& MatrixS4::RotateAroundZ(real32 Amount)
{
	M[0][0] *= (real32) cos(Amount);
	M[0][1] *= (real32) sin(Amount);
	M[1][0] *= (real32)-sin(Amount);
	M[2][2] *= (real32) cos(Amount);
	
	return *this;
}

//MatrixS4& MatrixS4::Scale(const Vec3& V)
//{
//	M[0][0] *= V.X;
//	M[1][1] *= V.Y;
//	M[2][2] *= V.Z;
//
//	return *this;
//}

MatrixS4& MatrixS4::Scale(real32 Amount)
{
	M[0][0] *= Amount; M[0][1] *= Amount; M[0][2] *= Amount; M[0][3] *= Amount;
	M[1][0] *= Amount; M[1][1] *= Amount; M[1][2] *= Amount; M[1][3] *= Amount;
	M[2][0] *= Amount; M[2][1] *= Amount; M[2][2] *= Amount; M[2][3] *= Amount;

	return *this;
}

MatrixS4& MatrixS4::Transpose()
{
	MatrixS4 _T;
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			_T[i][j] = M[i][j];
		}
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			M[i][j] = _T[j][i];
		}
	}

	return *this;
}

MatrixS4 MatrixS4::Transpose() const
{
	MatrixS4 _M;

	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			_M[i][j] = M[j][i];
		}
	}

	return _M;
}

real32* MatrixS4::GetPointer()
{
	return (real32*)(M);
}

MatrixS4& MatrixS4::MoveX(real32 Amount)
{
	M[3][0] += Amount;
	
	return *this;
}

MatrixS4& MatrixS4::MoveY(real32 Amount)
{
	M[3][1] += Amount;

	return *this;
}

MatrixS4 GetOrthographicProjectionMatrix(real32 Left, real32 Right, real32 Bottom, real32 Top, real32 Near, real32 Far)
{
	MatrixS4 _M;

	_M[0][0] = 2 / (Right - Left)                 ;  _M[0][1] = 0                                  ;  _M[0][2] = 0                              ;  _M[0][3] = 0 ;
	_M[1][0] = 0                                  ;  _M[1][1] = 2 / (Top - Bottom)                 ;  _M[1][2] = 0                              ;  _M[1][3] = 0 ;
	_M[2][0] = 0                                  ;  _M[2][1] = 0                                  ;  _M[2][2] = -2 / (Far - Near)              ;  _M[2][3] = 0 ;
	_M[3][0] = -((Right + Left) / (Right - Left)) ;  _M[3][1] = -((Top + Bottom) / (Top - Bottom)) ;  _M[3][2] = -((Far + Near) / (Far - Near)) ;  _M[3][3] = 1 ;
	
	return _M;
}

