#ifndef HDR_DEFNS_H
#define HDR_DEFNS_H
#include<iostream>
#include <vector>
#include <string>
using namespace std;
struct unavailableChannel_st
{
    int chId_u8;
    int duration_u16;
};


struct taskInfo_st
{
    int taskId_u8;  //Task identifier (Node)
    int period_u16;  //Period
    int computationTime_u16;  //Computation time (Equivalent to TOA)
    int arrivalTime_u16;  //Time when activated
    int deadline_u16;  //Deadline
    int laxity;  //Laxity calculated as per least laxity first algorithm
    float SpreadFactor;  //Spreading factor
    bool isTransmitting_bo = false;  //Value to indicate if the node is transmitting
    int remainingTxTime = computationTime_u16;  //Remaining time
    int currentChannel;  //Channel being used for ongoing transmission
    vector<unavailableChannel_st> blockedChannels_ast;  //List of channels blocked for the task
    bool operator() (taskInfo_st i,taskInfo_st j) { return (i.laxity<j.laxity);}  //Operator to perform sorting within a vector structure. Sorting based on laxity
   } sortWaitingList;


struct channelInfo
{
    int gravity_u16;  //Gravity of the channel. Calculated based on the gravity implementation
    bool availability_bo;  //Indicates whether the channel is available/not
    int channelID_u8;  //Channel identifier
    bool sfAvailable[6];  //Array of booleans. Each index corresponds to a SF. Ex: Index 0 corresponds to SF 7. If sfAvailable[0] is false then the channel is already transmitting a packet with SF 7 and cannot accomodate any other requests for the same SF
    int numberOfLinksOngoing = 0;  //Number of different packets currently being sent over the same channel
    bool operator() (channelInfo i,channelInfo j) { return (i.gravity_u16>j.gravity_u16);}  //Operator to perform sorting within a vector structure. Sorting based on gravity
}sortChannels;

#endif
