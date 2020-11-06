#include "headerDefinitions.h"
#include <iostream>
#include<stdio.h>
#include <vector>
#include<string>
#include<algorithm>
#include<limits.h>
#include"utils.h"


using namespace std;
/**************************Global variables*************************************************************/
vector <taskInfo_st> taskDetails_ast;
uint8_t numberOfTasks_u8 = 0;
vector <taskInfo_st> readyTasks_ast;
uint8_t tasksReady_u8 = 0;
/***********************Local functions*******************************************************************/
static void simulator();

/*Main function*/
int main(void)
{
    float U_ft = 0;  //Variable to calculate the Utilization factor
    uint16_t hyperperiod_u16 = 0; //Variable to calculate hyperperiod
    vector<uint16_t> taskPeriods_au16; //Variable to store periods (Used while calculating hyperperiod) TODO: Try to improve
    /*Get number of tasks*/
    printf("Enter the number of tasks\n");
    scanf("%hhu",&numberOfTasks_u8);
    /*Get the period for each task*/
    printf("%hhu tasks are created. Enter the task periods\n",numberOfTasks_u8);
    /*Loop until all periods are entered*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        taskDetails_ast.push_back(taskInfo_st());
        taskPeriods_au16.push_back(uint16_t());
        scanf("%hu",&taskDetails_ast[i].period_u16);
        taskPeriods_au16[i] = taskDetails_ast[i].period_u16;  //Copy onto local variable
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
    hyperperiod_u16 = lcm(taskPeriods_au16,numberOfTasks_u8);
    printf("Hyperperiod = %hu \n",hyperperiod_u16);
    for(int i = 0; i<hyperperiod_u16;i++)
        simulator();

//    for(int i = 0; i<tasksReady_u8;i++)
//    {
//        printf("Task ID : %hhu is in the ready list\n",taskList_au8[i]);
//    }
        //simulator();
    return 1;
}

/*Simulates the system*/
static void simulator()
{
    static uint16_t systemTime_u16 = 0;    //Simulates time
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        if((systemTime_u16 % taskDetails_ast[i].period_u16) == 0)
        {
            readyTasks_ast.push_back(taskInfo_st());
            readyTasks_ast[tasksReady_u8] = taskDetails_ast[i];
            tasksReady_u8++;
        }
    }
    std::sort(readyTasks_ast.begin(), readyTasks_ast.end(), sortWaitingList);
//     for(int i = 0; i<numberOfTasks_u8;i++)
//        cout<<"Task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" with period "<<readyTasks_ast[i].period_u16<<" at index "<<i<<endl;
    if(tasksReady_u8 > 0)
    {
        cout<<"System Time: "<<systemTime_u16<<" Scheduling task "<<(unsigned)readyTasks_ast[0].taskId_u8<<" with period"<<readyTasks_ast[0].period_u16<<endl;
        readyTasks_ast[0].computationTime_u16--;
        if(readyTasks_ast[0].computationTime_u16 == 0)
        {
            tasksReady_u8--;
            readyTasks_ast.erase(readyTasks_ast.begin());
        }
    }
    else
        cout<<"System Time: "<<systemTime_u16<<" Idle"<<endl;
    delay();
    systemTime_u16++;


    return;
}
