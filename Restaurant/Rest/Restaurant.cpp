#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include<string>
#include <math.h>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\PromotionEvent.h"


void Restaurant::printinfo(int time,Cook*pCok)
{
	//pGUI->PrintMessage("TS: " + to_string(time));

	pGUI->printInfo("# available Normal cooks: " + to_string(AVcookNorm.getcount()), 10, 1.5);
	pGUI->printInfo("# available Vegan  cooks: " + to_string(AVcookVeg.getcount()), 10, 2);
	pGUI->printInfo("# available VIP    cooks: " + to_string(AVcookVip.getcount()), 10, 3);

	pGUI->printInfo("# waiting Normal orders: " + to_string(normalqueue.getcount()), 220, 1.5);
	pGUI->printInfo("# waiting Vegan  orders: " + to_string(veganqueue.getcount()), 220, 2);
	pGUI->printInfo("# waiting VIP    orders: " + to_string(VIPqueue.getcount()), 220, 3);

	string number_of_served_orders = to_string(donenormalqueue.getcount() + doneVeganqueue.getcount() + doneVIPqueue.getcount());
	pGUI->printInfo("Served orders so far: "+number_of_served_orders,70,1.25);
	

}

Restaurant::Restaurant()
{
	pGUI = NULL;
	first_injury_normal = false;
	first_injury_vip = false;
	first_injury_vegan = false;
	injured_cooks = 0;
	auto_pormoted = 0;
	urgent_orders = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
		int CurrentTimeStep;
/////////////////////////// interactive mode /////////////////////////////////////////////////
	case MODE_INTR:
		FileLoading();
		 CurrentTimeStep = 1;
		//as long as events queue is not empty yet
		while (FinishProgram())
		{
			//print current timestep
			pGUI->PrintMessage("TS: " + to_string(CurrentTimeStep));
			ExecuteEvents(CurrentTimeStep);//execute all events at current time step
			BreakOrWorkNrm(CurrentTimeStep);
			BreakOrWorkVip(CurrentTimeStep);
			BreakOrWorkVeg(CurrentTimeStep);

			isFinishedNormal(CurrentTimeStep);
			isFinishedVip(CurrentTimeStep);
			isFinishedVegan(CurrentTimeStep);
			assignCokToVipOrd(CurrentTimeStep);
			assignCoktoNrmOrd(CurrentTimeStep);
			assignCoktoVegOrd(CurrentTimeStep);
			UrgentOrders(CurrentTimeStep);

			printinfo(CurrentTimeStep);
			FillDrawingList();
			pGUI->UpdateInterface();

			R = float(rand()) / (500 * CurrentTimeStep); // Generate  random number  each time step
			
			pGUI->ResetDrawingList();
			pGUI->waitForClick();

			CurrentTimeStep++;
		}
		Sleep(1000);
		printinfo(CurrentTimeStep);
		pGUI->waitForClick();
		pGUI->PrintMessage("Great! Generation is done. please, click to end the program ");
		pGUI->waitForClick();
		output_file();
		break;
/////////////////////////////// step by step mode/////////////////////////////////
	case MODE_STEP:
		FileLoading();
		 CurrentTimeStep = 1;

		//as long as events queue is not empty yet
		while (FinishProgram())
		{
			pGUI->PrintMessage("TS: " + to_string(CurrentTimeStep));
			//print current timestep
			ExecuteEvents(CurrentTimeStep);//execute all events at current time step
			BreakOrWorkNrm(CurrentTimeStep);
			BreakOrWorkVip(CurrentTimeStep);
			BreakOrWorkVeg(CurrentTimeStep);

			isFinishedNormal(CurrentTimeStep);
			isFinishedVip(CurrentTimeStep);
			isFinishedVegan(CurrentTimeStep);
			assignCokToVipOrd(CurrentTimeStep);
			assignCoktoNrmOrd(CurrentTimeStep);
			assignCoktoVegOrd(CurrentTimeStep);
			UrgentOrders(CurrentTimeStep);
			printinfo(CurrentTimeStep);

			FillDrawingList();
			pGUI->UpdateInterface();

			// Generate  random number  each time step
			R = float(rand()) / (500 * CurrentTimeStep);
		
			pGUI->ResetDrawingList();
			CurrentTimeStep++;
			Sleep(1000);
		}
		Sleep(1000);
		printinfo(CurrentTimeStep);
		Sleep(1000);
		pGUI->PrintMessage(" Program End, Thank you! ");
		Sleep(1000);
		output_file();
		break;
//////////////////////////// silent mode /////////////////////////////////////////
	case MODE_SLNT:
		FileLoading();
	    CurrentTimeStep = 1;
		//as long as events queue is not empty yet
		while (FinishProgram())
		{
			//print current timestep
			ExecuteEvents(CurrentTimeStep);//execute all events at current time step
			BreakOrWorkNrm(CurrentTimeStep);
			BreakOrWorkVip(CurrentTimeStep);
			BreakOrWorkVeg(CurrentTimeStep);

			isFinishedNormal(CurrentTimeStep);
			isFinishedVip(CurrentTimeStep);
			isFinishedVegan(CurrentTimeStep);
			assignCokToVipOrd(CurrentTimeStep);
			assignCoktoNrmOrd(CurrentTimeStep);
			assignCoktoVegOrd(CurrentTimeStep);
			UrgentOrders(CurrentTimeStep);
			// Generate  random number  each time step
			R = float(rand()) / (500 * CurrentTimeStep);

			CurrentTimeStep++;
		}
		output_file();
		break;

	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//////////// drawing orders
	int size = 0;
	Order* pOrd;
	Order** OrdArr = ords.toArray(size);
	for (int i = 0; i < size; i++) {
		Order* ptr = OrdArr[i];
		pGUI->AddToDrawingList(ptr);
	}
	///////////// drawing cooks
	Cook* co;
	int sizen; int sizeg; int sizev = 0;
	Cook** CokNorm = AVcookNorm.toArray(sizen);
	Cook** CokVAG = AVcookVeg.toArray(sizeg);
	Cook** CokVIP = AVcookVip.toArray(sizev);
	// merge the 3 arrays of cooks inone array
	int ntot = sizen + sizeg + sizev; // total size
	Cook** cookarr = new Cook * [ntot];
	int nn = 0; int ng = 0;  int nv = 0;
	for (int k = 0; k < ntot; k++)
	{
		if (nn < sizen)
		{
			cookarr[k] = CokNorm[nn];
			nn++;
		}
		else if (ng < sizeg)
		{
			cookarr[k] = CokVAG[ng];
			ng++;
		}
		else if (nv < sizev)
		{
			cookarr[k] = CokVIP[nv];
			nv++;
		}
	}
	// sorting them
	for (int i = 0; i < ntot; i++)
	{
		for (int j = 0;j < ntot - 1;j++)
		{
			if (cookarr[j]->GetID() > cookarr[j + 1]->GetID())
			{
				Cook* temp = cookarr[j];
				cookarr[j] = cookarr[j + 1];
				cookarr[j + 1] = temp;
			}
		}
	}
	// Draw
	for (int i = 0; i < ntot; i++)
	{
		co = cookarr[i];
		pGUI->AddToDrawingList(co);
	}
}

