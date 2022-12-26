/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#include "pch.h"
#include "ADCStats.h"
#include "value.h"
#include "Colors.h"
#include "GDIFLUSH.h"
#include "map.h"

static const int ArrowHeight = 30; // total height of arrow including head
static const int ArrowWidth = 9;   // should be an odd number
static const int ArrowDepth = 10;  // depth of arrowhead, in pixels
/*********************************************************
*        -->|   |<--Width
*           |  _|________________
*           | * |          ^  ^
*           |***|    Depth |  |
*           *****-----------  |
*             *             Height
*             *               |
*             *               |
*             *----------------
***********************************************************/

int ADCStats::lineCount()
{
    int count = 0;
    if (Percent)
        count++;
    if (Dec)
        count++;
    if (Hex)
        count++;
    if (Volts)
        count++;
    return count;
}

/********************************************************************
*                       ADCStats::Arrow
* Inputs:
*     int x: Horizontal position of arrow
*     int y0: Start of arrow
*     int y1: End of arrow (the end with the pointy on it)
*  Result: void
*  Effect:
*     Draws a vertical arrow from x,y0 to x,y1 with the arrowhead
*     on y1.  If -y0 < -y1, the arrowhead is pointing
*     If -y0 > -y1, the arrowhead is pointing 
*     Note that this is because the coordinates are y-increasing-down
********************************************************************/
void ADCStats::Arrow(CDC & dc, int x, int y0, int y1)
//                                    ------------->
{
    int n = dc.SaveDC();
    int Ty0 = y0; // Transformed coordinates
    int Ty1 = y1;

    CPen arrowPen(PS_SOLID, 1, Colors::Stats);
    dc.SelectObject(arrowPen);
    if(debug)
       { /* debug */
        int n = dc.SaveDC();
        static const int MarkSize = 10;
        Value::SelectFont(dc);
        dc.SetTextColor(Colors::Mark);
        dc.SetBkMode(TRANSPARENT);

        CPen mark(PS_DOT, 1, Colors::Mark);
        dc.SelectObject(mark);

        int textOffset;
        int align;
        if (Ty0 < Ty1)
           {
            textOffset = MarkSize;
            align = TA_LEFT;
           }
        else
           {
            textOffset = - MarkSize;
            align = TA_RIGHT;
           }
        dc.SetTextAlign(align);

        dc.MoveTo(x - MarkSize, Ty0);
        dc.LineTo(x + MarkSize + 1, Ty0);
        dc.MoveTo(x, Ty0 + MarkSize);
        dc.LineTo(x, Ty0 - MarkSize);
        dc.TextOut(x + textOffset, Ty0, _T("0"));
        CString s;
        s.Format(_T("%d"), Ty0);
        dc.TextOutW(x + textOffset, Ty0 - MarkSize, s);

        GDIFLUSH();

        CPen mark2(PS_DASH, 1, Colors::Mark);
        dc.SelectObject(mark2);

        dc.MoveTo(x - MarkSize, Ty1);
        dc.LineTo(x + MarkSize + 1, Ty1);
        dc.MoveTo(x, Ty1 + MarkSize);
        dc.LineTo(x, Ty1 - MarkSize);
        dc.TextOut(x + textOffset, Ty1, _T("1"));
        s.Format(_T("%d"), Ty1);
        dc.TextOutW(x + textOffset, Ty1 - MarkSize, s);
        
        GDIFLUSH();

        dc.RestoreDC(n);
       } /* debug */

    dc.MoveTo(x, Ty0);
    dc.LineTo(x, Ty1);
    GDIFLUSH();
    /***************************************************
    * The value Ty1 represents the pointy end of the arrow
    * 
    *          *  <--- x,Ty1      x,Ty0
    *         ***                  |
    *  x0,Ty2*****x1.Ty2           |
    *          |                   |
    *          |           x0,Ty2*****x1,Ty2
    *          |                  ***
    *        x,Ty0                 *  <-- x,Ty1
    ***************************************************/


    dc.BeginPath();
    int x0 = x - ArrowWidth / 2;
    int x1 = x + ArrowWidth / 2 + 1;
    

    if (Ty1 > Ty0)
    { /* points down */
        int Ty2 = (Ty1 - ArrowDepth);
        dc.MoveTo(x, Ty1);
 
        dc.LineTo(x0,Ty2);
        dc.LineTo(x1, Ty2);
        dc.LineTo(x, Ty1);
    } /* points down */
    else
       { /* points up */
        int Ty2 = (Ty1 + ArrowDepth);
        dc.MoveTo(x, Ty1);
        dc.LineTo(x0, Ty2);
        dc.LineTo(x1, Ty2);
        dc.LineTo(x, Ty1);
    } /* points up */
    dc.EndPath();

    CBrush br(Colors::Stats);
    dc.SelectObject(br);

    dc.FillPath();

    GDIFLUSH();

    dc.RestoreDC(n);
}
/****************************************************************************
*       ADCStats::ShowCorners
* Inputs:
*     CDC & dc: DC to draw on
* Result: void
* 
* Effect:
*     Draws the coordinate values in the top left and lower right corners
*****************************************************************************/
void ADCStats::ShowCorners(CDC & dc)
{
    if (!debug)
        return;

    int n = dc.SaveDC();

        CRect r;
        wnd->GetClientRect(&r);
        dc.DPtoLP(&r);

    
        CString s;
        dc.SetTextAlign(TA_TOP | TA_LEFT);
        s.Format(_T("(%d, %d)"), r.left, r.top);
        dc.TextOutW(r.left, r.top, s);
        s.Format(_T("(%d, %d)"), r.right, r.bottom);
        dc.SetTextAlign(TA_BOTTOM | TA_RIGHT);
        dc.TextOutW(r.right, r.bottom, s);

    dc.RestoreDC(n);
}
/********************************************************************
*                            ADCStats::IsUpArrow
*  Inputs:
*       CDC & dc: DC to use
*       int y: Y-position, in logical units 
*       int height: Text height, in device units
*  Result: bool
*       true if the arrow should point up
*       false if the arrow should point down
* 
*********************************************************************/

