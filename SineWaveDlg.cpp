
// SineWaveDlg.cpp : implementation file
//
/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/

#include "pch.h"
#include "framework.h"
#include "SineWave.h"
#include "SineWaveDlg.h"
#include "afxdialogex.h"
#include "map.h"
#include "Value.h"
#include "degrees.h"
#include "toclip.h"
#include "CSpinner.h"
#include "DelayDone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const double Capacitors[] = {
    1.0,
    1.5,
    2.2,
    3.3,
    4.7,
    6.8,
    8.2,
    10.0,
    15.0,
    22.0,
    33.0,
    47.0,
    68.0,
    82.0,
    100.0,
    150.0,
    220.0,
    330.0,
    470.0,
    680.0,
    820.0,
    1000.0,
    1500.0,
    2200.0,
    3300.0,
    4700.0,
    6800.0,
    8200.0,
    10000.0
    };



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSineWaveDlg dialog



SineWaveDlg::SineWaveDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_APPLICATIONSTEP2_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    waiting = false;
    initialized = false;

}

void SineWaveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_GRAPH, c_Graph);
    DDX_Control(pDX, IDC_SHOWSINE, c_ShowSine);
    DDX_Control(pDX, IDC_SHOWSTEP, c_ShowStep);
    DDX_Control(pDX, IDC_STEPS, c_Steps);
    DDX_Control(pDX, IDC_SPINSTEPS, c_SpinSteps);
    DDX_Control(pDX, IDC_SHOWDEBUG, c_ShowDebug);
    DDX_Control(pDX, IDC_THETA, c_Theta);
    DDX_Control(pDX, IDC_SPINTHETA, c_SpinTheta);
    DDX_Control(pDX, IDC_SAMPLE, c_Degrees);
    DDX_Control(pDX, IDC_SAMPLES, c_Samples);
    DDX_Control(pDX, IDC_SHOW_SAMPLES, c_ShowSamples);
    DDX_Control(pDX, IDC_SHOWAXIS, c_ShowAxis);
    DDX_Control(pDX, IDC_SPINWAVES, c_SpinWaves);
    DDX_Control(pDX, IDC_FRAME, c_Frame);
    DDX_Control(pDX, IDC_RECTIFY_NONE, c_RectifyNone);
    DDX_Control(pDX, IDC_RECTIFY_HALF, c_RectifyHalf);
    DDX_Control(pDX, IDC_RECTIFY_FULL, c_RectifyFull);
    DDX_Control(pDX, IDC_SPINPHASE, c_SpinPhase);
    DDX_Control(pDX, IDC_SHOW_RMS, c_ShowRMS);
    DDX_Control(pDX, IDC_SHOWCLASSD, c_ShowClassD);
    DDX_Control(pDX, IDC_PHASE, c_Phase);
    DDX_Control(pDX, IDC_ZEROPHASE, c_ZeroPhase);
    DDX_Control(pDX, IDC_SHOWBASE, c_ShowBase);
    DDX_Control(pDX, IDC_SHOWSUM, c_ShowSum);
    DDX_Control(pDX, IDC_SHOWDIFF, c_ShowDiff);
    DDX_Control(pDX, IDC_DEBUGXY, c_DebugXY);
    DDX_Control(pDX, IDC_TWOSEVENTYPHASE, c_TwoSeventyPhase);
    DDX_Control(pDX, IDC_NINETYPHASE, c_NinetyPhase);
    DDX_Control(pDX, IDC_ONEEIGHTYPHASE, c_OneEightyPhase);
    DDX_Control(pDX, IDC_ZERO_CENTER, c_ZeroCenter);
    DDX_Control(pDX, IDC_ZERO_BOTTOM, c_ZeroBottom);
    DDX_Control(pDX, IDC_ADC_10, c_ADC_10);
    DDX_Control(pDX, IDC_ADC_11, c_ADC_11);
    DDX_Control(pDX, IDC_ADC_12, c_ADC_12);
    DDX_Control(pDX, IDC_ADC_16, c_ADC_16);
    DDX_Control(pDX, IDC_ADC_8, c_ADC_8);
    DDX_Control(pDX, IDC_ADC, c_ADC);
    DDX_Control(pDX, IDC_ACD_CAPTION, x_ADC);
    DDX_Control(pDX, IDC_CAPACITOR, c_CapacitorSlider);
    DDX_Control(pDX, IDC_CAPACITANCE, c_CapacitorDisplay);
    DDX_Control(pDX, IDC_50HZ, c_50Hz);
    DDX_Control(pDX, IDC_60HZ, c_60Hz);
    DDX_Control(pDX, IDC_LOAD, c_ResistorSlider);
    DDX_Control(pDX, IDC_RESISTANCE, c_ResistorDisplay);
    DDX_Control(pDX, IDC_SHOWFILTERED, c_ShowFiltered);
    DDX_Control(pDX, IDC_HZ_CAPTION, x_Hz);
    DDX_Control(pDX, IDC_CAPACITANCE_CAPTION, x_Capacitance);
    DDX_Control(pDX, IDC_RESISTANCE_CAPTION, x_Resistance);
    DDX_Control(pDX, IDC_3V3, c_3V3);
    DDX_Control(pDX, IDC_5V0, c_5V0);
    DDX_Control(pDX, IDC_CURRENT_CAPTION, x_Current);
    DDX_Control(pDX, IDC_CURRENT, c_Current);
    DDX_Control(pDX, IDC_DEBUG_CAPTION, x_Debug);
    DDX_Control(pDX, IDC_INVALIDATE, c_Invalidate);
    DDX_Control(pDX, IDC_CIRCUIT, c_CircuitWindow);
    DDX_Control(pDX, IDC_SHOWREGULATED, c_ShowRegulated);
    DDX_Control(pDX, IDC_SHOWREGULATIONFAILURE, c_ShowRegulationFailure);
    DDX_Control(pDX, IDC_DROPOUT_CAPTION1, x_Dropout1);
    DDX_Control(pDX, IDC_DROPOUT, c_Dropout);
    DDX_Control(pDX, IDC_DROPOUT_CAPTION2, x_Dropout2);
    DDX_Control(pDX, IDC_SPIN_DROPOUT, c_SpinDropout);
    DDX_Control(pDX, IDC_RECTIFICATION_CAPTION, x_Rectification);
    DDX_Control(pDX, IDC_INPUT_VOLTAGE_CAPTION, x_InputVoltage);
    DDX_Control(pDX, IDC_OTHER_INPUT_VALUE, c_OtherInputVoltage);
    DDX_Control(pDX, IDC_OTHER_OUTPUT__VALUE, c_OtherOutputVoltage);
    DDX_Control(pDX, IDC_SPIN_OTHER_INPUT__VALUE, c_SpinOtherInput);
    DDX_Control(pDX, IDC_SPIN_OTHER_OUTPUT_VALUE, c_SpinOtherOutput);
    DDX_Control(pDX, IDC_6V3, c_6V3);
    DDX_Control(pDX, IDC_12V6, c_12V6);
    DDX_Control(pDX, IDC_OTHER_INPUT_V, c_OtherInputV);
    DDX_Control(pDX, IDC_OTHER_OUTPUT_V, c_OtherOutputV);
    DDX_Control(pDX, IDC_SHOWVALUES, c_ShowValues);
    DDX_Control(pDX, IDC_ZERO_CAPTION, x_Zero);
    DDX_Control(pDX, IDC_DROPOUT_2V, c_Dropout2V);
    DDX_Control(pDX, IDC_DROPOUT_700MV, c_Dropout700mV);
    DDX_Control(pDX, IDC_DROPOUT_200MV, c_Dropout200mV);
    DDX_Control(pDX, IDC_DROPOUT_350MV, c_Dropout350mV);
    DDX_Control(pDX, IDC_OTHER_AREF_VALUE, c_ArefVoltage);
    DDX_Control(pDX, IDC_SPIN_OTHER_AREF__VALUE, c_SpinOtherAref);
    DDX_Control(pDX, IDC_AREF_1V0, c_Aref_1V0);
    DDX_Control(pDX, IDC_AREF_1V1, c_Aref_1V1);
    DDX_Control(pDX, IDC_AREF_3V3, c_Aref_3V3);
    DDX_Control(pDX, IDC_AREF_5V0, c_Aref_5V0);
    DDX_Control(pDX, IDC_AREF_VALUE_CAPTION1, x_Aref1);
    DDX_Control(pDX, IDC_AREF_VALUE_CAPTION2, x_Aref2);
    DDX_Control(pDX, IDC_AREF_OTHER, c_Aref_Other);
    DDX_Control(pDX, IDC_POWER_FACTOR, c_PowerFactor);
    DDX_Control(pDX, IDC_ENABLE_SCHMITT, c_EnableSchmitt);
    DDX_Control(pDX, IDC_OFF, c_Off);
    DDX_Control(pDX, IDC_OFF_CAPTION, x_Off);
    DDX_Control(pDX, IDC_ON, c_On);
    DDX_Control(pDX, IDC_ON_CAPTION, x_On);
    DDX_Control(pDX, IDC_SPIN_OFF, c_SpinOff);
    DDX_Control(pDX, IDC_SPIN_ON, c_SpinOn);
    DDX_Control(pDX, IDC_PWM_CAPTION, x_PWM);
    DDX_Control(pDX, IDC_PWM_WIDTH, c_PWM_Width);
    DDX_Control(pDX, IDC_SHOWPWM, c_ShowPWM);
    DDX_Control(pDX, IDC_SPIN_PWM_WIDTH, c_SpinPWMWidth);
    DDX_Control(pDX, IDC_INVERT_SCHMITT, c_InvertSchmitt);
    DDX_Control(pDX, IDC_INVERT_PWM, c_InvertPWM);
    DDX_Control(pDX, IDC_SPIN_VIMAX, c_SpinViMax);
    DDX_Control(pDX, IDC_VIMAX, c_ViMax);
    DDX_Control(pDX, IDC_VIMAX_CAPTION, x_ViMax);
    DDX_Control(pDX, IDC_VI_SAFE, c_ViSafe);
    DDX_Control(pDX, IDC_CLIP_CLIENT, c_ClipClient);
    DDX_Control(pDX, IDC_CLIP_FULL, c_ClipFull);
    DDX_Control(pDX, IDC_CLIP_WAVEFORM, c_ClipWave);
    DDX_Control(pDX, IDC_CLIP_CAPTION, x_Clipboard);
    DDX_Control(pDX, IDC_CLIP_CIRCUIT, c_ClipCircuit);
    DDX_Control(pDX, IDC_SPIN_VF, c_SpinVF);
    DDX_Control(pDX, IDC_VF, c_VF);
    DDX_Control(pDX, IDC_VF_200MV, c_VF_200mV);
    DDX_Control(pDX, IDC_VF_300MV, c_VF_300mV);
    DDX_Control(pDX, IDC_VF_700MV, c_VF_700mV);
    DDX_Control(pDX, IDC_VF_BOX, x_VF);
    DDX_Control(pDX, IDC_VF_CAPTION2, x_VF2);
    DDX_Control(pDX, IDC_INVALIDATE_CIRCUIT, c_InvalidateCircuit);
    DDX_Control(pDX, IDC_SINE_DASHED, c_SineDashed);
    DDX_Control(pDX, IDC_ADC_DEC, c_ADC_Dec);
    DDX_Control(pDX, IDC_ADC_HEX, c_ADC_Hex);
    DDX_Control(pDX, IDC_ADC_PERCENT, c_ADC_Percent);
    DDX_Control(pDX, IDC_ADC_VOLTS, c_ADC_Volts);
    DDX_Control(pDX, IDC_SHOW_CAPTION, x_Show);
    DDX_Control(pDX, IDC_SHOWMARKS, c_ShowMarks);
    DDX_Control(pDX, IDC_SPINNER, c_Spinner);
    DDX_Control(pDX, IDC_CLIP_WAVEDELAY, c_ClipWaveDelay);

}

