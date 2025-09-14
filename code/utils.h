//namespace Math
//{
	struct MatrixS4;

	struct Vec3
	{
		Vec3();
		Vec3(const Vec3& V);
		Vec3(real32 ParamX, real32 ParamY, real32 ParamZ);

		real32 Length   () const;
		Vec3&  Normalize();
		real32 Dot      (const Vec3& V) const;
		Vec3   Cross    (const Vec3& V) const;

		Vec3& Translate    (const MatrixS4& M);
		Vec3& RotateAroundX(const MatrixS4& M);
		Vec3& RotateAroundY(const MatrixS4& M);
		Vec3& RotateAroundZ(const MatrixS4& M);
		Vec3& Scale        (const MatrixS4& M);

		Vec3 operator+(const Vec3& V) const;
		Vec3 operator-(const Vec3& V) const;
		Vec3 operator*(const Vec3& V) const;
		Vec3 operator*(const MatrixS4& M) const;
		Vec3 operator/(const Vec3& V) const;

		real32 X, Y, Z;
	};

	struct MatrixS4
	{
		MatrixS4();
		MatrixS4(real32 N);
		MatrixS4(const MatrixS4& Ma);

		const real32* operator[](uint8 i) const;
		real32* operator[](uint8 i);
		//MatrixS4 operator=(real32* M) const;
		//real32* operator*=(real32 N);
		MatrixS4 operator*(const MatrixS4& Ma) const;
		MatrixS4& Translate(const Vec3& V);
		MatrixS4& RotateAroundX(real32 Amount);
		MatrixS4& RotateAroundY(real32 Amount);
		MatrixS4& RotateAroundZ(real32 Amount);
		//MatrixS4& Scale(const Vec3& V);
		MatrixS4& Scale(real32 Amount);
		MatrixS4& Transpose();
		MatrixS4 Transpose() const;
		real32* GetPointer();


		real32 M[4][4] = {
						 	{1, 0, 0, 0},
						 	{0, 1, 0, 0},
						 	{0, 0, 1, 0},
						 	{0, 0, 0, 1}
						 };
	};

	static MatrixS4 GetOrthographicProjectionMatrix(real32 Left, real32 Right, real32 Bottom, real32 Top, real32 Near, real32 Far);
//}