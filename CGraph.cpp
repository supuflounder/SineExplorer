/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "CGraph.h"
#include "Value.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Colors.h"
#include "res\\resource.h"

#include "map.h"

#include "GDIFLUSH.h"


BEGIN_MESSAGE_MAP(CGraph, CStatic)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CGraph::CGraph()
    {
    CStatic();
    showSine = true;
    showStep = false;
    nSteps = 4;  // random number
    dtheta = 5;
    debug = false;
    flashing = false;
    invertPWM = false;
    showADCPercent = true;
    showADCDec = true;
    showADCHex = true;
    showADCVolts = true;
    showAxis = true;
    showBase = false;
    showSum = false;
    showDiff = false;
    showRegulated = false;
    sineDashed = false;
    Phase = 0;
    ZeroPos = IDC_ZERO_CENTER;
    Rectification = IDC_RECTIFY_NONE;
    Hz = 60; // TODO: localization
    filter = NULL;
    InputVoltage = 6.3; 
    DiodeDrop = 0.7; 
    RegulatorDrop = 0.7;
    OutputVoltage = 5.0;
    tracking = false;
    MousePoint = CPoint(0, 0);  // mouse point in screen coordinates
    }
/*************************************************************************************
*                       CGraph::Plot
* Inputs:
*     CDC & dc: The DC to draw to
*     CArray<CPoint> & pts: Array of points to plot
*     DWORD style: PS_SOLID or other PS_ selection
*     DWORD width: Pen width; must be 1 unless PS_SOLID is selected
*     COLORREF color: Color of line
* Result: void
* Effect:
*     Draws via PolyLine the line indicated by the sequence of points in pts, in the
*     styles, width, and color specified
* Notes:
*     
**************************************************************************************/

