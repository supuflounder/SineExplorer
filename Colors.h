/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
class Colors {
public:
    static const COLORREF DCurve = RGB(0, 128, 0);         // The Class-D input curve
    static const COLORREF Background = RGB(255, 255, 255); // The background
    static const COLORREF Analog = RGB(255, 0, 0);         // The sine curve
    static const COLORREF Step = RGB(0, 255, 0);           // The step approximation
    static const COLORREF Samples = RGB(0, 255, 0);
    static const COLORREF ClassDZero = RGB(255, 0, 0);     // The 0-axis line for Class D
    static const COLORREF Fiducial = RGB(0, 255, 0);
    static const COLORREF Base = RGB(0, 128, 0);
    static const COLORREF Sum = RGB(0, 128, 128);
    static const COLORREF Diff = RGB(128, 0, 128);
    static const COLORREF Filtered = RGB(0, 0, 255);
    static const COLORREF Regulated = RGB(255, 0, 255);
    static const COLORREF Error = RGB(255, 0, 0);
    static const COLORREF Schmitt = RGB(0, 0, 192);
    static const COLORREF PWM = RGB(128, 0, 128);
    static const COLORREF Stats = RGB(0, 128, 0);
    static const COLORREF Mark = RGB(0, 0, 255);
    static const COLORREF Flash = RGB(0, 0, 0);
    static const COLORREF TickStarting = RGB(0, 255, 0);
    static const COLORREF TickCaution = RGB(255, 255, 0);
    static const COLORREF TickOut = RGB(255, 0, 0);
    static const COLORREF Wire = RGB(0, 0, 0);
    static const COLORREF FlashWire = RGB(255, 255, 255);
    static const COLORREF Zero = RGB(0, 0, 0);
    static const COLORREF FlashZero = RGB(255, 255, 255);
};