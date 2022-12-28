/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
#include "Circuit.h"
#include "update.h"
#include "Value.h"

class CircuitWindow : public CStatic {
protected:
    void ACSourceOnly(CDC & dc);
    void FullWave(CDC & dc);
    void HalfWave(CDC & dc);
    void DrawFrame(CDC & dc);
    void SchmittCircuit(CDC & dc);
    ACSource* DrawSource(CDC & dc);
    void SetTextColor(CDC & dc);

public:
    CircuitWindow() {
        ViSafe = false;
        ViMax = 5.0;
        Vi = 8.9;
        Vf = Diode::DefaultVf;
        flashing = false;
    }
    void Flash();
    double GetCapacitorVoltage(double V);
    typedef enum {AConly, Halfwave, Fullwave, Schmitt} Configuration;
    void SetConfiguration(Configuration c)  { update(this, config,         c); }
    void SetShowFiltered(bool val)          { update(this, filtered,       val); }
    void SetShowRegulator(bool val)         { update(this, showRegulator,  val); }
    void SetShowValues(bool val)            { update(this, ShowValues,     val); }
    void SetInputVoltage(double val)        { update(this, InputVoltage,   val); }
    void SetOutputVoltage(double val)       { update(this, OutputVoltage,  val); }
    void SetCapacitance(double val)         { update(this, Capacitance,    val); }
    void SetResistance(double val)          { update(this, Resistance,     val); }
    void SetSchmittInvert(bool val)         { update(this, Invert,         val); }
    void SetSchmittOnThreshold(double val)  { update(this, Ton,            val); }
    void SetSchmittViSafe(bool val)         { update(this, ViSafe,         val); }
    void SetSchmittVi(double val)           { update(this, Vi,             val); }
    void SetSchmittViMax(double val)        { update(this, ViMax,          val); }
    void SetVf(double val)                  { update(this, Vf,             val); }
protected:
    Configuration config;
    bool filtered;
    bool flashing;
    bool ShowValues;
    // These are for the voltage regulator
    bool showRegulator;
    double InputVoltage;
    double OutputVoltage;
    // end of voltage regulator

    double Capacitance;
    double Resistance;

    double Vf; // diode forward voltage drop
    // These are for the Schmitt trigger circuit
    bool ViSafe;   // we want a voltage divider circuit
    bool Invert; // for Schmitt configuration
    double Vi;   // input voltage
    double ViMax; // max imput voltage
    double Vt; // min on- trigger voltage
    double Ton; // min on-trigger % as a double (e.g., 0.67 for 67%)
    // end of Schmitt trigger values

    double VDivide(double Vi, int R1, int R2) const;
    bool FindR(CDC & dc, int y, double ViMax, double Vi, double Vt, int& R1, int& R2);
    int ShowValue(CDC & dc, CPoint pt, Value & val, UINT position = TA_BOTTOM | TA_CENTER);
    void AnnotateCapacitor(CDC & dc, Capacitor & c, double Vin);
    void ShowX(CDC& dc, CPoint pt, UINT align = TA_TOP | TA_CENTER);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

    };
