#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "pandemic.h"


// This file includes general stuff that doesn't belong anywhere

void check(bool success, const char *err_format, ...)
{
	va_list vl;
	va_start(vl, err_format);
	
	if (!success)
	{
		if (err_format)
		{
			vfprintf(stderr, err_format, vl);
			fprintf(stderr, "\n");
		}
		
		abort();
	}
	
	va_end(vl);
}
