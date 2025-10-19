#if BUILD_ASSERT
#define ASSERT(Expression) if (!(Expression)) {*(int*)0 = 0;}
#else
#define ASSERT(Expression)
#endif

#define KILOBYTES(Value)   (         (Value) * 1024LL)
#define MEGABYTES(Value)   (KILOBYTES(Value) * 1024LL)
#define GIGABYTES(Value)   (MEGABYTES(Value) * 1024LL)

#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))


enum KeyState
{
	UP   = 0, 
	DOWN = 1
};


//
//


struct ReadFileResult
{
	uint32 ContentSize ;
	void*  Content     ;
};


//
//


struct GameState
{
	uint16 VBO           ;
	uint16 VAO           ;
	uint16 ShaderProgram ;
	MatrixS4 PlayerPos   ;
};

struct GameMemory
{
	bool32 IsInitialized        ;
	uint64 PermanentStorageSize ;
	void*  PermanentStorage     ;
};

struct GameKeyState
{
	int      HalfTransitionCount;
	KeyState State;
};

struct GameInputController
{
	union
	{
		GameKeyState Keys[4];

		struct
		{
			GameKeyState MoveLeft  ;
			GameKeyState MoveRight ;
			GameKeyState MoveUp    ;
			GameKeyState MoveDown  ;
		};
	};

	void CopyState(GameInputController* Input);
};

static ReadFileResult Win32_ReadFile(const char* Filename);
static void DebugLogVector(const Vec3& V, real32 W = 1.0f, uint8 Precision = 3, const char* Extra = "");
static void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision = 3);
//static void DebugLogMatrix(const MatrixS4& M);
