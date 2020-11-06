#include "headerDefinitions.h"
#include <iostream>
#include<stdio.h>
#include <vector>
#include<string>
#include<algorithm>
#include<limits.h>


using namespace std;
/**************************Global variables*************************************************************/
static vector<nodeStructure> Nodes;  //Holds the list of nodes and the nodes that are in the waiting list
static vector<dijkshtraParameters>waitingList;  //Holds the waiting list
static vector<string> Path,visitedList;  //Holds the path and the visited list
static int numberOfNodes,numberOfConnections,connectionLength;  //Variables for number of nodes, connection and length of the connection
/***********************Local functions*******************************************************************/
static void dijkstra(string startNode, string endNode); //Function of dijkshtra algorithm
static nodeStructure* getNode(string nodeName); //Function to get the pointer to the node from the node name


/*Main function*/
int main(void)
{
    string startPoint,endPoint;  //Variables to hold the start and end nodes.
    string choice;
Restart:   cout<<"Enter the number of nodes"<<endl;
    cin >> numberOfNodes;        //Get the number of nodes
    cout<<"Enter the name for each node"<<endl;
    for(int i = 0; i<numberOfNodes;i++)  //Read the nodes.
    {
        Nodes.push_back(nodeStructure());  //Pushback creates a new element in the vector
        cin>>Nodes[i].x;                   //The name is put into the new element
        Nodes[i].cost = INT_MAX;              //Set the cost (Used in algorithm) to a high value
    }
    cout<<"Node names are \n";    //Print out the node names for the user to read
    for(int i = 0; i<numberOfNodes;i++)
    {
        cout<<Nodes[i].x<<endl;
    }
    cout<<"Are you satisfied with the node names? Y to confirm, N to restart"<<endl;
    cin>>choice;
    if(choice == "N")
    {
        Nodes.clear();
        goto Restart;
    }
Restart2:    cout<<"Enter number of connections\n";
    cin>>numberOfConnections;           //Get the connections
    cout<<"Enter the connection as : Start point end point size"<<endl;
    for( int  i = 0; i<numberOfConnections; i++)
    {
        cin>>startPoint;                       //Get start point
        cin>>endPoint;                         //Get end point
        cin>>connectionLength;                  //Get length
        for(int j = 0; j<numberOfNodes; j++)    //Search the nodes for the start point and end point.
        {
            if((Nodes[j].x == startPoint))
            {
                Nodes[j].connections.push_back(connectionStruct{endPoint,connectionLength});  //Update the connection
            }
            if((Nodes[j].x == endPoint))
            {
                Nodes[j].connections.push_back(connectionStruct{startPoint,connectionLength});  //Update the connection
            }
        }

    }
    printf("Set of connections are \n");  //Print the connection for the user to verify
    for(int  i =0;i<numberOfNodes;i++)
    {
        cout<<"Connections for node "<<Nodes[i].x<<" are:"<<endl;
        for(int j =0; j<Nodes[i].connections.size(); j++)
        {
            cout<<"Connected to"<<Nodes[i].connections[j].connectingNode<<" with length of"<<Nodes[i].connections[j].connectionSize<<endl;
        }
    }
    cout<<"Are you satisfied with the node connections? Y to confirm, N to restart"<<endl;
    cin>>choice;
    if(choice == "N")
    {
        for(int i = 0; i<Nodes.size();i++)
            Nodes[i].connections.clear();

        goto Restart2;
    }

    //Get the start and end point from the user
searchAgain:    cout<<"Enter the nodes between which the path needs to be found"<<endl;
    cin>>startPoint;
    cin>>endPoint;
    //Call the algorithm
    dijkstra(startPoint,endPoint);
    //Display the path
    cout<<"The path is"<<endl;
    for(int j =0; j<Path.size();j++)
    {
        cout<<Path[j]<<endl;
    }
    //Check if user wants to find a new path
    cout<<"Would you like to find the path between other nodes? Press Y for yes, N for no"<<endl;
    cin>>choice;
    if(choice == "Y")
    {
        //Clear all previous searches and lists.
        visitedList.clear();
        waitingList.clear();
        Path.clear();
        for(int i = 0; i<numberOfNodes;i++)  //Reupdate the costs
        {

            Nodes[i].cost = INT_MAX;              //Set the cost (Used in algorithm) to a high value
        }
        goto searchAgain;  //Restart the algo again.
    }
    cout<<"Thank you. Exiting"<<endl;
    return(1);




}
/* Function: static void dijkstra(string startNode, string endNode)********
 * This is the function which has the algorithm to solve the path finding problem. It uses the Dijkshtra algorithm to find the shortest path.
 * Inputs:
 * startNode(String) : Name of the starting node
 * endNode(String)   : Name of the ending node
 * Output(bool)      : True if path found/ False if not
 * This function directly updates the Path variable which has the path to the final node */
