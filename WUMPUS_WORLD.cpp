//WUMPUS_WORLD.cpp

//Global Includes #include <iostream> #include <string> #include <stdio.h> #include <stdlib.h> #include <time.h> #include <windows.h> #include <vector> #include <cstdlib> #include <ctime> #include <cctype> #include <algorithm>

//Local Includes
#include "WUMPUS_WORLD.h"

//Namespace
using namespace std;

//Defines
//World size #define DIM_1 8
#define DIM_2 8
//World Attributes #define DIM_3 10
#define OFFSET 1
#define MAX_PITS 4
#define MAX_WUMPUS 1
#define MAX_GOLD 1
#define MAX_AGENT 4

#define AGENT 0	//Agent Location in world	A == Agent	1 == Agent 0 == No Agent
#define PIT 1	//Pits are in element 1 of Z P == Pit	1 == Pit 0 == No Pit
#define BREEZE 2	//Breeze is in element 2 of Z	B == Breeze	1 == Breeze 0 == No Breeze
#define WUMPUS 3	//Wumpus is in element 3 of Z	W == Wumpus	1 == Wumpus	0 == No Wumpus
#define STENCH 4	//Stench is in element 4 of Z	S == Stench	1 == Stench 0 == No Stench
#define GOLD 5	//Gold is in element 5 of Z	G == Gold	1 == Gold	0 == No Gold
#define FOUL 6	//Stench and Breeze in same element graphics display only #define GOLD_OTHER 7 //Gold is in a breeze or stench or foul element display only


//Agent array flags
#define VISITED 6	//Agent array to indicate agent has previously visited this square and it is clear #define CLEAR 7		//Agent array to indicate safe squares for avoiding Wumpus
#define MRV 8 //Minimum remaining value when value is equal to sum of number of Pits and number of Wumpus Gold must be in an unvisited square cleared square Set with function that sets squares as clear
 
#define UNSAFE 9 //Agent array to indicate unsafe squares for avoiding Pits
//Agent Attributes
#define MAX_ROUTE_LENGTH 1024 //Max route length for agent -- route managment is handled in UpdateAgentRoute()
//Variables

//Structures
struct sAgentRoute
{
int x; int y;
};
//End Structure

//Functions
void InitializeZeroArray(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
int x,y,z;
//Loop through all x Rows for(x=0; x<DIM_1; x++)
{
//Loop through all y Columns for(y=0; y<DIM_2; y++)
{
//Loop through all z Depth for(z=0; z<DIM_3; z++)
{
iArray[x][y][z] = 0;
iArray[x][y][MRV] = 1;
iArray[x][y][UNSAFE] = 1;
}
}
}
}
//End initialize zero array

void DisplayWorldView(int iArray[DIM_1][DIM_2][DIM_3])
{
int x,y;
int iElementValue = 0; char cChar;

printf("Displaying World View\n");
//Loop through all x Rows for(x=0; x<DIM_1; x++)
{
//Loop through all y Columns for(y=0; y<DIM_2; y++)
{
//Always Display Agent if(iArray[x][y][AGENT] == 1)
{
cChar = 'A'; printf("%c ", cChar); cChar = NULL;
}
 
//Diplay Pits
else if(iArray[x][y][PIT] == 1 && iArray[x][y][WUMPUS] == 0)
{
cChar = 'P'; printf("%c ", cChar); cChar = NULL;
}

//Display Wumpus
else if(iArray[x][y][WUMPUS] == 1 && iArray[x][y][PIT] == 0)
{
cChar = 'W'; printf("%c ", cChar); cChar = NULL;
}

//Display Breeze only
else if(iArray[x][y][BREEZE] == 1 && iArray[x][y][STENCH] == 0 && iArray[x][y][GOLD] == 0)
{
cChar = 'B'; printf("%c ", cChar); cChar = NULL;
}
// Display Stench Only
else if(iArray[x][y][STENCH] == 1 && iArray[x][y][BREEZE] == 0 && iArray[x][y][GOLD] == 0)
{
cChar = 'S'; printf("%c ", cChar); cChar = NULL;
}
//Display Stench and Breeze
else if(iArray[x][y][STENCH] == 1 && iArray[x][y][BREEZE] == 1 && iArray[x][y][GOLD] == 0)
{
iArray[x][y][FOUL] = 1; cChar = 'F';
printf("%c ", cChar); cChar = NULL;
}
//Display Gold Only
else if(iArray[x][y][GOLD] == 1 && iArray[x][y][BREEZE] == 0 && iArray[x][y][STENCH] == 0 && iArray[x][y][WUMPUS] == 0)
{
cChar = 'G'; printf("%c ", cChar); cChar = NULL;
}
//Display Gold with Stench or Breeze or Wumpus
else if(iArray[x][y][GOLD] == 1 && (iArray[x][y][STENCH] == 1 || iArray[x][y][BREEZE] == 1 || iArray[x][y][WUMPUS] == 1))
{
iArray[x][y][GOLD_OTHER] = 1;
cChar = 'X'; printf("%c ", cChar);
 
cChar = NULL;
}
//Display Empty Element else
{
printf("%i ", iElementValue);
}

}
printf("\n");
}
printf("\n");
//system("pause");
}
//End display World View

void DisplayAgentView(int iArray[DIM_1][DIM_2][DIM_3])
{
int x,y;
int iElementValue = 0; char cChar;

printf("Displaying Agent View\n");
//Loop through all x Rows for(x=0; x<DIM_1; x++)
{
//Loop through all y Columns for(y=0; y<DIM_2; y++)
{
//Always Display Pits if(iArray[x][y][PIT] == 1)
{
cChar = 'P'; printf("%c ", cChar); cChar = NULL;
}

//Always Display Wumpus
else if(iArray[x][y][WUMPUS] == 1)
{
cChar = 'W'; printf("%c ", cChar); cChar = NULL;
}

//Always Display Gold
else if(iArray[x][y][GOLD] == 1)
{
cChar = 'G'; printf("%c ", cChar); cChar = NULL;
}

else if(iArray[x][y][VISITED] == 1)
{
cChar = 'V';
 
printf("%c ", cChar); cChar = NULL;
}
else if(iArray[x][y][CLEAR] == 1 && iArray[x][y][UNSAFE] == 1)
{
cChar = 'C'; printf("%c ", cChar); cChar = NULL;
}
else if (iArray[x][y][CLEAR] == 0 && iArray[x][y][UNSAFE] == 1)
{
cChar = 'U'; printf("%c ", cChar); cChar = NULL;
}
else if (iArray[x][y][CLEAR] == 1 && iArray[x][y][UNSAFE] == 0)
{
cChar = 'S'; printf("%c ", cChar); cChar = NULL;
}
//Display Empty Element else
{
printf("%i ", iElementValue);
}

}
printf("\n");
}
printf("\n");
}
//End display Agent View

void PopulateWorld(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
//printf("Creating Pits\n"); CreatePits(iArray);
//printf("Creating Breezes\n"); CreateBreeze(iArray);
//printf("Placing Wumpus\n"); PlaceWumpus(iArray);
//printf("Creating Stench\n"); CreateStench(iArray);
//printf("Placing Gold\n"); PlaceGold(iArray);
}
//End Populate World

void CreatePits(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
int x = 0; int y = 0; int iPits = 0;
//Random RTC seed for rand function srand(time(0));
 
while(iPits != MAX_PITS)
{
x = rand()%DIM_1; y = rand()%DIM_2;
//printf("Coordinates for pit are (%i,%i)\n", x, y);

if((x == 0 && y == 0) || (x == DIM_1 - OFFSET && y == 0) || (x == 0 && y == DIM_2 - OFFSET) || (x == DIM_1 - OFFSET && y == DIM_2 - OFFSET))
//If pit is at an agent origin do nothing
{
//printf("Coordinates are at an origin\n");
}
//If iWorld element does not contiain a pit place a pit there else if(iArray[x][y][PIT] == 0)
{
 


}
else
{

}
}
}
 
printf("Placing Pit at (%i,%i)\n", x, y); iArray[x][y][PIT] = 1;
iPits = iPits +1;


//printf("Pit already exists here\n");
 
//End Create Pits

