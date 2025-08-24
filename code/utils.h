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

	struct MatrixS4
	{
		MatrixS4();

		const real32* operator[](uint8 i) const;
		real32* operator[](uint8 i);
		//real32* operator*=(real32 N);
		MatrixS4 operator*(const MatrixS4& Ma) const;
		MatrixS4& Translate(const Vec3& V);
		MatrixS4& Scale(const Vec3& V);


		real32 M[4][4] = {
						 	{1, 0, 0, 0},
						 	{0, 1, 0, 0},
						 	{0, 0, 1, 0},
						 	{0, 0, 0, 1}
						 };
	};
//}