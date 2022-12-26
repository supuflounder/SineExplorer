#include "pch.h"
#include "RC.h"
/*********************************************************************************
* This file is released under the terms of Creative Commons 4.0 CC BY-NC-SA
* This notice, attributing the original code to Joseph M. Newcomer,
* newcomer@flounder.com, must be retained in all derivative copies.
*********************************************************************************/

/*****************************************************************
* This moodule computes voltage values based on the RC time constants
*****************************************************************/

/****************************************************************
*               RC::SetV
* Inputs:
*    double volts: The current voltage on the wire
*    double dT: The amount of time that has elapsed
* Result: void
* Effect:
*     Sets the current RC network capacitor voltage based on whether the
*     capacitor is charging or discharging
*     Updates the voltage V based on the charge/discharge state
*****************************************************************/

void RC::SetV(double volts, double dT)
    {
		 /*************
        * Vc = Vs * e^(-t/R*C)
        * t is in seconds since Vs was removed
        * V is in volts
        * R is in ohms
        * C is in farads
        * Vs * exp(-t/R*C)
        * Note that if Vc < volts, we use volts and set V to volts
        * If Vc > volts, we set V to Vc and ignore volts
        **************/
    if(volts >= V)
        { /* charging*/
            V = volts;
            Vs = V;  // set our starting voltage as well
            T = 0.0; // No time has elapsed
            return;
        } /* charging */
    else
        { /* discharging */ 
        // TODO: Compute remaining charge base on RC time value
        T += dT;
       
        double Vc =  Vs * exp(-T / (R * C * 1.0E-6));
        V = max(volts, Vc);
        return;
        } /* discharging */

    }
