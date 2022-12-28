/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "Circuit.h"
#include "CircuitWindow.h"
#include "Value.h"
#include "GDIFLUSH.h"
#include "Colors.h"

BEGIN_MESSAGE_MAP(CircuitWindow, CStatic)
    ON_WM_PAINT()
    ON_WM_CREATE()
END_MESSAGE_MAP()
/***********************************************************************************
*                       CapacitorVoltages
* This is a table of capacitor voltages.  This table was created from the list
* of capacitor voltages found in DigiKey.com.  Not all capacitors are going to be 
* available in all voltages, but this is intended to be suggestive rather than
* definitive
************************************************************************************/
static const double CapacitorVoltages[] = {
    6.0,
    6.3,
    7.0,
    7.5,
    8.0,
    10.0,
    12.0,
    15.0,
    16.0,
    20.0,
    25.0,
    28.0,
    30.0,
    35.0,
    40.0,
    42.0,
    45.0,
    50.0,
    55.0,
    60.0,
    63.0,
    70.0,
    75.0,
    80.0,
    100.0,
    120.0,
    125.0,
    150.0,
    };

static const double ResistorPos = 0.78;

static const int ParameterVOffset = 5;
static const int ParameterHOffset = 7;


/****************************************************************************************
*                             CircuitWindow::GetCapacitorVoltage
* Inputs:
*     double V: The peak input voltage to the capacitor
* Result: double
*    The suggested minimum capacitor voltage
* Notes:
*    This uses an _ad hoc_ formula to compute a "safe" value
* 
*****************************************************************************************/
double CircuitWindow::GetCapacitorVoltage(double V)
    {
    double VS = 1.5 * V;  // The nominal "safe" voltage
    // TODO: determine if VS is the same conversion factor pre-regulator
    //       and post-regulator
    //       If not, make the conversion factor a second parameter.

    for(int i = 0; i < _countof(CapacitorVoltages); ++i)
        { /* search */
        if(CapacitorVoltages[i] < VS)
            continue;
        return CapacitorVoltages[i];
        } /* search */
    return 0.0;
    }
/*******************************************************************************************
*                                CircuitWindow::DrawFrame
* Inputs:
*    CDC & dc:
* Result: void
* 
* Effect:
*    In debug mode, this draws a frame around the area
*    In non-debug mode, this draws the innards of the window
*******************************************************************************************/
void CircuitWindow::DrawFrame(CDC & dc)
    {
    // In debug mode, draw a window so we can see what is happening
    CRect r;
    GetClientRect(&r);
    COLORREF bkg = dc.GetPixel(2, 2);

    int n = dc.SaveDC();
    CBrush brush;
    brush.CreateSolidBrush(bkg);
    dc.SelectObject(brush);
    dc.SelectStockObject(NULL_PEN);
    dc.Rectangle(&r);

#ifdef _DEBUG
    dc.SelectStockObject(BLACK_PEN);

    dc.SelectStockObject(NULL_BRUSH);
    dc.Rectangle(&r);
    dc.RestoreDC(n);
#endif
    }

/********************************************************************************
*                                CircuitWindow::VDivide
* Inputs:
*     double Vi: Input voltage ,
*     int R1: Top resistor
*     int R2: Bottom resistor
*  Result: double
*     The voltage from using the two resistors
********************************************************************************/
double CircuitWindow::VDivide(double Vi, int R1, int R2) const
{
    return Vi * ((double)R2) / (double)(R1 + R2);
}
/********************************************************************************
*                                CircuitWindow::FindR
* Inputs:
*     CDC & dc: For drawing
*     int y: Position to draw performance value
*     double ViMax: Maximum voltage permitted
*     double Vi: Input voltage (greater than ViMax)
*     double Vt: Trigger voltage (maximum trigger, 
*     int & R1: Place to store R1
*     int & R2: Plave to store R2
*  Result: bool
*     true if R1, R2 valid
*     false if no feasible solution
********************************************************************************/