void CreateBreeze(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
int x,y;
for(x = 0; x < DIM_1; x++)
{
for(y = 0; y < DIM_2; y++)
{	//If Pit exists at this spot place breeze in adjacent elements if(iArray[x][y][PIT] == 1)
{
if(x!=0)//Place Breeze Up
iArray[x-OFFSET][y][BREEZE] =1;

if(x<DIM_1-OFFSET)//Place Breeze Down iArray[x+OFFSET][y][BREEZE] =1;

if(y!=0)//Place Breeze Left iArray[x][y-OFFSET][BREEZE] =1;

if(y<DIM_2-OFFSET)//Place Breeze Right iArray[x][y+OFFSET][BREEZE] =1;
}
}
}
}
//End Create Breeze

void PlaceWumpus(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
 
int x = 0; int y = 0;
int iWumpus = 0;
//Random RTC seed for rand function srand(time(0));

while(iWumpus != MAX_WUMPUS)
{
x = (rand()*rand())%DIM_1; y = (rand()*rand())%DIM_2;
//printf("Coordinates for WUMPUS are (%i,%i)\n", x, y);

if(x == 0 && y == 0)
//If Wumpus is at Upper Left do nothing
{
//printf("Coordinates are at Upper Left\n");
}
else if(x == DIM_1 - OFFSET && y == 0)
//If Wumpus is at Lower Left do nothing
{
//printf("Coordinates are at Lower Left\n");
}
else if(x == 0 && y == DIM_2 - OFFSET)
//If Wumpus is at Upper Right do nothing
{
//printf("Coordinates are at Upper Right\n");
}
else if(x == DIM_1 - OFFSET && y == DIM_2 - OFFSET)
//If Wumpus is at Lower Right do nothing
{
//printf("Coordinates are at Lower Right\n");
}
else if(iArray[x][y][WUMPUS] == 0)
{	//If iWorld element does not contian a Wumpus place it there if(iArray[x][y][PIT] == 1)
{
 

}
else
{


}
}
}
}
 
//printf("Cannot Place WUMPUS in Pit!\n");


printf("Placing WUMPUS at (%i,%i)\n", x, y); iArray[x][y][WUMPUS] = 1;
iWumpus = iWumpus +1;
 
//End Place Wumpus

void CreateStench(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
int x,y;
for(x = 0; x < DIM_1; x++)
{
for(y = 0; y < DIM_2; y++)
{	//If Wumpus exists at this spot place Stench in adjacent elements
 
if(iArray[x][y][WUMPUS] == 1)
{
if(x!=0)//Place Breeze Up
iArray[x-OFFSET][y][STENCH] =1;

if(x<DIM_1-OFFSET)//Place Breeze Down iArray[x+OFFSET][y][STENCH] =1;

if(y!=0)//Place Breeze Left iArray[x][y-OFFSET][STENCH] =1;

if(y<DIM_2-OFFSET)//Place Breeze Right iArray[x][y+OFFSET][STENCH] =1;
}
}
}
}
//End Create Stench

void PlaceGold(int iArray[DIM_1][DIM_2][DIM_3]) //Functional DO NOT EDIT!
{
int x = 0; int y = 0;
int iGold = 0;
//Random RTC seed for rand function srand(time(0));

while(iGold != MAX_GOLD)
{
x = (rand()+rand())%DIM_1; y = (rand()+rand())%DIM_2;
//printf("Coordinates for Gold are (%i,%i)\n", x, y); if(x == 0 && y == 0)
{
//printf("Gold cant be placed at origin\n");
}
else if(iArray[x][y][PIT] == 1)
{
//printf("Gold Can't be placed with the Wumpus\n");
}
else if(iArray[x][y][WUMPUS] == 1)
{
//printf("Gold Can't be placed in a pit\n");
}
else if(iArray[x][y][GOLD] == 0)
{
printf("Placing Gold at (%i,%i)\n", x, y); iArray[x][y][GOLD] = 1;
iGold = iGold +1;
}
}
}
//End Place Gold

void FIXEDWORLD(int iArray[DIM_1][DIM_2][DIM_3])
{
 
iArray[2][2][PIT] = 1;
CreateBreeze(iArray);

iArray[1][1][WUMPUS] = 1;
CreateStench(iArray);

iArray[3][3][GOLD] =1;
}

//End Fixed World

void CreateAgents(int iWorld[DIM_1][DIM_2][DIM_3], int iAgent_1[DIM_1][DIM_2][DIM_3], int iAgent_2[DIM_1][DIM_2][DIM_3], int iAgent_3[DIM_1][DIM_2][DIM_3], int iAgent_4[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path_1[MAX_ROUTE_LENGTH], struct sAgentRoute Path_2[MAX_ROUTE_LENGTH], struct sAgentRoute Path_3[MAX_ROUTE_LENGTH], struct sAgentRoute Path_4[MAX_ROUTE_LENGTH])
{
//Create Agent 1 at origin (0,0) printf("Creating Agent 1 at Origin\n");
CreateAgent(iWorld, iAgent_1, Path_1, 0, 0); //Agent_1 initial location coordinates specified for all four NxN corners

printf("Creating Agent 2 at Upper Right\n");
//Create Agent 2 at Upper right
CreateAgent(iWorld, iAgent_2, Path_2, 0, DIM_2 - OFFSET); //Agent_2 initial location coordinates specified for all four NxN corners

printf("Creating Agent 3 at Lower Left\n");
//Create Agent 3 at Lower left
CreateAgent(iWorld, iAgent_3, Path_3, DIM_1 - OFFSET, 0); //Agent_3 initial location coordinates specified for all four NxN corners

printf("Creating Agent 4 at Lower Right\n");
//Create Agent 4 at Lower right
CreateAgent(iWorld, iAgent_4, Path_4, DIM_1 - OFFSET, DIM_2 - OFFSET); //Agent_4 initial location coordinates specified for all four NxN corners
}
//End create multiple agents

//Create agent at specified (x,y) and initialize path at (x,y) zero element in struct array for path history //Functional DO NOT EDIT!
void CreateAgent(int iWorldArray[DIM_1][DIM_2][DIM_3], int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int x, int y) //
{
printf("Creating Agent at (%i,%i)\n", x,y);
//Initialize path at (x,y) Path[0].x = x;
Path[0].y = y;

//Initialize agent at (x,y) in World iWorldArray[x][y][AGENT] = 1;

//Initialize agent in Agent Knowledge Base at (x,y) iAgentArray[x][y][AGENT] = 1;
iAgentArray[x][y][VISITED] = 1;
 
//Origin is clear iAgentArray[x][y][CLEAR] = 1;

//Origin is safe iAgentArray[x][y][UNSAFE] = 0;
}
//End Create Agent

//Check for previous visit to this spot in Path history if it is in history at a lower location than current
//reset iStep to lowest occurence in Path history and erase all exiting history forward int UpdateAgentRoute(struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
for(int s=0; s<iStep; s++)
{
if(Path[s].x == Path[iStep].x && Path[s].y == Path[iStep].y)
{
iStep = s;
for(int e = iStep + OFFSET; e < MAX_ROUTE_LENGTH; e++)
{
Path[e].x = 0;
Path[e].y = 0;
}
}
}
return iStep;
}
//End Update Agent Route

void DisplayAgentRoute(struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep) //Functional DO NOT EDIT!
{
//Display Agent route to date int x = 0;
int y = 0; printf("Route is:\n");
for(int i = 0; i <= iStep; i++)
{
 


}
printf("\n");
}
 
x = Path[i].x;
y = Path[i].y;
printf("Step %i is (%i,%i) \n", i, x, y);
 
//End Display Agent Route

//Make an Initial move of Agent 1 to either (0,1) or (1,0) Functional DO NOT EDIT!
int InitialAgent_1_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
int x,y,z; srand(time(0));

z = 0;

