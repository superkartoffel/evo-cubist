// $Id: compat-win32.cpp,v 845ec36a3b5b 2009/02/05 14:46:01 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _WIN32

#include <windows.h>
#include "compat-win32.h"


bool hasRand_s(void) 
{
    OSVERSIONINFO os;
    GetVersionEx(&os);
    return (os.dwMajorVersion > 5) && (os.dwMinorVersion >= 1);
}

#endif