bool CircuitWindow::FindR(CDC & dc, int y, double ViMax, double Vi, double Vt, int & R1, int & R2)
{
    if(Vt > ViMax)
       { /* error */
        return false;  // We should not have been called
       } /* error */

    // We want to find a voltage that triggers the Schmitt trigger but does not
    // exceed ViMax
    // If Vt% is over some value like 98%, this may be hard
    static const uint32_t Resistors[] = { 10000, 11000, 12000, 13000, 15000, 16000, 18000,
                                          20000, 22000, 24000, 27000,
                                          30000, 33000, 36000, 39000,
                                          43000, 47000,
                                          51000, 56000, 
                                          62000, 68000,
                                          75000, 
                                          82000,
                                          91000, 
                                         100000,
                                         110000,
                                         120000,
                                         130000,
                                         150000,
                                         160000,
                                         180000};

    // This is a terrible algorithm, but the set is so small that it takes almost
    // no time to execute

    int R1i = -1; // index of R1
    int R2i = -1; // index of R2
    double bestDV = HUGE_VAL;

#define MEASURE_TIME
#ifdef MEASURE_TIME
    LARGE_INTEGER T0;
    ::QueryPerformanceCounter(&T0);
    int count = 0;
#endif

    for(int i = 0; i < _countof(Resistors); ++i)
        for(int j = 0; j < _countof(Resistors); ++j)
        { /* try all pairs */
            double V = VDivide(Vi, Resistors[i], Resistors[j]);
//#define TRACE_RESISTORS
#ifdef TRACE_RESISTORS
            TRACE(_T("Vi %3.1f, R1 %d, R2 %d, V %3.1f %s\n"), 
                              Vi, Resistors[i], Resistors[j], V,
                                    V > ViMax ? _T(">") :
                                        V < Vt ? _T("<") : 
                                           ViMax - V < bestDV ? _T("ok") : _T("b.e."));
#endif
#ifdef MEASURE_TIME
            count++;
#endif
            if (V > ViMax)
                break;  // ignore useless values...too high and will not improve with larger R2 ([j])
            if (V < Vt)
                continue;  // ignore useless values...too low
            // It is between Vt and ViMax
            // is it better than the last estimate
            if(ViMax - V < bestDV)
                { /* better */ 
#ifdef TRACE_RESISTORS
                TRACE(_T("ViMax %3.1f, V %3.1f, ΔV %3.1f, bestDV %3.1f\n"), ViMax, V, ViMax - V, bestDV);
#endif
                R1i = i;
                R2i = j;
                bestDV = (ViMax - V);
                } /* better */
        } /* try all pairs */

#ifdef MEASURE_TIME
    LARGE_INTEGER T1;
    ::QueryPerformanceCounter(&T1);
    LARGE_INTEGER DT;
    DT.QuadPart = T1.QuadPart - T0.QuadPart;

    LARGE_INTEGER F;
    ::QueryPerformanceFrequency(&F);

    double dticks = (double)DT.QuadPart;
    double dfreq = (double)F.QuadPart;
    double dtime = dticks / dfreq;
    // When enabled, this measures the time for this algorithm as ~10.3μs (Debug)
    // and 2.2μs (Release)
    double us = dtime * 1000000.0;
    CString s;
    s.Format(_T("%3.1fμs (%d / %d, %3.1fns)\n"), us, count, _countof(Resistors) * _countof(Resistors), (us / (double) count) * 1000.0);
    int n = dc.SaveDC();
       Value::SelectFont(dc);
       dc.SetTextAlign(TA_BOTTOM | TA_LEFT);
       dc.SetBkMode(TRANSPARENT);
       dc.TextOut(3, y, s);
    dc.RestoreDC(n);
#endif

    if (R1i < 0)
        return false;
    R1 = Resistors[R1i];
    R2 = Resistors[R2i];
    return true;
} // CircuitWindow::FindR

/********************************************************************************
*                                CircuitWindow::ShowValue
* Inputs:
*     CDC & dc:
*     CPoint pt: Point at which it should be shown
*     const Value & val: Value to display
*     UINT position: TA_ values for position
* Result: int
*     The height of the line, or 0 if no line was put out
* Effect:
*     Draws the text at the indicate point
*********************************************************************************/

int CircuitWindow::ShowValue(CDC & dc, CPoint pt, Value & Val, UINT position)
    {
    if(!ShowValues)
        return 0;

    //Element::GDIFlash(dc, pt);

    int result = Val.draw(dc, pt, position);
    GDIFLUSH();
    return result;
    }

void CircuitWindow::SetTextColor(CDC& dc)
{
    dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);
}

