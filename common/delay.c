/**
  @file delay.c
  @brief Delay source file.
  @author Marcelo Simbana (https://github.com/marcelosr97)
  @date 02/02/2023
*/

#include "typedef.h"

void delayMilliseconds(uint32 t_milliseconds)
{
	while(t_milliseconds--)
	{
		__delay_cycles(16000);
	}
}

void delayMicroseconds(uint32 t_microseconds)
{
	while(t_microseconds--)
	{
		__delay_cycles(16);
	}	
}
