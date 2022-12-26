#pragma once

class CSpinner : public CProgressCtrl {
	static const int DELAY_TIME = 10000;  // delay time in ms
	static const int TICK_UNITS = 8;
	static const int TICK_CAUTION = 5;
	static const int TICK_OUT = 2;
	int ticks;
	UINT_PTR Timer;
public:
	CSpinner() : CProgressCtrl() { Timer = 0;  }
	void StartClock();
protected:
	DECLARE_MESSAGE_MAP()
	void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};