while(z < 1)
{
 
x = rand()%2; y = rand()%2;

if(x == 0 && y == 0)
{
//printf("Agent didn't move!\n");
}
else if(x == 1 && y == 1)
{
 

}
else
{
 
//printf("Agent can't move diagonally!\n");


//Update agents position in array to new x and y values iAgentArray[x][y][AGENT] = 1;
iAgentArray[0][0][AGENT] = 0;
//increment loop to terminate initial move z = z + 1;
//Increment iStep number iStep = iStep +1;
//Place new coordinates in path history at new location for iStep Path[iStep].x = x;
Path[iStep].y = y;
printf("Agent 1 is moving to (%i,%i)\n", x,y);
//printf("Agents new position based on random number is (%i,%i)\n", x,y);
//printf("Agents new position based on path history is (%i,%i)\n",
 
Path[iStep].x,Path[iStep].y);
}
}
return iStep;
}
//End Initial Agent 1 Move

//Make an Initial random move of Agent 2 to either (0,DIM_2 - OFFSET - OFFSET) or (1,DIM_2 - OFFSET) int InitialAgent_2_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
int x,y,z; srand(time(0));

z = 0;

while(z < 1)
{
x = rand()%2; y = rand()%2;

if(x == 0 && y == 0)
{
//printf("Agent didn't move!\n");
}
else if(x == 1 && y == 1)
{
//printf("Agent can't move diagonally!\n");
}
else
 
{//Update agents position in array to new x and y values

if(y == 1)
y = -1;
//Set agent in new position
iAgentArray[x][DIM_2 - OFFSET + y][AGENT] = 1;
//Clear agent from previous position iAgentArray[0][DIM_2 - OFFSET][AGENT] = 0;
//increment loop to terminate initial move z = z + 1;
//Increment iStep number iStep = iStep +1;

//Create new y Coord
y = DIM_2 - OFFSET + y;

//Place new coordinates in path history at new location for iStep Path[iStep].x = x;
Path[iStep].y = y;
printf("Agent 2 is moving to (%i,%i)\n", x,y);
//printf("Agents new position based on random number is (%i,%i)\n", x,y);
//printf("Agents new position based on path history is (%i,%i)\n", Path[iStep].x,Path[iStep].y);
}
}
return iStep;
}
//End Initial Agent 2 Move

//Make an Initial random move of Agent 3 to either (DIM_1 - OFFSET - OFFSET, 0) or (DIM1 - OFFSET, 1) int InitialAgent_3_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
int x,y,z; srand(time(0));

z = 0;

while(z < 1)
{
x = rand()%2; y = rand()%2;

if(x == 0 && y == 0)
{
//printf("Agent didn't move!\n");
}
else if(x == 1 && y == 1)
{
//printf("Agent can't move diagonally!\n");
}
else
{//Update agents position in array to new x and y values

if(x == 1)
x = -1;
 
//Set agent in new position
iAgentArray[DIM_1 - OFFSET + x][y][AGENT] = 1;
//Clear agent from previous position iAgentArray[DIM_1 - OFFSET][0][AGENT] = 0;
//increment loop to terminate initial move z = z + 1;
//Increment iStep number iStep = iStep +1;

//Create new y Coord
x = DIM_1 - OFFSET + x;

//Place new coordinates in path history at new location for iStep Path[iStep].x = x;
Path[iStep].y = y;
printf("Agent 3 is moving to (%i,%i)\n", x,y);
//printf("Agents new position based on random number is (%i,%i)\n", x,y);
//printf("Agents new position based on path history is (%i,%i)\n", Path[iStep].x,Path[iStep].y);
}
}
return iStep;
}
//End Initial Agent 3 Move

//Make an Initial random move of Agent 4 to either (DIM_1 - OFFSET - OFFSET, DIM_2 - OFFSET) or (DIM_1 - OFFSET,DIM_2 - OFFSET - OFFSET)
int InitialAgent_4_Move(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
int x,y,z; srand(time(0));

z = 0;

while(z < 1)
{
x = rand()%2; y = rand()%2;

if(x == 0 && y == 0)
{
//printf("Agent didn't move!\n");
}
else if(x == 1 && y == 1)
{
//printf("Agent can't move diagonally!\n");
}
else
{//Update agents position in array to new x and y values

if(x == 1)
x = -1;
else if(y == 1)
y = -1;
//Set agent in new position
 
iAgentArray[DIM_1 - OFFSET + x][DIM_2 - OFFSET + y][AGENT] = 1;
//Clear agent from previous position
iAgentArray[DIM_1 - OFFSET][DIM_2 - OFFSET][AGENT] = 0;
//increment loop to terminate initial move z = z + 1;
//Increment iStep number iStep = iStep +1;

//Create new x and y Coord x = DIM_1 - OFFSET + x; y = DIM_2 - OFFSET + y;

//Place new coordinates in path history at new location for iStep Path[iStep].x = x;
Path[iStep].y = y;
printf("Agent 4 is moving to (%i,%i)\n", x,y);
//printf("Agent 4 new position based on random number is (%i,%i)\n", x,y);
//printf("Agent 4 new position based on path history is (%i,%i)\n", Path[iStep].x,Path[iStep].y);
}
}
return iStep;
}
//End Initial Agent 4 Move

//If Agent finds gold begin reversing steps back to starting point and avoid collisions with other agents following their trail back home
int ReturnHome(int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iAgentStatus, int iStep)
{
//If Agent is dead it will not return home set iStep to zero for this agent and ignor it if(iAgentStatus == 0)
{
printf("Agent is dead and cannot return home!\n"); iStep = 0;
}
//If at home waiting on other Agents to return home else if(iAgentStatus == 1 && iStep == 0)
{
iStep = iStep;
printf("Agent is at home waiting on other agents to return home\n");
}
//If another Agent does not exist in this location make move to this location and update individual Agent array, AgentKB and decrement iStep
else if(iAgentArray[Path[iStep-1].x][Path[iStep-1].y][AGENT] == 0)
{
//Place agent in iArray according to previous next lowest iStep location iAgentArray[Path[iStep-1].x][Path[iStep-1].y][AGENT] = 1;
//Clear previous agent location in array iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Decrement iStep counter iStep = iStep -1;
}
//Else if another agent exists in this location do nothing, stall one cycle and return iStep unchanged waiting for next iteration through return home loop
else if(iAgentArray[Path[iStep-1].x][Path[iStep-1].y][AGENT] == 1)
 
{
iStep = iStep;
}

return iStep;
}
//End Return Home

void UpdateAgentInKB(int iKBArray[DIM_1][DIM_2][DIM_3],int iAgentArray_1[DIM_1][DIM_2][DIM_3], int iAgentArray_2[DIM_1][DIM_2][DIM_3], int iAgentArray_3[DIM_1][DIM_2][DIM_3], int iAgentArray_4[DIM_1][DIM_2][DIM_3])
{
for(int x = 0; x< DIM_1; x++)
{
for(int y = 0; y<DIM_2; y++)
{	//Set Agent in current position in array
if(iAgentArray_1[x][y][AGENT] == 1 || iAgentArray_2[x][y][AGENT] == 1 || iAgentArray_3[x][y][AGENT] == 1 || iAgentArray_4[x][y][AGENT] == 1)
{
iKBArray[x][y][AGENT] = 1;
}//Set AGENT array to 0 for any possible previous position else
{
iKBArray[x][y][AGENT] = 0;
}
}
}
}
//End Update Agent location in KB

//Update agents location in world
void UpdateAgentInWorld(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray_1[DIM_1][DIM_2][DIM_3], int iAgentArray_2[DIM_1][DIM_2][DIM_3], int iAgentArray_3[DIM_1][DIM_2][DIM_3], int iAgentArray_4[DIM_1][DIM_2][DIM_3])
{
for(int x = 0; x< DIM_1; x++)
{
for(int y = 0; y<DIM_2; y++)
{	//Set Agent in current position in array
if(iAgentArray_1[x][y][AGENT] == 1 || iAgentArray_2[x][y][AGENT] == 1 || iAgentArray_3[x][y][AGENT] == 1 || iAgentArray_4[x][y][AGENT] == 1)
{
iWorldArray[x][y][AGENT] = 1;
}//Set AGENT array to 0 for any possible previous position else
{
iWorldArray[x][y][AGENT] = 0;
}
}
}
}
//End Update Agent location in World

//Update Perceptions from new location
int UpdatePerceptions(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep, int iWumpus, int iPit)
 
