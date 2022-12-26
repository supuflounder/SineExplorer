/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "Value.h"

/****************************************************************************
*                         Value:: SelectFont
* Inputs:
*     CDC & dc:
*     int height: The height of the font; use negative values to get point height
*     CString & TextFont: The font to use; if not specified DefaultTextFont will be used
* Notes:
*    Because of how GDI works, although the font is nominally destroyed
*    when the CFont variable leaves scope, because it is selected into
*    a DC, the font is not actually destroyed until the handle is 
*    deselected from the DC, so while this looks a bit suspect, it depends
*    on some well-defined Windows behavior
*****************************************************************************/
/* static */ void Value::SelectFont(CDC & dc, int height, const CString & TextFont)
    {
    CFont font;
    CString fontname = (TextFont.IsEmpty() ? DefaultTextFont : TextFont);
    bool b = font.CreateFont(-height,
        0, // use default width
        0, // 0 escapement
        0, // 0 orientation
        FW_NORMAL, // weight
        FALSE, // no italic
        FALSE, // no underline
        FALSE, // no strikeout
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, // output precision
        CLIP_DEFAULT_PRECIS, // Clipping precision
        CLEARTYPE_QUALITY, // use ClearType
        VARIABLE_PITCH | FF_DONTCARE,
        fontname
    );

    dc.SelectObject(font);
    } // Value::SelectFont

/****************************************************************************
*                       Amps::toString
* Result: CString
*     The current, displayed as A, mA or μA
*     < 0.000999:  nnn.nnμA
*     0.001-0.999: nnn.nnmA
*     >= 1.0: nnn.nA
****************************************************************************/
CString Amps::toString()
    {
    if(value >= 1.0)
        { /* A */
        CString s;
        s.Format(_T("%3.1fA"), value + 0.05);
        return s;
        } /* A */
    if(value >= 0.001)
        { /* mA */
        CString s;
        // TODO: if value is n.00mA, display as nmA

        s.Format(_T("%3.2fmA"), value * 1000.0);
        s.Replace(_T(".00m"), _T("m"));
        return s;
        } /* mA */
    else
        { /* uA */
        CString s;
        // TODO: if value is n.00μA, display as nμA
        s.Format(_T("%3.2fμA"), value * 1E6);
        s.Replace(_T(".00μ"), _T("μ"));
        return s;
        } /* uA */
    } // Amps::toString

/******************************************************************************
*                              Ohms::toString
* Inputs:
*     int ohms: The value of the resistor, in ohms.
* Result: CString
*     A display of the resistance (0 represents one or more digits before 
*     the decimal point, or the number of decimal places after the decimal
*     point)
*     0-.999 "0mΩ"
*     1-999: "0Ω"
*     1,000-9,999: "0.0kΩ"
*     10,000 - 9,999,999: "0kΩ"
*     1,000,000 - 9,999,999: "0.0MΩ"
*     > 10,000,000: "0MΩ"
* 
******************************************************************************/
CString Ohms::toString()
    {
    if(value < 1.00)
        { /* m ohms*/
        CString s;
        s.Format(_T("%3.0fmΩ"), value * 1000.0);
        return s;
        } /* m ohms*/

    if(value < 10.0)
        { /* .1 ohms */
        CString s;
        s.Format(_T("%3.1fΩ"), value);
        s.Replace(_T(".0"), _T(""));
        return s;
        } /* .1 ohms */

    if(value < 1000)
        { /* ohms */
        CString s;
        s.Format(_T("%3.0fΩ"), value);
        return s;
        } /* ohms */

    if(value < 1000000)
        { /* k ohms*/
        CString s;
        if(value < 10000)
            s.Format(_T("%3.1fkΩ"), (double) value / 1000.0);
        else
            s.Format(_T("%3.0fkΩ"), (double) value / 1000.0);
        return s;
        } /* k ohms */
   
        { /* M ohms*/
        CString s;
        if(value < 10000000)
            s.Format(_T("%3.1fMΩ"), (double) value / 1000000.0);
        else
            s.Format(_T("%3.0fMΩ"), (double) value / 1000000.0);
        s.Replace(_T(".0"), _T(""));
        return s;
        } /* M ohms */
    } // Ohms::toString

/****************************************************************************
*                       Volts::toString
* Result: CString
*     The current, displayed as A, mA or μA
*     < 0.000999:  nnn.nnμA
*     0.001-0.999: nnn.nnmA
*     >= 1.0: nnn.nA
****************************************************************************/
CString Volts::toString()
    {
    if(fabs(value) < 1.0E-6)
        { /* less than microvolt */
        return CString(_T("0")) + suffix;
        } /* less than microvolt */
    if(fabs(value) >= 1.0)
        { /* n.nV */
        CString s;
        s.Format(_T("%3.1f%s"), value, suffix);
        s.Replace(_T(".0"), _T(""));
        return s;
        } /* n.nV */

    if(fabs(value) >= 0.001)
        { /* mV */
        CString s;
        // TODO: if value is n.00mV, display as nmV
        s.Format(_T("%3.1fm%s"), value * 1000.0, suffix);
        s.Replace(_T(".00"), _T(""));
        return s;
        } /* mV */

        { /* uV */
        CString s;

        s.Format(_T("%3.1fμ%s"), value * 1E6, suffix);
        s.Replace(_T(".0"), _T(""));
        return s;
        } /* uV */
    } // Volts::toString