static void dijkstra(string startNode, string endNode)
{
    int newCost = 0;   //Variable to calculate the cost/new cost in cases where there is already a cost previously calculated.
    bool pathFound = false;
    nodeStructure * ptrToNode,*ptrToConnectingNode;   //Pointers to the current node and the successor node
    ptrToNode = getNode(startNode);  //Get the pointer to the start node
    ptrToNode->cost = 0;    //There is no cost to reach the start node. Hence set it to zero
    waitingList.push_back({startNode,0}); //Add start node to waiting list
    while(1)    //Loop terminated through break
    {
        if(waitingList[0].nodeName == endNode)  //This is the condition where we have reached the destination. Break from loop.
        {
            pathFound = true;
            break;
        }
        if(waitingList.size() == 0)   //This is the condition where there are no more paths to be explored and a path is not found
            break;
        ptrToNode = getNode(waitingList[0].nodeName); //Get node which is currently being explored.

        waitingList.erase(waitingList.begin()); //Remove from waiting list as it is being explored
        for(int i =0;i<ptrToNode->connections.size();i++)  //Check all the connections of current node
        {
            newCost = ptrToNode->cost + ptrToNode->connections[i].connectionSize;  //Get the cost to get to the connecting node from the current node
            ptrToConnectingNode = getNode(ptrToNode->connections[i].connectingNode);  //Get the ptr to the connecting node
                for(int i = 0;i<visitedList.size();i++)   //Section to check if the connecting node has already been visited
                {
                    if((visitedList[i]) == (ptrToConnectingNode->x))   //If already visited, continue with outer loop. Hence
                    {
//                        cout<<"Here"<<endl;
                           goto label;  //Skip the rest of the loop
                    }
                }
                for(int i = 0;i<waitingList.size();i++)  //Check if the connecting node is already in the waiting list
                {
                    if((ptrToConnectingNode->x) == (waitingList[i].nodeName))
                    {  //It is already in the waiting list. Now check if its cost from the current node is lower
                        if(waitingList[i].cost > newCost)
                        {  //Cost from the current node is lower
                            waitingList[i].cost = newCost;  //Update cost in both the waiting list and the original list
                            ptrToConnectingNode->cost = newCost;
                            ptrToConnectingNode->pred = ptrToNode->x;  //Update predecessor
//                            cout<<"Here"<<waitingList[i].nodeName<<endl;
//                                   cout<<waitingList[i].cost<<endl;

                        }
                        goto label;  //Skip the rest of the loop
                    }
                }
              ptrToConnectingNode->cost = newCost;  //Update the cost
              waitingList.push_back({ptrToConnectingNode->x,ptrToConnectingNode->cost});  //Add the connecting node to the waiting list
              ptrToConnectingNode->pred = ptrToNode->x;  //Update the predecessor of the connecting node as the current node. This is used when calculating the path.
              label:;  //Label to skip iteration
        }
        visitedList.push_back({ptrToNode->x}); //All connections visited. Hence the node is now in the visited list. It will not be checked again.
         std::sort(waitingList.begin(), waitingList.end(), sortWaitingList);  //Sort the waiting list to put the node with the shortest cost at the top

    }
    if(pathFound)
    {
         ptrToNode = getNode(endNode);
         Path.push_back(endNode);
    //Construct the path
     while(1)
        {

        Path.push_back(ptrToNode->pred);
        ptrToNode = getNode(ptrToNode->pred);
        if(ptrToNode->x == startNode)
            break;
         }
    }
    else
        Path.push_back("NoPath");
}
/* Function: static nodeStructure* getNode(string nodeName)********
 * Based on the input string, this function finds the node in the original node list
 * Inputs:
 * nodeName(String) : Name of the node to be found
 *
 * Output(nodeStructure*) : Found node structure
 *  */
static nodeStructure* getNode(string nodeName)
{
    for(int i = 0; i<numberOfNodes;i++)  //Search amongst all nodes.
    {
        if(Nodes[i].x == nodeName)  //Node found. Return its address.
            return &Nodes[i];
    }
    return NULL;
}