BEGIN_MESSAGE_MAP(SineWaveDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(IDCLOSE, &SineWaveDlg::OnIdclose)
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_INVALIDATE, &SineWaveDlg::OnBnClickedInvalidate)
    ON_EN_CHANGE(IDC_STEPS, &SineWaveDlg::OnChangeSteps)
    ON_BN_CLICKED(IDC_SHOWSINE, &SineWaveDlg::OnClickedShowsine)
    ON_BN_CLICKED(IDC_SHOWSTEP, &SineWaveDlg::OnClickedShowstep)
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_SHOWDEBUG, &SineWaveDlg::OnClickedShowdebug)
    ON_EN_CHANGE(IDC_THETA, &SineWaveDlg::OnChangeTheta)
    ON_BN_CLICKED(IDC_SHOW_SAMPLES, &SineWaveDlg::OnBnClickedShowSamples)
    ON_BN_CLICKED(IDC_SHOWAXIS, &SineWaveDlg::OnClickedShowaxis)
    ON_EN_CHANGE(IDC_WAVES, &SineWaveDlg::OnEnChangeWaves)
    ON_COMMAND(IDC_RECTIFY_FULL, &SineWaveDlg::OnRectifyFull)
    ON_COMMAND(IDC_RECTIFY_HALF, &SineWaveDlg::OnRectifyHalf)
    ON_COMMAND(IDC_RECTIFY_NONE, &SineWaveDlg::OnRectifyNone)
    ON_EN_CHANGE(IDC_PHASE, &SineWaveDlg::OnChangePhase)
    ON_BN_CLICKED(IDC_SHOW_RMS, &SineWaveDlg::OnClickedShowRms)
    ON_BN_CLICKED(IDC_SHOWCLASSD, &SineWaveDlg::OnClickedShowclassd)
    ON_BN_CLICKED(IDC_ZEROPHASE, &SineWaveDlg::OnBnClickedZerophase)
    ON_BN_CLICKED(IDC_SHOWBASE, &SineWaveDlg::OnClickedShowbase)
    ON_BN_CLICKED(IDC_SHOWSUM, &SineWaveDlg::OnClickedShowsum)
    ON_BN_CLICKED(IDC_SHOWDIFF, &SineWaveDlg::OnClickedShowdiff)
    ON_BN_CLICKED(IDC_NINETYPHASE, &SineWaveDlg::OnClickedNinetyphase)
    ON_BN_CLICKED(IDC_ONEEIGHTYPHASE, &SineWaveDlg::OnClickedOneeightyphase)
    ON_BN_CLICKED(IDC_TWOSEVENTYPHASE, &SineWaveDlg::OnClickedTwoseventyphase)
    ON_COMMAND(IDC_ZERO_BOTTOM, &SineWaveDlg::OnZeroBottom)
    ON_COMMAND(IDC_ZERO_CENTER, &SineWaveDlg::OnZeroCenter)
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_SHOWFILTERED, &SineWaveDlg::OnClickedShowfiltered)
    ON_BN_CLICKED(IDC_3V3, &SineWaveDlg::OnClicked3v3)
    ON_COMMAND(IDC_5V0, &SineWaveDlg::On5v0)
    ON_BN_CLICKED(IDC_RESET_ALL, &SineWaveDlg::OnClickedResetAll)
    ON_BN_CLICKED(IDC_SHOWREGULATED, &SineWaveDlg::OnClickedShowregulated)
    ON_BN_CLICKED(IDC_SHOWREGULATIONFAILURE, &SineWaveDlg::OnClickedShowregulationfailure)
    ON_EN_CHANGE(IDC_DROPOUT, &SineWaveDlg::OnChangeDropout)
    ON_EN_CHANGE(IDC_OTHER_INPUT_VALUE, &SineWaveDlg::OnChangeOtherInputValue)
    ON_EN_CHANGE(IDC_OTHER_OUTPUT__VALUE, &SineWaveDlg::OnChangeOtherOutputValue)
    ON_BN_CLICKED(IDC_OTHER_INPUT_V, &SineWaveDlg::OnClickedOtherInputV)
    ON_BN_CLICKED(IDC_OTHER_OUTPUT_V, &SineWaveDlg::OnClickedOtherOutputV)
    ON_BN_CLICKED(IDC_6V3, &SineWaveDlg::OnClicked6v3)
    ON_BN_CLICKED(IDC_12V6, &SineWaveDlg::OnClicked12v6)
    ON_BN_CLICKED(IDC_SHOWVALUES, &SineWaveDlg::OnBnClickedShowvalues)
    ON_BN_CLICKED(IDC_DROPOUT_2V, &SineWaveDlg::OnClickedDropout2v)
    ON_BN_CLICKED(IDC_DROPOUT_700MV, &SineWaveDlg::OnClickedDropout700mv)
    ON_BN_CLICKED(IDC_DROPOUT_200MV, &SineWaveDlg::OnClickedDropout200mv)
    ON_BN_CLICKED(IDC_DROPOUT_350MV, &SineWaveDlg::OnClickedDropout350mv)
    ON_BN_CLICKED(IDC_AREF_1V0, &SineWaveDlg::OnClickedAref1v0)
    ON_BN_CLICKED(IDC_AREF_1V1, &SineWaveDlg::OnClickedAref1v1)
    ON_BN_CLICKED(IDC_AREF_3V3, &SineWaveDlg::OnClickedAref3v3)
    ON_BN_CLICKED(IDC_AREF_5V0, &SineWaveDlg::OnClickedAref5v0)
    ON_BN_CLICKED(IDC_AREF_OTHER, &SineWaveDlg::OnClickedArefOther)
    ON_EN_CHANGE(IDC_OFF, &SineWaveDlg::OnChangeOff)
    ON_EN_CHANGE(IDC_ON, &SineWaveDlg::OnChangeOn)
    ON_BN_CLICKED(IDC_ENABLE_SCHMITT, &SineWaveDlg::OnClickedEnableSchmitt)
    ON_BN_CLICKED(IDC_SHOWPWM, &SineWaveDlg::OnClickedShowPWM)
    ON_EN_CHANGE(IDC_PWM_WIDTH, &SineWaveDlg::OnChangePwmWidth)
    ON_BN_CLICKED(IDC_INVERT_SCHMITT, &SineWaveDlg::OnClickedInvertSchmitt)
    ON_BN_CLICKED(IDC_INVERT_PWM, &SineWaveDlg::OnClickedInvertPwm)
    ON_EN_CHANGE(IDC_VIMAX, &SineWaveDlg::OnChangeVimax)
    ON_BN_CLICKED(IDC_VI_SAFE, &SineWaveDlg::OnClickedViSafe)
    ON_BN_CLICKED(IDC_CLIP_CLIENT, &SineWaveDlg::OnClickedClipClient)
    ON_BN_CLICKED(IDC_CLIP_FULL, &SineWaveDlg::OnClickedClipFull)
    ON_BN_CLICKED(IDC_CLIP_WAVEFORM, &SineWaveDlg::OnClickedClipWaveform)
    ON_BN_CLICKED(IDC_CLIP_CIRCUIT, &SineWaveDlg::OnClickedClipCircuit)
    ON_BN_CLICKED(IDC_VF_200MV, &SineWaveDlg::OnClickedVf200mv)
    ON_BN_CLICKED(IDC_VF_700MV, &SineWaveDlg::OnClickedVf700mv)
    ON_BN_CLICKED(IDC_VF_300MV, &SineWaveDlg::OnClickedVf300mv)
    ON_EN_CHANGE(IDC_VF, &SineWaveDlg::OnChangeVf)
    ON_BN_CLICKED(IDC_INVALIDATE_CIRCUIT, &SineWaveDlg::OnClickedInvalidateCircuit)
    ON_BN_CLICKED(IDC_SHOWREGULATIONFAILURES, &SineWaveDlg::OnClickedShowregulationfailures)
    ON_BN_CLICKED(IDC_SINE_DASHED, &SineWaveDlg::OnClickedSineDashed)
    ON_BN_CLICKED(IDC_ZERO_CENTER, &SineWaveDlg::OnClickedZeroCenter)
    ON_BN_CLICKED(IDC_ZERO_BOTTOM, &SineWaveDlg::OnClickedZeroBottom)
    ON_BN_CLICKED(IDC_ADC_HEX, &SineWaveDlg::OnClickedAdcHex)
    ON_BN_CLICKED(IDC_ADC_PERCENT, &SineWaveDlg::OnClickedAdcPercent)
    ON_BN_CLICKED(IDC_ADC_VOLTS, &SineWaveDlg::OnClickedAdcVolts)
    ON_BN_CLICKED(IDC_ADC_DEC, &SineWaveDlg::OnClickedAdcDec)
    ON_BN_CLICKED(IDC_SHOWMARKS, &SineWaveDlg::OnClickedShowmarks)
    ON_BN_CLICKED(IDC_CLIP_WAVEDELAY, &SineWaveDlg::OnClickedClipWavedelay)
    ON_WM_TIMER()
    ON_REGISTERED_MESSAGE(UWM_DELAY_DONE, OnDelayDone)
