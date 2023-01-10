//WUPMUS_WORLD.h

//#inculdes
#include <iostream> #include <string> #include <stdio.h> #include <stdlib.h> #include <time.h> #include <windows.h> #include <vector> #include <cstdlib> #include <ctime> #include <cctype> #include <algorithm>

//Namespace
using namespace std;

//Defines
//World size #define DIM_1 8
#define DIM_2 8
///World Attributes #define DIM_3 10
#define OFFSET 1
#define MAX_PITS 4
#define MAX_WUMPUS 1
#define MAX_GOLD 1
#define MAX_AGENT 4

//World array flags
#define AGENT 0	//Agent Location in world	A == Agent	1 == Agent 0 == No Agent
#define PIT 1	//Pits are in element 1 of Z P == Pit	1 == Pit 0 == No Pit
#define BREEZE 2	//Breeze is in element 2 of Z	B == Breeze	1 == Breeze 0 == No Breeze
#define WUMPUS 3	//Wumpus is in element 3 of Z	W == Wumpus	1 == Wumpus	0 == No Wumpus	Wumpus in Pit == M
#define STENCH 4	//Stench is in element 4 of Z	S == Stench	1 == Stench 0 == No Stench
#define GOLD 5	//Gold is in element 5 of Z	G == Gold	1 == Gold	0 == No Gold
#define FOUL 6		//Stench and Breeze in same element graphics display only #define GOLD_OTHER 7	//Gold is in a breeze or stench or foul element display only

//Agent array flags
#define VISITED 6	//Agent array to indicate agent has previously visited this square and it is clear #define CLEAR 7		//Agent array to indicate safe squares for future moves
#define MRV 8 //Minimum remaining value when value is equal to sum of number of Pits and number of Wumpus Gold must be in an unvisited square cleared square
#define UNSAFE 9 //Agent array to indicate unsafe squares for avoiding Pits
//Agent Attributes
 
#define MAX_ROUTE_LENGTH 1024	//Max route length for agent -- route managment is handled in UpdateAgentRoute()

//Variables

//Functions
void InitializeZeroArray(int iArray[DIM_1][DIM_2][DIM_3]); void DisplayWorldView(int iArray[DIM_1][DIM_2][DIM_3]); void DisplayAgentView(int iArray[DIM_1][DIM_2][DIM_3]);

//Populate World with pits breeze wumpus, stench and gold void PopulateWorld(int iArray[DIM_1][DIM_2][DIM_3]);
//Subordinate Functions for Populate World
void CreatePits(int iArray[DIM_1][DIM_2][DIM_3]); void CreateBreeze(int iArray[DIM_1][DIM_2][DIM_3]); void PlaceWumpus(int iArray[DIM_1][DIM_2][DIM_3]); void CreateStench(int iArray[DIM_1][DIM_2][DIM_3]); void PlaceGold(int iArray[DIM_1][DIM_2][DIM_3]);

void FIXEDWORLD(int iArray[DIM_1][DIM_2][DIM_3]);
//Create agent at origin in world and Knowledge base and initialize path at origin

void CreateAgents(int iWorld[DIM_1][DIM_2][DIM_3], int iAgent_1[DIM_1][DIM_2][DIM_3], int iAgent_2[DIM_1][DIM_2][DIM_3], int iAgent_3[DIM_1][DIM_2][DIM_3], int iAgent_4[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path_1[MAX_ROUTE_LENGTH], struct sAgentRoute Path_2[MAX_ROUTE_LENGTH], struct sAgentRoute Path_3[MAX_ROUTE_LENGTH], struct sAgentRoute Path_4[MAX_ROUTE_LENGTH]);
//Subordinate Function for Create Agent
void CreateAgent(int iWorld[DIM_1][DIM_2][DIM_3], int iAgent[DIM_1][DIM_2][DIM_3],struct sAgentRoute Path[MAX_ROUTE_LENGTH], int x, int y);

//Check for previous visit to this spot in Path history if it is in history at a lower location than current
//reset iStep to lowest occurence in Path history and erase all esiting history forward
int UpdateAgentRoute( struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Update agents location in world
void UpdateAgentInWorld(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray_1[DIM_1][DIM_2][DIM_3], int iAgentArray_2[DIM_1][DIM_2][DIM_3], int iAgentArray_3[DIM_1][DIM_2][DIM_3], int iAgentArray_4[DIM_1][DIM_2][DIM_3]);
//Update agents location in KB
void UpdateAgentInKB(int iKBArray[DIM_1][DIM_2][DIM_3],int iAgentArray_1[DIM_1][DIM_2][DIM_3], int iAgentArray_2[DIM_1][DIM_2][DIM_3], int iAgentArray_3[DIM_1][DIM_2][DIM_3], int iAgentArray_4[DIM_1][DIM_2][DIM_3]);

//Display Agent route
void DisplayAgentRoute(struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Make an Initial random move of Agent 1 to either (0,1) or (1,0)
int InitialAgent_1_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);
//Make an Initial random move of Agent 2 to either (0,DIM_2 - OFFSET - OFFSET) or (1,DIM_2 - OFFSET) int InitialAgent_2_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);
//Make an Initial random move of Agent 3 to either (DIM_1 - OFFSET - OFFSET, 0) or (DIM1 - OFFSET, 1) int InitialAgent_3_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);
 
//Make an Initial random move of Agent 4 to either (DIM_1 - OFFSET - OFFSET, DIM_2 - OFFSET) or (DIM_1 - OFFSET,DIM_2 - OFFSET - OFFSET)
int InitialAgent_4_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//If Agent finds gold begin reversing steps back to starting point and avoid collosions with other agents in the most direct route available
int ReturnHome(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iAgentStatus, int iStep);

//Update Perceptions from new location
int UpdatePerceptions(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep, int iWumpus, int iPit);

//Calculate Agent next move based on Perceptions and Knowledge Rule Based! int CalculateAgentAction(int iWorldArray[DIM_1][DIM_2][DIM_3],int
iAgentRulesArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iAgentStatus, int iStep, int iArrow, int iWumpus, int iPit);

//Check if agent found gold
int AgentFoundGold(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Return to origin via route
int RetraceAgentRouteHome(int iArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Calculate Wumpus location from current location and KB
int CalculateWumpusLocation(int iArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Calculate Pit location from current location and KB
int CalculatePitLocation(int iArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep);

//Develop Common Multi-Agent KB and distribute data to all Agents
void DevelopCommonKB_ForAgents(int iMasterKBArray[DIM_1][DIM_2][DIM_3], int iKBArray_1[DIM_1][DIM_2][DIM_3], int iKBArray_2[DIM_1][DIM_2][DIM_3], int iKBArray_3[DIM_1][DIM_2][DIM_3], int iKBArray_4[DIM_1][DIM_2][DIM_3], int iWumpusFound, int iPitsFound);
// int iKBArray_5[DIM_1][DIM_2][DIM_3], int iKBArray_6[DIM_1][DIM_2][DIM_3], int iKBArray_7[DIM_1][DIM_2][DIM_3], int iKBArray_8[DIM_1][DIM_2][DIM_3]);

//Develop Inferences in Agent KB Array to assist agent movement
void UpdateInferenceInKB(int iKBArray[DIM_1][DIM_2][DIM_3], int iWumpusFound, int iPitsFound); int FireArrow(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3]);
//Dynamically create multiple agents
//void CreateAgents(int iNumb, string sAgentName);
