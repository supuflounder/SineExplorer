/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "circuit.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GDIFLUSH.h"
#include "Invert.h"
#include "Value.h"
#include "Colors.h"

/********************************************************************
* This module draws all the components for a circuit: resistors,
* capacitors, diodes, etc.
********************************************************************/

#define DEBUG_RELEASE_GDI

#ifdef DEBUG_RELEASE_GDI
#define GDICHECK(x) if(!(x)) DebugBreak();
#else
#define GDICHECK(x) x
#endif

const double Diode::DefaultVf = 0.7;

/**************************************************************************
*                           Element::rotate
* Inputs:
*     CDC & dc:
*     CPoint start: Starting point of element in logical coordinates
*     CPoint end: Ending point of element in logical coordinates
*     XFORM * M: Place to store the rotational matrix; if NULL, not stored
* Result: void
* 
* Effect:
*     Offsets the coordinate system so (0,0) is now at start and
*     rotates it so a nominally horizontal straight line of 
*     distance(start, end) will draw to end
**************************************************************************/
void Element::rotate(CDC & dc, CPoint start, CPoint end, XFORM * M)
    {
    dc.SetGraphicsMode(GM_ADVANCED);
    XFORM shift;
    shift.eM11 = (float) 1.0;
    shift.eM12 = (float) 0.0;
    shift.eM21 = (float) 0.0;
    shift.eM22 = (float) 1.0;
    shift.eDx = (float) start.x;
    shift.eDy = (float) start.y;
    dc.SetWorldTransform(&shift);

    // We have now set 0,0 to be the start of the element.  Now do a rotational transformation
    // based on the required angle

 /*****************************************************************************
 * The transformation matrix is, for rotation counterclockwise by θ:
 * [ cos θ   sin θ]
 * [-sin θ   cos θ]
 * [   0       0  ]
 *
 * [ m11   m12 ]
 * [ m21   m22 ]
 * [ dx    dy  ]
 *
 *
 ******************************************************************************/

    // First, we compute the angle θ
    //   o  start
    //    \
    //     \
    //      \
    //       o end
    

    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double theta = atan2(dy, dx);  // Note that atan2(y,x) has y and x coordinates transposed
    double sintheta = sin(theta);
    double costheta = cos(theta);
    XFORM rotate;
    rotate.eM11 = (float) costheta;
    rotate.eM12 = (float) sintheta;
    rotate.eM21 = (float) -sintheta;
    rotate.eM22 = (float) costheta;
    rotate.eDx = 0.0;
    rotate.eDy = 0.0;
    dc.ModifyWorldTransform(&rotate, MWT_LEFTMULTIPLY);

    if(M != NULL)
        { /* store current */
        dc.GetWorldTransform(M);
        } /* store current */
    }

/***************************************************************************
*                             Element::ShowConnection
* Inputs:
*    CDC & dc:
*    CPoint pt: Point at which connection is to be shown
*    COLORREF col: Color to use
*       For debugging points, this will be red
*       For real connection points, this will be black
* Result: void
* 
* Effect:
*    Draws a connection dot in the place specified by pt in the color
*    specified by col
***************************************************************************/
void Element::ShowConnection(CDC & dc, CPoint pt, COLORREF col)
    {
    static const int radius = 3;
    int n = dc.SaveDC();
        FlashPen pen(flashing);
        dc.SelectObject(pen);
        CBrush brush;
        brush.CreateSolidBrush(col);
        dc.SelectObject(brush);

        dc.Ellipse(pt.x - radius, pt.y - radius, pt.x + radius + 1, pt.y + radius + 1); // +1 for GDI "exclusive of endpoint" feature
    dc.RestoreDC(n);
    }

/***********************************************************************************
*                         Element::ShowConnections
* Inputs:
*     CDC & dc:
*     COLORREF col: The color to use
* Result: void
* 
* Effect:
*    Draws all the connection points in the specified color
***********************************************************************************/
void Element::ShowConnections(CDC & dc, COLORREF col)
    {
    for(int i = 0; i < points.GetSize(); ++i)
        { /* draw each */
        ShowConnection(dc, points[i], col);
        } /* draw each */
    }

