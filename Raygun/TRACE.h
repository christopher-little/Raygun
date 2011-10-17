#pragma once

#ifndef TRACE_H
#define TRACE_H

#include <crtdbg.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define TRACEMAXSTRING  1024

//char szBuffer[TRACEMAXSTRING]; // Breaks when linking!
inline void TRACE(const char* format,...)
{
	char szBuffer[TRACEMAXSTRING];

    va_list args;
    va_start(args,format);
    int nBuf;
    nBuf = _vsnprintf(szBuffer,
                                TRACEMAXSTRING,
                                format,
                                args);
    va_end(args);

    _RPT0(_CRT_WARN,szBuffer);
}
#define TRACEF _snprintf(szBuffer,TRACEMAXSTRING,"%s(%d): ", \
                                &strrchr(__FILE__,'\\')[1],__LINE__); \
                                _RPT0(_CRT_WARN,szBuffer); \
                                TRACE
#else
// Remove for release mode
#define TRACE  ((void)0)
#define TRACEF ((void)0)
#endif

#endif // TRACE_H