void CircuitWindow::ShowX(CDC& dc, CPoint pt, UINT align)
{
    int n = dc.SaveDC();
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextAlign(align);
    dc.SetTextColor(RGB(255, 0, 0));

    dc.TextOutW(pt.x, pt.y, _T("✘"));
    dc.RestoreDC(n);
}
/**********************************************************************************
*                             CircuitWindow::SchmittCircuit
* Inputs:
*     CDC & dc:
* Result: void
*
* Effect:
*     Draws the AC source
***********************************************************************************/

void CircuitWindow::SchmittCircuit(CDC& dc)
{
    CRect r;
    GetClientRect(&r);
    int n = dc.SaveDC();

    DrawFrame(dc);
/**************************************************************
*  | wire0  |       diode       | Schmitt   |
*  |0      1|0                 1|0         1|
*  |        |                   |   | \
*  *--------*------|>||---------*---|//\----*
*  |0                               |  /
*  |                                | /
*  |   start^                   ^end
*  |                       start^            ^end (SchmittDiodePos)
* (~)--ACSource
*  |1
*  *---------------------------------------*
*  |0                                     1|
*  |               wire1                   |
***************************************************************/
    ACSource source(this, flashing);
    source.origin = CPoint((int)(0.25 * r.Height()), r.Height() / 2);
    source.draw(dc);

    static const double SchmittDiodePct = 0.33;

    CPoint w0end((int)(SchmittDiodePct * r.Width()), source.GetP(0).y);
    SimpleWire wire0(this, source.GetP(0), w0end, flashing);
    wire0.draw(dc);
    //wire0->Connect(dc, wire0.GetP(1));

    CPoint w1start(source.GetP(1));
    CPoint w1end((int)(0.78 * (double)r.Width()), source.GetP(1).y);
    SimpleWire wire1(this, w1start, w1end, flashing);
    wire1.draw(dc);
    wire1.Connect(dc, wire1.GetP(1), flashing);

    CPoint start = wire0.GetP(1);
    CPoint end = CPoint((int)(0.5 * (double)r.Width()), start.y);

    Diode diode(this, start, end, flashing, Vf);
    diode.draw(dc);

 
    //Connect(ST.GetP(1));
    double Vin = sqrt(2) * InputVoltage - Vf;
    ShowValue(dc, source.GetP(0) + CSize(0, -ParameterVOffset), Volts(InputVoltage, CString(_T("V RMS"))));
    ShowValue(dc, diode.GetP(1) + CSize(0, -ParameterVOffset), Volts(Vin, CString(_T("V PP")))  );

    bool needX = false;

    start = diode.GetP(1); // May be overridden by voltage divider
    if (ViSafe)
        { /* make voltage divider */
         if(Vin > ViMax)
            { /* make voltage divider */ 
             /**************************************************************
             *  | wire0  |       diode  | Schmitt        |
             *  |0      1|0            1|0              1|
             *  |        |              |                |
             *  *--------*------|>||----*                |
             *  |0                      |                |
             *  |                       >                |
             *  |   start^          R1  <                |
             *  |                       >                |
             * (~)--ACSource            |        | \     |
             *  |1                      *--------|//\----*
             *  |                       |        |  /
             *  |                       <        | /
             *  |                   R2  >    ^end
             *  |                       <    ^start       ^end (SchmittDiodePos)
             *  |                       |
             *  *-----------------------*---------------*
             *  |0                                     1|
             *  |               wire1                   |
             ***************************************************************/
             // Vout = Vin * (R2)/(R1 + R2)
             // We are going to set Vout = 0.nn * Vcc
             // We will choose to resistors such that the range of Vout is 0.nn±0.tt of the nominal Vout
             CPoint Rcenter(diode.GetP(1).x,  diode.GetP(1).y + (wire1.GetP(1).y - diode.GetP(1).y) / 2);

             Resistor R1(this, diode.GetP(1), Rcenter, flashing);
           
             Resistor R2(this, Rcenter, CPoint(diode.GetP(1).x, wire1.GetP(1).y), flashing);
             dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);

             int R1val;
             int R2val;

             Vt = ViMax * Ton;
                                // use actual On% to compute this as well

             if(!FindR(dc, r.bottom - 3, ViMax, Vin, Vt, R1val, R2val))
                { /* failed */ 
                 int dy = (Rcenter.y - diode.GetP(1).y) / 2;
                 int df = Value::DefaultFontHeight / 2;
                 int x = diode.GetP(1).x;
                 int y = Rcenter.y - dy + df;
                 ShowX(dc, CPoint(x, y), TA_BASELINE | TA_CENTER);
                 
                 y = Rcenter.y + dy + df;
                 ShowX(dc, CPoint(x, y), TA_BASELINE | TA_CENTER);
                } /* failed */
             else
                { /* success */ 
                 R1.draw(dc);
                 R2.draw(dc);
                 CPoint pt = R1.GetP(3) - CSize(ParameterHOffset, ParameterVOffset);

                 ShowValue(dc, pt, Ohms(R1val), TA_RIGHT | TA_TOP);
                 pt = R2.GetP(3) - CSize(ParameterHOffset, ParameterVOffset);
                 ShowValue(dc, pt, Ohms(R2val), TA_RIGHT | TA_TOP);

                 double V = VDivide(Vin, R1val, R2val);
                 pt = Rcenter - CSize(ParameterHOffset, ParameterVOffset);
                 ShowValue(dc, pt, Volts(V), TA_RIGHT | TA_TOP);
                } /* success */
             R1.Connect(dc, Rcenter, flashing);
             R2.Connect(dc, R2.GetP(1), flashing);

             start = Rcenter;
            } /* make voltage divider */
        } /* make voltage divider */
    else
       { /* check voltage */ 
        if(Vin > ViMax)
           { /* overvoltage */ 
            needX = true;
           } /* overvoltage */
       } /* check voltage */


    end = CPoint((int)(0.78 * (double)r.Width()), start.y);
    SchmittTrigger ST(this, start, end, Invert, flashing);

    ST.draw(dc);

    if (needX)
       { /* need X */
        ShowX(dc, ST.GetP(2), TA_RIGHT | TA_TOP);
       } /* need X */

    SimpleWire wire2(this, ST.GetP(1), ST.GetP(1), flashing);
    wire2.draw(dc);
    wire2.Connect(dc, wire2.GetP(1), flashing);

    dc.RestoreDC(n);
} // CircuitWindow::SchmittCircuit

