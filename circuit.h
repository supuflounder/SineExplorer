/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
#include "Colors.h"

class Circuit {

    };



class Element {
public:
    Element(CWnd* w) { wnd = w; flashing = false;  }
    Element(CWnd* w, bool flashing) { wnd = w; this->flashing = flashing; }
    CWnd * wnd; 
    CPoint origin;
    virtual void draw(CDC& dc) PURE;
    CArray<CPoint> points;
    bool flashing;
    void ShowConnection(CDC & dc, CPoint pt, COLORREF col);
    void ShowConnections(CDC & dc, COLORREF col);
    void DebugConnection(CDC & dc, CPoint pt); 
    void DebugConnections(CDC & dc);
    void Connect(CDC & dc, CPoint pt, bool flashing);
    CPoint GetP(int n) const { if(points.GetSize() >= n + 1) return points[n]; else return CPoint(-1, -1); }
    int distance(CPoint p0, CPoint p1) const;
    void rotate(CDC & dc, CPoint start, CPoint end, XFORM * M = NULL);
    static void GDIFlash(CDC & dc, CPoint pt);

    class FlashPen : public CPen {
        public:
        FlashPen(bool flashing) { CPen::CreatePen(PS_SOLID, 1, flashing ? Colors::FlashWire : Colors::Wire);  }
        };
    class FlashBrush : public CBrush {
    public:
        FlashBrush(bool flashing) { CBrush::CreateSolidBrush(flashing ? Colors::FlashWire : Colors::Wire);
        }
       };
}; // class Element

/*********************************************************************************
*                          class ACSource
* 
* An AC source
*********************************************************************************/

class ACSource : public Element {
public:
    ACSource(CWnd* w, bool flashing) : Element(w, flashing) { }
    virtual void draw(CDC & dc);
    };

/**********************************************************************************
*                          class Orientation
* 
* The orientation of a line segment
***********************************************************************************/
enum class Orientation { Horizontal, Vertical, Any };
//public:
//     enum class Direction { Horizontal, Vertical, Any };
//    };

/***********************************************************************************
*                           class SimpleWire
* 
* A single straight wire
***********************************************************************************/
class SimpleWire : public Element {
public:
    SimpleWire(CWnd * w, CPoint p, int len, bool flashing, Orientation or=Orientation::Horizontal) : Element(w, flashing)
        {
        pt = p;
        orient = or;
        length = len;
        }
    SimpleWire(CWnd * w, CPoint s, CPoint e, bool flashing) : Element(w, flashing)
        {
        if(s.y == e.y)
            { /* horizontal */ 
            pt = s;
            orient = Orientation::Horizontal;
            length = e.x - s.x;
            } /* horizontal */
        else
        if(s.x == e.x)
            { /* vertical */
            pt = s;
            orient = Orientation::Vertical;
            length = e.y - s.y;
            } /* vertical */
        else
            { /* any */
            pt = s;
            orient = Orientation::Any;
            length = distance(s, e);
            }
        }
    CPoint  pt;
    Orientation orient;
    int length;

    virtual void draw(CDC & dc);
    };

/**************************************************************************************
*                      class ComplexWire
* 
* A wire that has a 90-degree bend in it
**************************************************************************************/
class ComplexWire : public Element {
protected:
    CPoint start;
    CPoint end;
    Orientation orient;
public:
    ComplexWire(CWnd* w, CPoint p0, CPoint p1, Orientation dir, bool flashing) : Element(w, flashing) { start = p0; end = p1; orient = dir;  }
    virtual void draw(CDC & dc);
    };

/****************************************************************************************
*                       class Diode
* 
* A simple diode
****************************************************************************************/
class Diode : public Element {
public:
    static const double DefaultVf; 
    Diode(CWnd * w, CPoint start, CPoint end, bool flashing, double Vf = DefaultVf) : Element(w, flashing)
        {
        this->start = start;
        this->end = end;
        this->Vf = Vf;
        }
    CPoint start;
    CPoint end;

    // Note that the "start" is always the anode and the "end" is always the cathode

    double GetVf() { return Vf; }

    virtual void draw(CDC & dc);
protected:
    double Vf;
    };

/******************************************************************************************
*                       class Capacitor
* 
* An electrolytic capacitor 
******************************************************************************************/
class Capacitor : public Element {
public:
    Capacitor(CWnd * w, CPoint start, CPoint end, bool flashing) : Element(w, flashing)
        {
        this->start = start;
        this->end = end;
        }
    CPoint start;
    CPoint end; // Note: Only the Y-value is used
    virtual void draw(CDC & dc);
    };

/*****************************************************************************************
*                        class FullWaveRectifier
* 
* A four-diode full-wave rectifier bridge
*****************************************************************************************/
class FullWaveRectifier : public Element {
public:
    FullWaveRectifier(CWnd * w, CPoint center, int size, bool flashing, double Vf = Diode::DefaultVf) : Element(w, flashing)
        {
        this->center = center;
        this->size = size;
        this->Vf = Vf; 
        }
    virtual void draw(CDC & dc);
    double GetVf() { return Vf; }
protected:
    CPoint center;
    int size;
    double Vf;
    };

/******************************************************************************************
*                       class Regulator
* 
* A classic 3-pin regulator, e.g., 7805
******************************************************************************************/
class Regulator : public Element {
public:
    Regulator(CWnd * w, CPoint start, CPoint end, CPoint ref, bool flashing, double V = 0.0) : Element(w, flashing)
        {
        this->start = start;
        this->end = end;
        this->ref = ref;
        this->V = V;
        }
    virtual void draw(CDC & dc);
    void SetV(double newV) { this->V = newV; }

protected:
    CPoint start;
    CPoint end;
    CPoint ref;
    double V;

    };
/*****************************************************************************************
*                    class Resistor
* 
* A classic resistor
*****************************************************************************************/
class Resistor : public Element {
public:
    Resistor(CWnd * w, CPoint start, CPoint end, bool flashing) : Element(w, flashing)
        {
        this->start = start;
        this->end = end;
        }
    virtual void draw(CDC & dc);

protected:
    CPoint start;
    CPoint end;
    };
/*****************************************************************************************
*                    class SchmittTrigger
*
* A classic Schmitt Trigger, inverting or non-inverting
*****************************************************************************************/
class SchmittTrigger : public Element {
public:
    SchmittTrigger(CWnd * w, CPoint start, CPoint end, bool invert, bool flashing) : Element(w, flashing)
    {
        this->start = start;
        this->end = end;
        this->invert = invert;
        //this->ViMax = ViMax;
        //this->Vi = Vi;
        //this->Vt = Vt; // max threshold voltage
    }
    virtual void draw(CDC& dc);
protected:
    CPoint start;
    CPoint end;
    bool invert;
    //double ViMax; // maximum input voltage
    //double Vi;    // actual input voltage
    //double Vt;    // maximum threshold voltage (max Von, Voff)

};