END_MESSAGE_MAP()


/****************************************************************************************
*                             SineWaveDlg::DoReset
* Result: void
* 
* Effect:
*     Resets (most of) the controls to their startup defaults
****************************************************************************************/
void SineWaveDlg::DoReset()
    {
    // NOTE: The capacitor and resistor sliders must be initialized early
    // because OnChangeSteps requires it be defined
    /*****************************************
    * [____]
    *
    *  <#>
    *   |
    *   |
    * Capacitor
    *****************************************/
    c_CapacitorSlider.SetRange(0, _countof(Capacitors) - 1, TRUE);
    c_CapacitorSlider.SetPos(0);

    /*****************************************
    * [____]
    *
    *  <#>
    *   |
    *   |
    *  Load
    *****************************************/
    static const int RMIN = 100;
    static const int RMAX = 20000;
    c_ResistorSlider.SetRange(RMIN, RMAX, TRUE);
    c_ResistorSlider.SetPos(RMIN);

    /*****************************************
    * [ ] Show PWM [nnn]↕%  [ ] Invert
    *****************************************/
    c_ShowPWM.SetCheck(BST_UNCHECKED);
    c_Graph.SetShowPWM(false);
    c_SpinPWMWidth.SetPos32(50);
    c_Graph.SetPWMWidth(c_SpinPWMWidth.GetPos32());
    c_InvertPWM.SetCheck(BST_UNCHECKED);

    /*****************************************
    * [x] Show Sine  [ ] ---
    ******************************************/
    c_Graph.SetShowSine(true);
    c_ShowSine.SetCheck(c_Graph.GetShowSine() ? BST_CHECKED : BST_UNCHECKED);

    c_Graph.SetDashed(false);
    c_SineDashed.SetCheck(c_Graph.GetDashed() ? BST_CHECKED : BST_UNCHECKED);


    /****************************************
    * [ ] Show Samples
    *****************************************/
    c_Graph.SetShowSamples(false);

    c_ShowSamples.SetCheck(c_Graph.GetShowSamples() ? BST_CHECKED : BST_UNCHECKED);

    /****************************************
    * [ ] Show RMS
    *****************************************/
    c_Graph.SetShowRMS(false);
    c_ShowRMS.SetCheck(c_Graph.GetShowRMS() ? BST_CHECKED : BST_UNCHECKED);

    /****************************************
    * [ ] Show Axis
    ****************************************/
    c_Graph.SetShowAxis(true);
    c_ShowAxis.SetCheck(c_Graph.GetShowAxis());

    /****************************************
    * [ ] Show Class D
    *****************************************/
    c_Graph.SetShowClassD(false);
    c_ShowClassD.SetCheck(c_Graph.GetShowClassD() ? BST_CHECKED : BST_UNCHECKED);

    /****************************************
    * [ ] Show Step  [    ]↕ = %d°/sample
    ****************************************/
    c_Graph.SetShowStep(false);
    c_ShowStep.SetCheck(c_Graph.GetShowStep());

    c_Graph.SetSteps(4);

    int n = c_Graph.GetSteps();
    c_SpinSteps.SetRange(1, 360);
    c_SpinSteps.SetPos(n);

    /******************************************
    * [ ] Show Filtered Wave
    *******************************************/
    c_ShowFiltered.SetCheck(c_Graph.GetShowFiltered() ? BST_CHECKED : BST_UNCHECKED);

    /******************************************
    * [ ] Show Filtered Wave
    *******************************************/
    c_Graph.SetShowRegulated(false);
    c_ShowRegulated.SetCheck(c_Graph.GetShowRegulated() ? BST_CHECKED : BST_UNCHECKED);


    /******************************************
    * Sine Interval, Degrees [____]↕
    ******************************************/
    c_Graph.SetTheta(4);

    c_SpinTheta.SetRange(1, 90);
    c_SpinTheta.SetPos(c_Graph.GetTheta());

    /******************************************
    * Show [____]↕ Waves
    ******************************************/
    c_Graph.SetWaves(1);

    c_SpinWaves.SetPos(c_Graph.GetWaves());

    /***************************************
    * Phase Shift, Degrees  [____]↕
    ***************************************/
    c_SpinPhase.SetRange(0, 359);
    c_SpinPhase.SetPos(0);

    /***************************************
    * [ ] Show Base wave
    * [ ] Show Sum
    * [ ] Show Difference
    ****************************************/
    c_Graph.SetShowBase(false);
    c_Graph.SetShowSum(false);
    c_Graph.SetShowDiff(false);

    c_ShowBase.SetCheck(c_Graph.GetShowBase() ? BST_CHECKED : BST_UNCHECKED);
    c_ShowSum.SetCheck(c_Graph.GetShowSum() ? BST_CHECKED : BST_UNCHECKED);
    c_ShowDiff.SetCheck(c_Graph.GetShowDiff() ? BST_CHECKED : BST_UNCHECKED);

    /**********************************************************************
    * [ ] Show Schmitt  On [nnn]↕%  Off [nnn]↕% [ ] Invert Vin(max) [   ]↕ [ ] Vi Safe
    ***********************************************************************/
    c_EnableSchmitt.SetCheck(BST_UNCHECKED);

    int on = (int)(c_Graph.GetSchmittOnDefault() * 100.0 + 0.5);
    c_SpinOn.SetPos32(on);

    int off = (int)(c_Graph.GetSchmittOffDefault() * 100.0 + 0.5);
    c_SpinOff.SetPos32(off);

    c_InvertSchmitt.SetCheck(BST_UNCHECKED);
    c_SpinViMax.SetPos32(5);  // default: 5V max

    c_ViSafe.SetCheck(BST_UNCHECKED);
    c_CircuitWindow.SetSchmittViSafe(c_ViSafe.GetCheck() == BST_CHECKED);

    /***************************************
    * Number  of waves [  ]↕
    ***************************************/
    c_Graph.SetWaves(1);

    c_SpinWaves.SetRange(1, 10);
    n = c_Graph.GetWaves();
    c_SpinWaves.SetPos(n);

    /****************************************
    * +--Hz--------+
    * | ( ) 50Hz   |
    * | (*) 60Hz   |
    * +------------+
     ****************************************/
    c_Graph.SetHz(60);

    int Freq = IDC_60HZ;
    if(c_Graph.GetHz() == 50)
        Freq = IDC_50HZ;
    CheckRadioButton(IDC_50HZ, IDC_60HZ, Freq);

    /************************************************************
    * +--Rectification----+
    * | (*) None          |
    * | ( ) Half          |
    * | ( ) Full          |
    * +-------------------+
    ************************************************************/
    c_Graph.SetRectification(IDC_RECTIFY_NONE);

    CheckRadioButton(IDC_RECTIFY_NONE, IDC_RECTIFY_FULL, c_Graph.GetRectification());

    /************************************************************
    *  +--Diode Vf--------------------------+
    *  | [200mV][300mV][700mV] [_700__] mV  |
    *  +------------------------------------+
    ************************************************************/
    c_SpinVF.SetPos32(700);

    /****************************************
    * +--Zero----------+
    * | (*) Center     |
    * | ( ) Bottom     |
    * +----------------+
    ****************************************/
    c_Graph.SetZeroPos(IDC_ZERO_CENTER);

    CheckRadioButton(IDC_ZERO_CENTER, IDC_ZERO_BOTTOM, c_Graph.GetZeroPos());

    /******************************************
    * [X] Show Component Values
    ******************************************/
    c_ShowValues.SetCheck(BST_CHECKED);

    /****************************************
    * +-Output Voltage-+
    * | ( ) 3.3V       |
    * | (*) 5V         |
    * | ( ) Other [__]↕|
    * +----------------+
    ****************************************/
    CheckRadioButton(IDC_3V3, IDC_OTHER_OUTPUT_V, IDC_5V0);
    
    // NOTE: It was a deliberate decision to not reset
    // the spin control to the original default
    // It is left at whatever value the user last used

    /****************************************
    * +-Input Voltage--+
    * | (*) 6.3V       |
    * | ( ) 12.6V      |
    * | ( ) Other [__]↕|
    * +----------------+
    ****************************************/
    CheckRadioButton(IDC_6V3, IDC_OTHER_INPUT_V, IDC_6V3);

    // NOTE: It was a deliberate decision to not reset
    // the spin control to the original default
    // It is left at whatever value the user last used


    /***************************************
    * Regulator Dropout  [____]↕ mV
    ***************************************/
    c_SpinDropout.SetRange(200, 2500);
    c_SpinDropout.SetPos(700);

    /************************************************
    * +--ADC Bits-----------------------------+
    * | ( ) 8  (*) 10  ( ) 11  ( ) 12  ( ) 16 |
    * | Show   [x] %   [x] Dec [x] Hex [x] V  |
    * +---------------------------------------+
    *************************************************/
    CheckRadioButton(IDC_ADC_8, IDC_ADC_16,IDC_ADC_10);
    c_ADC_Percent.SetCheck(BST_CHECKED);
    c_ADC_Dec.SetCheck(BST_CHECKED);
    c_ADC_Hex.SetCheck(BST_CHECKED);
    c_ADC_Volts.SetCheck(BST_CHECKED);
    c_Graph.SetShowADCPercent(c_ADC_Percent.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCDec(c_ADC_Dec.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCHex(c_ADC_Hex.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCVolts(c_ADC_Volts.GetCheck() == BST_CHECKED);
    
    /*********************************************
    * ( )1V ( )1.1V ( )3.3V (*)5V ( )[___]↕ mV
    *********************************************/
    CheckRadioButton(IDC_AREF_1V0, IDC_AREF_OTHER, IDC_AREF_5V0);
    // cSpinOtherAref deliberately unmodified
    waiting = false; // just to be safe
    } // DoReset
/*************************************************************************************
*                SineWaveDlg::ShowDebug
* Inputs:
*     bool opt: true to show, false to hide
* Result: void
* Effect:
*     Shows or hides the debug windows
*************************************************************************************/
void SineWaveDlg::ShowDebug(bool opt)
{
    UINT show = (opt ? SW_SHOW : SW_HIDE);
    x_Debug.ShowWindow(show);
    c_ShowDebug.ShowWindow(show);
    c_ShowMarks.ShowWindow(show);
    c_Invalidate.ShowWindow(show);
    c_InvalidateCircuit.ShowWindow(show);
}
/*************************************************************************************
*                  SineWaveDlg::OnInitDialog
* Result: BOOL
*     TRUE if normal control focus applies
*     FALSE if focus specifically set
*************************************************************************************/
BOOL SineWaveDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);



#ifndef _DEBUG
    ShowDebug(false);
    
#endif

    c_DebugXY.ShowWindow(SW_HIDE);  

    c_SpinPWMWidth.SetRange32(0, 100); // 0%..100%

    c_SpinOtherInput.SetRange32(2, 48);
    c_SpinOtherInput.SetPos32(6);
    c_SpinOtherOutput.SetRange32(4, 24);
    c_SpinOtherOutput.SetPos32(5);

    c_SpinOtherAref.SetRange32(1000, 5200);
    c_SpinOtherAref.SetPos32(2000);

    DoReset();

    /**********************************************
    * +--ADC-------------------------------+
    * | ( ) 8 ( ) 10 ( ) 11 ( ) 12 ( )16   |
    * | [________________________________] |
    * +------------------------------------+
    * Note that no radio button is checked.
    * This is deliberate
    **********************************************/
    c_ADC.SetWindowText(_T(""));

    /**********************************************
    * Graph display window
    * +------------------------------------------+
    * |                                          |
    * :                                          :
    * |                                          |
    * +------------------------------------------+
    **********************************************/
    CRect graphRect;
    c_Graph.GetWindowRect(&graphRect);
    c_Graph.SetFilter(filter);
    /*
    *  Now figure out what the margins are
    */
    CRect wr;
    GetWindowRect(&wr);
    LeftMargin = graphRect.left - wr.left;
    RightMargin = wr.right - graphRect.right;
    BottomMargin = wr.bottom - graphRect.bottom;
    TopMargin = (graphRect.top - wr.top) - (::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYSIZEFRAME));

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    {
    CString s;
    s.LoadStringW(IDS_APP_CAPTION);
    SetWindowText(s);
    }
    /**************************************************************
    * +--Schmitt Trigger-------------------------------------+
    * | [✓] Enable  On [_____] Off [______]  VI(max) [_____]|
    * +------------------------------------------------------+
    **************************************************************/
    c_EnableSchmitt.SetCheck(BST_UNCHECKED);
    c_SpinOn.SetRange32(1, 99);
    int on = (int)(c_Graph.GetSchmittOnDefault() * 100.0 + 0.5);
    c_SpinOn.SetPos32(on);
    c_SpinOff.SetRange(1, 99);
    int off = (int)(c_Graph.GetSchmittOffDefault() * 100.0 + 0.5);
    c_SpinOff.SetPos32(off);

    c_SpinViMax.SetRange32(1, 24);

    c_SpinVF.SetRange32(100, 10000);
    c_SpinVF.SetPos32(700);

    initialized = true;
    updateControls();
    return TRUE;  // return TRUE  unless you set the focus to a control
}
/***************************************************************************************
*                              SineWaveDlg::OnSysCommand
* Inputs:
*     UINT nID: ID of menu item
*     LPARAM lParam:
* Result: void
* 
* Effect:
*     Handles the About-box menu item being selected
****************************************************************************************/
void SineWaveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