void CGraph::Plot(CDC & dc, CArray<CPoint> & pts, DWORD style, DWORD width, COLORREF color)
{
    int n = dc.SaveDC();
        CPen pen;
        LOGPEN attr; // pen attributes

        DWORD xattrs = (style != PS_SOLID) ? 0 : PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND;
        
        attr.lopnStyle = style | xattrs;
        attr.lopnWidth = CPoint(width, 0);
        attr.lopnColor = color;

        pen.CreatePenIndirect(&attr);
        dc.SelectObject(pen);

        if(style != PS_SOLID)
            dc.SetBkMode(TRANSPARENT);
        dc.Polyline(&pts.ElementAt(0), (int)pts.GetCount());
        GDIFLUSH();
    dc.RestoreDC(n);

}
/************************************************************************************
*                       CGraph::Note
* Inputs:
*     CDC & dc: The DC to use
*     const CString & s: String to put out
*     int x: X-position
*     int y: Y-position
*     DWORD align: TA_ symbol for text alignment
* Result: void
* Effect: Writes the note using the current font and text color
************************************************************************************/
void CGraph::Note(CDC& dc, const CString& s, int x, int y, DWORD align)
{
    int n = dc.SaveDC();
    dc.SetTextAlign(align);
    dc.SetBkMode(TRANSPARENT);
    dc.TextOut(x, y, s);
    dc.RestoreDC(n);
}
/***********************************************************************************
*            CGraph::ShowSteps
* Inputs:
*     CDC & dc
*     CArray<Point> & pts: The array of points
*     int height:
*     double max_y:
*     double min_y:
* Result: void
* 
* Effect:
*     Calculates and plots the ADC step function
************************************************************************************/
void CGraph::ShowSteps(CDC& dc, CArray<CPoint> & pts, int height, double max_y, double min_y)
{
    CRect r;
    GetClientRect(&r);
    int width = r.Width() - 1;

    double out_min_y = 0.0;
    double out_max_y = 100.0;

    switch (ZeroPos)
    { /* ZeroPos */
    case IDC_ZERO_CENTER:
        out_min_y = -100.0;  // -100%
        out_max_y = 100.0;   // +100%
        break;
    case IDC_ZERO_BOTTOM:
        out_min_y = 0;
        out_max_y = 100.0;
        break;
    } /* ZeroPos */

    int n = dc.SaveDC();
    CFont coordFont;
    LOGFONT lf;

    if (debug)
    { /* create font */
        CFont* font = dc.GetCurrentFont();

        font->GetLogFont(&lf);

        lf.lfHeight /= 3;
        coordFont.CreateFontIndirect(&lf);
    } /* create font */

    CPen stepPen;
    stepPen.CreatePen(PS_SOLID, 2, Colors::Step);
    dc.SelectObject(stepPen);


    CPoint hzsp;  // The Horizontal start point
    CPoint hzep;  // The Horizontal end point

    hzsp = pts[0];

    dc.MoveTo(hzsp);

    if (debug)
        TRACE("*************************************************\n");

    /*
    * nSteps is the number of steps to look ahead.  It is based on
    * the resolution of sampling vs. the resolution of computation.
    *
    * We pick two points, i and i+nSteps
    * We compute the interval
    *        { i == 0  0
    *   x0 = {
    *        { i > 0 (pts[i].x - (pts[i].x - pts[i-nSteps].x) / 2)
    *
    *        { i == 0 (pts[i+nSteps].x + (pts[i+ 2 * nSteps].x - pts[i + nSteps].x) / 2)
    *   x1 = {
    *        { i > 0 && i + 2 * nSteps < pts.GetCount()
    *        {         (pts[i].x + (pts[i + 2 * nSteps].x - pts[i + nSteps].x / 2)
    *                 ___.___
    *                |   ^
    *         ___.___|   |
    *        |   ^       pts[i +  nSteps]
    *        |   |
    * ___.___|   pts[i]
    * ^  ^   ^
    * |  |   |
    * |  |   hzep
    * |  pts[i - nSteps]
    * hzsp
    *
    * We will add a "live box" for the mouse to be able to give a report of the value
    * The live box will run from
    *
    * hzsp, pts[i - nSteps].y + boxy
    * +-----------+
    * |***********|
    * +-----------+
    *             hzep, pts[i - nSteps].y - boxy
    */
    Boxes.RemoveAll();  // clear out box array
    static const int boxySlop = 1;  // amount of slop around line

    for (int i = 0; i < pts.GetCount(); i += nSteps)
    { /* plot points */
     /*
     * Compute the horizontal endpoint
     */
        CPoint hzlep = hzep; // save last endpoint

        hzsp.x = i == 0 ? 0
            : pts[i].x - (pts[i].x - pts[i - nSteps].x) / 2;
        hzep.x = i + nSteps >= pts.GetCount() ? width
            : (pts[i].x + (pts[i + nSteps].x - pts[i].x) / 2);
        hzsp.y = hzep.y = pts[i].y;

        CRect r(hzsp.x, hzsp.y - boxySlop, hzep.x, hzep.y + boxySlop);
        

        double val = map((double)hzep.y, max_y, min_y, out_min_y, out_max_y);
        Boxes.Add(BoxInfo(r, val));

        if (debug)
        {
            CPoint cp;
            cp = dc.GetCurrentPosition();
            TRACE("cp=<%d,%d>\n", cp.x, cp.y);
            TRACE("hzsp = <%d, %d>\n", hzsp.x, hzsp.y);
            TRACE("hzep = <%d, %d>\n", hzep.x, hzep.y);
        }
        /* draw the previous vertical line */
        if (i > 0)
            dc.LineTo(hzsp);

        /* draw the horizontal line */
        dc.LineTo(hzep);
        GDIFLUSH();
        if (debug)
            TRACE("hzsp <%d, %d>\n", hzsp.x, hzsp.y);
        /*
        * At this point, we have drawn a horizontal line
        * draw the vertical line
        */
        CPoint vtep; // vertical transition end point
        vtep = i + nSteps >= pts.GetCount() ? CPoint(hzep.x, hzep.y)
            : CPoint(hzep.x, pts[i + nSteps].y);

        if (debug)
        { /* debug */
            TRACE("vtep=<%d, %d>\n", vtep.x, vtep.y);
            int n1 = dc.SaveDC();

            dc.SelectObject(coordFont);
            CPen gridPen;
            gridPen.CreatePen(PS_DASHDOT, 1, RGB(255, 153, 0));
            dc.SelectObject(gridPen);
            dc.MoveTo(vtep.x, -height);
            dc.LineTo(vtep.x, height);
            CString s;
            s.Format(_T("%d"), vtep.x);

            static const int VTXMARGIN = 2;
            static const int VTYMARGIN = 2;
            static const int IYMARGIN = -(VTYMARGIN * 3 + lf.lfHeight);

            dc.TextOut(vtep.x + VTXMARGIN, VTYMARGIN, s);
            s.Format(_T("%d"), i);
            dc.TextOut(hzsp.x + VTXMARGIN, IYMARGIN, s);
            s.Format(_T("%d"), hzsp.y);
            dc.TextOutW(hzsp.x + VTXMARGIN, hzsp.y + VTYMARGIN, s);
            if (i == 0)
            { /* display size */
                s.Format(_T("%d×%d"), r.Width(), r.Height());
                dc.TextOut(VTXMARGIN, -height, s);
            } /* display size */
            GDIFLUSH();
            dc.RestoreDC(n1);
        } /* debug */

        if (showSamples)
        { /* show samples */
            int n = dc.SaveDC();
            CPen PointPen;
            PointPen.CreatePen(PS_DASH, 1, Colors::Samples);
            dc.SelectObject(PointPen);
            dc.MoveTo(pts[i].x, -height);
            dc.LineTo(pts[i].x, height);
            GDIFLUSH();
            dc.RestoreDC(n);
        } /* show samples */
    } /* plot points */

    // Pass in the display options
    stats.SetShowPercent(showADCPercent);
    stats.SetShowDec(showADCDec);
    stats.SetShowHex(showADCHex);
    stats.SetShowVolts(showADCVolts);

    // Set the parameters we will need
    stats.SetADCAref(ADCAref);
    stats.SetADCRange(ADCRange);
    stats.SetADCValue(ADCValue);
    stats.SetWindow(this);
    BoxInfo* b = NULL;
    if(tracking)
        b = GetBoxInfo(MousePoint);

    if (b != NULL)
    {
        stats.SetBoxInfo(b);
        double value = b->GetValue();
        stats.SetADCValue(value);
        stats.SetValid(true);
    }
    else
        stats.SetValid(false);

    stats.SetDebug(showMarks);
    stats.Draw(dc);
    dc.RestoreDC(n);
}
/*************************************************************************************
*                       CGraph::OnPaint
* Result: void
* 
* Effect:
*    Computes the points of the graph and then plots them
**************************************************************************************/

