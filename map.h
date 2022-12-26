/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once

template <typename T> T map(T x, T in_min, T in_max, T out_min, T out_max)
    {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }