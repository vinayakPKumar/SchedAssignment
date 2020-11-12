#ifndef HDR_DEFNS_H
#define HDR_DEFNS_H
#include<iostream>
#include <vector>
#include <string>
using namespace std;
struct taskInfo_st
{
    uint8_t taskId_u8;
    uint16_t period_u16;
    uint16_t computationTime_u16;
    uint16_t arrivalTime_u16;
    uint16_t deadline_u16;
    bool firstEntry_bo = true;
    bool operator() (taskInfo_st i,taskInfo_st j) { return (i.period_u16<j.period_u16);}  //Operator to perform sorting within a vector structure.
   } sortWaitingList;
struct taskMetrics_st
{
    uint16_t taskInstance = 0;
    vector<uint16_t>responseTimes_au16;
    vector<uint16_t>waitTime_au16;
};

#endif
