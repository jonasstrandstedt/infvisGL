#include "Assert.h"
#include <cstdio>
#include <cstdlib>

void _Assert(char *strFile, unsigned uLine)
{
	fflush(NULL);
	fprintf(stderr, "\nAssertion failed: %s, line %u\n", strFile, uLine);
	fflush(stderr);
	abort();
}