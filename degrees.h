#pragma once
/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#define _USE_MATH_DEFINES
#include <math.h>
double degreesToRadians(double deg) { return deg * M_PI / 180.0;  }