/***************************************************************************************
*                         SineWaveDlg::OnPaint
* Result: void
* 
* Effect:
*     If you add a minimize button to your dialog, you will need this code
*     to draw the icon.  For MFC applications using the document/view model,
*     this is automatically done for you by the framework.
* Notes:
*     It might be cute to animate this with a moving spot on the sine wave, but that's
*     a project for another day.
***************************************************************************************/ 

void SineWaveDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}
/**************************************************************************************
*                          SineWaveDlg::OnQueryDragIcon
* Result: HCURSOR
* 
* Effect:
*     The system calls this function to obtain the cursor to display while the user drags
*     the minimized window.
**************************************************************************************/

HCURSOR SineWaveDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

/********************************************************************************************
*                           SHOW/ENABLE/ACTIVATE
*  Inputs:
*      CWnd wnd: Window reference
* Effect:
*      The ACTIVATE macro applies to all the windows.  It willl either show/hide or enable/disable
*      the window based on whether SHOW_OPTION is defined
*********************************************************************************************/
#define SHOW(wnd, b) wnd.ShowWindow((b) ? SW_SHOW : SW_HIDE)
#define ENABLE(wnd, b) wnd.EnableWindow(b)

#ifdef SHOW_OPTION // Experimental: enable controls or hide controls that are not useful?
#define ACTIVATE(wnd) SHOW(wnd)
#else
#define ACTIVATE(wnd, b) ENABLE(wnd, (b))
#endif
/********************************************************************************************
*                        SineWaveDlg::ShowStepInfo
* Inputs:
*     bool b: TRUE to enable ADC display options, FALSE to disable  them
*     bool b2: TRUE to show dec/hex displays, FALSE to hide them
* Effect:
*     Shows/hides the ADC precision windows
********************************************************************************************/

void SineWaveDlg::ShowStepInfo(bool b, bool b2)
{
    ACTIVATE(c_ADC_Percent, b);
    ACTIVATE(c_ADC_Dec, b);
    ACTIVATE(c_ADC_Hex, b);
    ACTIVATE(c_ADC_Volts, b);
    ACTIVATE(x_Show, b);
    SHOW(c_ADC_Hex, b2);
    SHOW(c_ADC_Dec, b2);

}
/********************************************************************************************
*                        SineWaveDlg::ShowADC
* Inputs:
*     bool mode: TRUE to enable ADC precision windows, FALSE to hide them
* Effect:
*     Shows/hides the ADC precision windows
********************************************************************************************/

void SineWaveDlg::ShowADC(bool b)
    {
        ACTIVATE(c_ADC_8, b);
        ACTIVATE(c_ADC_10, b);
        ACTIVATE(c_ADC_11, b);
        ACTIVATE(c_ADC_12, b);
        ACTIVATE(c_ADC_16, b);
        ACTIVATE(c_ADC, b);
        ACTIVATE(x_ADC,b);
        ACTIVATE(c_ArefVoltage, b);
        ACTIVATE(c_Aref_1V0, b);
        ACTIVATE(c_Aref_1V1, b);
        ACTIVATE(c_Aref_3V3, b);
        ACTIVATE(c_Aref_5V0, b);
        ACTIVATE(c_Aref_Other, b);
        ACTIVATE(x_Aref1, b);
        ACTIVATE(x_Aref2, b);
      } // ShowADC