/**********************************************************************************
*                                 Element::GDIFlash
* Inputs:
*     CDC & dc:
*     CPoint pt:
* Result: void
* 
* Effect:
*      Causes a + to flash at the point PT
* Notes:
*      Useful for debugging graphics
***********************************************************************************/
/* static */ void Element::GDIFlash(CDC & dc, CPoint pt)
    {
#if 1
    int n = dc.SaveDC();
    dc.SetROP2(R2_XORPEN);
    dc.SelectStockObject(WHITE_PEN);
    dc.MoveTo(CPoint(pt.x - 10, pt.y));
    dc.LineTo(CPoint(pt.x + 10, pt.y));
    dc.MoveTo(CPoint(pt.x, pt.y - 10));
    dc.LineTo(CPoint(pt.x, pt.y + 10));
    ::Sleep(500);
    dc.MoveTo(CPoint(pt.x - 10, pt.y));
    dc.LineTo(CPoint(pt.x + 10, pt.y));
    dc.MoveTo(CPoint(pt.x, pt.y - 10));
    dc.LineTo(CPoint(pt.x, pt.y + 10));
    dc.RestoreDC(n);
#endif
    }

void Element::DebugConnection(CDC & dc, CPoint pt) 
    { 
#if 0
#ifdef _DEBUG
    ShowConnection(dc, pt, RGB(255, 0, 0)); 
#endif
#endif
    }
/*********************************************************************************
*                 Element::DebugConnections
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws all the connection points in the debug color
*********************************************************************************/
void Element::DebugConnections(CDC & dc)
    {
#if 0
#ifdef _DEBUG
    ShowConnections(dc, RGB(255, 0, 0));
#endif
#endif
    }

/*********************************************************************************
*                     Element::Connect
* Inputs:
*     CDC & dc:
*     CPoint pt: The connection point
* Result: void
* 
* Effect:
*     Places a connection point at pt
**********************************************************************************/

void Element::Connect(CDC & dc, CPoint pt, bool flashing)
    {
     ShowConnection(dc, pt, flashing ? Colors::FlashWire : Colors::Wire);
    } // Element::Connect

/**********************************************************************************
*                     ACSource::draw
* Inputs:
*     CDC & dc
* Result: void
* 
* Effect:
*    Draws the AC voltage source
**********************************************************************************/
void ACSource::draw(CDC & dc)
    {
    int n = dc.SaveDC();
    /****************************
    *         _____________
    *       #0             ^
    *       |              |
    *       *              |
    *      ***             |
    *     *   *            | 0.6 H
    *    *  +  *           |
    *     *   *            |
    *      ***             |  + is the origin
    *       *              |
    *       |              |
    *       #1_____________V
    ****************************/
    // Compute the circle size and item size
    // The entire item size is 60% of the height of the window
    CRect r;
    wnd->GetClientRect(&r);
    double itemHeight = 0.6 * (double)r.Height();
    double circle = 0.2 * (double) r.Height();
    
    FlashBrush br(!flashing);  // Note the brush is the _complement_ of the flashing mode!
    dc.SelectObject(br);
  
    // Draw the circle

        FlashPen pen(flashing);
        dc.SelectObject(pen);

        int radius = (int)(circle / 2.0);
        dc.Ellipse(origin.x - radius,
            origin.y - radius,
            origin.x + radius,
            origin.y + radius);
        // Draw the sine wave in it

        static const int dtheta = 20;
        static const int dx = (int)(0.75 * (double)radius);
        static const int height = radius / 3;

        CArray<CPoint> sine;
        int x = origin.x - dx;
        for (int i = 0; i <= 360; i += dtheta)
        {
            double rad = (((double)i) * M_PI) / 180.0;
            double xm = sin(rad);
            int y = origin.y - (int)(xm * height);
            sine.Add(CPoint(x++, y));
            //GDIFlash(dc, sine.GetAt(sine.GetCount() - 1));
        }
     
        dc.Polyline(&sine.GetAt(0), (int)sine.GetCount());
    
    // Now, compute the output lines and their connection
    CPoint L0s;
    L0s.x = origin.x;
    L0s.y = origin.y - radius;

    CPoint L0e;
    L0e.x = origin.x;
    L0e.y = origin.y - (int) (itemHeight / 2.0);

    CPoint L1s;
    L1s.x = origin.x;
    L1s.y = origin.y + radius;

    CPoint L1e;
    L1e.x = origin.x;
    L1e.y = origin.y + (int) (itemHeight / 2.0);

    dc.MoveTo(L0s);
    dc.LineTo(L0e);

    dc.MoveTo(L1s);
    dc.LineTo(L1e);

    GDIFLUSH();

    points.RemoveAll();
    points.Add(L0e);
    points.Add(L1e);

    // Show the connection points
    DebugConnection(dc, L0e);
    DebugConnection(dc, L1e);
    dc.RestoreDC(n);
    }
