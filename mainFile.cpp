#include "headerDefinitions.h"
#include <iostream>
#include<stdio.h>
#include <vector>
#include<string>
#include<algorithm>
#include<limits.h>
#include"utils.h"
#include<cmath>
#include<float.h>
#include<time.h>

using namespace std;

#define SF_7    7
/**************************Global variables*************************************************************/
//vector <taskInfo_st> taskDetails_ast; //Variable to hold the details of entered tasks
//uint8_t numberOfTasks_u8 = 0;  //Variable to hold the number of tasks
//vector <taskInfo_st> readyTasks_ast;  //Variable which holds the ready tasks, equivalent to ready list
//uint8_t tasksReady_u8 = 0;  //Variable which indicates the number of ready tasks
//int totalNumOfChannels_u8 = 0;
float dutyCycle_ft = 0.01;
//vector <channelInfo> channelDetails_ast;
/***********************Local functions*******************************************************************/
static int simulator(uint16_t duration_u16,uint8_t numberOfTasks_u8,vector <taskInfo_st> taskDetails_ast,int totalNumOfChannels_u8,vector <channelInfo> channelDetails_ast,bool useGravity);
static float calculateTOA(float SF_u8, float PL_u8);
static void simulationSets();

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
    vector<uint16_t> taskPeriods_au16;  //Variable to store periods (Used while calculating hyperperiod)
    float averageResponseTime_ft = 0,averageWaitTime_ft = 0, TOA = 0;  //Variable for calculating the average metrics
    float spread,size;
    srand(time(NULL));  //Initialize random seed
    /*Get number of tasks*/
    simulationSets();  //Function to generate simulation
    /************************EVERYTHING BELOW IS COMMENTED AND CAN BE IGNORED. GOTO LINE 313*************************/
//    cout<<"Enter payload size"<<endl;
//    cin>>size;
//    cout<<"Enter SF"<<endl;
//    cin>>spread;
//    TOA = calculateTOA(spread,size);
//    cout<<TOA;
//    return 0;



//    printf("Enter the number of tasks\n");
//    scanf("%hhu",&numberOfTasks_u8);

//    /*Get the period for each task*/
//    printf("%hhu tasks are created. Enter the task periods\n",numberOfTasks_u8);

//    /*Loop until all periods are entered. Also create the structures for each task*/
//    for(int i = 0; i<numberOfTasks_u8;i++)
//    {
//        /*Add a new element to the taskDetails_ast, metrics_ast, taskPeriods_au16 structure */
//        taskDetails_ast.push_back(taskInfo_st());
//        taskPeriods_au16.push_back(uint16_t());

//        /*Read user input and update in the task structures*/
//        scanf("%hu",&taskDetails_ast[i].period_u16);
//       // taskDetails_ast[i].deadline_u16 = taskDetails_ast[i].period_u16;  //Update deadline. Here deadline = period.
//        taskPeriods_au16[i] = taskDetails_ast[i].period_u16;  //Copy onto local variable, used for hyperperiod computation.
//        taskDetails_ast[i].taskId_u8 = i;  //Assign task ID. Here the task ID is equal to the index for simplicity
//    }

//    /*Display entered periods*/
//    printf("The entered periods are\n");

//    /*Loop until all periods are displayed*/
//    for(int i = 0; i<numberOfTasks_u8;i++)
//    {
//        printf("Period of task[%d] = %d\n",i,taskDetails_ast[i].period_u16);
//    }

//    /*Get the computation time for each task*/
//    printf("Enter computation times\n");

//    /*Loop until all computation times are entered*/
//    for(int i = 0; i<numberOfTasks_u8;i++)
//    {
//        scanf("%hu",&taskDetails_ast[i].computationTime_u16);  //Read computation time and update in the structure
//    }

//    /*Display entered compuation times*/
//    printf("The entered computation times are\n");

//    /*Loop until all computation times are displayed*/
//    for(int i = 0; i<numberOfTasks_u8;i++)
//    {
//        printf("Computation time of task[%d] = %d\n",i,taskDetails_ast[i].computationTime_u16);
//    }

//    /*Get the computation time for each task*/
//    printf("Enter deadline times\n");
//    for(int i = 0; i<numberOfTasks_u8;i++)
//    {
//        scanf("%hu",&taskDetails_ast[i].deadline_u16);  //Read computation time and update in the structure
//    }

//    cout<<"Enter the number of channels"<<endl;
//    cin>>totalNumOfChannels_u8;
//    for(int i = 0; i<totalNumOfChannels_u8;i++)
//    {
//        channelDetails_ast.push_back(channelInfo());
//        channelDetails_ast[i].availability_bo = true;
//        channelDetails_ast[i].gravity_u16 = 0;
//        channelDetails_ast[i].channelID_u8 = i;
//    }
//    cout<<"Enter the duty cycle in percentage"<<endl;
//    cin>>dutyCycle_ft;
//    dutyCycle_ft /= 100;
//    simulator(20);

//    /*Function complete, return*/
//    return 1;
}

/**********************simulator()************************/
/* Description: The simulator performs a simulation of the
 * schedule for the given node and channel set. It generates the
 * schedule by using the scheduling algorithm proposed in the paper.
 * of an overrun.
 * Input:
 *          duration_u16(int): Time duration for which the system is to be run
 *          numberOfTasks_u8(uint8_t): Number of nodes(tasks)
 *          taskDetails_ast(vector<taskInfo_st>): Node details such as ToA, deadline, ID, arrival time etc TODO: Rename to nodeDetails_ast
 *          totalNumOfChannels_u8(int): Number of channels available
 *          channelDetails_ast(vector<channelInfo>): Channel details such as ID, availability and gravity
 * Return:  None
**********************************************************/
static int simulator(int duration_u16,uint8_t numberOfTasks_u8,vector <taskInfo_st> taskDetails_ast,int totalNumOfChannels_u8,vector <channelInfo> channelDetails_ast,bool useGravity)
{
    int systemTime_u16 = 0;    //Simulates system time
    vector<taskInfo_st> transmittingList;
    vector <taskInfo_st> readyTasks_ast;
    uint8_t tasksReady_u8 = 0;
    uint8_t taskIndex = 0,numberOfOngoingTx = 0;
    int currentGravity = 0;
    int sfIndex = 0;
    transmittingList.clear();
    readyTasks_ast.clear();
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
        //Decrement gravity for all channels if they are less than zero
        for(int i = 0; i < totalNumOfChannels_u8; i++)
        {
            if(channelDetails_ast[i].gravity_u16>0)
                channelDetails_ast[i].gravity_u16--;
        }
        //Decrement block times. Each channel is blocked off for a certain node and at each passing of time, the blocked duration is decremented. Loop for each task
        //NOTE: THIS PARAMETER OF BLOCKED TIME IS STORED IN THE ORIGINAL STRUCTURE AND NOT IN THE READY TASKS STRUCTURE AS THE ELEMENTS IN THE READY TASK STRUCTURE WILL BE DELETED AS AND WHEN TASKS ARE COMPLETE
        for(int i = 0; i <numberOfTasks_u8; i++)
        {
            for(int j = 0;j< taskDetails_ast[i].blockedChannels_ast.size();j++)
            {  //Loop across all the blocked channels and decrement if they are more than 0
                if(taskDetails_ast[i].blockedChannels_ast[j].duration_u16 > 0)
                    taskDetails_ast[i].blockedChannels_ast[j].duration_u16--;
                //If blocked duration has elapsed, remove this channel from the blocked list for the task.
                if(taskDetails_ast[i].blockedChannels_ast[j].duration_u16 == 0)
                {
                    taskDetails_ast[i].blockedChannels_ast.erase( taskDetails_ast[i].blockedChannels_ast.begin()+j);
                    j--;
                }
            }
        }

         //Only run this section if there are ongoing transmissions or if there are ready tasks
        if((tasksReady_u8 > 0) || (numberOfOngoingTx > 0))
        {
            for(int i = 0;i<numberOfOngoingTx;i++)
            {
                //For all tasks transmitting, decrement the transmission time.
                transmittingList[i].remainingTxTime--;
                //If transmission complete, identify the channel used for transmission, release it and update its gravity.
                if(transmittingList[i].remainingTxTime == 0)
                {
                    sfIndex = int(transmittingList[i].SpreadFactor)%7;
                    for(int j = 0; j<totalNumOfChannels_u8;j++)
                    {
                        if(transmittingList[i].currentChannel == channelDetails_ast[j].channelID_u8)  //Search for the channel which is used
                        {
                            channelDetails_ast[j].sfAvailable[sfIndex] = true;  //Release the SF occupied and update the gravity
                            channelDetails_ast[j].numberOfLinksOngoing--;
                            if(channelDetails_ast[j].availability_bo == false)  //If a channel was blocked due to two links transmitting on it, unblock it since one of the links has now stopped.
                                channelDetails_ast[j].availability_bo = true;
                            currentGravity = ceil(transmittingList[i].computationTime_u16/dutyCycle_ft) - transmittingList[i].computationTime_u16;
                            if(currentGravity > channelDetails_ast[j].gravity_u16)  //If current gravity is greater than any previously generated gravity update in the gravity variable.
                            {
                                channelDetails_ast[j].gravity_u16 = currentGravity;// + transmittingList[i].deadline_u16-systemTime_u16;
                                cout<<"Channel "<<channelDetails_ast[j].channelID_u8<<" gravity is "<<currentGravity<<endl;
                            }
                            //Add this channel to the list of blocked channel for the node.
                            unavailableChannel_st addCh;
                            addCh.chId_u8 = transmittingList[i].currentChannel;
                            addCh.duration_u16 = currentGravity;
                            taskDetails_ast[transmittingList[i].taskId_u8].blockedChannels_ast.push_back(unavailableChannel_st(addCh)); //Adding to the list
                            break;
                        }
                    }
                    cout<<"Transmission of "<<(unsigned)transmittingList[i].taskId_u8<<" is complete at time "<<systemTime_u16<<" and the channel "<<(unsigned)transmittingList[i].currentChannel<<" is released"<<endl;
                    transmittingList.erase(transmittingList.begin()+i);  //Erase the trasmitting node
                    i--;  //Decrement the index i of the for loop since one of the elements in the structure has been erased.
                    numberOfOngoingTx--;  //Decrement the number of ongoing transmissions
                    continue;
                }
                //If deadline overrun is detected in the midst of transmission
                if(transmittingList[i].deadline_u16 <=systemTime_u16)
                {
                    /*When an overrun is detected. Print an error message with details and exit*/
                    cout<<"Schedule unfeasible. Deadline of transmission "<<(unsigned)transmittingList[i].taskId_u8<<" missed. System time: "<<systemTime_u16<<endl<<"Printing set:"<<endl;
                    for(int j = 0; j<numberOfTasks_u8;j++)   //Display the generated task set
                    {
                        cout<<"Task ID: "<<(unsigned)taskDetails_ast[j].taskId_u8<<" of period : "<<taskDetails_ast[j].period_u16<<" of TOA: "<<taskDetails_ast[j].computationTime_u16<<" of deadline: "<<taskDetails_ast[j].deadline_u16<<endl;

                    }
                    return 0;
                }
            }

            /*Check for deadline overruns in any of the tasks in the waiting list*/
            for(int i = 0;i<tasksReady_u8;i++)
            {
                /*Check for deadline overruns*/
                if(readyTasks_ast[i].deadline_u16 <= systemTime_u16)
                {
                    /*When an overrun is detected. Print an error message with details and exit*/
                    cout<<"Schedule unfeasible. Deadline of task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" missed. System time: "<<systemTime_u16<<endl<<"Printing set:"<<endl;

                    for(int j = 0; j<numberOfTasks_u8;j++)   //Display the generated task set
                    {
                        cout<<"Task ID: "<<(unsigned)taskDetails_ast[j].taskId_u8<<" of period : "<<taskDetails_ast[j].period_u16<<" of TOA: "<<taskDetails_ast[j].computationTime_u16<<" of deadline: "<<taskDetails_ast[j].deadline_u16<<endl;

                    }

                    return 0;
                }
            }


            //Sort channels based on their gravity
            //NOTE: COMMENTING THE LINE BELOW EFFECTIVELY ACTIVATES/DEACTIVATES THE ALGORITHM FOR CHANNEL SCHEDULING
            if(useGravity == true)
                std::sort(channelDetails_ast.begin(),channelDetails_ast.end(),sortChannels);

           //Least laxity implementation
            for(int i =0; i<tasksReady_u8;i++)
            {
                readyTasks_ast[i].laxity = readyTasks_ast[i].deadline_u16 - systemTime_u16 - readyTasks_ast[i].computationTime_u16;  //Update laxity
            }

            std::sort(readyTasks_ast.begin(), readyTasks_ast.end(), sortWaitingList); //Least laxity sorting


        //Loop for all the tasks ready
        for(int k = 0; k<tasksReady_u8;k++)
        {
            sfIndex = int(readyTasks_ast[k].SpreadFactor) % 7;
        //Find a channel for the ready task
      for(int l = 0;l<totalNumOfChannels_u8;l++)
            {
                //If current channel is already busy, goto the next channel
                if(channelDetails_ast[l].availability_bo == false)
                    continue;
                //The SF needed by this current link is taken in the channel goto next channel
                if(channelDetails_ast[l].sfAvailable[sfIndex] == false)
                    continue;
                /*Check if channel is blocked for the current transmitting node*/
                taskIndex = readyTasks_ast[k].taskId_u8;
                for(int j = 0; j<taskDetails_ast[taskIndex].blockedChannels_ast.size();j++)  //Here we reference back to the original structure taskDetails since this keeps track of all the blocked channels
                {
                    if(taskDetails_ast[taskIndex].blockedChannels_ast[j].chId_u8 == channelDetails_ast[l].channelID_u8)
                    {

                       goto skipch;
                    }
                }
                //If the channel is not busy and if the channel is not blocked, then assign the channel to the node.
                channelDetails_ast[l].sfAvailable[sfIndex] = false;  //Indicate this SF of the channel is now taken
                transmittingList.push_back(taskInfo_st());  //Add this node to the transmitting list
                transmittingList[numberOfOngoingTx] = readyTasks_ast[k];
                transmittingList[numberOfOngoingTx].currentChannel = channelDetails_ast[l].channelID_u8;  //Assign the channel
                transmittingList[numberOfOngoingTx].remainingTxTime = transmittingList[numberOfOngoingTx].computationTime_u16;  //The remaining Tx time becomes the TOA
                cout<<"Channel "<<(unsigned)channelDetails_ast[l].channelID_u8<<" assigned to node "<<(unsigned)transmittingList[numberOfOngoingTx].taskId_u8<<" at time "<<systemTime_u16<<endl;
                readyTasks_ast.erase(readyTasks_ast.begin() + k);  //Remove task from ready list
                k--;  //Decrement loop index since one of the elements in the list has been removed.
                tasksReady_u8--;  //Decrement the size of the ready list
                numberOfOngoingTx++;  //Increment the size of the Tx list
                channelDetails_ast[l].numberOfLinksOngoing++;
                if(channelDetails_ast[l].numberOfLinksOngoing > 1)
                    channelDetails_ast[l].availability_bo = false;
                break;  //Break from inner loop and beginning channel search for the next node


                skipch: ;  //Skip the channel because it is blocked for this node
            }
        }
        }

        /*This section is to view the working of the scheduler in depth. Uncomment for debugging*/
        //Print all status
//        cout<<"#############At time "<<systemTime_u16<<" :"<<endl;
//        for(int i = 0; i < tasksReady_u8; i++)
//        {
//            cout<<"Task "<<(unsigned)readyTasks_ast[i].taskId_u8<<" is in ready list"<<endl;
//        }
//        for(int i = 0; i < numberOfOngoingTx; i++)
//        {
//            cout<<"Task "<<(unsigned)transmittingList[i].taskId_u8<<" is transmitting with channel : "<<(unsigned)transmittingList[i].currentChannel<<" with remaining time "<<(unsigned)transmittingList[i].remainingTxTime<<endl;
//        }
//        for(int i = 0; i<totalNumOfChannels_u8; i++)
//        {
//            cout<<"Gravity of channel "<<(unsigned)channelDetails_ast[i].channelID_u8<<" is: "<<channelDetails_ast[i].gravity_u16<<" and its status is : "<<channelDetails_ast[i].availability_bo<<endl;
//        }
//        for(int i = 0; i <numberOfTasks_u8; i++)
//        {
//            if(taskDetails_ast[i].blockedChannels_ast.size()>0)
//                cout<<"Task : "<<(unsigned)taskDetails_ast[i].taskId_u8<<" has the following blocked channels"<<endl;
//            else
//                continue;
//            for(int j = 0;j< taskDetails_ast[i].blockedChannels_ast.size();j++)
//            {
//                cout<<"Channel: "<<(unsigned)taskDetails_ast[i].blockedChannels_ast[j].chId_u8<<" blocked for "<<taskDetails_ast[i].blockedChannels_ast[j].duration_u16<<endl;
//            }
//       }
//        cout<<"#####################"<<endl;

        systemTime_u16++;
       // delay();

    }


    return 1;
}
/**********************calculateTOA(float SF_u8, float PL_u8)************************/
/* Description: This function calculates the time over air value for a given payload and spread factor
 * of an overrun.
 * Input:
 *          SF_u8(float): Spreading factor
 *          PL_u8(float): Payload size
 * Return:  Calculated TOA
***************************************************************************************/
static float calculateTOA(float SF_u8, float PL_u8)
{
    float retval_ft = 0, calc_ft = 0, symbolTime_ft = 0, preambleTime_ft = 0 ;


    calc_ft = ceil(((8*PL_u8)-(4*SF_u8)+(8+16+20))/(4*SF_u8))*(4/0.8);  //Calculation of number of symbols

    symbolTime_ft = (pow(2,SF_u8))/125;  //Get the symbol time. This is used calculate the time taken for each symbol transmission.

    preambleTime_ft = (8+4.25)*symbolTime_ft;  //Calculate the time taken to send the fixed preamble by multiplying with symbo time.

    retval_ft = 8 + fmax(calc_ft,0);  //Calculate the max size of the payload

    retval_ft*=symbolTime_ft;   //Calculate the time required to send the payload
    retval_ft+=preambleTime_ft;  //Add the preamble time to the time required to send the payload to get the overall TOA.
    retval_ft = ceil(retval_ft);  //Ceil the value. Ex(29.5 = 30)

    return retval_ft;  //Return the calculated TOA
}

/**********************simulationSets************************/
/* Description: This function takes the user input and generates a task set to be simulated. The task set is then run on the simulator() function.
 * Node implies the end node performing the LoRa transmission/reception.
 * Input:   None
 * Return:  None
**************************************************************/

static void simulationSets()
{
    int numberOfChannels, numberOfNodes,numberOfSimulations,simulationTime;
    vector<taskInfo_st> nodeList;
    vector<channelInfo> channelList;
    float randomPayload,commonPeriod=0,periodCalc =0, average = 0;
    bool gravityOn = true;
    char enteredVal;
    //Get the number of nodes, number of channels and the number of simulation sets
    cout<<"Enter the number of nodes"<<endl;
    cin>>numberOfNodes;
    for(int i = 0; i<numberOfNodes; i++)
    {
        /*Add a new element to the taskDetails_ast, metrics_ast, taskPeriods_au16 structure */
        nodeList.push_back(taskInfo_st());

        nodeList[i].taskId_u8 = i;  //Assign task ID. Here the task ID is equal to the index for simplicity
    }
    cout<<"Enter the number of channels"<<endl;
    cin>>numberOfChannels;
    for(int i = 0; i<numberOfChannels;i++)
    {
        channelList.push_back(channelInfo());
        channelList[i].availability_bo = true;
        channelList[i].gravity_u16 = 0;
        channelList[i].channelID_u8 = i;
        channelList[i].numberOfLinksOngoing = 0;
        for(int j = 0;j<6;j++)
            channelList[i].sfAvailable[j] = true;
    }
    cout<<"Enter the number of simulations"<<endl;
    cin>>numberOfSimulations;
    cout<<"To disable gravity based sorting of channels, enter 'y'"<<endl;
    cin>>enteredVal;
    if(enteredVal == 'y')
        gravityOn = false;
    cout<<"Enter the simulation time as multiple of periods"<<endl;
    cin>>simulationTime;


    for(int i = 0;i<numberOfSimulations;i++)  //Create node set for each simulation set requested
    {
repeat: for(int j =0;j<numberOfNodes;j++)  //Generate TOA for a random payload for each node
        {
            nodeList[j].SpreadFactor = 7 + rand()%6;  //Assign a random spreading factor between 7 and 12
            randomPayload = 1 + (rand()%5);   //Assign a random payload between 1 and 5 bytes
            nodeList[j].computationTime_u16 = calculateTOA(nodeList[j].SpreadFactor,randomPayload);
            nodeList[j].deadline_u16 = nodeList[j].computationTime_u16 * (2+(rand()%5));  //Deadline assigned as a random value between TOA and 5*TOA
        }
        commonPeriod = FLT_MAX;
        for(int j =0;j<numberOfNodes;j++)  //Calculate the smallest period (See paper)
        {
            periodCalc = ceil((nodeList[j].computationTime_u16/0.01));
            if(periodCalc < commonPeriod)
                commonPeriod = periodCalc;
        }
        //commonPeriod = ceil((commonPeriod/numberOfChannels));
        for(int j = 0; j<numberOfNodes;j++)  //Assign this period to all the nodes
        {
            nodeList[j].period_u16 = commonPeriod;
            if((nodeList[j].period_u16 * numberOfChannels) < ceil((nodeList[j].computationTime_u16/0.01)))
                goto repeat;
        }
      //  cout<<"Generated task set "<<i<<" :"<<endl;
        for(int j = 0; j<numberOfNodes;j++)   //Display the generated task set
        {
            cout<<"Task ID: "<<(unsigned)nodeList[j].taskId_u8<<" of period : "<<nodeList[j].period_u16<<" of TOA: "<<nodeList[j].computationTime_u16<<" of deadline: "<<nodeList[j].deadline_u16<<endl;

        }
       average += simulator((int)(commonPeriod*simulationTime),numberOfNodes,nodeList,numberOfChannels,channelList,gravityOn);  //Simulate for 100 periods.
        cout<<"***********************"<<endl;
    }
    average/=numberOfSimulations;
    cout<<"Average simulation "<<average<<endl;


}