{
int iAgentStatus = 1; int iWumpusFound = 0; int iPitFound = 0;
int z = 0;
//printf("iStep Value in Update Perceptions function is:%i\n",iStep);
//printf("Agents perceptions at location (%i,%i)\n", Path[iStep].x,Path[iStep].y);

//Update Agent array with values from world array iAgentArray[Path[iStep].x][Path[iStep].y][PIT] = iWorldArray[Path[iStep].x][Path[iStep].y][PIT]; iAgentArray[Path[iStep].x][Path[iStep].y][BREEZE] =
iWorldArray[Path[iStep].x][Path[iStep].y][BREEZE]; iAgentArray[Path[iStep].x][Path[iStep].y][WUMPUS] =
iWorldArray[Path[iStep].x][Path[iStep].y][WUMPUS]; iAgentArray[Path[iStep].x][Path[iStep].y][STENCH] =
iWorldArray[Path[iStep].x][Path[iStep].y][STENCH];
iAgentArray[Path[iStep].x][Path[iStep].y][GOLD] = iWorldArray[Path[iStep].x][Path[iStep].y][GOLD];

// If no Breeze and no Stench all spaces are safe and clear if(iAgentArray[Path[iStep].x][Path[iStep].y][BREEZE] == 0 &&
iAgentArray[Path[iStep].x][Path[iStep].y][STENCH] == 0)
{
//Set all possible next moves as clear & safe in iAgentArray(); if(Path[iStep].y != 0)
{
iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][CLEAR] = 1; iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][MRV] = 0; iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][UNSAFE] = 0;
}
if(Path[iStep].y != DIM_2-OFFSET)
{
iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][CLEAR] = 1; iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][MRV] = 0; iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][UNSAFE] = 0;
}
if(Path[iStep].x != 0)
{
iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][CLEAR] = 1; iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][MRV] = 0; iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][UNSAFE] = 0;
}
if(Path[iStep].x != DIM_1-OFFSET)
{
iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][CLEAR] = 1; iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][MRV] = 0; iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][UNSAFE] = 0;
}

}

//If Breeze and No Stench all spaces are CLEAR
else if(iAgentArray[Path[iStep].x][Path[iStep].y][BREEZE] == 1 && iAgentArray[Path[iStep].x][Path[iStep].y][STENCH] == 0)
{
//Set all possible next moves as Clear in iAgentArray(); if(Path[iStep].y != 0)
 
{
iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][CLEAR] = 1; iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][MRV] = 0;
}
if(Path[iStep].y != DIM_2-OFFSET)
{
iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][CLEAR] = 1; iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][MRV] = 0;
}
if(Path[iStep].x != 0)
{
iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][CLEAR] = 1; iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][MRV] = 0;
}
if(Path[iStep].x != DIM_1-OFFSET)
{
iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][CLEAR] = 1; iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][MRV] = 0;
}
}
//If Stench and No Breeze all spaces are safe
else if(iAgentArray[Path[iStep].x][Path[iStep].y][BREEZE] == 0 && iAgentArray[Path[iStep].x][Path[iStep].y][STENCH] == 1)
{
//Set all possible next moves as safe in iAgentArray(); if(Path[iStep].y != 0)
{
iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][MRV] = 0; iAgentArray[Path[iStep].x][Path[iStep].y - OFFSET][UNSAFE] = 0;

}
if(Path[iStep].y != DIM_2-OFFSET)
{
iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][MRV] = 0; iAgentArray[Path[iStep].x][Path[iStep].y + OFFSET][UNSAFE] = 0;
}
if(Path[iStep].x != 0)
{
iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][MRV] = 0; iAgentArray[Path[iStep].x - OFFSET][Path[iStep].y][UNSAFE] = 0;
}
if(Path[iStep].x != DIM_1-OFFSET)
{
iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][MRV] = 0; iAgentArray[Path[iStep].x + OFFSET][Path[iStep].y][UNSAFE] = 0;
}
}
//If no Wumpus or Pit at current location set Visited and Clear to 1 and MRV and Unsafe to 0 if(iAgentArray[Path[iStep].x][Path[iStep].y][PIT] != 1 ||
iAgentArray[Path[iStep].x][Path[iStep].y][WUMPUS] !=1)
{
iAgentArray[Path[iStep].x][Path[iStep].y][VISITED] = 1;
iAgentArray[Path[iStep].x][Path[iStep].y][CLEAR] = 1;
iAgentArray[Path[iStep].x][Path[iStep].y][MRV] = 0;
iAgentArray[Path[iStep].x][Path[iStep].y][UNSAFE] = 0;
}
 
//Feels Breeze? if(iWorldArray[Path[iStep].x][Path[iStep].y][BREEZE] == 1)
{
//printf("Agent feels a breeze blowing\n"); iAgentArray[Path[iStep].x][Path[iStep].y][BREEZE] = 1;
}

//Smells Stench? if(iWorldArray[Path[iStep].x][Path[iStep].y][STENCH] == 1)
{
//printf("Agent smells the pungent stench of the nearby Wumpus!\n"); iAgentArray[Path[iStep].x][Path[iStep].y][STENCH] = 1;
}

//Fallen in pit?
//printf("Does the Agent fall to its death in a pit?\n"); if(iWorldArray[Path[iStep].x][Path[iStep].y][PIT] == 1)
{
printf("Agent has fallen to its death in a pit!\n"); iAgentStatus = 0;
}

//Killed by Wumpus?
//printf("Does the Agent get mauled to death by the Wumpus?\n"); if(iWorldArray[Path[iStep].x][Path[iStep].y][WUMPUS] == 1)
{
printf("Agent has been mauled to death by the Wumpus!\n"); iAgentStatus = 0;
}

return iAgentStatus;
}
//End Update Perceptions