void Restaurant::Addtonormalqueue(Order* pOrd) {
	normalqueue.enqueue(pOrd);
}

void Restaurant::Addtoveganqueue(Order* pOrd) {
	veganqueue.enqueue(pOrd);
}

void Restaurant::AddToordsqueueu(Order* ptr)
{
	ords.enqueue(ptr);
}

bool Restaurant::RemoveFromordqueue(Order*& ptr)
{
	return ords.dequeue(ptr);
}

bool Restaurant::RemoveFromnormqueue(Order*& ptr)
{
	return normalqueue.dequeue(ptr);
}

bool Restaurant::peekfrontorderqueue(Order*& ptr)
{

	return ords.peekFront(ptr);
}

bool Restaurant::peekfrontnormqueue(Order*& ptr)
{
	return normalqueue.peekFront(ptr);
}

void Restaurant::FileLoading()
{
	ifstream input;
	pGUI->PrintMessage("Please write the name of input file like (' inputfile1 ')   .    .    .  ");
	NameOfInputfile = pGUI->GetString();
	input.open(NameOfInputfile+".txt");
	int N, G, V, BO, AutoP, M;

	//update of phase two//
	int SN_min, SN_max, SG_min, SG_max, SV_min, SV_max;
	float InjProp, RstPrd;
	int VIP_WT;
	int BN_min, BN_max, BG_min, BG_max, BV_min, BV_max;
	////////////////////////////////////////////////////////
	char x, TYP;
	int TS, ID, Size, MONY;
	input >> N >> G >> V;//line 1//
	input >> SN_min >> SN_max >> SG_min >> SG_max >> SV_min >> SV_max;//line 2//
	input >> BO >> BN_min >> BN_max >> BG_min >> BG_max >> BV_min >> BV_max;//line 3//
	input >> InjProp >> RstPrd;//line 4//
	input >> AutoP >> VIP_WT;//ine 5//
	input >> M;//line 6//


	if ((N < 0 || G < 0 || V < 0) || (N == 0 && G == 0 && V == 0))
	{
		pGUI->PrintMessage(" There is no cook , click to end the program");
		pGUI->waitForClick();
		exit(0);
	}

	int EXMONEY;
	// marimss
	Prop = InjProp;
	Rest_Prd = RstPrd;
	NP = AutoP;

	VIP_WaitTime = VIP_WT;
	srand(time(0));
	int cID = 1;
	Cook* co;
	int n = 0; int g = 0; int v = 0;
	for (int i = 0; i < (N + V + G); i++) {
		if (n < N) {
			co = new Cook;
			co->setID(cID);
			co->setType(TYPE_NRM);
			co->setspeed((rand() % SN_max) + SN_min);
			co->setorderstoprepare(BO);
			co->setbreakduration((rand() % BN_max) + BN_min);
			AVcookNorm.enqueue(co, -cID);
			cID++;
			n++;
		}
		if (g < G) {
			co = new Cook;
			co->setID(cID);
			co->setType(TYPE_VGAN);
			co->setspeed((rand() % SG_max) + SG_min);
			co->setorderstoprepare(BO);
			co->setbreakduration((rand() % BG_max) + BG_min);
			AVcookVeg.enqueue(co, -cID);
			cID++;
			g++;
		}
		if (v < V) {
			co = new Cook;
			co->setID(cID);
			co->setType(TYPE_VIP);
			co->setspeed((rand() % SV_max) + SV_min);
			co->setorderstoprepare(BO);
			co->setbreakduration((rand() % BV_max) + BV_min);
			AVcookVip.enqueue(co, -cID);
			cID++;
			v++;
		}
	}


	Event* pEv;
	for (int i = 0; i < M; i++)
	{
		int OType;
		input >> x;
		if (x == 'R') {
			input >> TYP >> TS >> ID >> Size >> MONY;
			if (TYP == 'N')
				OType = 0;
			else if (TYP == 'V')
				OType = 2;
			else
				OType = 1;
			pEv = new ArrivalEvent(TS, ID, (ORD_TYPE)OType, MONY, Size, VIP_WT);
		}
		else if (x == 'X')
		{
			input >> TS >> ID;
			pEv = new CancellationEvent(TS, ID);
		}
		else if (x == 'P')
		{
			input >> TS >> ID >> EXMONEY;
			pEv = new PromotionEvent(TS, ID, EXMONEY);
		}
		EventsQueue.enqueue(pEv);
	}
}