/****************************************************************************
*                          SimpleWire::draw
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws a simple wire (straight line) from the start point of the wire
*     to the end point of the wire, in the direction speicifed by the orientation
*     used when the wire was created
* GetP:
*    0 - starting point
*    1 - ending point
****************************************************************************/
void SimpleWire::draw(CDC & dc)
    {
    CPoint start = pt;
    CPoint end;
    if(orient == Orientation::Horizontal)
        {
        end.x = start.x + length;
        end.y = start.y;
        }
    else
    if(orient == Orientation::Vertical)
        {
        end.x = start.x;
        end.y = start.y + length;
        }
    else
        {
        return;
        }
    
    int n = dc.SaveDC();
        FlashPen pen(flashing);
        dc.SelectObject(pen);

        dc.MoveTo(start);
        GDICHECK(dc.LineTo(end));
        GDIFLUSH();
    dc.RestoreDC(n);

    points.RemoveAll();
    points.Add(start);
    points.Add(end);
#ifdef _DEBUG
    //DebugConnection(dc, end);
    GDIFLUSH();
#endif
    }
/*******************************************************************************
*                                ComplexWire::draw
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws a 2-element "complex" wire from start to end.  The orientation of
*     the first segment is specified by the orientation chosen when the complex
*     wire was created
* GetP:
*     0 - start point
*     1 - end point
*     2 - inflection point
*******************************************************************************/

void ComplexWire::draw(CDC & dc)
    {
    int n = dc.SaveDC();
    /***********************************************************
    * A ComplexWire connects two points using a 90 degree bend
    * The orientation says which direction the first segment
    * goes
    *      
    ***********************************************************/
    points.RemoveAll();
    points.Add(start);
    points.Add(end);

    CPoint P2;

    FlashPen pen(flashing);
    dc.SelectObject(pen);

    switch(orient)
        { /* orient */
    case Orientation::Horizontal:
            /********************************************
            *     start           P2
            *     *---------------+  Horizontal
            *     P0              |
            *                     |
            *                  P1 * end
            ********************************************/
            P2 = CPoint(end.x, start.y);
            break;
    case Orientation::Vertical:
            /********************************************
            *      start
            *      * P0               Vertical
            *      |
            *      |               P1
            *      +---------------* end
            *      P2
            ********************************************/
            P2 = CPoint(start.x, end.y);
            break;
        } /* orient */

    //GDIFlash(dc, start);
    dc.MoveTo(start);
   // GDIFlash(dc, P2);
    dc.LineTo(P2);
   // GDIFlash(dc, end);
    dc.LineTo(end);
    GDIFLUSH();
    points.Add(P2);
    DebugConnections(dc);
    dc.RestoreDC(n);
    } // ComplexWire::draw

/*******************************************************************************
*                        Element::distance
* Inputs:
*    CPoint p0: A starting point of a line
*    CPoint p1: An ending point of a line
* Result: int
*    The length of the line between p0 and p1
*******************************************************************************/

int Element::distance(CPoint p0, CPoint p1) const
    {
     // Let's hear it for Pythagoras
    double a = (double) (p0.x - p1.x);
    double b = (double) (p0.y - p1.y);
    double len2 = a * a + b * b;
    return (int) sqrt(len2);
    }

