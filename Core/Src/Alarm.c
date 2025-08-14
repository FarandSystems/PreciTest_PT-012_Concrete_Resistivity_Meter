/* Includes ------------------------------------------------------------------*/


#include "Alarm.h"

uint32_t Pattern = 0;
uint32_t alarmCount = 0;
uint32_t Pattern_Shifted = 0;
uint8_t BeepOnOff = 0;

uint8_t Message_Queue_Index = 0;
uint8_t Message_Queue_CurrentIndex = 0;
uint8_t shiftCount = 0;
uint8_t alarm_Time = 0;

Message_TypeDef MessageQueue[MESSAGE_QUEUE_SIZE];

void Alarm(uint32_t PatternInit, uint8_t NumberInit, uint8_t AlarmTime , uint8_t BeepOnOffInit)
{		
	if(Message_Queue_Index < MESSAGE_QUEUE_SIZE)
	{
		Message_Queue_Index ++;
		MessageQueue[Message_Queue_Index].Pattern = PatternInit;		
		MessageQueue[Message_Queue_Index].AlarmCount = NumberInit;
		MessageQueue[Message_Queue_Index].BeepOnOff = BeepOnOffInit;	
		MessageQueue[Message_Queue_Index].TimePeriod = AlarmTime;		
		
		if(Message_Queue_Index == 1) // check if this is the first alarm message
		{	
			// load initial values for the first alarm
			Pattern = MessageQueue[1].Pattern;
			Pattern_Shifted = Pattern;		
			alarmCount = MessageQueue[1].AlarmCount;
			BeepOnOff = MessageQueue[1].BeepOnOff;
			MessageQueue[1].TimePeriod = AlarmTime;
			alarm_Time = AlarmTime;		
			shiftCount = alarm_Time; // if it's the first alarm,  shiftCount should be preset
			Message_Queue_CurrentIndex++;
		
		}	 
	}
}
void UpdateAlarm(void)
{			
	if(Message_Queue_Index > 0)
	{	
		if(Pattern_Shifted & 0x01)
		{
			if(BeepOnOff == 1)
			{				
				HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
			}
		}		
		else
		{	
			HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);			
		}			
		
		Pattern_Shifted = Pattern_Shifted >> 1;
		shiftCount--;		
		if(shiftCount ==0)
		{			
			alarmCount--; // Count Number of alarm messages
			if(alarmCount>0)
			{				
				Pattern_Shifted = Pattern; // Reload Pattern
				shiftCount = alarm_Time;
			}
			else
			{
				// Go to next alarm if any				
				if(Message_Queue_CurrentIndex == Message_Queue_Index)
				{
					// if all alarms are played reset everything to wait for new alarm messages
					Message_Queue_CurrentIndex = 0;
					Message_Queue_Index = 0;
					Pattern_Shifted = 0;	
					HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
				}
				else
				{
					// Load next alarm message
					Message_Queue_CurrentIndex++; // read next message
					Pattern = MessageQueue[Message_Queue_CurrentIndex].Pattern;
					Pattern_Shifted = Pattern;
					alarmCount = MessageQueue[Message_Queue_CurrentIndex].AlarmCount;
					BeepOnOff = MessageQueue[Message_Queue_CurrentIndex].BeepOnOff;	
					alarm_Time = MessageQueue[Message_Queue_Index].TimePeriod;							
					shiftCount = alarm_Time;					
				}
			}
		}	
	}	
}