void CGraph::OnPaint()
    {
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages



    /* get the window parameters */
    CRect r;
    GetClientRect(&r);

    /*******************************************
    * I got tired of getting stuff drawn outside
    * the boundaries of the graph because the 
    * DC has the limits of the parent window,
    * which is simply silly.  So I set a clipping
    * region so that I only color between the lines
    *******************************************/
    CRgn clip;
    clip.CreateRectRgnIndirect(&r);
    dc.SelectClipRgn(&clip, RGN_AND);

    /**********************************************************
    * Some values
    * org is the 0,0 point for our display.  Normally, (0,0)
    * is the top left corner of the client area, but we are
    * going to relocate it lower, depending on the nature
    * of the display
    **********************************************************/

    int height;
    CPoint org(0,0);

    if(!showClassD)
        { /* not Class D */
        switch(ZeroPos)
            { /* ZeroPos*/
            default:
            case IDC_ZERO_CENTER:
                org.y = r.Height() / 2;
                break;
            case IDC_ZERO_BOTTOM:
                org.y = r.Height() - 1;
                break;
            } /* ZeroPos */
        } /* not Class D */
    else
        { /* class D */
        org.y = r.Height() / 4;
        } /* class D */

    height = org.y - 1;
    int width = r.Width() - 1;

    /*********************************************
    * Relocate (0,0) based on the above computations
    *********************************************/
    dc.SetViewportOrg(org);
    dc.SetViewportExt(r.Width(), r.Height());

    /* Draw the background */
    dc.FillSolidRect(0, -(height+1), r.Width(), r.Height()+1, (flashing ? Colors::Flash : Colors::Background));
    GDIFLUSH();

    /*********************************************************
    * Draw the zero axis
    *********************************************************/
    if(showAxis)
        { /* show axis */
        int n = dc.SaveDC();
        CPen pen;
        pen.CreatePen(PS_SOLID, 1, flashing ? Colors::FlashZero : Colors::Zero);
        dc.SelectObject(pen);
        dc.MoveTo(org.x, 0);
        dc.LineTo(org.x + width, 0);
        GDIFLUSH();
        dc.RestoreDC(n);
        } /* show axis */

    /********************************************************************
    * These arrays represent all the curves that can be plotted
    * We will compute the values for all the points, and then use
    * PolyLine to plot each set
    *********************************************************************/
    CArray<CPoint> pts;          // The basic sine wave, including phase shifting
    CArray<CPoint> Dpts;         // The points of the Class D Amplifier curve
    CArray<CPoint> BasePts;      // The base points without phase shifting
    CArray<CPoint> SumPts;       // The sum of the base curve and the phase-shifted curve
    CArray<CPoint> DiffPts;      // The difference between the base curve and the phase-shifted curve
    CArray<CPoint> FilterPts;    // The set of points representing the curve with the filter capacitor in place
    CArray<CPoint> RegulatedPts; // The set of points representing the output regulated voltage
    CArray<CPoint> SchmittPts;   // The set of points of the Schmitt trigger
    CArray<CPoint> PWMPts;       // The set of points for the PWM waveform

    double min_y = INFINITY;
    double max_y = -INFINITY;
    if(filter != NULL)
        filter->Reset();          // Set the RC filter time to T=0

#ifdef _DEBUG
    double lastV = -1.0; // Debugging hack, not needed for any "real" computation, just for debugging
#endif

    int maxpts = 0;
#ifdef _DEBUG
    if(debug)
        TRACE("\n\"");
#endif
    /******************************************************
    * 1. Compute the array of points (pts) for the sine wave
    * 2. Compute the array of points (Dpts) for the class D curve
    *
    * The D-curve is 50% at 0, 0% at full negative, and
    * 100% at full positive
    * The D-curve points are for a polyline so each point is
    * one point on the line
    *    X
    *   X X
    *  X    X
    * X------X------X--
    *         X    X
    *           X X
    *            X
    *
    * (Expanded in the horizontal scale):
    *
    *                               X
    *                     X         :         X
    *                     :         :         :
    *           X         :         :         :          X
    * X---------:---------:---------:---------:---------:---------X---------+---------+---------+
    *                                                                       X         :         :
    *                                                                                 :         :
    *                                                                                 X         :
    *                                                                                           X
    * +----+    +------+  +-------+ +---------+-------+ +------+  +----+    +--+      +-+
    * |    |    |      |  |       | |                 | |      |  |    |    |  |      | |
    * |    |    |      |  |       | |                 | |      |  |    |    |  |      | |
    * +    +----+      +--+       +-+                 +-+      +--+    +----+  +------+ +-------+
    * X0--------X1        X0--------X1        X0--------X1        X0--------X1        X0--------X1
    *           X0--------X1        X0--------X1        X0--------X1        X0--------X1
    * x----->
    ******************************************************/

    CArray<CPoint> Tops;  // for drawing fiducial lines.  In device coordinates (don't ask)

    /******************************************
    *TempPhase represents phase shift
    * To simplify life showing the Class D
    * amplifier output, we ignore phase shift
    * of the input curve, so TempPhase is set
    * to zero where we want to ignore it
    *******************************************/
    int tempPhase = Phase;
    if(showClassD)
        tempPhase = 0;  // for class D, we ignore the phase setting

    /**************************************************************  
    * This is a cheat; we should normalize it to the maximum value
    * However, it is easier if we just assume we will need twice as
    * much space for the sum or difference
    ****************************************************************/
    if(!showClassD && (showSum || showDiff))
        height /= 2;

  
    /********************
    * If we are showing the filtered waveform, we need to compute DT here
    * DT is used to compute the negative exponential decay for the capacitor
    * 
    * T.cycle = 1/Hz          1/60 = 0.016666 s (16.6ms)
    * T.step = T.cycle / (360 / dtheta)   16.6ms / (360 / 5) = 16.6 / 72 = 0.000231 (231us)
    ********************/
    double DT = (1.0 / (double) Hz) / (360 / max(dtheta, 1));

    /**********************************************************************
    * Here we compute the points for the multiple waveforms at each
    * angle.  i is the angle, in degrees
    ***********************************************************************/
    int lastYSchmitt = 0; // used to compute Schmitt trigger wave slope
    int slope = 0; // last slope set to 0

    for(int32_t i = 0; i <= nWaves * 360; i += max(dtheta, 1))
        { /* all points */

        double rad = (((double) (i + Phase)) * M_PI) / 180.0;  // The angle in radians
        double rad0 = (((double) i) * M_PI / 180.0);           // The base waveform, without phase shift
        double s0 = sin(rad0);                                 // The y-value sine for the base waveform
        double s = sin(rad);                                   // The y-value for the actual waveform, including phase shift


        /* for a zero line at bottom, the values run from 0..2, not -1..1*/
        if(!showClassD)
            { /* not class D */
            /* We do not shift the zero position to the bottom if we are showing class D */
            switch(ZeroPos)
                { /* ZeroPos*/
                case IDC_ZERO_CENTER:
                    break;
                case IDC_ZERO_BOTTOM:
                    s += 1.0;
                    s0 += 1.0;
                    s /= 2;
                    s0 /= 2;
                    break;
                } /* ZeroPos */
            } /* not class D */

        /**********************************
        * x is the x-coordinate for the wave
        * y is the actual coordinate in the graphic area for the phase-shifted wave
        * y0 is the actual coordinate in the graphic area for the non-phase-shifted wave
        ***********************************/
        int x = ((int) ((double) width * ((double) i / (nWaves * 360.0))));
        int y = (int) -(((double) height) * s);
        int y0 = (int) -(((double) height) * s0);
        int ySchmitt = 0;     // Adjust this later if Schmitt output needed


        /********************************************
        * Comput the PWM points
        *********************************************/
        int PWMOn = invertPWM ? 0 : -height;
        int PWMOff = invertPWM ? -height : 0;

        int yPWM;

        int PWMValue = i % 360;
        // PWMvalue is in the range 0..360
        // PWMThreshold is the point at which it changes
        int PWMThreshold = (int)((360.0 * (double)PWMWidth / 100.0) + 0.5);
        if (PWMValue < PWMThreshold)
           {
            
            yPWM = PWMOn;
           }
        else
           { 
            yPWM = PWMOff;
           }
        if (PWMPts.GetSize() > 0)
           { /* adjust if necessary */
            if (PWMPts.GetAt(PWMPts.GetSize() - 1).y != yPWM)
                PWMPts.GetAt(PWMPts.GetSize() - 1).x = x;
           } /* adjust if necessary */

        PWMPts.Add(CPoint(x, yPWM));


        /*****************************************
        * We want to ignore the rectification selection
        * if we are showing the Class D curve
        * Because rectification makes no sense for
        * a class D input signal
        ******************************************/
        int tempRectification = Rectification;
        if(showClassD)
            tempRectification = IDC_RECTIFY_NONE;  // ignore rectification for class D display

        if (SchmittData.getEnable())
           { /* Schmitt or PWM */
            tempRectification = IDC_RECTIFY_HALF;
           } /* Schmitt */
        /************************************************************
        * Now we compute a new y-value based on the rectification
        *************************************************************/
        switch(tempRectification)
            { /* rectification */
            case IDC_RECTIFY_NONE:
                // y already set
                // y0 already set
                break;
            case IDC_RECTIFY_HALF:
                if(s < 0) // Eliminate bottom half of sine wave, clamp at zero
                    {
                    y = 0;
                    y0 = 0;
                    }

                if (SchmittData.getEnable())
                   {
                    double on = SchmittData.getOn();
                    double off = SchmittData.getOff();
                    int onLimit = (int)(on * (double)height);
                    int offLimit = (int)(off * (double)height);

                    /********************************************       
                    * The thing to watch out for here is that the
                    * curve is backwards, that is, as the curve goes
                    * up, "y" goes negative, so a "negative" slope
                    * on the line represents a rising voltage
                    * and a "positive" slope on the line represents
                    * a falling voltage.  To keep the 'slope' variable
                    * intuitively obvious, we change the sign so
                    * a rising voltage has a positive 'slope' value
                    ********************************************/

                    int tslope = -(y - lastYSchmitt);
                    if (tslope != 0 || y == 0)
                        slope = tslope;
                    lastYSchmitt = y;

                    int onValue = -height;
                    int offValue = 0;

                    if (SchmittData.getInvert())
                    {
                        onValue = 0;
                        offValue = -height;
                    }

                    if (-y < onLimit && slope >= 0)
                       { /* it is off */
                        ySchmitt = offValue;
                       } /* it is off */
                    else
                    if (-y >= onLimit && slope >= 0)
                       { /* it should be on */
                        // We want the transition to be vertical, independent of the dtheta resolution
                        // So if the last Schmitt point we added was at 0, we add a dummy point at 0 for the
                        // current X-position
                        if (SchmittPts.GetSize() > 0)
                           { /* check for transition */
                            CPoint pt = SchmittPts[SchmittPts.GetSize() - 1];
                            if (pt.y != onValue)
                                SchmittPts.Add(CPoint(x, offValue));
                           } /* check for transition */
                        ySchmitt = onValue;
                       } /* it should be on */
                    else
                    if (-y > offLimit && slope < 0)
                       {
                        ySchmitt = onValue;
                       }
                    else
                    if (-y <= offLimit && slope < 0)
                       { /* it should be off */
                        if (SchmittPts.GetSize() > 0)
                           { /* check for transition */
                            CPoint pt = SchmittPts[SchmittPts.GetSize() - 1];
                            if (pt.y != offValue)
                                SchmittPts.Add(CPoint(x, onValue));
                           } /* check for transition */
                        ySchmitt = offValue;
                       } /* it should be off */
                   
                    
                   }

                break;
            case IDC_RECTIFY_FULL:
                if(s < 0) // Invert the bottom half of the sine wave
                    {
                    y = -y;
                    y0 = -y0;
                    }
            } /* rectification */

        
        /*******************************************************
        * Now compute the y-coordinate for the sum and difference
        *******************************************************/
        int ysum = y + y0;
        int ydif = y - y0;

        CPoint pt = CPoint(x, y);
        min_y = min(y, min_y);
        max_y = max(y, max_y);
        pts.Add(pt);   // Add the point to the sine wave array

        Dpts.Add(CPoint(x, (int) ((s + 1) * 1000.0)));  // Class D point plot
        BasePts.Add(CPoint(x, y0)); // Base sine wave curve
        SumPts.Add(CPoint(x, ysum)); // Sum wave curve
        DiffPts.Add(CPoint(x, ydif)); // Difference wave curve
        SchmittPts.Add(CPoint(x, ySchmitt)); // Schmitt trigger curve


        /* Now compute the regulated output */
        /*************************************************************
        * We need to know the absolute voltage, not just the sine scale
        * The absolute voltage is 0-peak where peak = √2 * InputVoltage
        * We then deduct 0.7V for the diode that is doing the rectification
        * and another (user-specified) drop for the regulator
        * These are stored as DiodeDrop and RegulatorDrop
        * We then convert this back to a sine wave plot
        **************************************************************/

        if(filter != NULL && showFiltered && Rectification != IDC_RECTIFY_NONE)
            { /* compute filtered */
            filter->SetV((double) -y, DT); // Compute the negative exponential decay
            double V = filter->GetV();     // Read back the computed new value

            // The "voltage" is really the y-value, so we have to convert it back
#ifdef _DEBUG
            { /* debug hack */ 
             
            if(V == lastV)
                {
                TRACE(_T("V = %4.2f\n"), V);  // Report if no change in delta-V
                }
            lastV = V;
            } /* debug hack */
#endif

            FilterPts.Add(CPoint(x, (int) -V));

            double VDCpeak = (InputVoltage * sqrt(2)) - DiodeDrop;
            
            /**********************
            * We can invert the values y and V, which are functions of the height,
            * to the sine value, simply by dividing by the height
            * 
            **********************/
            double SinValueV = ((double) -V) / ((double)height); // SinValueV is between 0 and 1.0
            double volts = -SinValueV * VDCpeak;  // This is the actual voltage for 100%
            double Vreg = volts - RegulatorDrop;  // The input voltage to the regulator must be RegulatorDrop 
                                                  // volts higher than the desired output, that is, at least 5.7V for 5.0V
                                                  // output with .7V RegulatorDrop
            double Vin = min(Vreg, volts);
            Vin = max(Vin, 0);                    // If voltage has not yet reached RegulatorDrop, the output will be 0V
                                                  // It cannot go negative
            double Vout = min(OutputVoltage, Vin);
            int Ry = (int)((Vout / VDCpeak) * (double)height);
            Ry = max(0, Ry);
            RegulatedPts.Add(CPoint(x, -Ry));     // Show the output regulator voltage
            } /* compute filtered */

        // Because we will not have the viewport org the same, we want
        // the terminal position in device points
        dc.LPtoDP(&pt);
        Tops.Add(pt);

        if(debug)
        {
         int j = (int) pts.GetCount() - 1;
         TRACE(_T("[%d] %d, %d\n"), j, pts[j].x, pts[j].y);
        }

        maxpts++;
        } /* all points */

    /********************************************
    * Debugging output from the computation
    * for the class-D waveform
    ********************************************/
    if(debug && showClassD)
        { /* show Dpts */
        /*
        * Note that Dpts[i].y represents the percentage of the cycle that the 
        * waveform is on, expressed as 0 = 0%, 1000 = 50%, 2000 = 100%
        */ 

        TRACE(_T("******* Class D points\n"));
        for(int i = 0; i < Dpts.GetSize(); ++i)
            {
            TRACE(_T("[%d] y=%d <%d, %d>\n"), i, pts[i].y, Dpts[i].x, Dpts[i].y);
             }
        } /* show Dpts */

    /*********************************************
    * Plot the sine wave
    * This includes the phase shift
    *********************************************/

    if(showSine)
        {
         if (sineDashed)
            Plot(dc, pts, PS_DASH, 1, Colors::Analog);
         else
            Plot(dc, pts, PS_SOLID, 2, Colors::Analog);
        }

    /**********************************************
    * Plot the "base" sine wave
    * This does not include the phase shift
    ***********************************************/
    if(!showClassD && showBase)
        { /* show Class D */
         Plot(dc, BasePts, PS_DOT, 1, Colors::Base);
        } /* show Class D */

    /* Plot the filtered wave */
    if(showFiltered && Rectification != IDC_RECTIFY_NONE)
        { /* show filtered */
        Plot(dc, FilterPts, (showSine ? PS_DOT : PS_SOLID),
                           (showSine ? 1 : 2),
                           Colors::Filtered);
        } /* show filtered */

    /* Plot the regulated wave */
    /************************************************
    * First, we need to see if the output is a steady
    * voltage, i.e., our capacitor is large enough
    * that the voltage does not go below OutputVoltage
    * 
    *************************************************/
    BadRegulation = false; // Assume our regulation is perfect until proven otherwise

    /* we first need to get past the startup transient */
    int i;
    int lastY;
    bool first = true;
    int count = 0;
    static const int STABLE_THRESHOLD = 5; // We are stable if we have this many points
    CPoint BadPoint(-1, -1); // no bad points
    CRect BadRect;
    BadRect.SetRectEmpty();

    for(i = 0; i < RegulatedPts.GetCount(); ++i)
        {
        CPoint pt = RegulatedPts.GetAt(i);
        if(first)
            {
            lastY = pt.y;
            first = false;
            }

        /************************************************
        * Remember that Y coordinates are larger as we
        * move down on the screen, and smaller as we move
        * up.  So a curve that is getting "higher" in voltage
        * has a more negative coordinate.  We have moved
        * (0,0) "down" on the screen so higher voltages are
        * above (0,0), that is, negative
        *      |  /\            ^
        *      | /  \           | Positive voltages
        *      |/    \          | Negative coordinates
        * (0,0)+------\-------------------------------
        *      |       \    /   | Negative voltages
        *      |        \  /    | Positive coordinates
        *      |         \/     V
        * 
        Corresponding output curve
        *      |  _________________________________
        *      | /
        *      |/
        *      +------------------------------------
        ************************************************/
        if(pt.y < lastY) // more negative is higher
            { /* ramping up */
            // For the first wave, we are starting with no charge on the capacity
            // so the first curve
            count = 0;      // How many voltages have remained the same?  (None: it is changing, going up)
            lastY = pt.y;   // Save the current Y value so we can telll if the next one is higher (more
                            // negative)
            continue;       // Keep going until we get count values
            } /* ramping up */

        if(pt.y > lastY)    // More positive means voltage is dropping
            { /* failure */
            /*********************************************************
            *       |
            *       |.._____
            *       |       \
            *       |        ^
            *       +--------|-------------------------------------- 
            *                |
            *                You are here
            *********************************************************/
            BadRegulation = true;
            BadPoint = pt;
            // Now create the bounding rectangle.
            // Initially, it is one pixel wide
            BadRect.top = pt.y;
            BadRect.left = pt.x;
            BadRect.bottom = pt.y + 1;
            BadRect.right = pt.x + 1;
            break; // Stop this loo
            } /* failure */
         
        // It wasn't less than or greater than, it must be equal
        count++;
       // if(count > STABLE_THRESHOLD)
       //     break; // We have reached steady-state
        }

    /* if we get here, we have found a stable point or a dip */
    if(!BadRegulation)
        { /* at stable point */
        // We continue on, and if we see a dip, we are in trouble
         for(; i < RegulatedPts.GetCount(); ++i)
             { /* check for dip */
             CPoint pt = RegulatedPts.GetAt(i);
             if(pt.y > lastY)
                 { /* failed */
                 BadRegulation = true;
                 BadPoint = pt;
                 if(BadRect.IsRectEmpty())
                     {
                     BadRect.top = pt.y;
                     BadRect.left = pt.x;
                     BadRect.bottom = pt.y - 1;
                     BadRect.right = pt.x - 1;
                     } /* load rect */
                 break;
                 } /* failed */
             } /* check for dip */
        } /* at stable point */

    /* Whe we get here, BadRegulated is true if there was a dip, even one unit */
    // Now we scan forward to where the dip goes away
    if(BadRect.left > 0)
        { /* find recovery point */
         for(; i < RegulatedPts.GetCount(); ++i)
             { /* scan forward */
             CPoint pt = RegulatedPts.GetAt(i);
             if(pt.y == lastY)
                 { /* back to stable */
                 break;
                 } /* back to stable */
             BadRect.right = pt.x;
             BadRect.bottom = max(BadRect.bottom, pt.y);
             } /* scan forward */
        } /* find recovery point */
 
    if(showRegulated && Rectification != IDC_RECTIFY_NONE && RegulatedPts.GetCount() > 0)
        { /* show regulated */ 
         Plot(dc, RegulatedPts, PS_SOLID, 2, Colors::Regulated);
            if(BadRect.left > 0 && showRegulationFailure) // TODO: do we want to mark all bad points?
                { /* show error point */ 
                int n1 = dc.SaveDC();
                    CPen pen;
                    pen.CreatePen(PS_SOLID, 2, Colors::Error);
                    dc.SelectObject(pen);
                    dc.SelectStockObject(HOLLOW_BRUSH);
                    static const int BadRadius = 15;
                    BadRect.top -= BadRadius;
                    BadRect.left -= BadRadius;
                    BadRect.bottom += BadRadius;
                    BadRect.right += BadRadius;
                    //dc.Ellipse(BadRect);
                    dc.RoundRect(BadRect, CPoint(5, 5));
                    //dc.Ellipse(BadPoint.x - BadRadius, BadPoint.y - BadRadius,
                    //           BadPoint.x + BadRadius, BadPoint.y + BadRadius);
                dc.RestoreDC(n1);
                } /* show error point */
        } /* show regulated */


    /************************************************
    * Plot the sum of the base sine wave and the phase shifted wave
    ************************************************/
    if(!showClassD && showSum)
        {
         Plot(dc, SumPts, PS_DASH, 1, Colors::Sum);
        }

    /**************************************************
    * Plot the difference of the base sine wave and the phase shifted wave
    **************************************************/
    if(!showClassD && showDiff)
        {
        Plot(dc, DiffPts, PS_DASHDOT, 1, Colors::Diff);
        }

    /**************************************************
    * Plot the Schmitt Trigger curve
    **************************************************/
    if (SchmittData.getEnable() && !showPWM)
    {
        Plot(dc, SchmittPts, PS_SOLID, 2, Colors::Schmitt);
    }

    /**************************************************
    * Plot the PWM curve
    **************************************************/
    if (showPWM)
    {
        Plot(dc, PWMPts, PS_SOLID, 2, Colors::PWM);
        {
            CString t;
            t.Format(_T("PWM %d%%"), PWMWidth);
            // TODO: See if we can avoid conflict with the lines by computing msgX,msgY
            int msgX = 5;
            int msgY = 5;
            int n = dc.SaveDC();
            Value::SelectFont(dc, Value::DefaultFontHeight);
            dc.TextOut(msgX, msgY, t);
            dc.RestoreDC(n);
        }
    }

    /**************************************************
    * Plot the RMS line (only if the Zero is at center)
    **************************************************/
    if(ZeroPos == IDC_ZERO_CENTER && showRMS)
        { /* show RMS */
         int rms = (int)((1.0 / sqrt(2.0)) * (double)height);
         // TODO: simplify this to use Plot() method
         int n = dc.SaveDC();
    

            CPen pen;
            pen.CreatePen(PS_DOT, 1, Colors::DCurve);
            dc.SetBkMode(TRANSPARENT);
            dc.SelectObject(pen);
            dc.MoveTo(org.x, rms);
            dc.LineTo(org.x + width, rms);
            dc.MoveTo(org.x, -rms);
            dc.LineTo(org.x + width, -rms);
            GDIFLUSH();
        dc.RestoreDC(n);
        // Now figure out where to put the voltage indications
        // What we do is scan for the maximum and minimum points (remembering these are inverted in our
        // coordinate system)
        /*******************************************************
        *      |  /\______*______________-rms
        *      | /  \           | Positive voltages
        *      |/    \          | Negative coordinates
        * (0,0)+------\-------------------------------
        *      |       \    /   | Negative voltages
        *      |        \  /    | Positive coordinates
        *      |___*_____\/_____V________+rms
        *      |   |      |
        *      |   |      minyAt
        *      |   maxyAt
        *******************************************************/
        int maxyAt = 0;
        int minyAt = 0;

        for(int  i = 0; i < pts.GetSize(); ++i)
           { /* find high point */ 
            if (i > 0 && pts.GetAt(i).y < 0 && pts.GetAt(i).y > pts.GetAt(maxyAt).y)     // sine going up: 0, -1, -2, ... -n
                break; // we have alrleady found our max
            maxyAt = i;
           } /* find high point */ 

        for(int i = 0; i < pts.GetSize(); ++i)
           { /* find low point */ 
            if (i > 0 && pts.GetAt(i).y > 0 && pts.GetAt(i).y < pts.GetAt(minyAt).y)
                break; // we have found our min
            minyAt = i;
           } /* find low point */

           { /* draw notes */
            int n = dc.SaveDC();
                static const int NoteSpace = 2;
                CString s;
                Value::SelectFont(dc, Value::DefaultFontHeight);
                double RMS_V = InputVoltage;
                double P_V = RMS_V * sqrt(2.0);
                s.Format(_T("%3.1fV RMS"), RMS_V);
                Note(dc, s, pts.GetAt(maxyAt).x, -rms + NoteSpace, TA_TOP | TA_CENTER);
                s.Format(_T("%3.1fV RMS"), -RMS_V);
                Note(dc, s, pts.GetAt(minyAt).x, rms - NoteSpace, TA_BOTTOM | TA_CENTER);
                s.Format(_T("%3.1fV P"), P_V + 0.05);
                Note(dc, s, pts.GetAt(minyAt).x, -height + NoteSpace, TA_TOP | TA_CENTER);
                s.Format(_T("%3.1fV P"), -(P_V + 0.05));
                Note(dc, s, pts.GetAt(maxyAt).x, height - NoteSpace, TA_BOTTOM | TA_CENTER);
            dc.RestoreDC(n);
           } /* draw notes */
        } /* show RMS */

    /*******************************************************
    * Draw the ADC step function
    *******************************************************/


    if(showStep && Rectification == IDC_RECTIFY_NONE)
        { /* show steps*/
         ShowSteps(dc, pts, height, max_y, min_y);
        } /* show steps */

     /*******************************************************************
     * Draw the Class D graph
     ********************************************************************/
     if(showClassD)
         { /* show Class D curve */ 
         int n = dc.SaveDC();
             org.y = r.Height() - 1;
             static const int DY = 5;  // arbitrary separation
             height = (int) ((double) r.Height() / 2.0) - DY;
             dc.SetViewportOrg(0, r.Height() - DY);
             dc.SetViewportExt(r.Width(), r.Height() - DY);

             /*
             * Draw the zero line for the class D baseline
             */
             CPen DOrigin;
             DOrigin.CreatePen(PS_SOLID, 1, Colors::ClassDZero);
             dc.SelectObject(DOrigin);
             dc.MoveTo(0, 0);
             dc.LineTo(r.Width(), 0);  // Draw the origin line

             /**********************************************************
             * Note that Dpts[i].y represents the percentage of the cycle that the
             * waveform is on, expressed as 0 = 0%, 1000 = 50%, 2000 = 100%
             ***********************************************************/
             dc.MoveTo(0, 0);
             int y = -( r.Height() / 2 - 2 * DY);
             CPen DCurve;
             DCurve.CreatePen(PS_SOLID, 1, Colors::DCurve);
             dc.SelectObject(DCurve);
             /***************************************************************
             * Draw the lines connecting points on the sine wave to points
             * on the class-D input waveform
             ****************************************************************/
             CPen Dfiducial;
             Dfiducial.CreatePen(PS_SOLID, 1, Colors::Fiducial);

             for(int i = 0; i < Dpts.GetSize() -1; ++i)
                 { /* draw one wave */
                 dc.LineTo(Dpts[i].x, y);   // vertical line

                 {
                 /*************************************************************
                 * We saved the "top" of the line as a point on the sine wave
                 * This was stored in device points, which are window-relative
                 * points in the absolute coordinate space of the window
                 * 
                 * Since we have changed the logical origin, we could not use
                 * the actual point that was plotted, because that was plotted on
                 * the sine's coordinate system
                 * 
                 * So, now that we have a new coordinate system, we convert the
                 * device coordinates back to logical coordinates in the new 
                 * coordinate system and this gives us the correct starting point
                 * for the line in our new coordinate system
                 **************************************************************/
                    int n1 = dc.SaveDC();
                         dc.SelectObject(Dfiducial);
                         CPoint top = Tops[i];
                         dc.DPtoLP(&top);
                         dc.LineTo(top.x, top.y);
                     dc.RestoreDC(n1);
                     }

                /* now compute the dx */
                int dx = Dpts[i + 1].x - Dpts[i].x;  // 100% distance
                // Now compute the percentage
                dx = (int) ((double) dx * (double) Dpts[i].y / 2000.0);
                dc.LineTo(Dpts[i].x + dx,y);

                dc.LineTo(Dpts[i].x + dx, 0);
                dc.LineTo(Dpts[i + 1].x, 0);
                 } /* draw one wave */
             dc.RestoreDC(n);
         } /* show Class D curve */
    }
 