/******************************************************************************
*                     Diode::draw
* Inputs:
*     CDC & dc
* Result: void
* 
* Effect:
*     Draws a diode
* GetP:
*    0 - starting point of diode
*    1 - ending point of diode
******************************************************************************/

/* virtual */ void Diode::draw(CDC & dc)
    {
    int n = dc.SaveDC();

    CRect r;
    wnd->GetClientRect(&r);

    FlashPen pen(flashing);
    dc.SelectObject(pen);

    dc.MoveTo(start);
    dc.LineTo(end);
    GDIFLUSH();

    int len = distance(start, end);
    int center = len / 2;
    /********************************************
    *                                       |
    *                   |-->|__|<--cathode__V__
    *                   |\  |  |
    *    P0             | \ |  |                 P1
    *    *--------------|  >|  +-----------------*
    *                   | / |  |            ^
    *                   |/  |__|            |
    *          anode -->|   |<--            height
    ********************************************/
    // Here we do a rotation according to the angle dictated by start and end
    // We re-orient 0,0 to start

    


    //  We add the points *before* we do the transform
    points.Add(start);
    points.Add(end);
    DebugConnections(dc);


    rotate(dc, start, end);

 
    //GDIFlash(dc, CPoint(0,0));
    //GDIFlash(dc, CPoint(distance(start, end), 0));

    static const int anode = (int) (0.05 * r.Height());  // Width of the anode
    static const int cathode = anode / 2;               // Width of the cathode
    static const int height = (int) (0.05 * r.Height()); // Height of the anode/cathode

    FlashBrush br(flashing);
    dc.SelectObject(br);
    

    // Draw the anode   |>
    dc.BeginPath();
    dc.MoveTo(CPoint(center, 0));
    dc.LineTo(CPoint(center - anode, -height));
    dc.LineTo(CPoint(center - anode, height));
    dc.LineTo(CPoint(center, 0));
    dc.EndPath();
    dc.StrokeAndFillPath();
    GDIFLUSH();

    // Now draw the cathode    ||
    dc.BeginPath();
    dc.MoveTo(CPoint(center, -height));
    dc.LineTo(CPoint(center + cathode, -height));
    dc.LineTo(CPoint(center + cathode, height));
    dc.LineTo(CPoint(center, height));
    dc.LineTo(CPoint(center, -height));
    
    dc.EndPath();
    dc.StrokeAndFillPath();
    GDIFLUSH();

    dc.RestoreDC(n);
    }
