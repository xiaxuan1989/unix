#include "stdio.h"
#include "stdlib.h"
#include "sys.h"

#include "CommandTree.h"
#include "ExecuteCommand.h"
#include "globe.h"
#include "PreExecute.h"
#include "string.h"

int splash();  // splash.c


char line[1000] = {0};
char* args[50] = {0};
int argsCnt = 0;
int main1()
{
	
	printf("UNIX Version 6 - C/CPP\n");

	char lineInput[512];
	getPath( curPath );	
	int root;
	while( 1 )
	{
		root = -1;
		argsCnt = 0;
		InitCommandTree();
		printf("[%s]# ", curPath);
		gets( lineInput );		
		if ( strcmp( (unsigned char*) "shutdown", (unsigned char*) lineInput ) == 0 )
		{
			syncFileSystem();
			printf("It's now safe to turn off your computer.\n");
			break;
		}
		argsCnt = SpiltCommand(lineInput);
		root = AnalizeCommand(0, argsCnt - 1, 0);
		if ( root >= 0 )
		{
			ExecuteCommand( &commandNodes[root], 0, 0 );
		}		
	}

	return 0;
}