/**********************************************************************************************
*                     SineWaveDlg::ShowRectification
* Inputs:
*     bool b: TRUE to show rectification-related windows
*                FALSE to hide them
**********************************************************************************************/
void SineWaveDlg::ShowRectification(bool b)
    {
    ACTIVATE(x_Rectification, b);
    ACTIVATE(c_RectifyNone, b);
    ACTIVATE(c_RectifyHalf, b);
    ACTIVATE(c_RectifyFull,b);
    }

/*********************************************************************************************
*                     SinwWaveDlg::ShowControlsClassD
* Inputs:
*     boolinClassD mode: Indicates "class D" mode, 
*                                  TRUE will hide unrelated controls 
*                                  FALSE will show them
*********************************************************************************************/
void SineWaveDlg::ShowControlsClassD(bool inClassDmode)
    {
    bool b = !inClassDmode && !waiting;

    ShowRectification(b);

    ACTIVATE(c_Phase, b);
    ACTIVATE(c_SpinPhase, b);
    ACTIVATE(c_ZeroPhase, b);
    ACTIVATE(c_NinetyPhase, b);
    ACTIVATE(c_OneEightyPhase, b);
    ACTIVATE(c_TwoSeventyPhase, b);
    ACTIVATE(c_ShowBase, b);
    ACTIVATE(c_ShowSum, b);
    ACTIVATE(c_ShowDiff, b);
    ACTIVATE(c_ZeroCenter, b);
    ACTIVATE(c_ZeroBottom, b);
    ACTIVATE(x_Zero, b);
    }

/******************************************************************************
*                              SineWaveDlg::ShowRegulated
* Inputs:
*     bool b: TRUE to show the controls relative to regulated voltage parameters
*             FALSE to hide them
*  Effect:
*     Shows/hides the regulated controls
********************************************************************************/

void SineWaveDlg::ShowRegulated(bool b)
    {
    //int show = b ? SW_SHOW : SW_HIDE;

    ACTIVATE(c_3V3, b);
    ACTIVATE(c_5V0, b);
    ACTIVATE(c_OtherOutputV, b);
    ACTIVATE(c_SpinOtherOutput, b);
    ACTIVATE(c_OtherOutputVoltage, b);
//    ACTIVATE(x_InputVoltage, b);
//    ACTIVATE(c_6V3, b);
//    ACTIVATE(c_12V6, b);
//    ACTIVATE(c_OtherInputV, b);
//    ACTIVATE(c_SpinOtherInput, b);
//    ACTIVATE(c_OtherInputVoltage, b);
    ACTIVATE(c_ShowRegulationFailure, b);
    ACTIVATE(x_Dropout1, b);
    ACTIVATE(x_Dropout2, b);
    ACTIVATE(c_SpinDropout, b);
    ACTIVATE(c_Dropout2V, b);
    ACTIVATE(c_Dropout700mV, b);
    ACTIVATE(c_Dropout350mV, b);
    ACTIVATE(c_Dropout200mV, b);
    ACTIVATE(c_Dropout, b);
    }
/**************************************************************************
*                      SineWaveDlg::ShowFiltered
* Inputs:
*     bool b: TRUE to show controls, FALSE to hide them
* Result: void
* 
* Effect:
*     Shows/hides the controls related to filtered output
**************************************************************************/
void SineWaveDlg::ShowFiltered(bool b)
    {
    ACTIVATE(c_50Hz, b);
    ACTIVATE(c_60Hz, b);
    ACTIVATE(x_Hz, b);
    ACTIVATE(c_CapacitorSlider, b);
    ACTIVATE(c_ResistorSlider, b);
    ACTIVATE(c_ResistorDisplay, b);
    ACTIVATE(c_CapacitorDisplay, b);
    ACTIVATE(x_Resistance, b);
    ACTIVATE(x_Capacitance, b);
    ACTIVATE(c_Current, b);
    }

/******************************************************************************
*                        SineWaveDlg::ShowStepFunction
* Inputs:
*     bool b: TRUE to enable controls, FALSE to disable them
* 
* Effect:
*     Shows/hides the controls required when the step-function option is selected
* 
******************************************************************************/
void SineWaveDlg::ShowStepFunction(bool b)
    {
    ACTIVATE(c_ShowStep, b);
    ACTIVATE(c_SpinSteps, b);
    ACTIVATE(c_Steps, b);
    ACTIVATE(c_Degrees, b);
    ACTIVATE(c_Samples, b);

    b = b && c_ShowStep.GetCheck() == BST_CHECKED;
    
    ShowStepInfo(b, c_ZeroBottom.GetCheck() == BST_CHECKED);

    ACTIVATE(c_ShowSamples, b);
    }