/********************************************************************************
*                            Capacitor::draw
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws an electrolytic capacitor between start and end, with the + on the
*     start side
* GetP:
*     0 - start of the capacitor
*     1 - end of the capacitor
*     2 - Left top plate
*     3 - Left bottom plate
*     4 - Right bottom plate
********************************************************************************/
void Capacitor::draw(CDC & dc)
    {
    int n = dc.SaveDC();
    CRect r;
    wnd->GetClientRect(&r);

    /***********************************************
    *                start
    *               * 
    *            L0 | +
    *    plate0s *-----* plate0e
    *    plate1s *-----* plate1e
    *               | L1
    *               * L1e
    *               end
    ***********************************************/
    // TODO: If start is below end, the code below will
    // do the wrong thing.  The trick required is to
    // SetGraphicsMode(GM_ADVANCED) and then use
    // SetWorldTransform or ModifyWorldTransform to rotate everything
    // 180 degrees
    // This means we should always be using (0,0) for our start
    // See how Diode::draw does this

    static const int plateSep = 4;
    const int plateWidth = (int) (0.06 * (double) r.Width());
    int mid = (end.y - start.y) / 2;
    CPoint L0e = CPoint(start.x, start.y + mid - plateSep);
    
    FlashPen pen(flashing);
    dc.SelectObject(pen);

    dc.MoveTo(start); // Draw L0
    dc.LineTo(L0e);

    points.RemoveAll();

    /* Now draw the plates*/
    CPoint plate0s = CPoint(start.x - plateWidth, L0e.y);
    CPoint plate0e = CPoint(start.x + plateWidth, L0e.y);
    dc.MoveTo(plate0s);
    dc.LineTo(plate0e);
    GDIFLUSH();

#if 0  // Two parallel plates
    CPoint plate1s = CPoint(start.x - plateWidth, L0e.y + plateSep);
    CPoint plate1e = CPoint(start.x + plateWidth, plate1s.y);
    dc.MoveTo(plate1s);
    dc.LineTo(plate1e);  // TODO: Draw an arc instead of a flat plate
    GDIFLUSH();

    CPoint L1s(L0e.x, plate1e.y);
#else // + plate flat, - plate curved
    int platecurve = 5;
    CPoint plate1tl(start.x - plateWidth, L0e.y + plateSep); // Top left corner boundary of the arc, which is also the top of the arc
    CPoint plate1br(start.x + plateWidth, plate1tl.y + 2 * platecurve);
    /***********************************************************************
    * plate1tl
    *         *----------------------------*----------------------------+
    *         |       ..........''''''''''   ''''''''''..........       |
    * plate1s *'''''''                                           '''''''* plate1e
    *         :                                                         :
    *         |                                                         |
    *         +---------------------------------------------------------* plate1br
    ***********************************************************************/
    CPoint plate1s = CPoint(plate1tl.x, plate1tl.y + platecurve);
    CPoint plate1e = CPoint(plate1br.x, plate1s.y);

    // Note that to get the proper curvature to CDC::Arc, we have to draw the
    // arc in a counterclockwise direction, so we start on the right and end on the left

    dc.Arc(plate1tl.x, plate1tl.y,  // XUL, YYL
           plate1br.x, plate1br.y,  // XBR, YBR
           plate1e.x, plate1e.y,       // XBegin, YBegin
           plate1s.x + 1, plate1s.y + 1);      // XEnd, YEnd

    CPoint L1s(L0e.x, plate1tl.y);
#endif
    
    // Draw the + sign
    static const int plusSize = 4; // must be an even number!
    const int plusOffset = (int)(1.5 * (double)plusSize); // TODO: Need to rotate transform
    /*********************************************
    *                   * start
    *                   |       * plusTop
    *                   |       |
    *               plusLeft *--+--* plusRight   ------
    *                   |       |                    ^ plusOffset
    *                L0e|       * plusBottom         V
    * plate0s *---------*----------*  plate0e    -------
    *                   L1s
    * plate1s *---------*----------*  plate1e
    *                   |
    *                   |
    *                   * L1e = start.x, end.y
    *********************************************/

    CPoint plusLeft(plate0e.x - plusSize, plate0e.y - plusOffset);
    CPoint plusRight(plate0e.x + 1, plusLeft.y); // Add 1 because GDI is exclusive of endpoint

    dc.MoveTo(plusLeft);
    dc.LineTo(plusRight);
    GDIFLUSH();

    CPoint plusBottom(plusLeft.x + plusSize / 2, plusLeft.y + plusSize / 2);
    CPoint plusTop(plusBottom.x, plusBottom.y - plusSize - 1);  // Subtract 1 because GDI is exclusive of endpoint
    dc.MoveTo(plusBottom);
    dc.LineTo(plusTop);
    GDIFLUSH();

    // L1s is set when the plates are drawn for the capacitor
    CPoint L1e(L1s.x, end.y);
    dc.MoveTo(L1s);
    dc.LineTo(L1e);
    GDIFLUSH();

    Connect(dc, L1e, flashing);
    Connect(dc, start, flashing);

    points.Add(start);   // P0
    points.Add(L1e);     // P1
    points.Add(L0e);     // P2
    points.Add(plate1s); // P3
    points.Add(plate1e); // P4

    dc.RestoreDC(n);
    }