//Calculate Agent action and perform move or action based on Perceptions and Knowledge -- Separate into two functions for clarity??
//RULE BASED!
int CalculateAgentAction(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iAgentStatus, int iStep, int iArrow, int iWumpus, int iPit)
{

if(iAgentStatus == 0)
{
printf("Agent is dead and not moving anymore!\n"); iStep = iStep;
}
else if(iAgentStatus == 1)
{
int d,xCoord,yCoord,z;

//Perceptions at current location have been assessed random move from neighboring clear unvisited squares > visited squares > reverse one step

//Assign current coordinates values in xCoord and yCoord for comparisons in if statements xCoord = Path[iStep].x;
 
yCoord = Path[iStep].y;
//Reset z to zero for loop z = 0;

int iLeft = 0; int iRight = 0; int iUp = 0; int iDown = 0;

int iMoveLeft = 0; int iMoveRight = 0; int iMoveUp = 0; int iMoveDown = 0;


//Left move legal check
if(Path[iStep].y != 0 && iWorldArray[xCoord][yCoord - OFFSET][AGENT] != 1)
{	//If current location y coord is not equal to zero and no agent present in iWorld array at that location a left move is possible
iLeft = 1;
//iMoveLeft = 1;
 
}
else
{


}
 


iLeft = 0;
iMoveLeft = 0;
 

//Right move legal check
if(Path[iStep].y != DIM_2-OFFSET && iWorldArray[xCoord][yCoord + OFFSET][AGENT] !=
1)
{	//If current location y coord is not equal to DIM limit minus one and no agent present in
iWorld array at that location a right move is possible
iRight = 1;
//iMoveRight = 1;
 
}
else
{


}
 


iRight = 0;
iMoveRight = 0;
 

//Up move legal check
if(Path[iStep].x != 0 && iWorldArray[xCoord - OFFSET][yCoord][AGENT] != 1)
{	//If current location x coord is not equal to zero and no agent present in iWorld array at that location an upward move is possible
iUp = 1;
//iMoveUp = 1;
 
}
else
{


}
 


iUp = 0;
iMoveUp = 0;
 

//Down move legal check
 
if(Path[iStep].x != DIM_1-OFFSET && iWorldArray[xCoord + OFFSET][yCoord][AGENT] !=
1)
{	//If current location x coord is not equal to DIM limit minus one and no agent present in
iWorld array at that location a downward move is possible
iDown = 1;
//iMoveDown = 1;
 
}
else
{


}
/*
 


iDown = 0;
iMoveDown = 0;
 
//If all Wumpuses and pits have been found all other moves are clear and safe if(iWumpus == MAX_WUMPUS && iPit == MAX_PITS)
{
//Check for Wumpus and pits located and avoid those squares
if(iLeft == 1 && iAgentArray[xCoord][yCoord - OFFSET][WUMPUS] != 1 && iAgentArray[xCoord][yCoord - OFFSET][PIT] != 1)
{	//Check if previously visited
if(iAgentArray[xCoord][yCoord - OFFSET][VISITED] == 0 && iAgentArray[xCoord][yCoord - OFFSET][CLEAR] == 1 && iAgentArray[xCoord][yCoord - OFFSET][UNSAFE]
== 0)
 

else

}
 
iLeft = 4;

iLeft = 3;
 
//Check for Wumpus and pits located and avoid those squares
if(iRight == 1 && iAgentArray[xCoord][yCoord + OFFSET][WUMPUS] != 1 && iAgentArray[xCoord][yCoord + OFFSET][PIT] != 1)
{	//Check if previously visited
if(iAgentArray[xCoord][yCoord + OFFSET][VISITED] == 0 && iAgentArray[xCoord][yCoord + OFFSET][CLEAR] == 1 && iAgentArray[xCoord][yCoord + OFFSET][UNSAFE] == 0)
 

else

}
 
iRight = 4;

iRight = 3;
 
//Check for Wumpus and pits located and avoid those squares
if(iUp == 1 && iAgentArray[xCoord - OFFSET][yCoord][WUMPUS] != 1 && iAgentArray[xCoord - OFFSET][yCoord][PIT] != 1)
{	//Check if previously visited
if(iAgentArray[xCoord - OFFSET][yCoord][VISITED] == 0 && iAgentArray[xCoord - OFFSET][yCoord][CLEAR] == 1 && iAgentArray[xCoord - OFFSET][yCoord][UNSAFE]
== 0)
 

else

}
 
iUp = 4;

iUp = 3;
 
//Check for Wumpus and pits located and avoid those squares
if(iDown == 1 && iAgentArray[xCoord + OFFSET][yCoord][WUMPUS] != 1 && iAgentArray[xCoord + OFFSET][yCoord][PIT] != 1)
{	//Check if previously visited
if(iAgentArray[xCoord + OFFSET][yCoord][VISITED] == 0 && iAgentArray[xCoord + OFFSET][yCoord][CLEAR] == 1 && iAgentArray[xCoord + OFFSET][yCoord][UNSAFE] == 0)
 

else

}
 
iDown = 4;

iDown = 3;
 

//Accept moves where Wumpus and Pits are known not to be present while(z == 0 && (iLeft == 4 || iRight == 4 || iUp == 4 || iDown == 4))
{
d = rand()%4;
//Move Left
if(iLeft == 4 && d == 0)
{
//Set iMoveLeft = 1 iMoveLeft = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move Up
else if(iUp == 4 && d == 1)
{
//Set iMoveUp iMoveUp = 1;
//Set z = 1 to terminate loop z = 1;

}

//Move Right
else if(iRight == 4 && d == 2)
{
//Set iMoveRight = 1 iMoveRight = 1;
//Set z = 1 to terminate loop z = 1;

}

//Move down
else if(iDown == 4 && d == 3)
{
 



}
else

}
}
*/
 
//Set iMoveDown =1
iMoveDown =1;
//Set z = 1 to terminate loop z = 1;


z = 1;
 
//Check for clear and safe left move
if(iLeft == 1 && iAgentArray[xCoord][yCoord - OFFSET][CLEAR] == 1 && iAgentArray[xCoord][yCoord - OFFSET][UNSAFE] == 0)
{	//Check if previously visited
if(iAgentArray[xCoord][yCoord - OFFSET][VISITED] == 0)
 




OFFSET].y)
 
iLeft = 3;
else if(iAgentArray[xCoord][yCoord - OFFSET][VISITED] == 1)
{	//If previously visited prefer a least recent to revisit
if(xCoord != Path[iStep-OFFSET].x && yCoord != Path[iStep -

iLeft = 3;
else
iLeft = 2;
 
}
else

}
 


iLeft = 1;
 

//Check for clear and safe right move
if(iRight == 1 && iAgentArray[xCoord][yCoord + OFFSET][CLEAR] == 1 && iAgentArray[xCoord][yCoord + OFFSET][UNSAFE] == 0)
{	//Check if previously visited
if(iAgentArray[xCoord][yCoord + OFFSET][VISITED] == 0) iRight = 3;
else if(iAgentArray[xCoord][yCoord + OFFSET][VISITED] == 1)
{	//If previously visited prefer a least recent to revisit
if(xCoord != Path[iStep-OFFSET].x && yCoord != Path[iStep -
 
OFFSET].y)
 



}
else
 


else
 

iRight = 3;

iRight = 2;
 
iRight = 1;
}

//Check for clear and safe upward move
if(iUp == 1 && iAgentArray[xCoord - OFFSET][yCoord][CLEAR] == 1 && iAgentArray[xCoord - OFFSET][yCoord][UNSAFE] == 0)
{	//Check if previously visited
if(iAgentArray[xCoord - OFFSET][yCoord][VISITED] == 0) iUp = 3;
else if(iAgentArray[xCoord - OFFSET][yCoord][VISITED] == 1)
{	//If previously visited prefer a least recent to revisit
if(xCoord != Path[iStep-OFFSET].x && yCoord != Path[iStep -
 
OFFSET].y)
 



}
else

}
 


else


iUp = 1;
 

iUp = 3;

iUp = 2;
 

//Check for clear and safe downward move
if(iDown == 1 && iAgentArray[xCoord + OFFSET][yCoord][CLEAR] == 1 && iAgentArray[xCoord + OFFSET][yCoord][UNSAFE] == 0)
{	//Check if previously visited
if(iAgentArray[xCoord + OFFSET][yCoord][VISITED] == 0) iDown = 3;
else if(iAgentArray[xCoord + OFFSET][yCoord][VISITED] == 1)
 


OFFSET].y)
 
{	//If previously visited prefer a least recent to revisit
if(xCoord != Path[iStep-OFFSET].x && yCoord != Path[iStep -

iDown = 3;
else
iDown = 2;
 
}
else

}
 


iDown = 1;
 

//Accept clear and safe unvisited moves if available
while(z == 0 && (iLeft == 3 || iRight == 3 || iUp == 3 || iDown == 3)) //&& (iLeft != 4 && iRight != 4 && iUp != 4 && iDown !=4))
{
d = rand()%4;
//Move Left
if(iLeft == 3 && d == 0)
{
//Set iMoveLeft = 1 iMoveLeft = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move Up
else if(iUp == 3 && d == 1)
{
//Set iMoveUp iMoveUp = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move Right
else if(iRight == 3 && d == 2)
{
//Set iMoveRight = 1 iMoveRight = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move down
else if(iDown == 3 && d == 3)
{
//Set iMoveDown =1
iMoveDown = 1;
//Set z = 1 to terminate loop z = 1;
}
}

//Accept clear and safe visited moves if available
while(z == 0 && (iLeft == 2 || iRight == 2 || iUp == 2 || iDown == 2)) // && (iLeft != 3 && iRight != 3 && iUp != 3 && iDown !=3))
 
{
d = rand()%4;
//Move Left
if(iLeft == 2 && d == 0)
{
//Set iMoveLeft = 1 iMoveLeft = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move Up
else if(iUp == 2 && d == 1)
{
//Set iMoveUp iMoveUp = 1;
//Set z = 1 to terminate loop z = 1;
}

//Move Right
else if(iRight == 2 && d == 2)
{
//Set iMoveRight = 1 iMoveRight = 1;
//Set z = 1 to terminate loop z = 1;
}
//Move down
else if(iDown == 2 && d == 3)
{
//Set iMoveDown =1
iMoveDown = 1;
//Set z = 1 to terminate loop z = 1;
}
}


//Perform actual move here if none are set revert to previous step
//Move Left if(iMoveLeft == 1)
{
//Adjust x coord, move agent, update iStep, path and z
//decrement yCoord to move left yCoord = yCoord - OFFSET;
//Clear previous iAgentRulesArray location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Increment iStep before applying to path iStep = iStep + OFFSET;
//Set new y coordinate in path Path[iStep].y = yCoord;
//Carry in previous x coord for new path step Path[iStep].x = xCoord;
//Place agent in new location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 1;
 
}

//Move Up
else if(iMoveUp == 1)
{
//Adjust x coord, move agent, update iStep, path and z
//Decrement xCoord to move up xCoord = xCoord - OFFSET;
//Clear previous iAgentRulesArray location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Increment iStep before applying to path iStep = iStep + OFFSET;
//Set new x coordinate in path Path[iStep].x = xCoord;
//Carry in previous y coord for new path step Path[iStep].y = yCoord;
//Place agent in new location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 1;
}

//Move Right
else if(iMoveRight == 1)
{
//Adjust y coord, move agent, update iStep, path and z
//Increment yCoord to move right yCoord = yCoord + OFFSET;
//Clear previous iAgentRulesArray location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Increment iStep before applying to path iStep = iStep + OFFSET;
//Set new y coordinate in path Path[iStep].y = yCoord;
//Carry in previous x coord for new path step Path[iStep].x = xCoord;
//Place agent in new location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 1;
}

//Move down
else if(iMoveDown == 1)
{
//Adjust x coord, move agent, update iStep, path and z
//Increment xCoord to move down xCoord = xCoord + OFFSET;
//Clear previous iAgentRulesArray location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Increment iStep before applying to path iStep = iStep + OFFSET;
//Set new x coordinate in path Path[iStep].x = xCoord;
//Carry in previous y coord for new path step Path[iStep].y = yCoord;
//Place agent in new location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 1;
}
 
//If agent has no move revert to previous square and seek alternate else
{
//printf("Agent is backing up to look for alternate paths!\n");
//Move agent, update iStep and path int iNew_x = 0;
int iNew_y = 0;
//Clear previous iAgentRulesArray location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 0;
//Set previous coordinate pair as next coordinate pair in temp iNew_x = Path[iStep-OFFSET].x;
iNew_y = Path[iStep-OFFSET].y;
//Increment iStep before applying to path iStep = iStep + OFFSET;
//Set next coordinate pair as current location Path[iStep].x = iNew_x;
Path[iStep].y = iNew_y;
//Place agent in new location iAgentArray[Path[iStep].x][Path[iStep].y][AGENT] = 1;
}

int x = Path[iStep].x; int y = Path[iStep].y;
printf("Agent is moving to (%i,%i)\n", x,y);
}
return iStep;
}
//End CalculateAgentAction

int AgentFoundGold(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3], struct sAgentRoute Path[MAX_ROUTE_LENGTH], int iStep)
{
int iGoldStatus =0;

if(iWorldArray[Path[iStep].x][Path[iStep].y][GOLD] == 1)
{
printf("An Agent has found the Gold!\n"); iAgentArray[Path[iStep].x][Path[iStep].y][GOLD] = 1;
iGoldStatus = 1;
}

return iGoldStatus;
}
//End Agent Found Gold

//Develop Common Multi-Agent KB and distribute data to all Agents Maximum 8 in this iteration Dynamically creating agents can come later
void DevelopCommonKB_ForAgents(int iMasterKBArray[DIM_1][DIM_2][DIM_3], int iAgentArray_1[DIM_1][DIM_2][DIM_3], int iAgentArray_2[DIM_1][DIM_2][DIM_3], int iAgentArray_3[DIM_1][DIM_2][DIM_3], int iAgentArray_4[DIM_1][DIM_2][DIM_3], int iWumpusFound, int iPitsFound) //,
// int iAgentArray_5[DIM_1][DIM_2][DIM_3], int iAgentArray_6[DIM_1][DIM_2][DIM_3], int iAgentArray_7[DIM_1][DIM_2][DIM_3], int iAgentArray_8[DIM_1][DIM_2][DIM_3]
{
 
//Update Agent locations based on current positions in individual Agent arrays in Master KB Array and remove previous Agent locations
for(int x = 0; x < DIM_1; x++)
{
for(int y = 0; y <DIM_2; y++)
{	//Set Agent in current position in KB array
if(iAgentArray_1[x][y][AGENT] == 1 || iAgentArray_2[x][y][AGENT] == 1 || iAgentArray_3[x][y][AGENT] == 1 || iAgentArray_4[x][y][AGENT] == 1)
{
iMasterKBArray[x][y][AGENT] = 1;
}//Set AGENT array to 0 for any possible previous position else
{
iMasterKBArray[x][y][AGENT] = 0;
}
}
}

//Transfer Breeze and Stench perceptions and Pit and Wumpus locations to Master array from individual Agent arrays
for(int x = 0; x < DIM_1; x++)
{
for(int y = 0; y < DIM_2; y++)
{	//Process Agent locations
if(iAgentArray_1[x][y][AGENT] == 1 || iAgentArray_2[x][y][AGENT] == 1 || iAgentArray_3[x][y][AGENT] == 1 || iAgentArray_4[x][y][AGENT] == 1)
{
iMasterKBArray[x][y][AGENT] = 1;
}
//Process Pits
if(iAgentArray_1[x][y][PIT] == 1 || iAgentArray_2[x][y][PIT] == 1 || iAgentArray_3[x][y][PIT] == 1 || iAgentArray_4[x][y][PIT] == 1 ) //|| iAgentArray_5[x][y][PIT] == 1 || iAgentArray_6[x][y][PIT] == 1 || iAgentArray_7[x][y][PIT] == 1 || iAgentArray_8[x][y][PIT] == 1)
{
iMasterKBArray[x][y][PIT] = 1;
}
//Process Breezes
if(iPitsFound != MAX_PITS && (iAgentArray_1[x][y][BREEZE] == 1 || iAgentArray_2[x][y][BREEZE] == 1 || iAgentArray_3[x][y][BREEZE] == 1 || iAgentArray_4[x][y][BREEZE] == 1
)) //|| iAgentArray_5[x][y][BREEZE] == 1 || iAgentArray_6[x][y][BREEZE] == 1 || iAgentArray_7[x][y][BREEZE]
== 1 || iAgentArray_8[x][y][BREEZE] == 1))
{
iMasterKBArray[x][y][BREEZE] = 1;
}
//Process Wumpus
if(iAgentArray_1[x][y][WUMPUS] == 1 || iAgentArray_2[x][y][WUMPUS] == 1 || iAgentArray_3[x][y][WUMPUS] == 1 || iAgentArray_4[x][y][WUMPUS] == 1 ) //|| iAgentArray_5[x][y][WUMPUS] == 1 || iAgentArray_6[x][y][WUMPUS] == 1 || iAgentArray_7[x][y][WUMPUS]
== 1 || iAgentArray_8[x][y][WUMPUS] == 1)
{
iMasterKBArray[x][y][WUMPUS] = 1;
}
//Process Stenches
if(iWumpusFound != MAX_WUMPUS && (iAgentArray_1[x][y][STENCH] == 1 || iAgentArray_2[x][y][STENCH] == 1 || iAgentArray_3[x][y][STENCH] == 1 || iAgentArray_4[x][y][STENCH] ==
 
1 )) //|| iAgentArray_5[x][y][STENCH] == 1 || iAgentArray_6[x][y][STENCH] == 1 || iAgentArray_7[x][y][STENCH] == 1 || iAgentArray_8[x][y][STENCH] == 1))
{
iMasterKBArray[x][y][STENCH] = 1;
}
//Process Gold
if(iAgentArray_1[x][y][GOLD] == 1 || iAgentArray_2[x][y][GOLD] == 1 || iAgentArray_3[x][y][GOLD] == 1 || iAgentArray_4[x][y][GOLD] == 1 ) //|| iAgentArray5[x][y][GOLD] == 1 || iAgentArray_6[x][y][GOLD] == 1 || iAgentArray_7[x][y][GOLD] == 1 || iAgentArray_8[x][y][GOLD] == 1))
{
iMasterKBArray[x][y][GOLD] = 1;
}

//Process Visited
if(iAgentArray_1[x][y][VISITED] ==1 || iAgentArray_2[x][y][VISITED] == 1 || iAgentArray_3[x][y][VISITED] == 1 || iAgentArray_4[x][y][VISITED] == 1 ) //|| iAgentArray_5[x][y][VISITED]
== 1 || iAgentArray_6[x][y][VISITED] == 1 || iAgentArray_7[x][y][VISITED] == 1 || iAgentArray_8[x][y][VISITED] == 1))
{
iMasterKBArray[x][y][VISITED] = 1;
}

//Process Clear
if(iAgentArray_1[x][y][CLEAR] == 1 || iAgentArray_2[x][y][CLEAR] == 1 || iAgentArray_3[x][y][CLEAR] == 1 || iAgentArray_4[x][y][CLEAR] == 1 ) //|| iAgentArray5[x][y][CLEAR] == 1 || iAgentArray_6[x][y][CLEAR] == 1 || iAgentArray_7[x][y][CLEAR] == 1 || iAgentArray_8[x][y][CLEAR] == 1))
{
iMasterKBArray[x][y][CLEAR] = 1;
}

//Process MRV
if(iAgentArray_1[x][y][MRV] == 0 || iAgentArray_2[x][y][MRV] == 0 || iAgentArray_3[x][y][MRV] == 0 || iAgentArray_4[x][y][MRV] == 0 ) //|| iAgentArray_5[x][y][MRV] == 0 || iAgentArray_6[x][y][MRV] == 0 || iAgentArray_7[x][y][MRV] == 0 || iAgentArray_8[x][y][MRV] == 0))
{
iMasterKBArray[x][y][MRV] = 0;
}

//Process Unsafe
if(iAgentArray_1[x][y][UNSAFE] == 0 || iAgentArray_2[x][y][UNSAFE] == 0 || iAgentArray_3[x][y][UNSAFE] == 0 || iAgentArray_4[x][y][UNSAFE] == 0 ) //|| iAgentArray_5[x][y][UNSAFE]
== 0 || iAgentArray_6[x][y][UNSAFE] == 0 || iAgentArray_7[x][y][UNSAFE] == 0 || iAgentArray_8[x][y][UNSAFE] == 0))
{
iMasterKBArray[x][y][UNSAFE] = 0;
}
}
}

