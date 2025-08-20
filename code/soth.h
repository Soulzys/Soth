struct ReadFileResult
{
	uint32 ContentSize ;
	void*  Content     ;
};

static ReadFileResult Win32_ReadFile(const char* Filename);