/**********************************************************************************
*                             CircuitWindow::ACSourceOnly
* Inputs:
*     CDC & dc:
* Result: void
* 
* Effect:
*     Draws the AC source
***********************************************************************************/

void CircuitWindow::ACSourceOnly(CDC & dc)
    {
    CRect r;
    GetClientRect(&r);
    int n = dc.SaveDC();

    DrawFrame(dc);
    ACSource * source = new ACSource(this, flashing);
    source->origin = CPoint((int) (0.25 * r.Height()), r.Height() / 2);
    source->draw(dc);

    //int wlen = (int) (ResistorPos * r.Width());
    CPoint w0end((int) (ResistorPos * r.Width()), source->GetP(0).y);
    SimpleWire wire0(this, source->GetP(0), w0end, flashing);
    wire0.draw(dc);
    wire0.Connect(dc, wire0.GetP(1), flashing);

    CPoint w1end(w0end.x, source->GetP(1).y);
    SimpleWire wire1(this, source->GetP(1), w1end, flashing);
    wire1.draw(dc);
    wire1.Connect(dc, wire1.GetP(1), flashing);

    dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);

    ShowValue(dc, source->GetP(0) + CSize(0, -ParameterVOffset), Volts(InputVoltage, CString(_T("V RMS"))));
    ShowValue(dc, wire0.GetP(1) + CSize(0, -ParameterVOffset), Volts(sqrt(2.0) * InputVoltage, CString(_T("V PP"))));
    delete source; 

    dc.RestoreDC(n);
    } // CircuitWindow::ACSourceOnly

/**********************************************************************************************
*                             CircuitWindow::DrawSource
* Inputs:
*     CDC & dc:
* Result: ACSource *
* 
* Effect: 
*     Creates and draws the AC source
**********************************************************************************************/
ACSource * CircuitWindow::DrawSource(CDC & dc)
    {
    CRect r;
    GetClientRect(&r);

    ACSource * source = new ACSource(this, flashing);
    source->origin = CPoint((int) (0.25 * r.Height()), r.Height() / 2);
    source->draw(dc);
    return source;
    }