/**************************************************************************************
*                               FullWaveRectifier::draw
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*    Draws a 4-diode fullwave bridge centered on the coordinate 'origin' specified
*    when the FullWaveRectifier object was created
* GetP:
*    0 - top of rectifier (input AC voltage)
*    1 - bottom of rectifier (input AC voltage)
*    2 - rightmost pin of rectifier (output DC +V)
*    3 - leftmost pin of rectifier (output DC -V/GND
**************************************************************************************/
void FullWaveRectifier::draw(CDC & dc)
    {
    /****************************************************************
    *         * P0
    *      D4/ \D1
    *       /   \
    *   P3 *  +  * P2     + is the center
    *       \   /
    *      D3\ /D2
    *         * P1
    ****************************************************************/
    points.RemoveAll();
    CPoint P0(center.x, center.y - size);
    points.Add(P0);

    CPoint P1(center.x, center.y + size);
    points.Add(P1);

    CPoint P2(center.x + size, center.y);
    points.Add(P2);

    CPoint P3(center.x - size, center.y);
    points.Add(P3);

//#ifdef _DEBUG
    // This just draws the lines and was used before we added diodes
    dc.MoveTo(P0);
    dc.LineTo(P2);
    dc.LineTo(P1);
    dc.LineTo(P3);
    dc.LineTo(P0);
//#endif

    Diode D1(wnd, P0, P2, flashing, Vf);  //  P0 --->|----P2
    D1.draw(dc);

    Diode D2(wnd, P1, P2, flashing, Vf);  //  P1 --->|----P2
    D2.draw(dc);

    Diode D3(wnd, P3, P0, flashing, Vf);  //  P3 --->|----P0
    D3.draw(dc);

    Diode D4(wnd, P3, P1, flashing, Vf);  //  P3 --->|----P1
    D4.draw(dc);

    Connect(dc, P0, flashing);
    Connect(dc, P1, flashing);
    Connect(dc, P2, flashing);
    Connect(dc, P3, flashing);
    }

/************************************************************************************
*                           Regulator::draw
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws a 3-pin regulator connecting start and end, whose ground terminal
*     connects to ref.y (ref.x is ignored)
* GetP:
*     0 - start of regulator input line
*     1 - end of regulator output line
*     2 - connection point of GND of regulator
*     3 - right edge of regulator box
*     4 - left edge of regulator box
*************************************************************************************/
void Regulator::draw(CDC & dc)
    {
    int n = dc.SaveDC();

    CRect r;
    wnd->GetClientRect(&r);

    /********************************************
    * Draws a simple linear regulator (e.g., 7805)
    * Takes 3 parameters: start, end, and ref
    *                         +-------+
    * start *-----------------+       +------------*end
    *                         +---+---+
    *                             |
    *                             |
    *                             *
    *                             ref
    ********************************************/
    points.RemoveAll();

    int len = distance(start, end);
    int box0 = len / 3;
    int box1 = (int) (0.2 * (double) r.Width());
    int height = (int) (0.15 * (double) r.Width());

    FlashPen pen(flashing);
    dc.SelectObject(pen);

    // We want the box to ideally be box1 width, but if box1 > len / 2,
    // we use box0 instead
    int box = 0;
    if(box1 > len / 2)
        box = box0;
    else
        box = box1;
    int mid = len / 2;
    /**********************************************************
    *          R0
    *          *------------------+
    *          |                  |
    *          +------------------+ R0.x + box, R0.y + height
    **********************************************************/
    // Now compute the space on each side
    int deltaX = (len - box) / 2;

    /**********************************************************
    *          |<------ box ----->|
    *          R0                 |
    *start     *------------------+   L2  end
    *  *-------*L1e=P4 L3s  P3=L2s*--------*P1
    * P0  L1   +---------*--------+ R0.x + box, R0.y + height
    *  |       |         |L3
    *  |deltaX |         |
    *  |<----->|       P2* L3e
    **********************************************************/


    CPoint L1e(start.x + deltaX, start.y);
    dc.MoveTo(start);
    dc.LineTo(L1e);
    GDIFLUSH();

    CPoint R0(L1e.x, L1e.y - height / 2);

    CPoint L2s(end.x - deltaX, end.y);
    dc.MoveTo(L2s);
    dc.LineTo(end);
    GDIFLUSH();

    CPoint L3s(L1e.x + box / 2, R0.y + height);
    CPoint L3e(L3s.x, ref.y);

    dc.MoveTo(L3s);
    dc.LineTo(L3e);
    Connect(dc, L3e, flashing);
    GDIFLUSH();
    
    FlashBrush br(flashing);
    dc.SelectObject(br);

    CRect regBox(R0.x, R0.y, R0.x + box + 1, R0.y + height + 1);
    dc.Rectangle(regBox.left, regBox.top, regBox.right, regBox.bottom);
    GDIFLUSH();

    if(V > 0.0)
        { /* draw voltage */
        Value::SelectFont(dc, Value::DefaultFontHeight);
        dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);

        dc.SetBkMode(TRANSPARENT);
        dc.DrawText(Volts(V, CString(_T("V"))).toString(), &regBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } /* draw voltage */

    points.RemoveAll();
    points.Add(start);  // P0
    points.Add(end);    // P1
    points.Add(L3e);    // P2
    points.Add(L2s);    // P3
    points.Add(L1e);    // P4

    dc.RestoreDC(n);
    } // Regulator::draw

