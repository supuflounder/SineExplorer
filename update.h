/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
template <typename T> void update(CWnd * w, T & var, T val)
    {
    if(var != val)
        w->Invalidate();
    var = val;
    }