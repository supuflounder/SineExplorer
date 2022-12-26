/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "Invert.h"

template<typename T> void swap(T & a, T & b)
    {
    T t = a;
    a = b;
    b = t;
    }

void Invert(XFORM & M)
    {
    /************************************************
    *  The matrix for a counterclockwise rotation is
     * [ cos θ   sin θ]
     * [-sin θ   cos θ]
     * [   0       0  ]
     *
     * [ m11   m12 ]
     * [ m21   m22 ]
     * [ dx    dy  ]
     * 
     * The inverse matrix for the rotation is just
     * the transpose of the above matrix
     * 
     * [cos θ   -sin θ]   [m11 m21]
     * [sin θ    cos θ]   [m12 m22]
     * [    0        0]   [0   0  ]
     * 
    ************************************************/
    swap(M.eM12, M.eM21);
    }

CPoint Transform(CPoint pt, XFORM & M)
    {
    /*****************************************************************
    * [ a  b]
    * [ c  d]   x' = a·x + c·y + Δx
    * [Δx Δy]   y' = b·x + d·y + Δy
    * 
    * [ eM11   eM12 ]
    * [ eM21   eM22 ]
    * [ eDx    eDy  ]
    *****************************************************************/

    CPoint np;

    np.x = (int)(M.eM11 * (double) pt.x + M.eM21 * (double) pt.y + M.eDx);
    np.y = (int)(M.eM12 * (double) pt.x + M.eM22 * (double) pt.y + M.eDy);
    return np;
    }