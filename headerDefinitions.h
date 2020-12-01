#ifndef HDR_DEFNS_H
#define HDR_DEFNS_H
#include<iostream>
#include <vector>
#include <string>
using namespace std;
struct unavailableChannel_st
{
    uint8_t chId_u8;
    uint16_t duration_u16;
};


struct taskInfo_st
{
    uint8_t taskId_u8;
    uint16_t period_u16;
    uint16_t computationTime_u16;
    uint16_t arrivalTime_u16;
    uint16_t deadline_u16;
    bool isTransmitting_bo = false;
    uint8_t remainingTxTime = computationTime_u16;
    uint8_t currentChannel;
    vector<unavailableChannel_st> blockedChannels_ast;
    bool operator() (taskInfo_st i,taskInfo_st j) { return (i.period_u16<j.period_u16);}  //Operator to perform sorting within a vector structure.
   } sortWaitingList;
struct taskMetrics_st
{
    uint16_t taskInstance = 0;
    vector<uint16_t>responseTimes_au16;
    vector<uint16_t>waitTime_au16;
};
struct channelInfo
{
    uint16_t gravity_u16;
    bool availability_bo;
    uint16_t channelID_u8;
    bool operator() (channelInfo i,channelInfo j) { return (i.gravity_u16<j.gravity_u16);}
}sortChannels;

#endif
