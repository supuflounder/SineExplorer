/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
#include "BoxInfo.h"
class ADCStats {
    BoxInfo b;
    int Yzero; // Place in box where zero-line is
    //CRect WR;  // 
    // Things to put out
    bool Percent;
    bool Dec;
    bool Hex;
    bool Volts;
    bool valid;
    bool debug;
    double ADCAref;
    int ADCRange;
    double ADCValue;
    CWnd* wnd;
    CRect invalidation;  // TODO: if too much flicker compute this value to reduce it

    int lineCount();
    void Arrow(CDC& dc, int x0, int y0, int y1);
    bool IsUpArrow(CDC& dc, int y, int height);
    void ShowText(CDC& dc, int x, int y);
    void MarkText(CDC& dc, CSize sz, int x, int y);
    void ShowCorners(CDC& dc);
    int ShowStatText(CDC & dc, int x, int y, const CString & s, int max_x);
    int range;
public:
    ADCStats() {
        Percent = false;
        Dec = false;
        Hex = false;
        Volts = false;
        valid = false;
        ADCAref = 0.0;
        ADCRange = 1;
        ADCValue = 0.0;
        debug = true;
        wnd = NULL;
        invalidation = CRect(-1, -1, -1, -1);
    }

    void SetDebug(bool val) { debug = val; }
    void SetShowPercent(bool b) { Percent = b; }
    void SetShowDec(bool b) { Dec = b; }
    void SetShowHex(bool b) { Hex = b; }
    void SetShowVolts(bool b) { Volts = b; }
    void SetADCRange(int val) { ADCRange = val; }
    void SetADCAref(double val) { ADCAref = val; }
    void SetADCValue(double val) { ADCValue = val; }
    void SetWindow(CWnd* w) { wnd = w;  }
#if 0 // obsolete
    ADCStats(BoxInfo* b, bool Percent, bool Dec, bool Hex, bool V)
    {
        this->Percent = Percent;
        this->Dec = Dec;
        this->Hex = Hex;
        this->V = V;
        this->b = *b;
        valid = true;
    }
#endif // obsolete

#if 0 // obsolete
    bool equals(ADCStats* e) const
    {
        return this->b.equals(e->b) &&
            this->Percent != e->Percent &&
            this->Dec != e->Dec &&
            this->Hex != e->Hex &&
            this->V != e->V;
    }
#endif // obsolete

public:
    void Draw(CDC & dc);
    void SetBoxInfo(BoxInfo * b) { this->b = *b;  }
    BoxInfo * GetBoxInfo() { return & b; }
    void SetValid(bool b) { valid = b; }
    bool GetValid() const { return valid;  }
};