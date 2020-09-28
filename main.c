#include<stdio.h>
#include<stdlib.h>
#include"defn.h"
#include"utils.h"

static void scheduler();
static void simulator();
taskInfo_st taskDetails_ast[10];
uint8_t numberOfTasks_u8 = 0;
uint8_t taskList_au8[40];
uint8_t tasksReady_u8 = 0;
void main(void)
{       
	float U_ft = 0;  //Variable to calculate the Utilization factor
	uint16_t hyperperiod_u16 = 0; //Variable to calculate hyperperiod
	uint16_t periods_au16[10]; //Variable to store periods (Used while calculating hyperperiod) TODO: Try to improve
	/*Get number of tasks*/
	printf("Enter the number of tasks\n");
	scanf("%hhu",&numberOfTasks_u8);
	/*Get the period for each task*/
	printf("%hhu tasks are created. Enter the task periods\n",numberOfTasks_u8);
	/*Loop until all periods are entered*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		scanf("%hu",&taskDetails_ast[i].period_u16);	
		periods_au16[i] = taskDetails_ast[i].period_u16;  //Copy onto local variable
		taskDetails_ast[i].taskId_u8 = i;
	}
	/*Display entered periods*/
	printf("The entered periods are\n");
	/*Loop until all periods are displayed*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		printf("Period of task[%d] = %d\n",i,taskDetails_ast[i].period_u16);	
	}
	/*Get the computation time for each task*/
	printf("Enter computation times\n");
	/*Loop until all computation times are entered*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		scanf("%hu",&taskDetails_ast[i].computationTime_u16);	
	}
	/*Display entered compuation times*/
	printf("The entered computation times are\n");
	/*Loop until all computation times are displayed*/
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		printf("Computation time of task[%d] = %d\n",i,taskDetails_ast[i].computationTime_u16);	
	}
	printf("Checking for feasibility \n");
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		U_ft = U_ft + ((float)(taskDetails_ast[i].computationTime_u16))/((float)(taskDetails_ast[i].period_u16));	
	}
	printf("U = %f \n",U_ft);
	if(U_ft >= 1)
	{
		printf("ERROR: Tasks not schedulable\n");
		exit(1);
	}
	hyperperiod_u16 = lcm(periods_au16,numberOfTasks_u8);
	printf("Hyperperiod = %hu \n",hyperperiod_u16);
	for(int i = 0; i<hyperperiod_u16;i++)
		simulator();
		
	for(int i = 0; i<tasksReady_u8;i++)
	{
		printf("Task ID : %hhu is in the ready list\n",taskList_au8[i]);
	}
	return;
}

/*Simulates the system*/
static void simulator()
{
	static uint16_t systemTime_u16 = 0;    //Simulates time
	for(int i = 0; i<numberOfTasks_u8;i++)
	{
		if((systemTime_u16 % taskDetails_ast[i].period_u16) == 0)
		{
			taskList_au8[tasksReady_u8] = taskDetails_ast[i].taskId_u8;	
			tasksReady_u8++;
		}
	}
	delay();
	systemTime_u16++;
	return;	
}