void Restaurant::AddtoVIPqueue(Order* pOrd)
{
	VIPqueue.enqueue(pOrd,pOrd->priorityofviporder());
}

//phase two first test//


void Restaurant::assignCoktoNrmOrd(int time)
{
	Order* pOrd = nullptr;
	Cook* pCok = nullptr;
	string msg1;
	string msg2;
	int Choice = 0;
	if(!normalqueue.isEmpty())
	   normalqueue.peekFront(pOrd);
	while (pOrd && time - pOrd->getArrTime() > NP)
	{
		if (time - pOrd->getArrTime() > NP) {
			PromotionEvent* p = new PromotionEvent(time, pOrd->GetID(), 0);
			p->Execute(this);
			EventsQueue.enqueue(p);
			auto_pormoted++;
			pOrd = nullptr;
			normalqueue.peekFront(pOrd);
		}
	}
	pOrd = nullptr;
	while (!AVcookNorm.isEmpty() && !normalqueue.isEmpty())
	{
		normalqueue.dequeue(pOrd);
		Choice = 1;
		pOrd->setServTime(time);
		AVcookNorm.dequeue(pCok);
		pOrd->setStatus(SRV);
		pCok->assign(pOrd);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevnormalqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookNorm.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID()) + " ";
	}
	printInfo_of_assigned_noramlcooks(msg1,msg2,Choice);
	Choice = 0;
	msg1 = "";
	msg2 = "";
	while (!AVcookVip.isEmpty() && !normalqueue.isEmpty())
	{
		normalqueue.dequeue(pOrd);
		Choice = 2;
		AVcookVip.dequeue(pCok);
		pCok->assign(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevnormalqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookVip.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID()) + " ";
	}
	printInfo_of_assigned_noramlcooks(msg1 , msg2, Choice);
}

