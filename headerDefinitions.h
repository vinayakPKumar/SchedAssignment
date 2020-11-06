#ifndef HDR_DEFNS_H
#define HDR_DEFNS_H
#include<iostream>
#include <vector>
#include <string>

typedef struct
{
    //Structure to keep track the connections
    std::string connectingNode;   //Node to which the connection is made.
    int connectionSize;           //Size of the connection.
} connectionStruct;

 struct nodeStructure{
   //Main structure of the node
   std::string x;  //Name of the node
   std::vector<connectionStruct> connections;  //Connections from the node. This is a vector and is therefore dynamic and changes as connections are added.
   int cost;       //Cost to get here from the start node. Used during path search.
   std::string pred;  //Predecessor to this node. Used during path construction.
} ;
 struct dijkshtraParameters {
     //Holds some of the node parameters needed for the search algorithm. This is a cut down version of nodeStructure.
    std::string nodeName;  //Node name
    int cost;  //Cost to reach here.
    bool operator() (dijkshtraParameters i,dijkshtraParameters j) { return (i.cost<j.cost);}  //Operator to perform sorting within a vector structure.
} sortWaitingList;


#endif
