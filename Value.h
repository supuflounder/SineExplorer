/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
/*********************************************************************************
* This file converts values into suitably-scaled output strings.
* It handles Volts, Ohms, Amps, Farads and Watts    
*********************************************************************************/
#pragma once
class Value {
protected:
    double value;
    static const CString DefaultTextFont;

    CString TextFont;
public:
    Value(double d) { value = d; }
    virtual int draw(CDC & dc, CPoint pt, UINT align = TA_TOP | TA_LEFT, int height = DefaultFontHeight);
    virtual CString toString() PURE;
    void SetFont(CString f) { TextFont = f; }
    void SetValue(double d) { value = d; }
    double GetValue() const { return value; }
    static void SelectFont(CDC & dc, int height = DefaultFontHeight, const CString & TextFont = CString(_T("")));
    static const int DefaultFontHeight = 10;
    };

class Ohms : public Value {
public:
    Ohms(double d) : Value(d) { }
    virtual CString toString();
    };

class Amps : public Value {
public:
    Amps(double d) : Value(d) { }
    virtual CString toString();

    };

class Volts : public Value {
protected:
    CString suffix;
public:
    virtual CString toString();
    Volts(double d, CString & suf = CString(_T("V"))) : Value(d)
        {
        suffix = suf;
        }
    };

class Farads : public Value {
    public:
        Farads(double d) : Value(d) { }
        virtual CString toString();
    };

class Watts : public Value {
protected:
    static CString GetSafeWatts(double W);
public:
    Watts(double d) : Value(d) { }
    virtual CString toString();
    };

class Time : public Value {
public:
    Time(double d) : Value(d) { }
    virtual CString toString();
};