void Restaurant::assignCoktoVegOrd(int time) {
	Order* pOrd = nullptr;
	Cook* pCok = nullptr;
	string msg1;
	string msg2;
	int Choice = 0;
	while (!AVcookVeg.isEmpty() && !veganqueue.isEmpty()) {
		Choice = 3;
		veganqueue.dequeue(pOrd);
		AVcookVeg.dequeue(pCok);
		pCok->assign(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevVeganqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookVeg.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID())+" ";
	}
	printInfo_of_assigned_noramlcooks(msg1 , msg2,Choice);
}

void Restaurant::assignCokToVipOrd(int time) {
	Order* pOrd = nullptr;
	Cook* pCok = nullptr;
	VIPqueue.peekFront(pOrd);
	string msg1;
	string msg2;
	int Choice = 0;
	while (!AVcookVip.isEmpty() && !VIPqueue.isEmpty()) {
		Choice = 4;
		VIPqueue.dequeue(pOrd);
		AVcookVip.dequeue(pCok);
		pCok->assign(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookVip.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID()) + " ";
	}
	printInfo_of_assigned_noramlcooks(msg1 , msg2, Choice);
	Choice = 0;
	msg1 = "";
	msg2 = "";
	while (!AVcookNorm.isEmpty() && !VIPqueue.isEmpty()) {
		Choice = 5;
		VIPqueue.dequeue(pOrd);
		AVcookNorm.dequeue(pCok);
		pCok->assign(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookNorm.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID()) + " ";
	}
	printInfo_of_assigned_noramlcooks(msg1 , msg2,Choice);
	Choice = 0;
	msg1 = "";
	msg2 = "";
	while (!AVcookVeg.isEmpty() && !VIPqueue.isEmpty()) {
		Choice = 6;
		VIPqueue.dequeue(pOrd);
		AVcookVeg.dequeue(pCok);
		pCok->assign(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);
		pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));//not sure//
		msg2 += to_string(pOrd->GetID()) + " ";
		sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
		pCok->setordersize(pOrd->getSize());
		pCok->incrementordresprepared();
		pCok->setassignTS(time);
		SERcookVeg.enqueue(pCok, -pCok->getfinishTS());
		msg1 += to_string(pCok->GetID()) + " ";
	}
	printInfo_of_assigned_noramlcooks(msg1 , msg2,Choice);
}