/*********************************************************************************
*                            Resistor::draw
* Inputs:
*    CDC & dc:
* Result: void
* 
* Effect:
*     Draws a resistor
* GetP:
*     0 - start
*     1 - end
*     2 - center left/top
*     3 - center right/bottom
*********************************************************************************/
void Resistor::draw(CDC & dc)
    {
    int n = dc.SaveDC();
    CRect r;
    wnd->GetClientRect(&r);

    FlashPen pen(flashing);
    dc.SelectObject(pen);

     /*******************************************************
     *        |<- wlen ->|<----- element ------>|<- wlen ->|
     *        |                    * P2       __|__________|________________ __
     *      P0|           /\    /\    /\    /\  |0 W1     1|P1              A
     *  start *----------   \  /  \  /  \  /  \  ----------* end            | height
     *        ^0 W0      1   \/    \/    \/    \/__________^________________V__
     *        |                     * P3  |     |          |
     *        |                           |<dx->|          |
     *        |<------------------ len ------------------->|
     ********************************************************/
    int nPeaks = 8;
    int height = (int) (0.05 * (double) r.Height());
    int dx = height / 2; // cheat for now
    int len = distance(start, end);
    while(dx * nPeaks > len)
        nPeaks--;
    if(nPeaks == 0)
        {
        dc.RestoreDC(n);
        dc.MoveTo(start);
        dc.LineTo(end);
        return;  // bad data, do nothing
        }
    
    XFORM M;
    rotate(dc, start, end, &M);
    // Compute the line length at the ends
    int element = (dx * nPeaks); // the width of the resistive element
    int wlen = (len - element) / 2;
    SimpleWire * wire0 = new SimpleWire(wnd, CPoint(0, 0), wlen, flashing);
    wire0->draw(dc);
    // Now we draw the peaks
    CPoint pt0(wire0->GetP(1));
    CPoint pt1(0, 0); // filled in in loop

    for(int i = 0; i < nPeaks; ++i)
        { /* draw each peak */ 
        pt1 = CPoint(wire0->GetP(1).x + (i + 1) * dx, ((i & 1) ? height : -height) / 2);
        dc.MoveTo(pt0);
        dc.LineTo(pt1);
        pt0 = pt1;
        } /* draw each peak */

    // Now draw wire1
    CPoint w1start(pt0.x + (3 * dx)/4, 0);
    CPoint w1end(pt0.x + wlen, 0);
    SimpleWire * wire1 = new SimpleWire(wnd, w1start, w1end, flashing);
    wire1->draw(dc);

    // Finally, connected the last point (pt0) of the resistor to the end of wire1 
    // Note that wire1 is drawn _upwards_ or _leftwards_ so that it ends near the unconnected
    // resistor point
    dc.MoveTo(pt0);
    dc.LineTo(wire1->GetP(0));

    int center = wlen + (element / 2);

    // TODO: These points are in transformed coordinates and they have to be
    // rendered in logical coordinates

    CPoint P2(center, - (height / 2));
    CPoint P3(center,   (height / 2));
    //DebugConnection(dc, P2);
    //DebugConnection(dc, P3);

    CPoint P2t = Transform(P2, M);
    CPoint P3t = Transform(P3, M);
#ifdef _DEBUG
    CPoint debug = Transform(CPoint(0,0), M);
#endif 
    dc.RestoreDC(n);

#ifdef _DEBUG
    //GDIFlash(dc, debug);
#endif

    points.Add(start);  // P0
    points.Add(end);    // P1
    points.Add(P2t);     // P2
    points.Add(P3t);     // P3

    delete wire0;
    delete wire1;
    } // Resistor::draw

