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
vector <taskMetrics_st> metrics_ast;
/***********************Local functions*******************************************************************/
static void simulator();

/*Main function*/
int main(void)
{
    float U_ft = 0;  //Variable to calculate the Utilization factor
    uint16_t hyperperiod_u16 = 0; //Variable to calculate hyperperiod
    vector<uint16_t> taskPeriods_au16; //Variable to store periods (Used while calculating hyperperiod) TODO: Try to improve
    float averageResponseTime_ft = 0;
    /*Get number of tasks*/
    printf("Enter the number of tasks\n");
    scanf("%hhu",&numberOfTasks_u8);
    /*Get the period for each task*/
    printf("%hhu tasks are created. Enter the task periods\n",numberOfTasks_u8);
    /*Loop until all periods are entered*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        taskDetails_ast.push_back(taskInfo_st());
        metrics_ast.push_back(taskMetrics_st());
        taskPeriods_au16.push_back(uint16_t());
        scanf("%hu",&taskDetails_ast[i].period_u16);
        taskDetails_ast[i].deadline_u16 = taskDetails_ast[i].period_u16;
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
    if(U_ft > 1)
    {
        printf("ERROR: Tasks not schedulable\n");
        exit(1);
    }
    hyperperiod_u16 = lcm(taskPeriods_au16,numberOfTasks_u8);
    printf("Hyperperiod = %hu \n",hyperperiod_u16);
    for(int i = 0; i<hyperperiod_u16;i++)
        simulator();
    for(int i = 0;i<numberOfTasks_u8;i++)
    {
        for(int j = 0;j<metrics_ast[i].responseTimes_au16.size();j++)
            averageResponseTime_ft += metrics_ast[i].responseTimes_au16[j];
        averageResponseTime_ft = (float)averageResponseTime_ft/(float)(metrics_ast[i].responseTimes_au16.size());
        cout<<"Average response time of task "<<i<<" is "<<averageResponseTime_ft<<endl;
        averageResponseTime_ft = 0;
    }

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
            readyTasks_ast[tasksReady_u8].arrivalTime_u16 = systemTime_u16;
            readyTasks_ast[tasksReady_u8].deadline_u16+=systemTime_u16;
            tasksReady_u8++;
        }
    }
    std::sort(readyTasks_ast.begin(), readyTasks_ast.end(), sortWaitingList);

    if(tasksReady_u8 > 0)
    {
        for(int i = 0;i<tasksReady_u8;i++)
        {
            if(readyTasks_ast[i].deadline_u16 <= systemTime_u16)
            {
                cout<<"Schedule unfeasible. Deadline of task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" missed. System time: "<<systemTime_u16<<endl;
                exit(-1);
            }
        }
        cout<<"System Time: "<<systemTime_u16<<" Scheduling task "<<(unsigned)readyTasks_ast[0].taskId_u8<<" with period"<<readyTasks_ast[0].period_u16<<endl;
        readyTasks_ast[0].computationTime_u16--;
        if(readyTasks_ast[0].computationTime_u16 == 0)
        {
            tasksReady_u8--;
            metrics_ast[readyTasks_ast[0].taskId_u8].responseTimes_au16.push_back(uint16_t());
            metrics_ast[readyTasks_ast[0].taskId_u8].responseTimes_au16[metrics_ast[readyTasks_ast[0].taskId_u8].taskInstance] = systemTime_u16 - readyTasks_ast[0].arrivalTime_u16 + 1;
            metrics_ast[readyTasks_ast[0].taskId_u8].taskInstance++;
            readyTasks_ast.erase(readyTasks_ast.begin());
        }
    }
    else
        cout<<"System Time: "<<systemTime_u16<<" Idle"<<endl;
    delay();
    systemTime_u16++;


    return;
}