void Restaurant::isFinishedNormal(int time)
{
	Cook* pCok = nullptr;
	Order* pOrd = nullptr;


	Cook* pInjCok;
	if (hospital.peekFront(pInjCok) && pInjCok->GetType() == TYPE_NRM)
		if (time == pInjCok->getfinishTS() + Rest_Prd)    // rest is finished
		{

			first_injury_normal = false;
			hospital.dequeue(pInjCok);
			AVcookNorm.enqueue(pInjCok, -(pInjCok->GetID()));
			pInjCok->is_injured = false; // great! you are not injured anymore

		}




	while (SERcookNorm.peekFront(pCok))
	{

		if (pCok->getfinishTS() == time)
		{


			if (pCok->is_injured)
			{
				if (pCok->start_rest) // to do this only once
				{
					SERcookNorm.dequeue(pCok);
					hospital.enqueue(pCok);
					pCok->start_rest = false;  // you have already started your rest
				}
			}
			else
			{
				SERcookNorm.dequeue(pCok);
				if (pCok->getordersprepared() % pCok->getorderstoprepare() == 0) {
					//updated phase two//
					int time_break_ends = time + pCok->getbreakduration();
					BreakNrmCokqueue.enqueue(pCok, -time_break_ends);

				}
				else
					AVcookNorm.enqueue(pCok, -(pCok->GetID()));

			}

		}
		else
		{
			if (pCok->start_rest && !first_injury_normal && R <= Prop && !pCok->is_injured)
			{
				first_injury_normal = true; // only the first busy cook will be injured each time step
				pCok->is_injured = true;
				if (pCok->getOrder()->GetType() == TYPE_VIP)
				{
					sevVIPqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
					injured_cooks++;
				}
				else if (pCok->getOrder()->GetType() == TYPE_NRM)
				{
					sevnormalqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevnormalqueue.enqueue(pOrd, -pOrd->getFinishTime());
					injured_cooks++;

				}
				else
				{
					sevVeganqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVeganqueue.enqueue(pOrd, -pOrd->getFinishTime());
					injured_cooks++;
				}
				SERcookNorm.dequeue(pCok);
				pCok->update_finish_TS(time);
				SERcookNorm.enqueue(pCok, -pCok->getfinishTS());


			}
			break;
		}

	}




	pCok = nullptr;
	while (sevnormalqueue.peekFront(pOrd)) {
		if (pOrd->getFinishTime() == time) {
			sevnormalqueue.dequeue(pOrd);
			pOrd->setStatus(DONE);
			donenormalqueue.enqueue(pOrd);
		}
		else
			break;
	}




	pOrd = nullptr;
}

void Restaurant::isFinishedVip(int time)
{
	Cook* pCok = nullptr;
	Order* pOrd = nullptr;

	Cook* pInjCok;
	if (hospital.peekFront(pInjCok) && pInjCok->GetType() == TYPE_VIP)
		if (time == pInjCok->getfinishTS() + Rest_Prd)    // rest is finished
		{


			first_injury_vip = false;
			hospital.dequeue(pInjCok);
			AVcookVip.enqueue(pInjCok, -(pInjCok->GetID()));
			pInjCok->is_injured = false; // great! you are not injured anymore
		}



	while (SERcookVip.peekFront(pCok))
	{


		if (pCok->getfinishTS() == time)
		{

			if (pCok->is_injured)
			{
				if (pCok->start_rest) // to do this only once
				{
					SERcookVip.dequeue(pCok);
					hospital.enqueue(pCok);
					pCok->start_rest = false;  // you have already started your rest
				}
			}
			else
			{
				SERcookVip.dequeue(pCok);
				if (pCok->getordersprepared() % pCok->getorderstoprepare() == 0) {
					//update phase two//
					int time_break_ends = time + pCok->getbreakduration();
					BreakVipCokqueue.enqueue(pCok, -time_break_ends);
				}
				else
					AVcookVip.enqueue(pCok, -(pCok->GetID()));
			}
		}
		else
		{
			if (pCok->start_rest && !first_injury_vip && R <= Prop && !pCok->is_injured)
			{
				first_injury_vip = true; // only the first busy cook will be injured each time step
				pCok->is_injured = true;
				if (pCok->getOrder()->GetType() == TYPE_VIP)
				{
					sevVIPqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
				}
				else if (pCok->getOrder()->GetType() == TYPE_NRM)
				{
					sevnormalqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevnormalqueue.enqueue(pOrd, -pOrd->getFinishTime());

				}
				else
				{
					sevVeganqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVeganqueue.enqueue(pOrd, -pOrd->getFinishTime());
				}

				SERcookVip.dequeue(pCok);
				pCok->update_finish_TS(time);
				SERcookVip.enqueue(pCok, -pCok->getfinishTS());

			}
			break;
		}
	}

	pCok = nullptr;
	while (sevVIPqueue.peekFront(pOrd)) {
		if (pOrd->getFinishTime() == time) {
			sevVIPqueue.dequeue(pOrd);
			pOrd->setStatus(DONE);
			doneVIPqueue.enqueue(pOrd);
		}
		else
			break;
	}
	pOrd = nullptr;
}

