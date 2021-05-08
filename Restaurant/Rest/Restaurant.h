#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "../Events/CancellationEvent.h"
#include "..\Generic_DS\LinkedBag.h"
#include"../Generic_DS/PriorityQueue.h"
#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	void printinfo(int time,Cook*pCok=nullptr);
	
	//
	// TODO: Add More Data Members As Needed
	//

	Queue<Order*>normalqueue;//waiting queue of type normal
	PriorityQueue<Order*>VIPqueue;//waiting queue of type VIP
	Queue<Order*>veganqueue;//waiting queue of type vegan
	Queue<Order*>ords;

	PriorityQueue<Order*>sevnormalqueue;
	PriorityQueue<Order*>sevVIPqueue;
	PriorityQueue<Order*>sevVeganqueue;

	Queue<Order*>donenormalqueue;
	Queue<Order*>doneVIPqueue; 
	Queue<Order*>doneVeganqueue;

	PriorityQueue<Cook*>AVcookNorm;
	PriorityQueue<Cook*>AVcookVeg;
	PriorityQueue<Cook*>AVcookVip;

	PriorityQueue<Cook*>SERcookNorm;
	PriorityQueue<Cook*>SERcookVeg;
    PriorityQueue<Cook*>SERcookVip; 

	//updated phase two//
	PriorityQueue<Cook*>BreakNrmCokqueue;///not sure//
	PriorityQueue<Cook*>BreakVipCokqueue;
	PriorityQueue<Cook*>BreakVegCokqueue;
	
	Queue <Cook*> hospital; // in-rest cooks 
	bool first_injury_normal;//flag indicates that the first cook of type normal is injured
	bool first_injury_vip;
	bool first_injury_vegan;


	float R; //  random number 
	float Prop; // injure probability
	int Rest_Prd; // rest period
	int NP;
	int injured_cooks;
	int auto_pormoted=0;
	int urgent_orders=0;
	int VIP_WaitTime;

	string NameOfInputfile;
	//phase two test first try//
	void assignCoktoNrmOrd(int time);
	void assignCoktoVegOrd(int time);
	void assignCokToVipOrd(int time);


	void isFinishedNormal(int time);
	void isFinishedVip(int time);
	void isFinishedVegan(int time);


	void BreakOrWorkNrm(int time);
	void BreakOrWorkVip(int time);
	void BreakOrWorkVeg(int time);
	bool FinishProgram();

	//not sure //
	void printInfo_of_assigned_noramlcooks(string msg_cook,string msg_order,int choice=0);

public:
	
	

	Restaurant();
	~Restaurant();
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();
	
	//
	// TODO: Add More Member Functions As Needed
	//
	int original_normal_orders = 0;
	void Addtonormalqueue(Order*pn);
	void AddtoVIPqueue(Order* pVIP);
	void Addtoveganqueue(Order* pvegan);
	void AddToordsqueueu(Order* ptr);
	bool RemoveFromordqueue(Order *&ptr);
	bool RemoveFromnormqueue(Order*& ptr);

	bool peekfrontorderqueue(Order *&ptr);
	bool peekfrontnormqueue(Order*& ptr);

	void FileLoading();

	//marim
	void output_file();
	
	// robert
	void UrgentOrders(int time);
/// ================================================================================================== 
};

#endif