/***************************************************************************
****************************************************************************/

/***************************************************************************
*                         Farads::toString
* Result: CString
*     0.000000000001 - 0.000000000999  *10E12 xpF
*     0.000000001    - 0.000000999     *10E9  xnF 
*     0.1            - 0.000999        *10E6  xμF
*     >= 1.0                           *1     xF
****************************************************************************/

CString Farads::toString()
    {
     if(value < 1E-9)
         { /* pf */
         CString s;
         s.Format(_T("%3.0fpF"), value * 1E12);
         return s;
         } /* pf */

     if(value < 1E-6)
         { /* nF */
         CString s;
         s.Format(_T("%3.0fnF"), value * 1E9);
         return s;
         } /* nF */

     if(value < 1.0)
         { /* uF */ 
         CString s;
         s.Format(_T("%3.0fμF"), value * 1E6);
         return s;
         } /* uF */
     
     { /* F */
     CString s;
     s.Format(_T("%3.0fF"), value);
     return s;
     } /* F */
    } // Farads::toString

    /***********************************************************************************
    *                     The wattage decode table
    * Notes:
    *    The purpose of this table is to render the wattage required for the resistor
    *    as a printable string
    ************************************************************************************/
    typedef struct { const double W; const TCHAR * s; } Wattages;

    static Wattages WattStrings[] = {
            //{1.0 / 20.0 , _T("1/20")},
            //{1.0 / 10.0,  _T("1/10")},
            {1.0 / 8.0,   _T("⅛")},
            {1.0 / 4.0,   _T("¼")},
            {1.0 / 2.0,   _T("½")},
            {1.0,         _T("1")},
            {2.0,         _T("2")},
            {5.0,         _T("5")}
        };

/***********************************************************************************
*                         Watts::GetSafeWatts
* Inputs:
*     double W: The wattage used by the circuit.  1W is 1.0.
* Result: CString
*     A printable string for the wattage
*     If W is 0.0, the empty string is returned
*     If W exceeds the largest value in the table, ">" that value is returned
* Notes:
*     TODO: if we are too close to the value of the wattage, select the next
*     higher wattage, for some value of "too close"
***********************************************************************************/
static const double TooCloseWattage = 0.9;

/* static */ CString Watts::GetSafeWatts(double W)
    {
    if(W == 0.0)
        return CString(_T(""));
    if(W / WattStrings[0].W < TooCloseWattage)
        { /* small */
        CString s;
        s.Format(_T("%3.2f$"), W);
        s.Replace(_T("0$"), _T("$"));
        s.Replace(_T("0$"), _T("$"));
        s.Replace(_T(".$"), _T("$"));
        s.Replace(_T("$"), _T(""));
        if(s == CString("0"))
            s = CString(_T(""));
        if(s[0] == _T('0'))
            { /* 0.xx */
            s.Replace(_T("0."), _T("."));
            } /* 0.xx */
        return s;
        } /* small */
    for(int i = 0; i < (int) _countof(WattStrings); ++i)
        { /* search */
        if(W > WattStrings[i].W)
            continue;
        if(W / WattStrings[i].W > TooCloseWattage)
            continue;
        return CString(WattStrings[i].s);
        } /* search */
    return CString(_T(">") + CString(WattStrings[_countof(WattStrings) - 1].s));
    }
/******************************************************************************
*                           Watts::toString
* Inputs:
*     double W: The wattage to show
* Result: CString
*     a string translating the watts into a "printable" value
******************************************************************************/

CString Watts::toString()
    {
        CString s = GetSafeWatts(value);
        if(s.IsEmpty())
            return s;
        s += _T("W");
        return s;
    } // Watts:: toString


/*****************************************************************************
*                             Value::draw
* Inputs:
*    CDC & dc:
*    CPoint pt: Place to draw it
*    UINT align: TA_ flags for the alignment
*    int height: The height of the font
* Result: void
* 
* Effect:
*     Draws the text at the specified position
******************************************************************************/

/* static */ const CString Value::DefaultTextFont(_T("Arial Unicode MS"));

int Value::draw(CDC & dc, CPoint pt, UINT align, int height)
    {
    int n = dc.SaveDC();
    dc.SetTextAlign(align);
    dc.SetBkMode(TRANSPARENT);
    SelectFont(dc, height);
   
    CString s = toString();
    dc.TextOut(pt.x, pt.y, s);
    dc.RestoreDC(n);
    return abs(height);
    }