/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once

/*****************************************************************
* This moodule computes voltage values based on the RC time constants
*****************************************************************/

class RC {
protected:
    double R; // resistance of load
    double C; // capacitance in uF
    double V; // voltage in C at the moment
    double T; // current time, in sec
    double Vs; // Initial voltage when discharge begins
public:
    RC() { R = 0.0; C = 0.0; Reset(); }
    void SetR(double res) { R = res; }
    void SetC(double cap) { C = cap; }
    void SetV(double volts, double dT);
    double GetV() { return V; }
    void Reset() {V = 0.0; Vs = 0.0; T = 0.0; }
    };