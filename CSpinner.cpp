#include "pch.h"
#include "CSpinner.h"
#include "DelayDone.h"
#include "Colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSpinner::DoDataExchange(CDataExchange* pDX)
{
    CProgressCtrl::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSpinner, CProgressCtrl)
    ON_WM_TIMER()
END_MESSAGE_MAP()

void CSpinner::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == Timer)
       { /* our timer */
        ticks--;
        if (ticks <= TICK_CAUTION && ticks > TICK_OUT)
            SetBarColor(Colors::TickCaution);
        if (ticks <= TICK_OUT)
            SetBarColor(Colors::TickOut);
        SetPos(ticks);
        UpdateWindow();
        if (ticks <= 0)
           { /* done */
            GetParent()->KillTimer(Timer);
            ShowWindow(SW_HIDE);
            GetParent()->PostMessage(UWM_DELAY_DONE);
            Timer = 0;
           } /* done */
       } /* our timer */
    CProgressCtrl::OnTimer(nIDEvent);
}

#define msTo100ns(t) ((t) * 1000 * 10)

void CSpinner::StartClock()
{
    if (Timer != 0)
        return; // already timing, should not be here
    int ms = DELAY_TIME / TICK_UNITS;
    Timer = GetParent()->SetTimer(1, ms, NULL);
    SetBarColor(Colors::TickStarting);
    if (Timer == 0)
        return; // TODO: Handle this problem (it should never occur!)

    ShowWindow(SW_SHOW);
    EnableWindow(true);
    SetRange32(0, TICK_UNITS);
    ticks = TICK_UNITS;
    SetPos(ticks);
    UpdateWindow();
}