//Update Inferences in MasterKBArray UpdateInferenceInKB(iMasterKBArray, iWumpusFound, iPitsFound);


//Redistribute Perceptions and Inferences to individual Agent arrays for(int x = 0; x < DIM_1; x++)
{
 
for(int y = 0; y < DIM_2; y++)
{
//Process Pits if(iMasterKBArray[x][y][PIT] == 1)
{
iAgentArray_1[x][y][PIT] = 1;
iAgentArray_2[x][y][PIT] = 1;
iAgentArray_3[x][y][PIT] = 1;
iAgentArray_4[x][y][PIT] = 1;
//iAgentArray_5[x][y][PIT] = 1;
//iAgentArray_6[x][y][PIT] = 1;
//iAgentArray_7[x][y][PIT] = 1;
//iAgentArray_8[x][y][PIT] = 1;
}
//Process Breezes if(iMasterKBArray[x][y][BREEZE] == 1)
{
iAgentArray_1[x][y][BREEZE] = 1;
iAgentArray_2[x][y][BREEZE] = 1;
iAgentArray_3[x][y][BREEZE] = 1;
iAgentArray_4[x][y][BREEZE] = 1;
//iAgentArray_5[x][y][BREEZE] = 1;
//iAgentArray_6[x][y][BREEZE] = 1;
//iAgentArray_7[x][y][BREEZE] = 1;
//iAgentArray_8[x][y][BREEZE] = 1;
}
//Process Wumpus if(iMasterKBArray[x][y][WUMPUS] == 1)
{
iAgentArray_1[x][y][WUMPUS] = 1;
iAgentArray_2[x][y][WUMPUS] = 1;
iAgentArray_3[x][y][WUMPUS] = 1;
iAgentArray_4[x][y][WUMPUS] = 1;
//iAgentArray_5[x][y][WUMPUS] = 1;
//iAgentArray_6[x][y][WUMPUS] = 1;
//iAgentArray_7[x][y][WUMPUS] = 1;
//iAgentArray_8[x][y][WUMPUS] = 1;
}
//Process Stenches if(iMasterKBArray[x][y][STENCH] == 1)
{
iAgentArray_1[x][y][STENCH] = 1;
iAgentArray_2[x][y][STENCH] = 1;
iAgentArray_3[x][y][STENCH] = 1;
iAgentArray_4[x][y][STENCH] = 1;
//iAgentArray_5[x][y][STENCH] = 1;
//iAgentArray_6[x][y][STENCH] = 1;
//iAgentArray_7[x][y][STENCH] = 1;
//iAgentArray_8[x][y][STENCH] = 1;
}
//Process Gold if(iMasterKBArray[x][y][GOLD] == 1)
{
iAgentArray_1[x][y][GOLD] = 1;
iAgentArray_2[x][y][GOLD] = 1;
iAgentArray_3[x][y][GOLD] = 1;
 
iAgentArray_4[x][y][GOLD] = 1;
//iAgentArray_5[x][y][GOLD] = 1;
//iAgentArray_6[x][y][GOLD] = 1;
//iAgentArray_7[x][y][GOLD] = 1;
//iAgentArray_8[x][y][GOLD] = 1;
}
//Process Visited if(iMasterKBArray[x][y][VISITED] == 1)
{
iAgentArray_1[x][y][VISITED] = 1;
iAgentArray_2[x][y][VISITED] = 1;
iAgentArray_3[x][y][VISITED] = 1;
iAgentArray_4[x][y][VISITED] = 1;
//iAgentArray_5[x][y][VISITED] = 1;
//iAgentArray_6[x][y][VISITED] = 1;
//iAgentArray_7[x][y][VISITED] = 1;
//iAgentArray_8[x][y][VISITED] = 1;
}
//Process Clear if(iMasterKBArray[x][y][CLEAR] == 1)
{
iAgentArray_1[x][y][CLEAR] = 1;
iAgentArray_2[x][y][CLEAR] = 1;
iAgentArray_3[x][y][CLEAR] = 1;
iAgentArray_4[x][y][CLEAR] = 1;
//iAgentArray_5[x][y][CLEAR] = 1;
//iAgentArray_6[x][y][CLEAR] = 1;
//iAgentArray_7[x][y][CLEAR] = 1;
//iAgentArray_8[x][y][CLEAR] = 1;
}
//Process MRV if(iMasterKBArray[x][y][MRV] == 0)
{
iAgentArray_1[x][y][MRV] = 0;
iAgentArray_2[x][y][MRV] = 0;
iAgentArray_3[x][y][MRV] = 0;
iAgentArray_4[x][y][MRV] = 0;
//iAgentArray_5[x][y][MRV] = 0;
//iAgentArray_6[x][y][MRV] = 0;
//iAgentArrayy_7[x][y][MRV] = 0;
//iAgentArray_8[x][y][MRV] = 0;
}
//Process Unsafe if(iMasterKBArray[x][y][UNSAFE] == 0)
{
 







}
else
{
 
iAgentArray_1[x][y][UNSAFE] = 0;
iAgentArray_2[x][y][UNSAFE] = 0;
iAgentArray_3[x][y][UNSAFE] = 0;
iAgentArray_4[x][y][UNSAFE] = 0;
//iAgentArray_5[x][y][UNSAFE] = 0;
//iAgentArray_6[x][y][UNSAFE] = 0;
//iAgentArray_7[x][y][UNSAFE] = 0;
//iAgentArray_8[x][y][UNSAFE] = 0;
 
iAgentArray_1[x][y][UNSAFE] = 1;
iAgentArray_2[x][y][UNSAFE] = 1;
iAgentArray_3[x][y][UNSAFE] = 1;
iAgentArray_4[x][y][UNSAFE] = 1;
//iAgentArray_5[x][y][UNSAFE] = 1;
//iAgentArray_6[x][y][UNSAFE] = 1;
//iAgentArray_7[x][y][UNSAFE] = 1;
//iAgentArray_8[x][y][UNSAFE] = 1;
}
}
}
}
//End Develop Common Multi-Agent KB

