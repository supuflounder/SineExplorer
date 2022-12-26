/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/
// SineWaveDlg.h : header file
//

#pragma once
#include "cgraph.h"
#include "RC.h"
#include "CircuitWindow.h"
#include "CSpinner.h"
// SineWaveDlg dialog
class SineWaveDlg : public CDialogEx
{
// Construction
public:
	SineWaveDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATIONSTEP2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void ShowControlsClassD(bool mode);

// Implementation
protected:
	HICON m_hIcon;
	bool initialized;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void updateControls();
	void ShowClip(bool doShow);
	void ShowDebug(bool show);
	void ShowVF(bool b);
	void ShowStepInfo(bool b, bool b2);
	DECLARE_MESSAGE_MAP()

	int LeftMargin;
	int RightMargin;
	int BottomMargin;
	int TopMargin;
	bool waiting; // waiting for snapshot


	double ArefVoltage;

	void ShowStepFunction(bool mode);
	afx_msg void OnIdclose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CGraph c_Graph;
	afx_msg void OnBnClickedInvalidate();
	afx_msg void OnChangeSteps();
	afx_msg void OnClickedShowsine();
	afx_msg void OnClickedShowstep();
	CButton c_ShowSine;
	CButton c_ShowStep;
	CEdit c_Steps;
	CSpinButtonCtrl c_SpinSteps;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	afx_msg void OnClickedShowdebug();
	CButton c_ShowDebug;
	CEdit c_Theta;
	CSpinButtonCtrl c_SpinTheta;
	afx_msg void OnChangeTheta();
	CStatic c_Degrees;
	CStatic c_Samples;
	afx_msg void OnBnClickedShowSamples();
	CButton c_ShowSamples;
	afx_msg void OnClickedShowaxis();
	CButton c_ShowAxis;
	CSpinButtonCtrl c_SpinWaves;
	afx_msg void OnEnChangeWaves();
	CStatic c_Frame;
	CButton c_RectifyNone;
	CButton c_RectifyHalf;
	CButton c_RectifyFull;
	afx_msg void OnRectifyFull();
	afx_msg void OnRectifyHalf();
	afx_msg void OnRectifyNone();
	CSpinButtonCtrl c_SpinPhase;
	afx_msg void OnChangePhase();
	CButton c_ShowRMS;
	afx_msg void OnClickedShowRms();
	afx_msg void OnClickedShowclassd();
	CButton c_ShowClassD;
	void ShowRectification(bool mode);
	afx_msg void OnBnClickedZerophase();
	CEdit c_Phase;
	CButton c_ZeroPhase;
	CButton c_ShowBase;
	CButton c_ShowSum;
	CButton c_ZeroCenter;
	CButton c_ZeroBottom;
	afx_msg void OnClickedShowbase();
	afx_msg void OnClickedShowsum();
	afx_msg void OnClickedShowdiff();
	CButton c_ShowDiff;
	afx_msg void OnClickedNinetyphase();
	afx_msg void OnClickedOneeightyphase();
	afx_msg void OnClickedTwoseventyphase();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CStatic c_DebugXY;
	afx_msg void OnZeroBottom();
	afx_msg void OnZeroCenter();
	CButton c_TwoSeventyPhase;
	CButton c_NinetyPhase;
	CButton c_OneEightyPhase;
	CButton c_ADC_10;
	CButton c_ADC_11;
	CButton c_ADC_12;
	CButton c_ADC_16;
	CButton c_ADC_8;
	CStatic c_ADC;
	void ShowADC(bool mode);
	void SetADCVals();
	void ShowRegulated(bool mode);
    CStatic x_ADC;
	CSliderCtrl c_CapacitorSlider;
	CStatic c_CapacitorDisplay;
	CButton c_50Hz;
	CButton c_60Hz;
	CSliderCtrl c_ResistorSlider;
	CStatic c_ResistorDisplay;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	RC filter;
	CButton c_ShowFiltered;
	afx_msg void OnClickedShowfiltered();
	CStatic x_Hz;
	CStatic x_Capacitance;
	CStatic x_Resistance;
	void ShowFiltered(bool b);
	CButton c_3V3;
	CButton c_5V0;
	CStatic x_Current;
	CStatic c_Current;
	void DoReset();
	afx_msg void OnClicked3v3();
	afx_msg void On5v0();
	CStatic x_Debug;
	afx_msg void OnClickedResetAll();
	CButton c_Invalidate;
	CircuitWindow c_CircuitWindow;
	afx_msg void OnClickedShowregulated();
	CButton c_ShowRegulated;
	afx_msg void OnClickedShowregulationfailure();
	CButton c_ShowRegulationFailure;
	CStatic x_Dropout1;
	CEdit c_Dropout;
	CStatic x_Dropout2;
	CSpinButtonCtrl c_SpinDropout;
	afx_msg void OnChangeDropout();
	CStatic x_Rectification;
	CStatic x_InputVoltage;
	CEdit c_OtherInputVoltage;
	CEdit c_OtherOutputVoltage;
	CSpinButtonCtrl c_SpinOtherInput;
	CSpinButtonCtrl c_SpinOtherOutput;
	CButton c_6V3;
	CButton c_12V6;
	CButton c_OtherInputV;
	afx_msg void OnChangeOtherInputValue();
	afx_msg void OnChangeOtherOutputValue();
	afx_msg void OnClickedOtherInputV();
	afx_msg void OnClickedOtherOutputV();
	CButton c_OtherOutputV;
	afx_msg void OnClicked6v3();
	afx_msg void OnClicked12v6();
	afx_msg void OnBnClickedShowvalues();
	CButton c_ShowValues;
	CStatic x_Zero;
	afx_msg void OnClickedDropout2v();
	afx_msg void OnClickedDropout700mv();
	CButton c_Dropout2V;
	CButton c_Dropout700mV;
	afx_msg void OnClickedDropout200mv();
	CButton c_Dropout200mV;
	afx_msg void OnClickedDropout350mv();
	CEdit c_PowerFactor;
	CButton c_Dropout350mV;
	CEdit c_ArefVoltage;
	CSpinButtonCtrl c_SpinOtherAref;
	CButton c_Aref_1V0;
	CButton c_Aref_1V1;
	CButton c_Aref_3V3;
	CButton c_Aref_5V0;
	CStatic x_Aref1;
	CStatic x_Aref2;
	CButton c_Aref_Other;
	afx_msg void OnClickedAref1v0();
	afx_msg void OnClickedAref1v1();
	afx_msg void OnClickedAref3v3();
	afx_msg void OnClickedAref5v0();
	afx_msg void OnClickedArefOther();
	afx_msg void OnChangeOff();
	afx_msg void OnChangeOn();
	CButton c_EnableSchmitt;
	CEdit c_Off;
	CStatic x_Off;
	CEdit c_On;
	CStatic x_On;
	afx_msg void OnClickedEnableSchmitt();
	CSpinButtonCtrl c_SpinOff;
	CSpinButtonCtrl c_SpinOn;
	CStatic x_PWM;
	CEdit c_PWM_Width;
	CButton c_ShowPWM;
	afx_msg void OnClickedShowPWM();
	afx_msg void OnChangePwmWidth();
	CSpinButtonCtrl c_SpinPWMWidth;
	CButton c_InvertSchmitt;
	afx_msg void OnClickedInvertSchmitt();
	afx_msg void OnClickedInvertPwm();
	CButton c_InvertPWM;
	afx_msg void OnChangeVimax();
	CSpinButtonCtrl c_SpinViMax;
	CEdit c_ViMax;
	CStatic x_ViMax;
	afx_msg void OnClickedViSafe();
	CButton c_ViSafe;
	afx_msg void OnClickedClipClient();
	afx_msg void OnClickedClipFull();
	afx_msg void OnClickedClipWaveform();
	CButton c_ClipClient;
	CButton c_ClipFull;
	CButton c_ClipWave;
	CStatic x_Clipboard;
	afx_msg void OnClickedClipCircuit();
	CButton c_ClipCircuit;
	afx_msg void OnClickedVf200mv();
	afx_msg void OnClickedVf700mv();
	afx_msg void OnClickedVf300mv();
	CSpinButtonCtrl c_SpinVF;
	CEdit c_VF;
	CButton c_VF_200mV;
	CButton c_VF_300mV;
	CButton c_VF_700mV;
	CStatic x_VF;
	CStatic x_VF2;
	afx_msg void OnChangeVf();
	afx_msg void OnClickedInvalidateCircuit();
	CButton c_InvalidateCircuit;
	afx_msg void OnClickedShowregulationfailures();
	afx_msg void OnClickedSineDashed();
	afx_msg void OnClickedZeroCenter();
	CButton c_SineDashed;
	afx_msg void OnClickedZeroBottom();
	afx_msg void OnClickedAdcHex();
	afx_msg void OnClickedAdcPercent();
	afx_msg void OnClickedAdcVolts();
	afx_msg void OnClickedAdcDec();
	CStatic x_ADC2;
	CButton c_ADC_Dec;
	CButton c_ADC_Hex;
	CButton c_ADC_Percent;
	CButton c_ADC_Volts;

	CStatic x_Show;
	afx_msg void OnClickedShowmarks();
	CButton c_ShowMarks;
	afx_msg void OnClickedClipWavedelay();
	CButton c_ClipWaveDelay;
	afx_msg LRESULT OnDelayDone(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR id);
	CSpinner c_Spinner;
};
