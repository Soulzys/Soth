#pragma once

#include <stdint.h>
#include "agnostic_opengl.h"

#if BUILD_ASSERT
#define ASSERT(Expression) if (!(Expression)) {*(int*)0 = 0;}
#else
#define ASSERT(Expression)
#endif

#define internal static

#define KILOBYTES(Value)   (         (Value) * 1024LL)
#define MEGABYTES(Value)   (KILOBYTES(Value) * 1024LL)
#define GIGABYTES(Value)   (MEGABYTES(Value) * 1024LL)

#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))



typedef int8_t   int8   ;
typedef int16_t  int16  ;
typedef int32_t  int32  ;
typedef int64_t  int64  ;
typedef uint8_t  uint8  ;
typedef uint16_t uint16 ;
typedef uint32_t uint32 ;
typedef uint64_t uint64 ;

typedef float    real32 ;
typedef double   real64 ;

typedef int32    bool32 ;


#include "utils.cpp"


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

	OpenGL* OpenGL;
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
		GameKeyState Keys[3];

		struct
		{
			GameKeyState MoveLeft  ;
			GameKeyState MoveRight ;
			GameKeyState Jump      ;
		};
	};

	void CopyState(GameInputController* Input);
};

static ReadFileResult Win32_ReadFile(const char* Filename);
static void DebugLogVector(const Vec3& V, real32 W = 1.0f, uint8 Precision = 3, const char* Extra = "");
static void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision = 3);

void UpdateGame(GameMemory* Memory, GameInputController* Input);
void ExitGame();
//static void DebugLogMatrix(const MatrixS4& M);
