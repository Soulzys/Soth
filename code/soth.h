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


struct GameScreenSize
{
	int Width;
	int Height;
};


struct GameState
{
	uint16 VBO           ;
	uint16 VAO           ;
	uint16 ShaderProgram ;
	MatrixS4 PlayerPos   ;
};

// Platform layer functions : declared here, defined in win32_soth.cpp, called here
#define DEBUG_PLATFORM_READ_FILE(name) ReadFileResult name(const char* Filename)
typedef DEBUG_PLATFORM_READ_FILE(debug_platform_read_file);
//ReadFileResult Win32_ReadFile(const char* Filename);

#define DEBUG_PLATFORM_LOG_VECTOR(name) void name(const Vec3& V, real32 W, uint8 Precision, const char* Extra)
typedef DEBUG_PLATFORM_LOG_VECTOR(debug_platform_log_vector);
//void DebugLogVector(const Vec3& V, real32 W = 1.0f, uint8 Precision = 3, const char* Extra = "");

#define DEBUG_PLATFORM_LOG_MATRIXS4(name) void name(const MatrixS4& M, uint8 Precision)
typedef DEBUG_PLATFORM_LOG_MATRIXS4(debug_platform_log_matrixs4);
//void DebugLogMatrixS4(const MatrixS4& M, uint8 Precision = 3);

#define DEBUG_PLATFORM_LOG_MESSAGE(name) void name(const char* Message)
typedef DEBUG_PLATFORM_LOG_MESSAGE(debug_platform_log_message);
//void DebugLogMessage(const char* Message);

struct GameMemory
{
	bool32 IsInitialized        ;
	uint64 PermanentStorageSize ;
	void*  PermanentStorage     ;

	OpenGL* OpenGL;

	debug_platform_read_file*    DebugPlatformReadFile    ;
	debug_platform_log_vector*   DebugPlatformLogVector   ;
	debug_platform_log_matrixs4* DebugPlatformLogMatrixS4 ;
	debug_platform_log_message*  DebugPlatformLogMessage  ;
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




// Game layer functions : declared and defined here, called in win32_soth.cpp
#define GAME_UPDATE(name) void name(GameMemory* Memory, GameInputController* Input, GameScreenSize* ScreenSize)
typedef GAME_UPDATE(game_update);
GAME_UPDATE(GameUpdateStub) {}
void GameUpdate(GameMemory* Memory, GameInputController* Input);

#define GAME_EXIT(name) void name()
typedef GAME_EXIT(game_exit);
GAME_EXIT(GameExitStub) {}
void GameExit();
//static void DebugLogMatrix(const MatrixS4& M);
