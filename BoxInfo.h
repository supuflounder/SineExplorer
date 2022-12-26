/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
/********************************************************************************
*                             class BoxInfo
* This class defines a representation of the step functions of an ADC wave
* It is used for a variety of purposes to allow the discovery and display of the
* relevant data about the ADC approximation
* 
* It contains the bounding rectangle of the line.
* For our purposes, we care primarily about the left and right coordinates
* which allows the determination that the mouse is within the vertical bounds of
* the rectangle
* It is also used by ADCStats to decide where to place the output of the statistics
* 
* The predicate InBox is used to determine if the mouse is inside the points of
* the left and right margins of the box, and return the box pointer if it is
* within the limits and NULL if it is not.  The points are in the window coordinates
* of the graph and must be converted to window coordinates from the mouse screen coordinates
* before calling the InBox function
********************************************************************************/
class BoxInfo {

protected:
    CRect r;
    double value;
public:
    BoxInfo() { r.SetRect(0, 0, 0, 0); value = 0.0; }
    BoxInfo(CRect r, double d) { this->r = r; this->value = d; }
    BoxInfo * InBox(int x, int y) { return InBox(CPoint(x, y)); }
    BoxInfo * InBox(CPoint pt)
        {
         if(pt.x >= r.left && pt.x < r.right)
            return this;
        else
            return NULL;
        }
    const CRect & GetRect() const { return r; }
    double GetValue() { return value; }
    bool equals(BoxInfo & b) const { return equals(&b); }
    bool equals(BoxInfo * b) const {
        return this == b ||
               (this->r.top == b->r.top &&
                this->r.left == b->r.left &&
                this->r.right == b->r.right &&
                this->r.bottom == b->r.bottom &&
                this->value == b->value);
              }
    };