//Develop Inferences Multi-Agent Agent KB to assist agent movement
void UpdateInferenceInKB(int iKBArray[DIM_1][DIM_2][DIM_3], int iWumpusFound, int iPitsFound)
{
for(int x = 0; x < DIM_1; x++)
{
for(int y = 0; y < DIM_2; y++)
{
 

UPPER ROW ***
 
//Process WUMPUS from LEFT, RIGHT and BOTTOM STENCH LOCATIONS *** IN

if(iKBArray[x][y][STENCH] == 1 && iKBArray[x][y + OFFSET +
 
OFFSET][STENCH] == 1 && iKBArray[x + OFFSET][y + OFFSET][STENCH] == 1 && x == 0 && y < DIM_2
- OFFSET - OFFSET)
 


OFFSET);
 
{
printf("KB Array Inferred WUMPUS is at (%i,%i) in process 1\n", x, y +

iKBArray[x][y + OFFSET][WUMPUS] = 1;
iWumpusFound = iWumpusFound +1;
}
 

BOTTOM ROW ***
 
//Process WUMPUS from TOP, LEFT, AND RIGHT STENCH LOCATIONS *** IN

else if(iKBArray[x][y][STENCH] == 1 && iKBArray[x + OFFSET][y -
 
OFFSET][STENCH] == 1 && iKBArray[x + OFFSET][y + OFFSET][STENCH] == 1 && x == DIM_1 - OFFSET
- OFFSET && y != 0 && y != DIM_2 - OFFSET)
{
 

OFFSET, y);
 
printf("KB Array Inferred WUMPUS is at (%i,%i) in process 2\n", x +

iKBArray[x][y + OFFSET][WUMPUS] = 1;
iWumpusFound = iWumpusFound +1;
}
 
//Process WUMPUS from TOP, LEFT, RIGHT AND BOTTOM STENCH LOCATIONS
*** OUT IN THE FIELD ***
else if(iKBArray[x][y][STENCH] == 1 && iKBArray[x + OFFSET][y - OFFSET][STENCH] == 1 && iKBArray[x + OFFSET][y + OFFSET][STENCH] == 1 && iKBArray[x + OFFSET
+ OFFSET][y][STENCH] && x < DIM_1 - OFFSET - OFFSET &&  y != 0 && y < DIM_2 - OFFSET - OFFSET)
{
 

OFFSET, y);
 
printf("KB Array Inferred WUMPUS is at (%i,%i) in process 3\n", x +

iKBArray[x + OFFSET][y][WUMPUS] = 1;
iWumpusFound = iWumpusFound +1;
}
 
//Process WUMPUS from TOP, BOTTOM, and RIGHT STENCH LOCATIONS *** ALONG LEFT BOUNDARY ***
 
else if(iKBArray[x][y][STENCH] == 1 && iKBArray[x + OFFSET + OFFSET][y][STENCH] == 1 && iKBArray[x + OFFSET][y + OFFSET][STENCH] == 1 && x < DIM_1 - OFFSET - OFFSET && y == 0)
{
 

OFFSET, y);
 
printf("KB Array Inferred WUMPUS is at (%i,%i) in process 4\n", x +

iKBArray[x + OFFSET][y][WUMPUS] = 1;
iWumpusFound = iWumpusFound +1;
}
 
//Process WUMPUS from TOP, BOTTOM, and LEFT STENCH LOCATIONS *** ALONG RIGHT BOUNDARY ***
else if(iKBArray[x][y][STENCH] == 1 && iKBArray[x + OFFSET + OFFSET][y][STENCH] == 1 && iKBArray[x + OFFSET][y - OFFSET][STENCH] == 1 && x < DIM_1 - OFFSET - OFFSET && y == DIM_2 - OFFSET)
{
 

OFFSET, y);


}
}
}
 