/***********************************************************************
*                       CGraph::GetBoxInfo
* Inputs:
*     CPoint pt: The mouse position, in device points in the graph box
* Result: BoxInfo *
*     NULL if the mouse is not in a box
*     Otherwise, the boxInfo value
***********************************************************************/
BoxInfo * CGraph::GetBoxInfo(CPoint pt)
{
     if(!showStep)
        return NULL;  // values are not valid if steps are not being shown

     for(int i = 0; i < Boxes.GetCount(); ++i)
        { /* scan each */ 
         BoxInfo * b = Boxes.GetAt(i).InBox(pt);
         if(b != NULL)
            return b;
        } /* scan each */
    return NULL;
} // CGraph::GetBoxInfo

#if 0 // obsolete
    void CGraph::SetADCStats(ADCStats* val)
    {
        if (val == NULL)
           {
            if (stats != NULL)
                delete stats;
            stats = NULL;
            Invalidate();
            return;
           }
        if(stats == NULL)
           { /* first time */
            stats = val;
            Invalidate();
            return;
           } /* first time */

        if(stats->equals(val))
           { /* no change */ 
            delete val;
            return;
           } /* no change */
        if (stats != NULL)
            delete stats;
        stats = val;
        Invalidate();
    }

#endif // obsolete
/****************************************************************************************
*                 CGraph:OnMouseMove
* Inputs:
*     UINT nFlags: ignored
*     CPoint point: Mouse position in device coordinates relative to the top of the window
****************************************************************************************/
void CGraph::OnMouseMove(UINT nFlags, CPoint point)
    {
        // TODO: Add your message handler code here and/or call default
        if(!tracking)
        { /* set up tracking */
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = this->m_hWnd;
            VERIFY(::TrackMouseEvent(&tme));
            tracking = true;
            Invalidate();  // first time in, force redraw
        } /* set up tracking */

        MousePoint = point;
        BoxInfo * b = GetBoxInfo(MousePoint);
        if (b == NULL && stats.GetValid())
           {
            stats.SetValid(false);
            Invalidate();
           }
        else
           {
            BoxInfo* oldb = stats.GetBoxInfo();
            if (b == NULL || !b->equals(oldb))
                Invalidate();
           }

        CStatic::OnMouseMove(nFlags, point);
    }


void CGraph::OnMouseLeave()
    {
        tracking = false;
        if (stats.GetValid())
            Invalidate();
        stats.SetValid(false);


        CStatic::OnMouseLeave();
    }

void CGraph::Flash()
{
    flashing = true;
    Invalidate();
    UpdateWindow();
    Sleep(250);
    flashing = false;
    Invalidate();
    UpdateWindow();
}