bool ADCStats::IsUpArrow(CDC & dc, int y, int height)
{

    CRect r;
    wnd->GetClientRect(&r);
    dc.DPtoLP(&r);
    /******************************************************
    * Remembering that negative coordinates go up and positive
    * coordinates go down, we might have something like
    * (0,-112)___________________________________________________
    * 
    *                                       
    *                        ______(x,53)
    * (0,112)____________________________________________________
    ******************************************************/
    //         ________________ r.top - ArrowHeight - textHeight                |         
    //    nn.n%      ^                                                          |
    //    nnnn       |  textheight                                              |
    //    0xdddd     |                                                          | increasing y values
    //    n.nnV______v_________ r.top - ArrowHeight (y0)                        |
    //    ┃                                                                     |
    //    ▼________ r.mid (y)                                                   V

    if (y - (height + ArrowHeight) < r.top)
        return true;

    //   _____________________________ (r.mid) (y)                          |
    //  ▲           ^ arrowHeight                                           |
    //  ┃    _______V_________________ (r.mid  + ArrowHeight) (y0, texty)   |
    // nn.n%        ^                                                       |increasing y values
    // nnnn         |                                                       |
    // 0xdddd       | Text Height                                           |
    // n.nnV _______V___(r.mid + arrowHeight + textHeight)                  V

    if (y + (height + ArrowHeight) > r.bottom)
        return false;

    // Note that one of the two above should always be satisfied
    // But if for some incomprehehsible reason we end up here...
    return true;
}
/***********************************************************************
*                       ADCStats::MarkText
* Inputs:
*     CDC & dc: DC to use
*     CSize sz: The size of the text
*     int x: The center position
*     int y: The vertical position
***********************************************************************/

void ADCStats::MarkText(CDC& dc, CSize sz, int x, int y)
{
    if (!debug)
        return;

    int n = dc.SaveDC();
        CPen markPen(PS_DASHDOT, 1, Colors::Mark);
        dc.SelectObject(markPen);
        dc.MoveTo(x - sz.cx / 2, y);
        // int end = x + 2 * sz.cx;
        int end = x + 80;
        dc.LineTo(end, y); 
        dc.SetTextColor(Colors::Mark);
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextAlign(TA_BASELINE);
        CString s;
        s.Format(_T("%d"), y);
        CPoint pt = dc.GetCurrentPosition();
        dc.TextOutW(pt.x, y, s);
    dc.RestoreDC(n);
}