void Restaurant::isFinishedVegan(int time)
{

	Cook* pCok = nullptr;
	Order* pOrd = nullptr;
	Cook* pInjCok;
	if (hospital.peekFront(pInjCok) && pInjCok->GetType() == TYPE_VGAN)
		if (time == pInjCok->getfinishTS() + Rest_Prd)    // rest is finished
		{

			first_injury_vegan = false;
			hospital.dequeue(pInjCok);
			AVcookVeg.enqueue(pInjCok, -(pInjCok->GetID()));
			pInjCok->is_injured = false; // great! you are not injured anymore
		}


	while (SERcookVeg.peekFront(pCok)) {



		if (pCok->getfinishTS() == time)
		{
			if (pCok->is_injured)
			{
				if (pCok->start_rest) // to do this only once
				{
					SERcookVeg.dequeue(pCok);
					hospital.enqueue(pCok);
					pCok->start_rest = false;  // you have already started your rest
				}
			}
			else
			{
				SERcookVeg.dequeue(pCok);
				if (pCok->getordersprepared() % pCok->getorderstoprepare() == 0) {
					//update phaase two/
					int time_break_ends = time + pCok->getbreakduration();
					BreakVegCokqueue.enqueue(pCok, -time_break_ends);
				}
				else
					AVcookVeg.enqueue(pCok, -(pCok->GetID()));
			}
		}
		else
		{
			if (pCok->start_rest && !first_injury_vegan && R <= Prop && !pCok->is_injured)
			{
				first_injury_vegan = true; // only the first busy cook will be injured each time step
				pCok->is_injured = true;

				if (pCok->getOrder()->GetType() == TYPE_VIP)
				{
					sevVIPqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
				}
				else if (pCok->getOrder()->GetType() == TYPE_NRM)
				{
					sevnormalqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevnormalqueue.enqueue(pOrd, -pOrd->getFinishTime());

				}
				else
				{
					sevVeganqueue.dequeue(pOrd);
					pOrd->update_finish_TS(time);
					sevVeganqueue.enqueue(pOrd, -pOrd->getFinishTime());
				}

				SERcookVeg.dequeue(pCok);
				pCok->update_finish_TS(time);
				SERcookVeg.enqueue(pCok, -pCok->getfinishTS());



			}
			break;
		}

	}

	pCok = nullptr;
	while (sevVeganqueue.peekFront(pOrd)) {
		if (pOrd->getFinishTime() == time) {
			sevVeganqueue.dequeue(pOrd);
			pOrd->setStatus(DONE);
			doneVeganqueue.enqueue(pOrd);
		}
		else
			break;
	}
	pOrd = nullptr;
}

void Restaurant::BreakOrWorkNrm(int time)
{
	Cook* pCok = nullptr;
	while (BreakNrmCokqueue.peekFront(pCok)) {
		if (time == pCok->getfinishTS() + pCok->getbreakduration()) {
			BreakNrmCokqueue.dequeue(pCok);
			AVcookNorm.enqueue(pCok, -pCok->GetID());
		}
		else
			break;
	}
}

void Restaurant::BreakOrWorkVip(int time)
{
	Cook* pCok = nullptr;
	while (BreakVipCokqueue.peekFront(pCok)) {
		if (time == pCok->getfinishTS() + pCok->getbreakduration()) {
			BreakVipCokqueue.dequeue(pCok);
			AVcookVip.enqueue(pCok, -pCok->GetID());
		}
		else
			break;
	}
}

void Restaurant::BreakOrWorkVeg(int time)
{
	Cook* pCok = nullptr;
	while (BreakVegCokqueue.peekFront(pCok)) {
		if (time == pCok->getfinishTS() + pCok->getbreakduration()) {
			BreakVegCokqueue.dequeue(pCok);
			AVcookVeg.enqueue(pCok, -pCok->GetID());
		}
		else
			break;
	}
}

