/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once

#ifdef _DEBUG
#define GDIFLUSH() ::GdiFlush();
#else
#define GDIFLUSH()
#endif