/***********************************************************************************
*                     SineWaveDlg::ShowVF
* Inputs:
*     bool b: true to show, false to hide
* Result: void
*
* Effect:
*    Shows or hides the Vf (forward voltage drop) controls
************************************************************************************/
void SineWaveDlg::ShowVF(bool b)
{
    UINT show = b ? SW_SHOW : SW_HIDE;
    x_VF.ShowWindow(show);
    x_VF2.ShowWindow(show);
    c_SpinVF.ShowWindow(show);
    c_VF_200mV.ShowWindow(show);
    c_VF_300mV.ShowWindow(show);
    c_VF_700mV.ShowWindow(show);
    c_VF.ShowWindow(show);

}
/***********************************************************************************
*                     SineWaveDlg::updateControls
* Result: void
* 
* Effect:
*    Handles all interactions between controls, and setting values established
*    by controls
************************************************************************************/
void SineWaveDlg::updateControls()
{
    if (!initialized)
        return;

    int degrees = c_SpinTheta.GetPos32();
    int interval = c_SpinSteps.GetPos32();
   

    { /* PWM */
        bool enable = c_ShowPWM.GetCheck() == BST_CHECKED && !waiting;
        x_PWM.EnableWindow(enable);
        c_SpinPWMWidth.EnableWindow(enable);
        c_PWM_Width.EnableWindow(enable);
        c_InvertPWM.EnableWindow(enable);
    } /* PWM */

    { /* Show degrees */
        CString s;
        s.Format(IDS_SHOW_DEGREES, degrees * interval);
        c_Degrees.SetWindowText(s);
    } /* show degrees */

    { /* show wave count */
        CString s;

        double samples = (360.0 / (double)(degrees * interval)) * c_Graph.GetWaves();
        s.Format(IDS_SAMPLES, samples);
        c_Samples.SetWindowText(s);
    } /* show wave count */

    c_ShowClassD.EnableWindow(c_EnableSchmitt.GetCheck() != BST_CHECKED && !waiting);

    bool isClassD = c_ShowClassD.IsWindowEnabled() && c_ShowClassD.GetCheck() == BST_CHECKED;
    ShowControlsClassD(isClassD && !waiting);
    ShowRectification(!(isClassD ||
        c_ZeroBottom.GetCheck() == BST_CHECKED ||
        c_EnableSchmitt.GetCheck() == BST_CHECKED) && !waiting);
    ShowRegulated(c_RectifyNone.GetCheck() != BST_CHECKED &&
                  c_EnableSchmitt.GetCheck() != BST_CHECKED &&
                  c_ShowRegulated.GetCheck() == BST_CHECKED &&
                  !waiting);

    if (c_RectifyNone.GetCheck())
        c_Graph.SetRectification(IDC_RECTIFY_NONE);
    else
    if (c_RectifyHalf.GetCheck())
         c_Graph.SetRectification(IDC_RECTIFY_HALF);
    else
    if (c_RectifyFull.GetCheck())
          c_Graph.SetRectification(IDC_RECTIFY_FULL);

    c_CircuitWindow.SetShowFiltered(c_Graph.GetShowFiltered());
    c_CircuitWindow.SetShowRegulator(c_Graph.GetShowRegulated());

    bool IsSchmitt = c_EnableSchmitt.GetCheck() == BST_CHECKED && c_ShowPWM.GetCheck() == BST_UNCHECKED;

    if (!IsSchmitt)
       { /* !Schmitt */
        switch (c_Graph.GetRectification())
        { /* set drawing*/
        case IDC_RECTIFY_NONE:
           { /* none */

            c_CircuitWindow.SetConfiguration(CircuitWindow::AConly);

            ACTIVATE(c_ShowFiltered, false);
            ACTIVATE(c_ShowRegulated, false);

            ShowStepFunction(!waiting );
            ShowRegulated(false);
            ShowVF(false);
           } /* none */
           break;
        case IDC_RECTIFY_HALF:
           { /* half */
            c_CircuitWindow.SetConfiguration(CircuitWindow::Halfwave);

            ACTIVATE(c_ShowFiltered, !waiting);
            ACTIVATE(c_ShowRegulated, !waiting);

            ShowStepFunction(false);
            ShowVF(true);
           } /* half */
           break;
        case IDC_RECTIFY_FULL:
           { /* full */
            c_CircuitWindow.SetConfiguration(CircuitWindow::Fullwave);

            ACTIVATE(c_ShowFiltered, !waiting);

            bool b = c_ShowFiltered.GetCheck() == BST_CHECKED && !waiting;

            ACTIVATE(c_ShowRegulated, b);

            ShowStepFunction(false);
            ShowVF(true);
           } /* full */
           break;
        } /* set drawing*/
       } /* !Schmitt */
    else
    { /* Schmitt */
        c_CircuitWindow.SetConfiguration(CircuitWindow::Schmitt);
        c_CircuitWindow.SetSchmittInvert(c_InvertSchmitt.GetCheck() == BST_CHECKED);
        ShowVF(true);
    } /* Schmitt */

    /**********************************************************************************
    * +-- Schmitt Trigger ------------------------------------------------------------+
    * | [x] Enable On [____] %  Off [_____] % [x] Invert Vi(max) [_____] V [x]Vi safe |
    * +-------------------------------------------------------------------------------+
    **********************************************************************************/
    { /* Schmitt controls*/
        UINT show = (c_ShowPWM.GetCheck() == BST_UNCHECKED && c_EnableSchmitt.GetCheck() == BST_CHECKED) ? SW_SHOW : SW_HIDE;

        if (c_ShowPWM.GetCheck() == BST_CHECKED)
            c_EnableSchmitt.ShowWindow(show);
        else
            c_EnableSchmitt.ShowWindow(SW_SHOW);

        x_On.ShowWindow(show);
        x_On.EnableWindow(!waiting);

        c_On.ShowWindow(show);
        c_On.EnableWindow(!waiting);

        c_SpinOn.ShowWindow(show);
        c_SpinOn.EnableWindow(!waiting);

        x_Off.ShowWindow(show);
        x_Off.EnableWindow(!waiting);

        c_Off.ShowWindow(show);
        c_Off.EnableWindow(!waiting);

        c_SpinOff.ShowWindow(show);
        c_SpinOff.EnableWindow(!waiting);

        c_InvertSchmitt.ShowWindow(show);
        c_InvertSchmitt.EnableWindow(!waiting);

        c_ViMax.ShowWindow(show);
        c_ViMax.EnableWindow(!waiting);

        c_SpinViMax.ShowWindow(show);
        c_SpinViMax.EnableWindow(!waiting);

        x_ViMax.ShowWindow(show);
        x_ViMax.EnableWindow(!waiting);

        c_ViSafe.ShowWindow(show);
        c_ViSafe.EnableWindow(!waiting);

        c_Graph.SetSchmittEnable(c_EnableSchmitt.GetCheck() == BST_CHECKED);
    } /* Schmitt controls*/

    { /* Power factor */
        if (c_SpinPhase.GetSafeHwnd() != NULL)
        {
            int n = c_SpinPhase.GetPos32();
            c_Graph.SetPhase(n);

            // Compute the power factor.  The power factor is the cosine of the angle n
            // but we first have to convert it to a double and in radians
            double theta = degreesToRadians((double)n);
            double pf = cos(theta);
            CString s;
            s.Format(_T("%3.0f"), fabs((pf * 100) + 0.5));
            c_PowerFactor.SetWindowText(s);
        }
    } /* Power factor*/

    { /* VF */
     double Vf = (double)c_SpinVF.GetPos32() / 1000.0;
     c_CircuitWindow.SetVf(Vf);
    } /* VF */

    c_Graph.SetShowRegulationFailure(c_ShowRegulationFailure.GetCheck() == BST_CHECKED);
    c_CircuitWindow.SetShowRegulator(c_ShowRegulated.GetCheck() == BST_CHECKED);
    c_Graph.SetShowRegulated(c_ShowRegulated.GetCheck() == BST_CHECKED);
    /*********************************************************************************
    *  +--ADC Bits-----------------------------+
    *  | ( ) 8  (*) 10  ( ) 11  ( ) 12  ( ) 16 |
    *  | Show:  [x] %   [x] Dec [x] Hex [x] V  |
    *  +---------------------------------------+
    **********************************************************************************/
    ShowADC((c_ShowStep.GetCheck() == BST_CHECKED && 
              c_ZeroBottom.GetCheck() == BST_CHECKED));


    ShowFiltered(c_ShowFiltered.GetCheck() == BST_CHECKED && c_RectifyNone.GetCheck() != BST_CHECKED);

    c_Graph.SetShowADCPercent(c_ADC_Percent.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCDec(c_ZeroCenter.GetCheck() == BST_CHECKED ? false : c_ADC_Dec.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCHex(c_ZeroCenter.GetCheck() == BST_CHECKED ? false : c_ADC_Hex.GetCheck() == BST_CHECKED);
    c_Graph.SetShowADCVolts(c_ADC_Volts.GetCheck() == BST_CHECKED);

    /********************************************************************************
    * Capacitor slider
    * Top                                                     Bottom
    * nuF -[]------------------------------------------------- C
    * 
    ********************************************************************************/

        { /* capacitor value */
            int cap = c_CapacitorSlider.GetPos();
            double Capacitance = Capacitors[cap] * 1E-6;
            CString s = Farads(Capacitance).toString();
 
            c_CapacitorDisplay.SetWindowText(s);
            filter.SetC(Capacitors[cap]);
            c_CircuitWindow.SetCapacitance(Capacitance);
        } /* capacitor value */

    /*******************************************************************************
    * Resistor slider
    * Top                                                          Bottom
    * nΩ -[]------------------------------------------------------- Load  n mA
    * 
    *******************************************************************************/
        { /* resistor value */
#ifdef _DEBUG
            int Low;                               
            int High;
            c_ResistorSlider.GetRange(Low, High);
#endif
            int res = c_ResistorSlider.GetPos();
            Ohms R((double) res);
            CString s = R.toString();
            c_ResistorDisplay.SetWindowText(s);
            filter.SetR((double)res);
            c_CircuitWindow.SetResistance((double) res);
        } /* resistor value */

        { /* Current value */
            int R = c_ResistorSlider.GetPos();

            double V = 5.0;  // Assume 5V unless overridden
            if(c_3V3.GetCheck() == BST_CHECKED)
                V = 3.3;
            if(c_OtherOutputV.GetCheck() == BST_CHECKED)
                V = (double)c_SpinOtherOutput.GetPos32();

            c_Graph.SetOutputVoltage(V);
            c_CircuitWindow.SetOutputVoltage(V);
        
            Amps I(V / (double) R);
            CString s = I.toString();
            c_Current.SetWindowText(s);
        } /* Current value */

        /*******************************
        * +--Input Voltage (RMS)---+
        * | (*) 6.3V               |
        * | ( ) 12.6V              |
        * | ( ) Other [____]       |
        * +------------------------+
        ********************************/
        if(c_6V3.GetCheck() == BST_CHECKED)
            { /* 6.3V */
            c_Graph.SetInputVoltage(6.3);
            c_CircuitWindow.SetInputVoltage(6.3);
            c_CircuitWindow.SetSchmittVi(sqrt(2.0) * 6.3);
            } /* 6.3V */
        else
        if(c_12V6.GetCheck() == BST_CHECKED)
            { /* 12.6V */
            c_Graph.SetInputVoltage(12.6);
            c_CircuitWindow.SetInputVoltage(12.6);
            c_CircuitWindow.SetSchmittVi(sqrt(2.0) * 12.6);
            } /* 12.6V */
        else
        if(c_OtherInputV.GetCheck() == BST_CHECKED)
            { /* other */
            int n = c_SpinOtherInput.GetPos32();
            c_Graph.SetInputVoltage((double) n);
            c_CircuitWindow.SetInputVoltage((double) n);
            c_CircuitWindow.SetSchmittVi(sqrt(2.0) * (double) n);
            } /* other */
        else
            { /* impossible */
            c_Graph.SetInputVoltage(6.3);
            c_CircuitWindow.SetInputVoltage(6.3);
            c_CircuitWindow.SetSchmittVi(sqrt(2.0) * 6.3);
            } /* impossible */
        /**************************************
        * [x] Show Step Function 
        **************************************/
        c_Graph.SetShowStep(c_ShowStep.GetCheck() == BST_CHECKED);

        /**************************************
        * [x] Show Component Values
        **************************************/
        c_CircuitWindow.SetShowValues(c_ShowValues.GetCheck() == BST_CHECKED);

        /*******************************************************
        * +--ARef---------------------------------------------+
        * | ( ) 1V  ( ) 1.1V  ( ) 3.3V  (*)5V  ( ) [_____] mV |
        * +---------------------------------------------------+
        ********************************************************/
        { /* AREF */
            if(c_Aref_1V0.GetCheck() == BST_CHECKED)
                { /* 1.0V */
                ArefVoltage = 1.0;
                } /* 1.0V */
            else
            if(c_Aref_1V1.GetCheck() == BST_CHECKED)
                { /* 1.1V */
                ArefVoltage = 1.1;
                } /* 1.1V */
            else
            if(c_Aref_3V3.GetCheck() == BST_CHECKED)
                { /* 3.3V */
                ArefVoltage = 3.3;
                } /* 3.3V */
            else
            if(c_Aref_5V0.GetCheck() == BST_CHECKED)
                { /* 5.0V */
                ArefVoltage = 5.0;
                } /* 5.0V */
            else
            if(c_Aref_Other.GetCheck() == BST_CHECKED)
                { /* other */
                int val = c_SpinOtherAref.GetPos32();
                ArefVoltage = ((double) val) / 1000.0;
                } /* other */
        } /* AREF */ 

        SetADCVals();

        UINT show = c_ShowBase.GetCheck() ? SW_SHOW : SW_HIDE;
        c_PowerFactor.ShowWindow(show);

    } // updateControls



/*********************************************************************************
*                            SineWaveDlg::OnIdclose()
* Result: void
* 
* Effect:
*     Does nothing to prevent inadvertent closure of the main window
**********************************************************************************/
void SineWaveDlg::OnIdclose()
    {
    // DO NOTHING
    }

/*********************************************************************************
*                            SineWaveDlg::OnCancel()
* Result: void
*
* Effect:
*     Does nothing to prevent inadvertent closure of the main window
**********************************************************************************/
void SineWaveDlg::OnCancel()
    {
    // DO NOTHING
    }

/*********************************************************************************
*                            SineWaveDlg::OnOK()
* Result: void
*
* Effect:
*     Does nothing to prevent inadvertent closure of the main window
**********************************************************************************/
void SineWaveDlg::OnOK()
    {
    // DO NOTHING
    }

/*********************************************************************************
*                            SineWaveDlg::Onclose()
* Result: void
*
* Effect:
*     Closes the app when the close box is clicked
**********************************************************************************/
void SineWaveDlg::OnClose()
    {
    CDialogEx::OnOK();
    }

/******************************************************************************************
*                               SineWaveDlg::OnSize
* Inputs:
*     UINT nType: Type of resizing
*     int cx: New width
*     int cy: New height
* Result: void
* 
* Effect:
*     Resizes the graph window to fill in all available space below the controls
******************************************************************************************/

void SineWaveDlg::OnSize(UINT nType, int cx, int cy)
    {
     CDialogEx::OnSize(nType, cx, cy);

     if(c_Graph.GetSafeHwnd() != NULL)
         {
         c_Graph.SetWindowPos(0, 0, 0, cx - RightMargin, cy - BottomMargin - TopMargin, SWP_NOMOVE | SWP_NOZORDER);
         c_Graph.Invalidate();
         c_CircuitWindow.Invalidate();
         }
    } // SineWaveDlg::OnSize

/******************************************************************************************
*                     SineWaveDlg::OnBnClickedInvalidate
* Result: void
* 
* Effect:
*     Invalidates the graph and schematic windows.
* Notes:
*     This is convenient for debugging
******************************************************************************************/

void SineWaveDlg::OnBnClickedInvalidate()
    {
     c_Graph.Invalidate(true);
     c_CircuitWindow.Invalidate();
    }

/******************************************************************************************
*                              SineWaveDlg::OnChangeSteps
* Result: void
* 
* Effect:
*    The number of steps per sample has changed; update the graph control so it can redraw
******************************************************************************************/
void SineWaveDlg::OnChangeSteps()
    {
    if(c_SpinSteps.GetSafeHwnd() == NULL)
        return;
    if (!initialized)
        return;
    int n = (short)c_SpinSteps.GetPos();

    c_Graph.SetSteps(n);
    updateControls();
    }


void SineWaveDlg::OnClickedShowsine()
    {
    c_Graph.SetShowSine(c_ShowSine.GetCheck());
    updateControls();
    }


void SineWaveDlg::OnClickedShowstep()
    {
    c_Graph.SetShowStep(c_ShowStep.GetCheck());
    updateControls();
    }

/******************************************************************************
*                     SineWaveDlg::OnGetMinMaxInfo
* Inputs:
*     MIMAXINFO * lpMMI:
* Result: void
* 
* Effect:
*     Updates lpMMI to limit the size to which the dialog can be shrunken
* Notes:
*     The size is limited by the c_Frame window
*******************************************************************************/
void SineWaveDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
    {
    if(c_Frame.GetSafeHwnd() != NULL)
        { /* can limit */
        CRect r;
        c_Frame.GetWindowRect(&r);
        ScreenToClient(&r);
        CalcWindowRect(&r);
        lpMMI->ptMinTrackSize.x = r.Width();
        lpMMI->ptMinTrackSize.y = r.Height();
        return;
        } /* can limit */
    CDialogEx::OnGetMinMaxInfo(lpMMI);
    } // SineWaveDlg::OnGetMinMaxInfo


void SineWaveDlg::OnClickedShowdebug()
    {
    c_Graph.SetDebug(c_ShowDebug.GetCheck());
    updateControls();
    }

/***************************************************************************************
*                              SineWaveDlg::OnChangeTheta
* Result: void
* 
* Effect:
*      The angle for the sine approximation has changed.  It will be in the range 0..359
*      The graph window is notified of this change
****************************************************************************************/
void SineWaveDlg::OnChangeTheta()
    {
    if(c_SpinTheta.GetSafeHwnd() == NULL)
        return;
    if (!initialized)
        return;

    int n = c_SpinTheta.GetPos32();
    c_Graph.SetTheta(n);
    updateControls();
    }


void SineWaveDlg::OnBnClickedShowSamples()
    {
    c_Graph.SetShowSamples(c_ShowSamples.GetCheck());
    updateControls();
    }


void SineWaveDlg::OnClickedShowaxis()
    {
    c_Graph.SetShowAxis(c_ShowAxis.GetCheck());
    updateControls();
    }

/*****************************************************************************
*                      SineWaveDlg::OnEnChangeWaves
* Result: void
* 
* Effect:
*    The number of waves to display has changed.  Update the graph window value
*****************************************************************************/
void SineWaveDlg::OnEnChangeWaves()
    {
    if(c_SpinWaves.GetSafeHwnd() == NULL)
        return;

    int n = (short) c_SpinWaves.GetPos();
    c_Graph.SetWaves(n);
    updateControls();
    }


void SineWaveDlg::OnRectifyFull()
    {
    updateControls();
    }


void SineWaveDlg::OnRectifyHalf()
    {
    updateControls();
    }


void SineWaveDlg::OnRectifyNone()
    {
    updateControls();
    }


void SineWaveDlg::OnChangePhase()
    {
     updateControls();
    }


void SineWaveDlg::OnClickedShowRms()
    {
     c_Graph.SetShowRMS((c_ShowRMS.GetState() & BST_CHECKED) != 0);
    }


void SineWaveDlg::OnClickedShowclassd()
    {
     c_Graph.SetShowClassD((c_ShowClassD.GetState() & BST_CHECKED) != 0);
     updateControls();
    }


void SineWaveDlg::OnBnClickedZerophase()
    {
     c_SpinPhase.SetPos32(0);
    }


void SineWaveDlg::OnClickedShowbase()
    {
     c_Graph.SetShowBase((c_ShowBase.GetState() & BST_CHECKED) != 0);
     updateControls();
    }


void SineWaveDlg::OnClickedShowsum()
    {
     c_Graph.SetShowSum((c_ShowSum.GetState() & BST_CHECKED) != 0);
    }


void SineWaveDlg::OnClickedShowdiff()
    {
     c_Graph.SetShowDiff((c_ShowDiff.GetState() & BST_CHECKED) != 0);
    }


void SineWaveDlg::OnClickedNinetyphase()
    {
     c_SpinPhase.SetPos32(90);
    }


void SineWaveDlg::OnClickedOneeightyphase()
    {
     c_SpinPhase.SetPos32(180);
    }


void SineWaveDlg::OnClickedTwoseventyphase()
    {
     c_SpinPhase.SetPos32(270);
    }

/***************************************************************************
*             SineWaveDlg::SetADCVals
****************************************************************************/
void SineWaveDlg::SetADCVals()
    {
    int ADClimit;

    if(c_ADC_8.GetCheck() == BST_CHECKED)
        {
         ADClimit = 256;
        }
    else
    if(c_ADC_10.GetCheck() == BST_CHECKED)
        {
         ADClimit = 1023;
        }
    else
    if(c_ADC_11.GetCheck() == BST_CHECKED)
         {
          ADClimit = 2047;
         }
    else
    if(c_ADC_12.GetCheck() == BST_CHECKED)
         {
          ADClimit = 4095;
         }
    else
    if(c_ADC_16.GetCheck() == BST_CHECKED)
         {
          ADClimit = 65535;
         }
    else
         {
            // This is optimized to reduce flicker
            // This puts out a message that a resolution should be chosen
            CString s;
            s.LoadString(IDS_SELECT_BIT);
            CString old;
            c_ADC.GetWindowText(old);
            if(old != s)
                c_ADC.SetWindowText(s);
             return;
          }

    // If a resolution was chosen, we get here, so update the output
   
     c_Graph.SetADCRange(ADClimit);
     c_Graph.SetADCAref(ArefVoltage);
    }



void SineWaveDlg::OnZeroBottom()
    {
    c_Graph.SetZeroPos(IDC_ZERO_BOTTOM);
    updateControls();
    }

void SineWaveDlg::OnZeroCenter()
    {
    c_Graph.SetZeroPos(IDC_ZERO_CENTER);
    updateControls();
    }


void SineWaveDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
    {
     if((CSliderCtrl *) pScrollBar == &c_CapacitorSlider)
         { /* capacitor*/
         updateControls();
         c_Graph.Invalidate();
         return;
         } /* capacitor*/
     if((CSliderCtrl *) pScrollBar == & c_ResistorSlider)
         { /* resistor */
         updateControls();
         c_Graph.Invalidate();
         return;
         } /* resistor */

    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
    }


void SineWaveDlg::OnClickedShowfiltered()
    {
    c_Graph.SetShowFiltered(c_ShowFiltered.GetCheck() == BST_CHECKED);
    updateControls();
    }


void SineWaveDlg::OnClicked3v3()
    {
    updateControls();
    }


void SineWaveDlg::On5v0()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedResetAll()
    {
    DoReset();
    updateControls();
    }


void SineWaveDlg::OnClickedShowregulated()
    {
    updateControls();

    }


void SineWaveDlg::OnClickedShowregulationfailure()
    {
    updateControls();
    }


void SineWaveDlg::OnChangeDropout()
    {
     if(c_SpinDropout.GetSafeHwnd() == NULL)
        return;
     int n = c_SpinDropout.GetPos32();
     c_Graph.SetDropout((double)n / 1000.0);
    
    }


void SineWaveDlg::OnChangeOtherInputValue()
    {
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    if(c_SpinOtherInput.GetSafeHwnd() == NULL)
        return;
    if (!initialized)
        return;
    if(c_OtherInputV.GetCheck() != BST_CHECKED)
        CheckRadioButton(IDC_6V3, IDC_OTHER_INPUT_V, IDC_OTHER_INPUT_V);
    // it has changed, and other is selected

    updateControls();
    }


void SineWaveDlg::OnChangeOtherOutputValue()
    {
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    if(c_SpinOtherOutput.GetSafeHwnd() == NULL)
        return;
    if (!initialized)
        return;
    if(c_OtherOutputV.GetCheck() != BST_CHECKED)
        CheckRadioButton(IDC_3V3, IDC_OTHER_OUTPUT_V, IDC_OTHER_OUTPUT_V);
    int n = c_SpinOtherOutput.GetPos32();
    c_Graph.SetOutputVoltage((double) n);
    c_CircuitWindow.SetOutputVoltage((double) n);
    updateControls();
    }


void SineWaveDlg::OnClickedOtherInputV()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedOtherOutputV()
    {
    updateControls();
    }


void SineWaveDlg::OnClicked6v3()
    {
    updateControls();
    }


void SineWaveDlg::OnClicked12v6()
    {
    updateControls();
    }


void SineWaveDlg::OnBnClickedShowvalues()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedDropout2v()
    {
    c_SpinDropout.SetPos32(2000);
    updateControls();
    }


void SineWaveDlg::OnClickedDropout700mv()
    {
    c_SpinDropout.SetPos32(700);
    updateControls();
    }


void SineWaveDlg::OnClickedDropout200mv()
    {
    c_SpinDropout.SetPos32(200);
    updateControls();
    }


void SineWaveDlg::OnClickedDropout350mv()
    {
    c_SpinDropout.SetPos32(350);
    updateControls();
    }


void SineWaveDlg::OnClickedAref1v0()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedAref1v1()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedAref3v3()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedAref5v0()
    {
    updateControls();
    }


void SineWaveDlg::OnClickedArefOther()
    {
    updateControls();
    }


void SineWaveDlg::OnChangeOff()
{
    if (!initialized)
        return;
    int off = c_SpinOff.GetPos32();
    double v = (double)off / 100.0;
    c_Graph.SetSchmittOff(v);
}


void SineWaveDlg::OnChangeOn()
{
    if (!initialized)
        return;
    int on = c_SpinOn.GetPos32();
    double v = (double)on / 100.0;
    c_Graph.SetSchmittOn(v);
    c_CircuitWindow.SetSchmittOnThreshold(v);
}


void SineWaveDlg::OnClickedEnableSchmitt()
{
    updateControls();

}


void SineWaveDlg::OnClickedShowPWM()
{
    c_Graph.SetShowPWM(c_ShowPWM.GetCheck() == BST_CHECKED);
    updateControls();
}


void SineWaveDlg::OnChangePwmWidth()
{
    if (!initialized)
        return;
    c_Graph.SetPWMWidth(c_SpinPWMWidth.GetPos32());

}


void SineWaveDlg::OnClickedInvertSchmitt()
{
    bool invert = c_InvertSchmitt.GetCheck() == BST_CHECKED;
    c_Graph.SetSchmittInvert(invert);
    c_CircuitWindow.SetSchmittInvert(invert);
}


void SineWaveDlg::OnClickedInvertPwm()
{
    bool invert = c_InvertPWM.GetCheck() == BST_CHECKED;
    c_Graph.SetInvertPWM(invert);
}


void SineWaveDlg::OnChangeVimax()
{
    if (!initialized)
        return;
    int v = c_SpinViMax.GetPos32();
    c_Graph.SetSchmittViMax((double)v);
    c_CircuitWindow.SetSchmittViMax((double)v);
}


void SineWaveDlg::OnClickedViSafe()
{
    bool enable = c_ViSafe.GetCheck() == BST_CHECKED;
    c_CircuitWindow.SetSchmittViSafe(enable);

}

/***************************************************************
*                SineWaveDlg::ShowClip
* Inputs:
*     bool doShow: false to hide the controls, true to show them
* Effect:
*     Shows or hides the clipboard controls and, it present, the
*     debug controls, so they do not clutter up the saved image
***************************************************************/
void SineWaveDlg::ShowClip(bool doShow)
{
    UINT show = doShow ? SW_SHOW : SW_HIDE;
    x_Clipboard.ShowWindow(show);
    c_ClipFull.ShowWindow(show);
    c_ClipClient.ShowWindow(show);
    c_ClipWave.ShowWindow(show);
    c_ClipWaveDelay.ShowWindow(show);
    c_ClipCircuit.ShowWindow(show);

#ifndef _DEBUG
    ShowDebug(false);  // Hide debug controls in the release version
#else
    ShowDebug(doShow); // Show debug controls if in debug version according to doShow request
#endif


}

/*******************************************************************
*                 SineWaveDlg::OnClickedClipClient
* Result: void
* Effect:
*    Hides the debug controls and the clip constrols so they don't appear
*    in the screenshot.  Then transfers the entire client area bitmap
*    to the clipboard.
*    Finally, it unhides the controls
********************************************************************/
void SineWaveDlg::OnClickedClipClient()
{
    ShowClip(false);           // Hide the cliboard commands
    toClipboard(this, false);
    ShowClip(true);            // Re-show the clipboard commands
}
/*******************************************************************
*                 SineWaveDlg::OnClickedClipFull
* Result: void
* Effect:
*    Hides the debug controls and the clip constrols so they don't appear
*    in the screenshot.  Then transfers the entire window area bitmap
*    to the clipboard.
*    Finally, it unhides the controls
* Notes:
*    This used to work correctly, but now it gives 9 extra pixels to
*    the left, right, and bottom (but not the top) of the window, for
*    reasons that do no make any sense whatsoever.
********************************************************************/

void SineWaveDlg::OnClickedClipFull()
{
    ShowClip(false);
    toClipboard(this, true);
    ShowClip(true);
}

/*******************************************************************
*                 SineWaveDlg::OnClickedClipWaveform
* Result: void
* Effect:
*    Transfers a bitmap image of the waveform to the clipboard
********************************************************************/
void SineWaveDlg::OnClickedClipWaveform()
{
    toClipboard(&c_Graph, true);
    c_Graph.Flash();
}
/****************************************************************************************
*                            SineWaveDlg::OnDelayDone
* Inputs:
*     WPARAM: unused
*     LPARAM: unused
* Result: LRESULT
*     Logically void, 0, always
****************************************************************************************/
LRESULT SineWaveDlg::OnDelayDone(WPARAM, LPARAM)
{
    OnClickedClipWaveform();
    waiting = false;
    updateControls();
    return 0;
}
/*******************************************************************
*                 SineWaveDlg::OnClickedClipCircuit
* Result: void
* Effect:
*    Transfers a bitmap image of the circuit to the clipboard
********************************************************************/

void SineWaveDlg::OnClickedClipCircuit()
{
    toClipboard(&c_CircuitWindow, true);
    c_CircuitWindow.Flash();
}


void SineWaveDlg::OnClickedVf200mv()
{
    c_SpinVF.SetPos32(200);
}


void SineWaveDlg::OnClickedVf700mv()
{
    c_SpinVF.SetPos32(700);
}


void SineWaveDlg::OnClickedVf300mv()
{
    c_SpinVF.SetPos32(300);
}


void SineWaveDlg::OnChangeVf()
{
    if (!initialized)
        return;
    updateControls();
}


void SineWaveDlg::OnClickedInvalidateCircuit()
{
    c_CircuitWindow.Invalidate();
}


void SineWaveDlg::OnClickedShowregulationfailures()
{
    c_Graph.SetShowRegulationFailure(c_ShowRegulationFailure.GetCheck() == BST_CHECKED);
}


void SineWaveDlg::OnClickedSineDashed()
{
    bool dashed = c_SineDashed.GetCheck() == BST_CHECKED;
    c_Graph.SetDashed(dashed);
}


void SineWaveDlg::OnClickedZeroCenter()
{
    // TODO: Add your control notification handler code here
}


void SineWaveDlg::OnClickedZeroBottom()
{
    // TODO: Add your control notification handler code here
}


void SineWaveDlg::OnClickedAdcHex()
{
    c_Graph.SetShowADCHex(c_ADC_Hex.GetCheck() == BST_CHECKED);
}


void SineWaveDlg::OnClickedAdcPercent()
{
    updateControls();
}


void SineWaveDlg::OnClickedAdcVolts()
{
    c_Graph.SetShowADCVolts(c_ADC_Volts.GetCheck() == BST_CHECKED);
}


void SineWaveDlg::OnClickedAdcDec()
{
    c_Graph.SetShowADCDec(c_ADC_Dec.GetCheck() == BST_CHECKED);
}





void SineWaveDlg::OnClickedShowmarks()
{
    c_Graph.SetShowMarks(c_ShowMarks.GetCheck() == BST_CHECKED);
}


void SineWaveDlg::OnClickedClipWavedelay()
{
    c_Spinner.StartClock();
    waiting = true;
    updateControls();
}

void SineWaveDlg::OnTimer(UINT_PTR id)
{
    c_Spinner.OnTimer(id);
}