/*********************************************************************
*                            ADCStats::ShowStatText
* Inputs:
*     CDC & dc: Device context
*     int x: Position of arrow end
*     int y: Vertical text position
*     const CString & s: String to print
**********************************************************************/
int ADCStats::ShowStatText(CDC & dc, int x, int y, const CString& s, int max_x)
{
    CRect r;
    wnd->GetClientRect(&r);
    dc.DPtoLP(&r);
    static const int BORDER = 3;

    int n = dc.SaveDC();
        dc.SetTextAlign(TA_CENTER);  // assume centered

       
        if (x - (max_x / 2) < r.left + BORDER)
            dc.SetTextAlign(TA_LEFT);
        else
            if (x + (max_x / 2) > r.right - BORDER)
                dc.SetTextAlign(TA_RIGHT);

        dc.TextOutW(x, y, s);

        CSize sz = dc.GetTextExtent(s);
        MarkText(dc, sz, x, y);
    dc.RestoreDC(n);
    return sz.cy;
}
/*********************************************************************
*                            ADCStats::ShowText
* Inputs:
*    CDC & dc: Current DC, already mapped to logical points
*    int x: X-coordinate of center of text in logical points
*    int y: Y-coordinate of center of text in logical points
* Result: void
* 
* Effect:
*     Writes out the selected lines of text, starting at y and
*     proceeding downward
*          .  (x, y)
*        xx.xx%
*        nnnn
*        0xnnnn
*        n.nnV
*  It is base on the booleans which are in turn based on the user
*  selction
* 
* In addition, the invalidation rectangle is updated
*********************************************************************/
void ADCStats::ShowText(CDC& dc, int x, int y)
{
    int n = dc.SaveDC();
    dc.SetTextAlign(TA_CENTER);
    dc.SetBkMode(TRANSPARENT);

    // We compute all the strings here so we can handle the max width properly
    int max_x = 0;

    CString pct_s;

    if (Percent)
        { /* Percent string*/
         double adcval = ADCValue;
         pct_s.Format(_T("%4.2f%%"), adcval);
         CSize sz = dc.GetTextExtent(pct_s);
         max_x = max(max_x, sz.cx);
        } /* Percent string */

    CString dec_s;
    if (Dec)
       { /* decimal string */
        int val = (int)map(ADCValue, 0.0, 100.0, 0.0, (double)ADCRange);
        CString s;
        dec_s.Format(_T("%d"), val);
        CSize sz = dc.GetTextExtent(dec_s);
        max_x = max(max_x, sz.cx);
       } /* Decimal string*/

    CString hex_s;
    if (Hex)
       { /* hex string */
        unsigned short val = (unsigned short)ADCValue;
        CString s;
        hex_s.Format(_T("0x%04x"), val);
        CSize sz = dc.GetTextExtent(hex_s);
        max_x = max(max_x, sz.cx);
       } /* hex string*/

    CString volt_s;
    if (Volts)
        {
         double volts = map((double)ADCValue, 0.0, 100.0, 0.0, ADCAref);
         CString s;
         volt_s.Format(_T("%s"), Volts::Volts(volts).toString());
         CSize sz = dc.GetTextExtent(volt_s);
         max_x = max(max_x, sz.cx);
        }

    // Now, actually print out the strings
    if(Percent)
       { /* percent */
        y += ShowStatText(dc, x, y, pct_s, max_x);
       } /* percent */

    if(Dec)
       { /* Dec */
        y += ShowStatText(dc, x, y, dec_s, max_x);
       } /* Dec */

    if(Hex)
       { /* hex */
        y += ShowStatText(dc, x, y, hex_s, max_x);
       } /* hex */

    if(Volts)
       { /* volts */
        y += ShowStatText(dc, x, y, volt_s, max_x);
       } /* volts */

    // Now add a final mark
    CSize sz = dc.GetTextExtent(CString(_T("XXXXXX")));
    MarkText(dc, sz, x, y);

    dc.RestoreDC(n);
}
/********************************************************************
*                             ADCStats::Draw
* Inputs:
*     CDC & dc: The DC to use
* Result: void
* Effect:
*    Draws the statistics for the line
* 
*            +-----+         
*            |  ▲    ...   1.36%    
*      +-----+  ┃           13
*      |       95%        0x000d
* -----+       973         67.9mV
*             0x03cc         ┃
*             4.8V        |  ▼
*                         +----+     +-----+
*                              +-----+
*********************************************************************/

void ADCStats::Draw(CDC& dc)
{
    if (!valid)
        return;  // nothing to do

    int n = dc.SaveDC(); 
    Value::SelectFont(dc, Value::DefaultFontHeight);
    dc.SetTextColor(Colors::Stats);
    
    ShowCorners(dc);

    int count = lineCount();

    // Determine the height of each line
    CString s(_T("X"));
    CSize sz = dc.GetTextExtent(s);

    // Now determine the height of the entire text block
    int textHeight = count * sz.cy;
    CRect r = b.GetRect();

    // Now, decide whether the arrow should point from the top to the bottom, or the bottom to the top
 
    int x = r.left + ((r.right - r.left) / 2);
    int texty = r.bottom - ((r.top - r.bottom) / 2);

    if (IsUpArrow(dc, texty, textHeight))
        { /* up */
            //   _____________________________ (r.bottom) (y1)                      |
            //  ▲           ^ arrowHeight                                           |
            //  ┃    _______V_________________ (r.rottom  + ArrowHeight) (y0, texty)|
            // nn.n%        ^                                                       |increasing y values
            // nnnn         |                                                       |
            // 0xdddd       | Text Height                                           |
            // n.nnV _______V___(r.bottom + textHeight)                             V
         texty = r.bottom + ArrowHeight;
         int y = texty;
         Arrow(dc, x,   // x0
                   y,   // y0           | (arrowtail)
                   r.bottom);  // y1    V (arrowhead)
        } /* up*/
    else
        { /* down*/ 
         //         ________________ r.top - ArrowHeight - textHeight                |         
         //    nn.n%      ^                                                          |
         //    nnnn       |  textheight                                              |
         //    0xdddd     |                                                          | increasing y values
         //    n.nnV______v_________ r.top - ArrowHeight (y0)                        |
         //    ┃                                                                     |
         //    ▼________ r.top (y1)                                                  V
          Arrow(dc, x,
                    r.top - ArrowHeight,   // y0   | (arrowtail)
                    r.top);                // y1   V (arrowhead)
          texty = r.top - ArrowHeight - textHeight;
         } /* down */
    ShowText(dc, x, texty);
    dc.RestoreDC(n);
}