printf("KB Array Inferred WUMPUS is at (%i,%i) in process 5\n", x +

iKBArray[x + OFFSET][y][WUMPUS] = 1;
iWumpusFound = iWumpusFound +1;
 

//Process PIT Rules
for(int x = 0; x < DIM_1; x++)
{
for(int y = 0; y < DIM_2; y++)
{
 

UPPER ROW ***
 
//Process PIT from LEFT, RIGHT and BOTTOM BREEZE LOCATIONS *** IN

if(iKBArray[x][y][BREEZE] == 1 && iKBArray[x][y + OFFSET +
 
OFFSET][BREEZE] == 1 && iKBArray[x + OFFSET][y + OFFSET][BREEZE] == 1 && x == 0 && y < DIM_2 - OFFSET - OFFSET)
{
printf("KB Array Inferred PIT is at (%i,%i) in process 1\n", x, y + OFFSET); iKBArray[x][y + OFFSET][PIT] = 1;
iPitsFound = iPitsFound +1;
}
 

BOTTOM ROW ***
 
//Process PIT from TOP, LEFT, AND RIGHT BREEZE LOCATIONS *** IN

else if(iKBArray[x][y][BREEZE] == 1 && iKBArray[x + OFFSET][y -
 
OFFSET][BREEZE] == 1 && iKBArray[x + OFFSET][y + OFFSET][BREEZE] == 1 && x == DIM_1 - OFFSET
- OFFSET && y != 0 && y != DIM_2 - OFFSET)
{
printf("KB Array Inferred PIT is at (%i,%i) in process 2\n", x + OFFSET, y); iKBArray[x][y + OFFSET][PIT] = 1;
iPitsFound = iPitsFound +1;
}
 

OUT IN THE FIELD ***
 
//Process PIT from TOP, LEFT, RIGHT AND BOTTOM BREEZE LOCATIONS ***

else if(iKBArray[x][y][BREEZE] == 1 && iKBArray[x + OFFSET][y -
 
OFFSET][BREEZE] == 1 && iKBArray[x + OFFSET][y + OFFSET][BREEZE] == 1 && iKBArray[x + OFFSET
+ OFFSET][y][BREEZE] && x < DIM_1 - OFFSET - OFFSET &&  y != 0 && y < DIM_2 - OFFSET - OFFSET)
{
printf("KB Array Inferred PIT is at (%i,%i) in process 3\n", x + OFFSET, y); iKBArray[x + OFFSET][y][PIT] = 1;
 
iPitsFound = iPitsFound +1;
}
 

LEFT BOUNDARY ***
 
//Process PIT from TOP, BOTTOM, and RIGHT BREEZE LOCATIONS *** ALONG

else if(iKBArray[x][y][BREEZE] == 1 && iKBArray[x + OFFSET +
 
OFFSET][y][BREEZE] == 1 && iKBArray[x + OFFSET][y + OFFSET][BREEZE] == 1 && x < DIM_1 - OFFSET - OFFSET && y == 0)
{
printf("KB Array Inferred PIT is at (%i,%i) in process 4\n", x + OFFSET, y); iKBArray[x + OFFSET][y][PIT] = 1;
iPitsFound = iPitsFound +1;
}
 

RIGHT BOUNDARY ***
 
//Process PIT from TOP, BOTTOM, and LEFT BREEZE LOCATIONS *** ALONG

else if(iKBArray[x][y][BREEZE] == 1 && iKBArray[x + OFFSET +
 
OFFSET][y][BREEZE] == 1 && iKBArray[x + OFFSET][y - OFFSET][BREEZE] == 1 && x < DIM_1 - OFFSET
- OFFSET && y == DIM_2 - OFFSET)
{
printf("KB Array Inferred PIT is at (%i,%i) in process 5\n", x + OFFSET, y); iKBArray[x + OFFSET][y][PIT] = 1;
iPitsFound = iPitsFound +1;
}
}
}
}

int FireArrow(int iWorldArray[DIM_1][DIM_2][DIM_3],int iAgentArray[DIM_1][DIM_2][DIM_3])
{
//Wumpus Status
int iWumpusDead = 0;

//Determine direction agent facing

//Fire arrow in world if agent in row or column in front of agent Wumpus is dead else wumpus is alive



return iWumpusDead;
}
//End Fire Arrow
/*
//Maybe try to implement this later going to use 4 fixed agents for now void CreateAgents(int iNumb, string sAgentName)
{

for(int x = 0; x < iNumb; x++)
{	//Create an agent with the name provided in sAgentName with number x appended to it

//malloc(


}
}

*/
