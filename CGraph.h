/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
#pragma once
#include <afxwin.h>
#include "boxinfo.h"
#include "RC.h"
#include "update.h"
#include "ADCStats.h"

class CGraph :
    public CStatic
    {
    protected:
        bool BadRegulation;
        CArray<BoxInfo> Boxes;
    protected: // Variables which can be access by setters and getters
        bool debug;

        ADCStats stats;
        double DiodeDrop;
        int dtheta;
        RC * filter;
        bool flashing;
        int Hz; // line frequency
        double InputVoltage;
        int nSteps;
        int nWaves;
        double OutputVoltage;
        int Phase;
        int PWMWidth; // 0..100%
        UINT Rectification;
        double RegulatorDrop;
        bool showADCPercent;
        bool showADCDec;
        bool showADCHex;
        bool showADCVolts;
        bool sineDashed;
        bool showMarks;
        double ADCAref;   // ADC reference voltage
        int ADCRange;     // ADC range, 256, 1024, 2048, 65536, etc.
        double ADCValue;  // ADC value as a signed value above or below zero
        UINT ZeroPos;
        
        /****************************************************************************
        * class Schmitt
        ****************************************************************************/
        class Schmitt {
        protected:
            bool enabled;
            double on;    // On threshold, as a decimal number, e.g., 67% = 0.67
            double off;   // Off threshold, as a decimal number, e.g., 33% = 0.33
            bool invert;
            double ViMax;
            double Vi; // actual input voltage
        public:
            Schmitt() { enabled = false; 
                        invert = false;  
                        on = getOnDefault(); 
                        off = getOffDefault();
                        ViMax = 5.0;
                        Vi = 5.0;
                       }
            void setEnable(bool v) { enabled = v; }
            bool getEnable() const { return enabled; }
            bool getInvert() const { return invert;  }
            void setInvert(bool b) { invert = b; }
            void setOn(double d) { on = d; }
            double getOn() const { return on; }
            void setOff(double d) { off = d; }
            double getOff() const { return off; }
            double getOnDefault() const { return 0.667; }
            double getOffDefault() const { return 0.333; }
            void setViMax(double v) { ViMax = v; }
            double getViMax() const { return ViMax; }
            void setVi(double v) { Vi = v;  }
            double getVi() const { return Vi; }
            
        } SchmittData;
        /*****************************************************************************/
        bool invertPWM;
        // Flags to indicate what should be shown
        bool showAxis;
        bool showBase; // true to show base waveform without phase shift
        bool showClassD;
        bool showDiff;
        bool showFiltered;
        bool showPWM;
        bool showRegulated;
        bool showRegulationFailure;
        bool showRMS;
        bool showSamples;
        bool showSine;
        bool showStep;
        bool showSum;  // shows the sum of the base waveform and the phase-shifted waveform

        void Plot(CDC& dc, CArray<CPoint> & pts, DWORD style, DWORD width, COLORREF color);
        void Note(CDC& dc, const CString& s, int x, int y, DWORD align);
        bool tracking;
        CPoint MousePoint; // mouse position in client coordinates (DP); valid only if tracking is true
    protected:
        void ShowSteps(CDC& dc, CArray<CPoint>& pts, int height, double max_y, double min_y);
    protected:
        DECLARE_MESSAGE_MAP()
        afx_msg void OnPaint();
    public:
        CGraph();
        BoxInfo * GetBoxInfo(CPoint pt);
        void Flash();

    public: // Setters
        void SetADCAref(double val)       { update(this, ADCAref,         val); }
        void SetADCRange(int val)         { update(this, ADCRange,        val); }
        //void SetADCValue(double val)      { update(this, ADCValue,        val); }
        void SetDashed(bool val)          { update(this, sineDashed,      val); }
        void SetDebug(bool val)           { update(this, debug,           val); }
        void SetDropout(double val)       { update(this, RegulatorDrop,   val); }
        void SetFilter(RC & f)            { filter = &f; }
        void SetHz(int freq)              { update(this, Hz,             freq); }
        void SetInvertPWM(bool invert)    { update(this, invertPWM,    invert); }
        void SetInputVoltage(double val)  { update(this, InputVoltage,    val); }
        void SetOutputVoltage(double val) { update(this, OutputVoltage,   val); }
        void SetPhase(int n)              { update(this, Phase,             n); }
        void SetPWMWidth(int n)           { update(this, PWMWidth,          n); }
        void SetRectification(UINT val)   { update(this, Rectification,   val); }
        void SetSchmittEnable(bool enable) 
           { 
            if(enable != SchmittData.getEnable()) 
                Invalidate(); 
            SchmittData.setEnable(enable); 
           }
        void SetSchmittOff(double v)      // Set the "off" threshold for the Schmitt trigger
           { 
            if(v != SchmittData.getOff()) 
                Invalidate(); 
             SchmittData.setOff(v);
           }
        void SetSchmittOn(double v)       // Set the "on" threshold for the Schmitt trigger
           { 
            if(v != SchmittData.getOn()) 
                Invalidate(); 
            SchmittData.setOn(v); 
           }
        void SetSchmittInvert(bool b)
            {
            if (b != SchmittData.getInvert())
                Invalidate();
                           
            SchmittData.setInvert(b);
            }
        void SetSchmittViMax(double v)
        {
            if (v != SchmittData.getViMax())
                Invalidate();
            SchmittData.setViMax(v);
        }
        void SetShowADCDec(bool val)    { update(this, showADCDec,     val); }
        void SetShowADCHex(bool val)    { update(this, showADCHex,     val); }
        void SetShowADCPercent(bool val){ update(this, showADCPercent, val); }
        void SetShowADCVolts(bool val)  { update(this, showADCVolts,   val); }
        void SetShowAxis(bool val)      { update(this, showAxis,       val); }
        void SetShowBase(bool val)      { update(this, showBase,       val); }
        void SetShowDiff(bool val)      { update(this, showDiff,       val); }
        void SetShowFiltered(bool val)  { update(this, showFiltered,   val); }
        void SetShowClassD(bool val)    { update(this, showClassD,     val); }
        void SetShowMarks(bool val)     { update(this, showMarks,      val); }
        void SetShowPWM(bool val)       { update(this, showPWM,        val); } 
        void SetShowRegulated(bool val) { update(this, showRegulated,  val); }
        void SetShowRegulationFailure(bool val) { update(this, showRegulationFailure, val); }
        void SetShowRMS(bool val)       { update(this, showRMS,        val); }
        void SetShowSamples(bool val)   { update(this, showSamples,    val); }
        void SetShowSine(bool val)      { update(this, showSine,       val); }
        void SetShowStep(bool val)      { update(this, showStep,       val); }
        void SetShowSum(bool val)       { update(this, showSum,        val); }
        void SetSteps(int val)          { update(this, nSteps,         val); }
        void SetTheta(int val)          { update(this, dtheta,         val); }
        void SetWaves(int val)          { update(this, nWaves,         val); }
        void SetZeroPos(UINT val)       { update(this, ZeroPos,        val); }
    public: // Getters
        bool GetBadRegulation() const { return BadRegulation; }
        bool GetDashed() const { return sineDashed; }
        bool GetDebug() const { return debug; }
        double GetDropout() const { return RegulatorDrop; }
        int GetHz() const { return Hz; }
        int GetPhase() const { return Phase; }
        UINT GetRectification() const { return Rectification; }
        double GetRegulatorVoltage() const { return OutputVoltage;  }
        double GetSchmittOff() const { return SchmittData.getOff(); }
        double GetSchmittOn() const { return SchmittData.getOn(); }
        double GetSchmittOffDefault() const { return SchmittData.getOffDefault(); }
        double GetSchmittOnDefault() const { return SchmittData.getOnDefault(); }
        bool GetShowAxis() { return showAxis; }
        bool GetShowBase() const { return showBase; }
        bool GetShowClassD() const { return showClassD; }
        bool GetShowDiff() const { return showDiff; }
        bool GetShowFiltered() const { return showFiltered; }
        bool GetShowRegulated() const { return showRegulated; }
        bool GetShowRegulationFailure() const { return showRegulationFailure;  }
        bool GetShowRMS() const { return showRMS; }
        bool GetShowSamples() const { return showSamples; }
        int  GetShowSine() const { return showSine; }
        bool GetShowStep() const { return showStep; }
        bool GetShowSum() const { return showSum; }
        int GetSteps() const { return nSteps; }
        int GetTheta() const { return dtheta; }
        double GetTransformerVoltage() const { return InputVoltage; }
        int GetWaves() const { return nWaves; }
        UINT GetZeroPos() const { return ZeroPos; }
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnMouseLeave();
};