/*********************************************************************************************
*                              CircuitWindow::AnnotateCapacitor
* Inputs:
*     CDC & dc:
*     Capacitor & c: The capacitor whose properties are to be annotated
*     double Vin: The input voltage present at the capacitor + terminal
*********************************************************************************************/
void CircuitWindow::AnnotateCapacitor(CDC & dc, Capacitor & c, double Vin)
    {
    CPoint left = c.GetP(3);
    CPoint right = c.GetP(4);
    CPoint center(left.x + (right.x - left.x) / 2, left.y);
    CPoint ptF(center.x - 2 , center.y - 2);
    dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);
    int height = ShowValue(dc, ptF, 
                                Farads(Capacitance), 
                                TA_TOP | TA_RIGHT);

     double Vsafe = GetCapacitorVoltage(Vin);

     CPoint ptV(center.x + 2, ptF.y);
     ShowValue(dc, ptV, Volts(Vsafe, CString(_T("V"))), TA_TOP | TA_LEFT);
     //ShowValue(dc, pt + CSize(0, height + 2), Volts(Vsafe, CString(_T("V"))), TA_TOP | TA_RIGHT);
    }
/*********************************************************************************************
*                                 CircuitWindow::HalfWave
* Inputs:
*    CDC & dc:
* Result: void
* 
* Effect:
*     Draws the halfwave rectifier schematic
**********************************************************************************************/
void CircuitWindow::HalfWave(CDC & dc)
    {
    CRect r;
    GetClientRect(&r);
    int n = dc.SaveDC();

    DrawFrame(dc);
    dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);

    ACSource * source = DrawSource(dc);
    ShowValue(dc, source->GetP(0) + CSize(0, -ParameterVOffset), Volts(InputVoltage, CString(_T("V RMS"))));

    /**************************************************************
    *  | wire0  |       diode       | wire1    |
    *  |0      1|0                 1|0        1|
    *  *--------*------|>||---------*----------* 
    *  |0  start^                   ^end
    *  : ACSource
    *  |1
    *  *---------------------------------------*
    *  |0                                     1|
    *  |               wire2                   |
   ***************************************************************/
    int w0len = (int) (0.1 * r.Width());
    SimpleWire wire0(this, source->GetP(0), w0len, flashing);
    wire0.draw(dc);

    CPoint start = wire0.GetP(1);
    CPoint end = CPoint((int) (0.4 * (double) r.Width()), start.y);

    Diode diode(this, start, end, flashing, Vf);  
    diode.draw(dc);

    SimpleWire * wire1;
    CPoint w1end((int) (ResistorPos * (double) r.Width()), start.y);

    if(!showRegulator)
        { /* no regulator */
        wire1 = new SimpleWire(this, diode.GetP(1), w1end, flashing);
        wire1->draw(dc);
        wire1->DebugConnections(dc);
        wire1->Connect(dc, wire1->GetP(1), flashing);
        ShowValue(dc, wire1->GetP(1) + CPoint(0, -ParameterVOffset), Volts(sqrt(2) * InputVoltage - diode.GetVf(), CString(_T("V P"))));
        } /* no regulator */
    else
        { /* has regulator */ 
        CPoint regend((int) (ResistorPos * r.Width()), diode.GetP(1).y);

        Regulator  reg(this, end, regend, source->GetP(1), flashing);
        reg.SetV(ShowValues ? OutputVoltage : 0.0);

        reg.draw(dc);
        
        wire1 = new SimpleWire(this, reg.GetP(1), regend, flashing);
        wire1->draw(dc);
        wire1->DebugConnections(dc);
        wire1->Connect(dc, wire1->GetP(1), flashing);
        ShowValue(dc, wire1->GetP(1) + CSize(0, -ParameterVOffset), Volts(OutputVoltage, CString(_T("V DC"))));
        } /* has regulator */

    CPoint w2start = source->GetP(1);
    CPoint w2end = CPoint(wire1->GetP(1).x, w2start.y);

    SimpleWire wire2(this, w2start, w2end, flashing);
    wire2.draw(dc);
    wire2.Connect(dc, wire2.GetP(1), flashing);

    double Vin = sqrt(2) * InputVoltage - diode.GetVf();

    if(filtered)
        { /* draw filter capacitor */
        CPoint left = diode.GetP(1);
        CPoint capStart = diode.GetP(1);
        CPoint capEnd = wire2.GetP(1); // note that x-coordinate of this is ignored

        Capacitor c(this, capStart, capEnd, flashing);
        c.draw(dc);
        if(showRegulator)
            ShowValue(dc, capStart + CSize(0, -ParameterVOffset), 
                          Volts(sqrt(2) * InputVoltage - diode.GetVf(), CString(_T("V P"))) );
        AnnotateCapacitor(dc, c, Vin);
        } /* draw filter capacitor */

    // Now draw the load resistor
    Resistor res(this, wire1->GetP(1), wire2.GetP(1), flashing);
    res.draw(dc);
    static const int OhmsOffset = -8;
    ShowValue(dc, res.GetP(2) + CSize(OhmsOffset,0), Ohms(Resistance), TA_BASELINE | TA_RIGHT);
    // W = I * E
    // E = I * R
    // I = E / R
    // W = (E / R) * E = E² / R
    ShowValue(dc, res.GetP(2) + CSize(2, 0), Watts((OutputVoltage * OutputVoltage) / Resistance),
        TA_LEFT | TA_BASELINE);

    dc.RestoreDC(n);

    delete wire1;
    delete source;
    } // CircuitWindow::Halfwave

