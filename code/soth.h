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
