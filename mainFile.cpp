#include "headerDefinitions.h"
#include <iostream>
#include<stdio.h>
#include <vector>
#include<string>
#include<algorithm>
#include<limits.h>
#include"utils.h"
#include<cmath>

using namespace std;
/**************************Global variables*************************************************************/
vector <taskInfo_st> taskDetails_ast; //Variable to hold the details of entered tasks
uint8_t numberOfTasks_u8 = 0;  //Variable to hold the number of tasks
vector <taskInfo_st> readyTasks_ast;  //Variable which holds the ready tasks, equivalent to ready list
uint8_t tasksReady_u8 = 0;  //Variable which indicates the number of ready tasks
int totalNumOfChannels_u8 = 0;
float dutyCycle_ft = 0;
vector <channelInfo> channelDetails_ast;
/***********************Local functions*******************************************************************/
static void simulator(uint16_t duration_u16);  //Definition of the simulator function

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
    float averageResponseTime_ft = 0,averageWaitTime_ft = 0;  //Variable for calculating the average metrics
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
        taskPeriods_au16.push_back(uint16_t());

        /*Read user input and update in the task structures*/
        scanf("%hu",&taskDetails_ast[i].period_u16);
       // taskDetails_ast[i].deadline_u16 = taskDetails_ast[i].period_u16;  //Update deadline. Here deadline = period.
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

    /*Get the computation time for each task*/
    printf("Enter deadline times\n");
    for(int i = 0; i<numberOfTasks_u8;i++)
    {
        scanf("%hu",&taskDetails_ast[i].deadline_u16);  //Read computation time and update in the structure
    }

    cout<<"Enter the number of channels"<<endl;
    cin>>totalNumOfChannels_u8;
    for(int i = 0; i<totalNumOfChannels_u8;i++)
    {
        channelDetails_ast.push_back(channelInfo());
        channelDetails_ast[i].availability_bo = true;
        channelDetails_ast[i].gravity_u16 = 0;
        channelDetails_ast[i].channelID_u8 = i;
    }
    cout<<"Enter the duty cycle in percentage"<<endl;
    cin>>dutyCycle_ft;
    dutyCycle_ft /= 100;
    simulator(20);

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
static void simulator(uint16_t duration_u16)
{
    static uint16_t systemTime_u16 = 0;    //Simulates system time
    vector<taskInfo_st> transmittingList;
    uint8_t taskIndex = 0,numberOfOngoingTx = 0;
    uint16_t currentGravity = 0;
    while(systemTime_u16 < duration_u16)
    {
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


                /*Increment the number of tasks that are ready*/
                tasksReady_u8++;
            }
        }
        //Decrement gravity
        for(int i = 0; i < totalNumOfChannels_u8; i++)
        {
            if(channelDetails_ast[i].gravity_u16>0)
                channelDetails_ast[i].gravity_u16--;
        }
        //Decrement block times
        for(int i = 0; i <numberOfTasks_u8; i++)
        {
            for(int j = 0;j< taskDetails_ast[i].blockedChannels_ast.size();j++)
            {
                if(taskDetails_ast[i].blockedChannels_ast[j].duration_u16 > 0)
                    taskDetails_ast[i].blockedChannels_ast[j].duration_u16--;
                if(taskDetails_ast[i].blockedChannels_ast[j].duration_u16 == 0)
                {
                    taskDetails_ast[i].blockedChannels_ast.erase( taskDetails_ast[i].blockedChannels_ast.begin()+j);
                    j--;
                }
            }
        }

        /*Sort the ready list in the ascending order of periods. Hence at the end of the sort, the highest priority task is at the start*/
        std::sort(readyTasks_ast.begin(), readyTasks_ast.end(), sortWaitingList);

        if((tasksReady_u8 > 0) || (numberOfOngoingTx > 0))
        {
            for(int i = 0;i<numberOfOngoingTx;i++)
            {
                transmittingList[i].remainingTxTime--;
                if(transmittingList[i].remainingTxTime == 0)
                {
                    for(int j = 0; j<totalNumOfChannels_u8;j++)
                    {
                        if(transmittingList[i].currentChannel == channelDetails_ast[j].channelID_u8)
                        {
                            channelDetails_ast[j].availability_bo = true;
                            currentGravity = ceil(transmittingList[i].computationTime_u16/dutyCycle_ft) - transmittingList[i].computationTime_u16;
                            if(currentGravity > channelDetails_ast[j].gravity_u16)
                            {
                                channelDetails_ast[j].gravity_u16 = currentGravity;
                            }
                            unavailableChannel_st addCh;
                            addCh.chId_u8 = transmittingList[i].currentChannel;
                            addCh.duration_u16 = currentGravity;
                            taskDetails_ast[transmittingList[i].taskId_u8].blockedChannels_ast.push_back(unavailableChannel_st(addCh));
                            break;
                        }
                    }
                    cout<<"Transmission of "<<(unsigned)transmittingList[i].taskId_u8<<" is complete at time "<<systemTime_u16<<" and the channel "<<(unsigned)transmittingList[i].currentChannel<<" is released"<<endl;
                    transmittingList.erase(transmittingList.begin()+i);
                    i--;
                    numberOfOngoingTx--;
                    continue;
                }
                if(transmittingList[i].deadline_u16 <=systemTime_u16)
                {
                    /*When an overrun is detected. Print an error message with details and exit*/
                    cout<<"Schedule unfeasible. Deadline of transmission "<<(unsigned)transmittingList[i].taskId_u8<<" missed. System time: "<<systemTime_u16<<endl;
                    exit(-1);
                }
            }
            //Sort channels based on their gravity
            //NOTE: COMMENTING THE LINE BELOW EFFECTIVELY ACTIVATES/DEACTIVATES THE ALGORITHM FOR CHANNEL SCHEDULING
            std::sort(channelDetails_ast.begin(),channelDetails_ast.end(),sortChannels);

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
        for(int k = 0; k<tasksReady_u8;k++)
        {

      for(int l = 0;l<totalNumOfChannels_u8;l++)
            {
                if(channelDetails_ast[l].availability_bo == false)
                    continue;
                /*Check if channel is blocked for the current transmitting node*/
                taskIndex = readyTasks_ast[k].taskId_u8;
                for(int j = 0; j<taskDetails_ast[taskIndex].blockedChannels_ast.size();j++)
                {
                    if(taskDetails_ast[taskIndex].blockedChannels_ast[j].chId_u8 == channelDetails_ast[l].channelID_u8)
                    {
                        //cout<<"** "<<k<<"**"<<(unsigned)taskDetails_ast[taskIndex].taskId_u8<<"**"<<(unsigned)channelDetails_ast[l].channelID_u8<<"**"<<systemTime_u16<<endl;
                       goto skipch;
                    }
                }
                channelDetails_ast[l].availability_bo = false;
                transmittingList.push_back(taskInfo_st());
                transmittingList[numberOfOngoingTx] = readyTasks_ast[k];
                transmittingList[numberOfOngoingTx].currentChannel = channelDetails_ast[l].channelID_u8;
                transmittingList[numberOfOngoingTx].remainingTxTime = transmittingList[numberOfOngoingTx].computationTime_u16;
                cout<<"Channel "<<(unsigned)channelDetails_ast[l].channelID_u8<<" assigned to node "<<(unsigned)transmittingList[numberOfOngoingTx].taskId_u8<<" at time "<<systemTime_u16<<endl;
                readyTasks_ast.erase(readyTasks_ast.begin() + k);
                k--;
                tasksReady_u8--;
                numberOfOngoingTx++;
                break;
                skipch: ;
            }
        }
        }
        //Print all status
        cout<<"#############At time "<<systemTime_u16<<" :"<<endl;
        for(int i = 0; i < tasksReady_u8; i++)
        {
            cout<<"Task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" is in ready list"<<endl;
        }
        for(int i = 0; i < numberOfOngoingTx; i++)
        {
            cout<<"Task "<<(unsigned)transmittingList[i].taskId_u8<<" is transmitting with channel : "<<(unsigned)transmittingList[i].currentChannel<<" with remaining time "<<(unsigned)transmittingList[i].remainingTxTime<<endl;
        }
        for(int i = 0; i<totalNumOfChannels_u8; i++)
        {
            cout<<"Gravity of channel "<<(unsigned)channelDetails_ast[i].channelID_u8<<" is: "<<channelDetails_ast[i].gravity_u16<<" and its status is : "<<channelDetails_ast[i].availability_bo<<endl;
        }
        for(int i = 0; i <numberOfTasks_u8; i++)
        {
            if(taskDetails_ast[i].blockedChannels_ast.size()>0)
                cout<<"Task : "<<(unsigned)taskDetails_ast[i].taskId_u8<<" has the following blocked channels"<<endl;
            else
                continue;
            for(int j = 0;j< taskDetails_ast[i].blockedChannels_ast.size();j++)
            {
                cout<<"Channel: "<<(unsigned)taskDetails_ast[i].blockedChannels_ast[j].chId_u8<<" blocked for "<<taskDetails_ast[i].blockedChannels_ast[j].duration_u16<<endl;
            }
        }
        cout<<"#####################"<<endl;

        systemTime_u16++;
        delay();

    }


    return;
}