/**************************************************************************
*                      CircuitWindow::FullWave
* Inputs:
*     CDC & dc: 
* Result: void
* 
* Draws a fullwave rectifier
**************************************************************************/
void CircuitWindow::FullWave(CDC & dc)
    {
    CRect r;
    GetClientRect(&r);
    int n = dc.SaveDC();

    DrawFrame(dc);

    ACSource * source = DrawSource(dc);

    dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);
    ShowValue(dc, source->GetP(0) + CSize(0, -ParameterVOffset), Volts(InputVoltage, CString(_T("V RMS"))));

    int height = (int) (0.20 * (double) r.Height());
    CPoint center((int)( (double)source->origin.x + 1.75 * (double) height), source->origin.y);

    FullWaveRectifier rectifier(this, center, height, flashing, Vf);
    /**************************************************************
    *    0  wire0             2
    *   *---------------------+
    *   |P0                   |1
    *   |                     *P0        w2e (regulator)
    *   :                    / \  rectifier  +-----+    w2e (no regulator) (xend, rect->GetP(2).y)
    *   : source         P3 * P2*--------*---*   P3*----* 
    *   :                  0|\ /P0 wire2 P1  +--*--+   1
    *   |              |    | *P1               |
    *   |P1            V    | |P1               |
    *   *-------------------|-+                 |
    *    P0  wire1          | P2                |
    *                 ---  2+-------------------*-------* w3e
    *                  ^     0     wire3                1
    *                  |
    *                 drop
    ***************************************************************/

    CPoint R0;
    CPoint R1;

    rectifier.draw(dc);
    ComplexWire * wire0 = new ComplexWire(this, source->GetP(0), rectifier.GetP(0), Orientation::Horizontal, flashing);
    wire0->draw(dc);

    ComplexWire * wire1 = new ComplexWire(this, source->GetP(1), rectifier.GetP(1), Orientation::Horizontal, flashing);
    wire1->draw(dc);

    int w2len = 0; // set based on regulator, see below

    int xend = (int) (ResistorPos * (double) r.Width());

    static const int drop = (int) (0.1 * (double)r.Height());
    CPoint W3e(xend, source->GetP(1).y + drop);
    ComplexWire wire3(this, rectifier.GetP(3), W3e, Orientation::Vertical, flashing);
    wire3.draw(dc);
    R1 = wire3.GetP(1);
    wire3.Connect(dc, R1, flashing);

    SimpleWire * wire2 = NULL;
    Regulator * reg = NULL;

    double Vin = sqrt(2) * InputVoltage - 2 * rectifier.GetVf();

    if(!showRegulator)
        { /* no regulator */
         w2len = xend - rectifier.GetP(2).x;

         wire2 = new SimpleWire(this, rectifier.GetP(2), w2len, flashing);
         wire2->draw(dc);  
         R0 = wire2->GetP(1);
         wire2->Connect(dc, R0, flashing);
         dc.SetTextColor(flashing ? Colors::FlashWire : Colors::Wire);
         ShowValue(dc, R0 + CSize(0, -ParameterVOffset), Volts(Vin, CString(_T("V P"))));
        } /* no regulator */
    else
        { /* has regulator */
        w2len = (int) (0.05 * (double) r.Width());
        wire2 = new SimpleWire(this, rectifier.GetP(2), w2len, flashing);
        wire2->draw(dc);
        //Element.GDIFlash(dc, wire2->GetP(0));
        //Element.GDIFlash(dc, wire2->GetP(1));

        //CPoint regend((int) (0.7 * r.Width()), rectifier->GetP(2).y);
        reg = new Regulator(this, wire2->GetP(1), 
                                  CPoint(xend, rectifier.GetP(2).y),
                                  wire3.GetP(1), flashing);
        reg->SetV(ShowValues ? OutputVoltage : 0.0);
       
        reg->draw(dc);
        R0 = reg->GetP(1);
        reg->Connect(dc, R0, flashing);
        ShowValue(dc, R0 + CSize(0, -ParameterVOffset), Volts(OutputVoltage, CString(_T("V DC"))));
        } /* has regulator */

    if(filtered)
        { /* draw filter capacitor */
        if(!showRegulator)
            { /* no regulator*/
            int len = xend - rectifier.GetP(2).x;
            CPoint capStart(rectifier.GetP(2).x + len / 2, rectifier.GetP(2).y);
            CPoint capEnd(wire3.GetP(1));  // x value is ignored
            Capacitor c(this, capStart, capEnd, flashing);
            c.draw(dc);
            AnnotateCapacitor(dc, c, Vin);
            //int height = ShowValue(dc,
            //                           Farads(Capacitance),
            //                           TA_BOTTOM | TA_RIGHT);

            } /* no regulator */
        else
            { /* regulator */
            int len = reg->GetP(4).x - rectifier.GetP(2).x;
            CPoint capStart(rectifier.GetP(2).x + len / 2, rectifier.GetP(2).y);
            CPoint capEnd(wire3.GetP(1));
            Capacitor c(this, capStart, capEnd, flashing);
            c.draw(dc);
            ShowValue(dc, capStart + CSize(0, -ParameterVOffset), Volts(Vin, CString(_T("V P"))));
            AnnotateCapacitor(dc, c, Vin);
            

            } /* regulator */
 
        } /* draw filter capacitor */


    Resistor res(this, R0, R1, flashing);
    res.draw(dc);

    ShowValue(dc, res.GetP(3), Ohms(Resistance), TA_RIGHT | TA_BASELINE);

    // W = I * E
    // E = I * R
    // I = E / R
    // W = (E / R) * E = E² / R
    ShowValue(dc, res.GetP(2) + CSize(2, 0), Watts((OutputVoltage * OutputVoltage) / Resistance), 
        TA_LEFT | TA_BASELINE);


    dc.RestoreDC(n);
    delete source;
    delete reg; // Note that if it is NULL, this does nothing
    delete wire0;
    delete wire1;
    delete wire2;
    } // CircuitWindow::FullWave