void SchmittTrigger::draw(CDC& dc)
{
    int n = dc.SaveDC();
    CRect r;
    wnd->GetClientRect(&r);

    FlashPen pen(flashing);
    dc.SelectObject(pen);
    /********************************************************************
    *   [0]start(x0,y0)                    [1]end(x1,y1)
    *      |              x0       x1 x2        |
    *      |              |    \   |  |         |
    *      |              |     \  |  |         |
    *      |              |      \ |  |         |
    *      ____________[2]|  ____ \|/\|__________
    *                     |_/_/   / \/
    *                     |      /
    *                     |     /
    *                     |    /
    * 
    * Note that for a noninverting Schmitt trigger x1 == x2
    *********************************************************************/

    //  We add the points *before* we do the transform
    points.Add(start);     // [0]
    points.Add(end);       // [1]
    DebugConnections(dc);

    static const int height = (int)(0.15 * r.Height()); // Height
    CPoint LeftCenter(start.x + (end.x - start.x) / 3, start.y);
    CPoint RightCenter(end.x - (end.x - start.x) / 3, start.y);

    points.Add(LeftCenter); // [2]
    dc.MoveTo(start);
    // .

    dc.LineTo(LeftCenter);
    // .___________

    dc.LineTo(LeftCenter.x, LeftCenter.y + height);

    //  .___________
    //              |
    //              |
    dc.LineTo(RightCenter.x, RightCenter.y);
       
    //   .___________
    //               | /
    //               |/

    dc.LineTo(LeftCenter.x, LeftCenter.y - height);
    //                \
    //   .___________  \
    //               | /
    //               |/

    dc.LineTo(LeftCenter);
    //               |\
    //   .___________| \
    //               | /
    //               |/
    dc.MoveTo(RightCenter);
    //               |\
    //   .___________| \.
    //               | /
    //               |/

    if(invert)
       { /* draw inversion circle */
        const int circleSize = 6; // should be an even number to make it look symmetric
        CRect r(RightCenter.x,                  // left
                RightCenter.y - circleSize / 2, // top
                RightCenter.x + circleSize,     // right
                RightCenter.y + circleSize / 2);// bottom
        dc.Ellipse(&r);
        dc.MoveTo(RightCenter.x + circleSize, RightCenter.y);
        //               |\
        //   .___________| \/\.
        //               | /\/
        //               |/
       } /* draw inversion circle */

    dc.LineTo(end);
    //   [0]      [2]|\            [1]
    //   .__________.| \??.________.
    //               | /??
    //               |/

    // Now, draw the hysteresis symbol
    /**********************************
    *          ________
    * _       /   /
    *    ____/___/
    * ^  ^  ^ ^ ^  ^   ^
    * |  |  | | |  |   |
    * |  h0 h1h2h3 h4 h5
    * LeftCenter
    ***********************************/
    int h = (int)(0.03 * r.Height()); // Hysteresis feature height/width

    CPoint h0(LeftCenter.x + h, LeftCenter.y + h);
    CPoint h1(h0.x + h, h0.y);
    CPoint h2(h1.x + h, LeftCenter.y - h);
    CPoint h3(h2.x + h, h0.y);
    CPoint h4(h3.x + h, h2.y);
    CPoint h5(h4.x + h, h2.y);

    dc.MoveTo(h0);
    dc.LineTo(h3);
    dc.LineTo(h4);
    dc.LineTo(h5);
    dc.LineTo(h2);
    dc.LineTo(h1);

    dc.RestoreDC(n);
} // SWchmittTrigger::draw