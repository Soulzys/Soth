#if BUILD_ASSERT
#define ASSERT(Expression) if (!(Expression)) {*(int*)0 = 0;}
#else
#define ASSERT(Expression)
#endif

#define KILOBYTES(Value) (         (Value) * 1024LL)
#define MEGABYTES(Value) (KILOBYTES(Value) * 1024LL)
#define GIGABYTES(Value) (MEGABYTES(Value) * 1024LL)

struct ReadFileResult
{
	uint32 ContentSize ;
	void*  Content     ;
};

struct GameState
{
	uint16 VBO           ;
	uint16 VAO           ;
	uint16 ShaderProgram ;
};

struct GameMemory
{
	bool32 IsInitialized        ;
	uint64 PermanentStorageSize ;
	void*  PermanentStorage     ;
};

static ReadFileResult Win32_ReadFile(const char* Filename);
static void ResizeWindow();
static void DebugLogVector(const Vec3& V, real32 W = 1.0f, uint8 Precision = 3, const char* Extra = "");
static void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision = 3);
//static void DebugLogMatrix(const MatrixS4& M);
