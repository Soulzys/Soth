//namespace Math
//{
	struct Vec3
	{
		Vec3();
		Vec3(const Vec3& V);
		Vec3(real32 ParamX, real32 ParamY, real32 ParamZ);

		real32 Length   () const;
		Vec3&  Normalize();
		real32 Dot      (const Vec3& V) const;
		Vec3   Cross    (const Vec3& V) const;

		Vec3 operator+(const Vec3& V) const;
		Vec3 operator-(const Vec3& V) const;
		Vec3 operator*(const Vec3& V) const;
		Vec3 operator/(const Vec3& V) const;

		real32 X, Y, Z;
	};
//}