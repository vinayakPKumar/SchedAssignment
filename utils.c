#include<stdio.h>
#include"utils.h"
#include<limits.h>

uint16_t gcd(uint16_t a, uint16_t b)
{
	if (b == 0) 
        	return a; 
    return gcd(b, a % b); 	
}

extern uint16_t lcm (uint16_t arr_au16[], uint16_t n_u16)
{
	uint16_t ans_u16 = arr_au16[0];
	 for (int i = 1; i < n_u16; i++) 
        ans_u16 = (((arr_au16[i] * ans_u16)) / 
                (gcd(arr_au16[i], ans_u16))); 
  
    return ans_u16;
}

extern void delay(void)
{
	for(int i =0; i<SHRT_MAX;i++)
	{
		for(int j =0; j<SHRT_MAX;j++);
	}
	return;
}