bool Restaurant::FinishProgram()
{
	if (!EventsQueue.isEmpty() | !normalqueue.isEmpty() || !sevnormalqueue.isEmpty()  || !sevVIPqueue.isEmpty() || !sevVeganqueue.isEmpty() || !hospital.isEmpty() || !BreakVipCokqueue.isEmpty() || !BreakVegCokqueue.isEmpty() || !BreakNrmCokqueue.isEmpty())
        return true;
	if (!veganqueue.isEmpty()&&!AVcookVeg.isEmpty())
		return true;

	return false;

}
void Restaurant::printInfo_of_assigned_noramlcooks(string msg_cook,string msg_order,int choice)
{
	string msg1;
	string msg2;
	if (choice == 1) {
		msg1 = "Normal Cook(s)#";
		msg2 = "assigned Normal Order(s)";
		pGUI->printInfo(msg1+msg_cook+msg2+msg_order, 450, 1.5);
	}
	else if (choice==2) {
		msg1 = "Vip Cook(s)#";
		msg2 = "assigned Normal Order(s)";
		pGUI->printInfo(msg1 + msg_cook + msg2 + msg_order, 450, 2);
	}
	else if (choice == 3) {
		msg1 = "Vegan Cook(s)#";
		msg2 = "assigned Vegan Order(s)";
		pGUI->printInfo(msg1 + msg_cook + msg2 + msg_order, 450, 3);
	}
	else if (choice == 4) {
		msg1 = "Vip Cook(s)#";
		msg2 = "assigned Vip Order(s)";
		pGUI->printInfo(msg1 + msg_cook + msg2 + msg_order, 800, 1.5);
	}
	else if (choice == 5) {
		msg1 = "Normal Cook(s)#";
		msg2 = "assigned Vip Order(s)";
		pGUI->printInfo(msg1 + msg_cook + msg2 + msg_order, 800, 2);
	}
	else if (choice == 6) {
		msg1 = "Vegan Cook(s)#";
		msg2 = "assigned Vip Order(s)";
		pGUI->printInfo(msg1 + msg_cook + msg2 + msg_order, 800, 3);
	}
	else {}
}

//MARIM
void Restaurant::output_file()
{
	ofstream OutFile;
	OutFile.open("Out_"+NameOfInputfile+".txt");

	Order* pOrder;
	int NumberOfDoneOrders = donenormalqueue.getcount() + doneVeganqueue.getcount() + doneVIPqueue.getcount();
	Order** total_done_orders = new Order * [NumberOfDoneOrders];
	int norm_Count = 0;
	int veg_Count = 0;
	int VIP_Count = 0;
	for (int i = 0; i < NumberOfDoneOrders;i++)
	{
		if (donenormalqueue.dequeue(pOrder))
		{

			total_done_orders[i] = pOrder;
			norm_Count++;

		}

		else if (doneVIPqueue.dequeue(pOrder))
		{

			total_done_orders[i] = pOrder;
			VIP_Count++;

		}
		else if (doneVeganqueue.dequeue(pOrder))
		{

			total_done_orders[i] = pOrder;
			veg_Count++;

		}
	}
	for (int i = 0; i < NumberOfDoneOrders; i++)
	{
		for (int j = 0;j < NumberOfDoneOrders - 1;j++)
		{
			if (total_done_orders[j]->getFinishTime() > total_done_orders[j + 1]->getFinishTime())
			{
				Order* temp = total_done_orders[j];
				total_done_orders[j] = total_done_orders[j + 1];
				total_done_orders[j + 1] = temp;
			}
			else if (total_done_orders[j]->getSevTime() > total_done_orders[j + 1]->getSevTime())
			{
				Order* temp = total_done_orders[j];
				total_done_orders[j] = total_done_orders[j + 1];
				total_done_orders[j + 1] = temp;
			}
		}
	}
	float total_waiting_time = 0;
	int number_waiting_orders = 0;
	float total_service_time = 0;
	int number_service_orders = 0;
	for (int i = 0; i < NumberOfDoneOrders; i++)
	{
		OutFile << total_done_orders[i]->getFinishTime() << "  " << total_done_orders[i]->GetID() << "  " << total_done_orders[i]->getArrTime() << "  " << total_done_orders[i]->getSevTime() - total_done_orders[i]->getArrTime() << "  " << total_done_orders[i]->getSevTime() << endl;
		total_waiting_time += float(total_done_orders[i]->getSevTime() - total_done_orders[i]->getArrTime());
		if (total_done_orders[i]->getSevTime() - total_done_orders[i]->getArrTime() != 0)
			number_waiting_orders++;
		total_service_time += total_done_orders[i]->getSevTime();
		number_service_orders++;
	}
	float auto_percentage = 0;
	float avg_time, srv_time;
	if (original_normal_orders != 0)
		 auto_percentage = (float(auto_pormoted) / float(original_normal_orders)) * 100;
	if (number_waiting_orders != 0)
		  avg_time = total_waiting_time / number_waiting_orders;
	else
		 avg_time = 0;
	if (number_service_orders != 0)
		  srv_time = total_service_time / number_service_orders;
	else
		  srv_time = 0;
	OutFile << "Orders: " << NumberOfDoneOrders << " [Norm: " << norm_Count << ", Veg : " << veg_Count << ", VIP : " << VIP_Count << "]" << endl;
	OutFile << "Cooks: " << AVcookNorm.getcount() + AVcookVeg.getcount() + AVcookVip.getcount() << " [Norm: " << AVcookNorm.getcount() << ", Veg :" << AVcookVeg.getcount() << ", VIP : " << AVcookVip.getcount() << ", injured :" << injured_cooks << "]" << endl;
	OutFile << "Avg Wait = " << avg_time << ", " << " Avg Serv = " << srv_time << endl;
	OutFile << "Urgent orders :" << urgent_orders << "," << " Auto - promoted : " << auto_percentage << "%" << endl;
	OutFile.close();

}

