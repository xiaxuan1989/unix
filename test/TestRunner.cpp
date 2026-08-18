#include "TestInclude.h"
#include "TestUtility.h"

#include "TerminalDisplay.h"
#include "Video.h"

void RunKernelTests()
{
	TerminalDisplay::ROWS = 0;
	Diagnose::ROWS = Diagnose::SCREEN_ROWS;
	Diagnose::ClearScreen();
	Diagnose::TraceOn();

	Diagnose::Write("=== UNIX kernel tests ===\n");
	PrintResult("SwapperManager", TestSwapperManager());
	PrintResult("FileSystem", TestFileSystem());
	PrintResult("File read/write", FileRWTest());
	PrintResult("BufferManager", TestBufferManager());
	Diagnose::Write("=== ALL TESTS PASSED ===\n");
}
