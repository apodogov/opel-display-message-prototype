#include <stdio.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "Log.h"
#include "Timer.h"
#include "CanConnection/OpelCan.h"

#include "MessageSender.h"

#include "CanConnection/PacketsProcessors/WheelButtonsProcessor.h"
#include "CanConnection/PacketsProcessors/IgnitionStateProcessor.h"

static bool enterDeepSleep = false;

int main(void)
{
	Log_Init();
	LogLine("Start");

	Timer_Init();
	OpelCan_Init();
	
	MessageSender_Init();
	MessageSender_SetMessage("Bluetooth");
	
	sei();
	while (true)
	{
		OpelCan_ProcessRxBuffers();
		MessageSender_ProcessOverride();
		
		if (MessageSender_IsSendingNow())
		{
			
		}
		else if (enterDeepSleep)
		{
			enterDeepSleep = false;
			
			LogLine("Sleep");
			OpelCan_Sleep();
			
			set_sleep_mode(SLEEP_MODE_PWR_SAVE);
			sleep_mode();
			
			LogLine("Wake");
			OpelCan_Reset();
		}
		else
		{
			set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
			sleep_mode();
		}
	}
}

void OnTimer()
{
}

void OnWheelButtonPressed(WheelButtonCode code)
{
	LogText("Wheel button pressed: 0x");
	LogHexLn(code);
	
	if (code == WheelButtonCode_Arrow)
	{
		MessageSender_ActivateOverride();
	}
	else if (code == WheelButtonCode_VolumeDown)
	{
		MessageSender_DeactivateOverride();
	}
	else if (code == WheelButtonCode_VolumeUp)
	{
		enterDeepSleep = true;
	}
}

void OnWheelButtonReleased(WheelButtonCode code)
{
	LogText("Wheel button released: 0x");
	LogHexLn(code);
}

void OnIgnitionStateChanged(IgnitionStateCode state)
{
	LogText("Ignition state: 0x");
	LogHexLn(state);
}