/*****************************************************************************
*                          CircuitWindow::OnPaint
* Result: void
* 
* Effect:
*    Computes the graph and plots the points
******************************************************************************/
void CircuitWindow::OnPaint()
    {
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages
    CRect r;

    GetClientRect(&r);
    CRgn clip;
    clip.CreateRectRgnIndirect(&r);
    dc.SelectClipRgn(&clip, RGN_AND);
    COLORREF bk = ::GetSysColor(COLOR_3DFACE);
    dc.FillSolidRect(&r, flashing ? Colors::Flash : bk);

    int n = dc.SaveDC();
    CPen pen;
    pen.CreatePen(PS_SOLID, 1, flashing ? Colors::FlashWire : Colors::Wire);
    dc.SelectObject(pen);

    switch(config)
        { /* configuration */ 
        case Configuration::AConly:
            ACSourceOnly(dc);
            break;
        case Configuration::Halfwave:
            HalfWave(dc);
            break;
        case Configuration::Fullwave:
            FullWave(dc);
            break;
        case Configuration::Schmitt:
            SchmittCircuit(dc);
            break;
        } /* configuration */
     dc.RestoreDC(n);
    }

void CircuitWindow::Flash()
{
    flashing = true;
    Invalidate();
    UpdateWindow();
    ::Sleep(250);
    flashing = false;
    Invalidate();
    UpdateWindow();
}