#include<stdio.h>
#include"utils.h"
#include<limits.h>
#include<vector>
using namespace std;

static uint16_t gcd(uint16_t a, uint16_t b);

/**********************gcd()*******************************/
/* Description: Calculates the Greatest common divisor of
 * the two input variables. Used by the LCM function. Algorithm
 * based on recursive calculation. Local function.
 * Inputs:
 *          a (uint16_t) : First number
 *          b (uint16_t) : Second number
 * Return:  uint16_t : GCD
**********************************************************/

static uint16_t gcd(uint16_t a, uint16_t b)
{
	if (b == 0) 
        	return a; 
    return gcd(b, a % b); 	
}


/**********************lcm()*******************************/
/* Description: The least common multiple of a given array of
 * numbers.
 * Inputs:
 *          a (uint16_t array) : Set of numbers
 *          b (uint16_t) : Number of values
 * Return:  uint16_t : LCM
**********************************************************/

extern uint16_t lcm (vector<uint16_t> &arr_au16, uint16_t n_u16)
{
	uint16_t ans_u16 = arr_au16[0];
	 for (int i = 1; i < n_u16; i++) 
        ans_u16 = (((arr_au16[i] * ans_u16)) / 
                (gcd(arr_au16[i], ans_u16))); 
  
    return ans_u16;
}

/**********************delay()*****************************/
/* Description: Generates a small delay.
 * Inputs: None
 * Return: None
**********************************************************/

extern void delay(void)
{
	for(int i =0; i<SHRT_MAX;i++)
	{
		for(int j =0; j<SHRT_MAX;j++);
	}
	return;
}
