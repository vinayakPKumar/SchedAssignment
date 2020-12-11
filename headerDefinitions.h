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
    int taskId_u8;
    int period_u16;
    int computationTime_u16;
    int arrivalTime_u16;
    int deadline_u16;
    int laxity;
    float SpreadFactor;
    bool isTransmitting_bo = false;
    int remainingTxTime = computationTime_u16;
    int currentChannel;
    vector<unavailableChannel_st> blockedChannels_ast;
    bool operator() (taskInfo_st i,taskInfo_st j) { return (i.laxity<j.laxity);}  //Operator to perform sorting within a vector structure.
   } sortWaitingList;
struct taskMetrics_st
{
    int taskInstance = 0;
    vector<int>responseTimes_au16;
    vector<int>waitTime_au16;
};
struct channelInfo
{
    int gravity_u16;
    bool availability_bo;
    int channelID_u8;
    bool operator() (channelInfo i,channelInfo j) { return (i.gravity_u16>j.gravity_u16);}
}sortChannels;

#endif
