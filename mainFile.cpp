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
vector <taskInfo_st> taskDetails_ast; //Variable to hold the details of entered tasks
uint8_t numberOfTasks_u8 = 0;  //Variable to hold the number of tasks
vector <taskInfo_st> readyTasks_ast;  //Variable which holds the ready tasks, equivalent to ready list
uint8_t tasksReady_u8 = 0;  //Variable which indicates the number of ready tasks
vector <taskMetrics_st> metrics_ast;  //Variable for metric calculation for each task
/***********************Local functions*******************************************************************/
static void simulator();  //Definition of the simulator function

/**********************Main function***********************/
/* Description: The main function is the entry point for
 * this program. It solicits the input from the user and
 * creates the data structures for each task. It performs
 * the check of the necessary condition and calls the
 * simulator over the hyperperiod. After successful simulation
 * it prints the metrics such as idle time, average response
 *  time and average waiting time for each task.
 * Input:   None
 * Return: int
**********************************************************/
int main(void)
{
    float U_ft = 0;  //Variable to calculate the Utilization factor
    uint16_t hyperperiod_u16 = 0;  //Variable to calculate hyperperiod
    vector<uint16_t> taskPeriods_au16;  //Variable to store periods (Used while calculating hyperperiod)
    float averageResponseTime_ft,averageWaitTime_ft = 0;  //Variable for calculating the average metrics
    /*Get number of tasks*/

    printf("Enter the number of tasks\n");
    scanf("%hhu",&numberOfTasks_u8);

    /*Get the period for each task*/
    printf("%hhu tasks are created. Enter the task periods\n",numberOfTasks_u8);

    /*Loop until all periods are entered. Also create the structures for each task*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        /*Add a new element to the taskDetails_ast, metrics_ast, taskPeriods_au16 structure */
        taskDetails_ast.push_back(taskInfo_st());
        metrics_ast.push_back(taskMetrics_st());
        taskPeriods_au16.push_back(uint16_t());

        /*Read user input and update in the task structures*/
        scanf("%hu",&taskDetails_ast[i].period_u16);
        taskDetails_ast[i].deadline_u16 = taskDetails_ast[i].period_u16;  //Update deadline. Here deadline = period.
        taskPeriods_au16[i] = taskDetails_ast[i].period_u16;  //Copy onto local variable, used for hyperperiod computation.
        taskDetails_ast[i].taskId_u8 = i;  //Assign task ID. Here the task ID is equal to the index for simplicity
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
        scanf("%hu",&taskDetails_ast[i].computationTime_u16);  //Read computation time and update in the structure
    }

    /*Display entered compuation times*/
    printf("The entered computation times are\n");

    /*Loop until all computation times are displayed*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        printf("Computation time of task[%d] = %d\n",i,taskDetails_ast[i].computationTime_u16);
    }

    /*Check the necessary condition, i.e. Utilization factor (U) must be less than or equal to 1*/
    printf("Checking for feasibility \n");

    /*Loop for all tasks and calculate the utilization factor*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        U_ft = U_ft + ((float)(taskDetails_ast[i].computationTime_u16))/((float)(taskDetails_ast[i].period_u16));  //Calculation of U
    }

    /*Print the utilization factor*/
    printf("U = %f \n",U_ft);

    /*Necessary condition fails. Exit with error message*/
    if(U_ft > 1)
    {
        printf("ERROR: Tasks not schedulable\n");
        exit(1);
    }

    /*Necessary condition does not fail. Find hyperperiod over which the task needs to be simulated*/
    hyperperiod_u16 = lcm(taskPeriods_au16,numberOfTasks_u8);  //Call to function for calculation of hyperperiod

    /*Print the hyperperiod for the user*/
    printf("Hyperperiod = %hu \n",hyperperiod_u16);

    /*Call the simulator. Repeat for the duration of the hyperperiod*/
    for(int i = 0; i<hyperperiod_u16;i++)
        simulator();

    /*Display the idle time during the simulation. This metric is calculated. It could be manually measured as well by having another variable*/
    cout<<"Idle Time of the system over the hyperperiod = "<<((1-U_ft)*hyperperiod_u16)<< " time units"<<endl;

    /*Print the average response time metric for each task*/
    for(int i = 0;i<numberOfTasks_u8;i++)
    {
        for(int j = 0;j<metrics_ast[i].responseTimes_au16.size();j++)
            averageResponseTime_ft += metrics_ast[i].responseTimes_au16[j];  //Add all the response times
        averageResponseTime_ft = (float)averageResponseTime_ft/(float)(metrics_ast[i].responseTimes_au16.size());  //Divide by number of elements to find the average
        cout<<"Average response time of task "<<i<<" is "<<averageResponseTime_ft<<endl;  //Print the average
        averageResponseTime_ft = 0;  //Reset to 0 for next computation
    }

    /*Print the average wait times for all tasks*/
    for(int i = 0;i<numberOfTasks_u8;i++)
    {
        for(int j = 0;j<metrics_ast[i].waitTime_au16.size();j++)
            averageWaitTime_ft += metrics_ast[i].waitTime_au16[j];  //Add all the wait times.

        averageWaitTime_ft = (float)averageWaitTime_ft/(float)(metrics_ast[i].waitTime_au16.size());  //Divide by number of elements to find the average
        cout<<"Average wait time of task "<<i<<" is "<<averageWaitTime_ft<<endl;  //Print the average
        averageWaitTime_ft = 0;  //Reset to 0 for next computation
    }

    /*Function complete, return*/
    return 1;
}

