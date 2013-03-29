#pragma once

#ifndef DEBUG
#define DEBUG 1
#endif

#ifdef DEBUG
	void _Assert(char *, unsigned);

	#define ASSERT(f)	\
		if(f)			\
			{}			\
		else			\
			_Assert(__FILE__, __LINE__);
#else
	#define ASSERT(f)
#endif