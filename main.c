#include<stdio.h>
#include"defn.h"

taskInfo_st taskDetails_ast[10];
uint8_t numberOfTasks_u8 = 0;
void main(void)
{       /*Get number of tasks*/
	float U_ft = 0;
	printf("Enter the number of tasks\n");
	scanf("%hhu",&numberOfTasks_u8);
	/*Get the period for each task*/
	printf("%d tasks are created. Enter the task periods\n",numberOfTasks_u8);
	/*Loop until all periods are entered*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		scanf("%hhu",&taskDetails_ast[i].period_u8);	
	}
	/*Display entered periods*/
	printf("The entered periods are\n");
	/*Loop until all periods are displayed*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		printf("Period of task[%d] = %d\n",i,taskDetails_ast[i].period_u8);	
	}
	/*Get the computation time for each task*/
	printf("Enter computation times\n");
	/*Loop until all computation times are entered*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		scanf("%hhu",&taskDetails_ast[i].computationTime_u8);	
	}
	/*Display entered compuation times*/
	printf("The entered computation times are\n");
	/*Loop until all computation times are displayed*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		printf("Computation time of task[%d] = %d\n",i,taskDetails_ast[i].computationTime_u8);	
	}
	printf("Checking for feasibility \n");
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		U_ft = U_ft + ((float)(taskDetails_ast[i].computationTime_u8))/((float)(taskDetails_ast[i].period_u8));	
	}
	printf("U = %f \n",U_ft);
	return;
}