/**********************simulator()************************/
/* Description: The simulator performs a simulation of the
 * schedule for the given task set. It generates the
 * schedule by using the Rate Monotonic algorithm. It updates
 * the metrics such as response time and wait time for
 * each task it simulates. It also detects deadline
 * overruns and terminates with an error message in case
 * of an overrun.
 * Input:   None
 * Return:  None
**********************************************************/
static void simulator()
{
    static uint16_t systemTime_u16 = 0;    //Simulates system time

    /*Check for released tasks*/
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        if((systemTime_u16 % taskDetails_ast[i].period_u16) == 0)  //Task is released when the system time is a multiple of its period
        {
            /*Add the task to the ready list*/
            readyTasks_ast.push_back(taskInfo_st());  //Create new element in the ready list

            /*Update task details such as arrival time, deadlines*/
            readyTasks_ast[tasksReady_u8] = taskDetails_ast[i];
            readyTasks_ast[tasksReady_u8].arrivalTime_u16 = systemTime_u16;
            readyTasks_ast[tasksReady_u8].deadline_u16+=systemTime_u16;  //Calculate absolute deadline from the relative deadline.

            /*Create new elements for metric calculation*/
            metrics_ast[i].responseTimes_au16.push_back(uint16_t());
            metrics_ast[i].waitTime_au16.push_back(uint16_t());

            /*Increment the number of tasks that are ready*/
            tasksReady_u8++;
        }
    }

    /*Sort the ready list in the ascending order of periods. Hence at the end of the sort, the highest priority task is at the start*/
    std::sort(readyTasks_ast.begin(), readyTasks_ast.end(), sortWaitingList);

    /*Begin scheduling when at least one task is ready*/
    if(tasksReady_u8 > 0)
    {
        for(int i = 0;i<tasksReady_u8;i++)
        {
            /*Check for deadline overruns*/
            if(readyTasks_ast[i].deadline_u16 <= systemTime_u16)
            {
                /*When an overrun is detected. Print an error message with details and exit*/
                cout<<"Schedule unfeasible. Deadline of task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" missed. System time: "<<systemTime_u16<<endl;
                exit(-1);
            }
        }

        /*Schedule the first task in the ready list since its the highest priority*/
        cout<<"System Time: "<<systemTime_u16<<" Scheduling task "<<(unsigned)readyTasks_ast[0].taskId_u8<<" with period "<<readyTasks_ast[0].period_u16<<endl;

        /*When its the first time being scheduled, update the response time variable*/
        if(readyTasks_ast[0].firstEntry_bo == true)
        {
            // If its the first activation, then update the response time

            metrics_ast[readyTasks_ast[0].taskId_u8].responseTimes_au16[metrics_ast[readyTasks_ast[0].taskId_u8].taskInstance] = systemTime_u16 - readyTasks_ast[0].arrivalTime_u16; //RT = Time of first activation - Arrival Time
            readyTasks_ast[0].firstEntry_bo = false;  //Set flag to false to prevent re-entry and recalculation
        }

        /*Decrement the computation time by 1 since its scheduled for 1 time unit and check if computation is completed*/
        readyTasks_ast[0].computationTime_u16--;

        /*For all tasks not scheduled, increment their wait times*/
        for(int i = 1; i<tasksReady_u8;i++)
        {
            metrics_ast[readyTasks_ast[i].taskId_u8].waitTime_au16[metrics_ast[readyTasks_ast[i].taskId_u8].taskInstance]++;
        }

        /*Check if the computation of the current scheduled task is complete*/
        if(readyTasks_ast[0].computationTime_u16 == 0)
        {
            /*Computation complete, erase task from list and decrement number of ready tasks*/
            tasksReady_u8--;
            metrics_ast[readyTasks_ast[0].taskId_u8].taskInstance++;  //Update task instance variable so that the metrics for the next instance will be recorded in a different array location
            readyTasks_ast.erase(readyTasks_ast.begin());
        }

    }
    else
    {
        /*If no task present in the queue, then indicate idle*/
        cout<<"System Time: "<<systemTime_u16<<" Idle"<<endl;
    }
    delay();  //Delay between each iteration

    systemTime_u16++; //Increment system time for next iteration

    return;
}