void Restaurant::UrgentOrders(int time)
{
	Cook* pCok = nullptr;
	Order* pOrd = nullptr;

	while (VIPqueue.peekFront(pOrd) && (time - (pOrd->getArrTime()) >= VIP_WaitTime) && AVcookNorm.isEmpty() && AVcookVip.isEmpty() && AVcookVeg.isEmpty())
	{
		if (BreakNrmCokqueue.isEmpty() && BreakVegCokqueue.isEmpty() && BreakVipCokqueue.isEmpty() && hospital.isEmpty())
			break;



		VIPqueue.dequeue(pOrd);
		urgent_orders++;
		pOrd->setStatus(SRV);
		pOrd->setServTime(time);


		if (BreakVipCokqueue.peekFront(pCok))
		{
			BreakVipCokqueue.dequeue(pCok);

			pCok->setordersize(pOrd->getSize());
			pCok->setassignTS(time);
			pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));
			sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
			SERcookVip.enqueue(pCok, -pCok->getfinishTS());
			pCok->incrementordresprepared();
			pCok->assign(pOrd);

		}
		else if (BreakNrmCokqueue.peekFront(pCok))
		{
			BreakNrmCokqueue.dequeue(pCok);

			pCok->incrementordresprepared();
			pCok->setordersize(pOrd->getSize());
			pCok->setassignTS(time);
			pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));
			sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
			SERcookNorm.enqueue(pCok, -pCok->getfinishTS());
			pCok->assign(pOrd);

		}
		else if (BreakVegCokqueue.peekFront(pCok))
		{
			BreakVegCokqueue.dequeue(pCok);

			pCok->incrementordresprepared();
			pCok->setordersize(pOrd->getSize());
			pCok->setassignTS(time);
			pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));
			sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
			SERcookVeg.enqueue(pCok, -pCok->getfinishTS());
			pCok->assign(pOrd);


		}
		else if (hospital.peekFront(pCok))
		{
			hospital.dequeue(pCok);

			pCok->is_injured = false;
			pCok->setordersize(pOrd->getSize());
			pCok->setassignTS(time);
			pOrd->setFinishTime(time + ceil(double(pOrd->getSize()) / double(pCok->getspeed())));
			pOrd->update_finish_TS(time);
			sevVIPqueue.enqueue(pOrd, -pOrd->getFinishTime());
			pCok->update_finish_TS(time);
			pCok->SetOrdersPrepared(pCok->getorderstoprepare());
			pCok->assign(pOrd);

			if (pCok->GetType() == TYPE_NRM)
			{
				SERcookNorm.enqueue(pCok, -pCok->getfinishTS());

			}
			else if (pCok->GetType() == TYPE_VIP)
			{
				SERcookVip.enqueue(pCok, -pCok->getfinishTS());

			}
			else
			{
				SERcookVeg.enqueue(pCok, -pCok->getfinishTS());
			}

		}
		else
			break;

	}

}

