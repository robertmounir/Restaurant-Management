#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"
#include "..\Generic_DS\Queue.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double oMoney,int oSize,int Vip_wt):Event(eTime,oID)
{
	OrdType = oType;
	OrdMoney = oMoney;
	OrdSize = oSize;
	VIP_WT = Vip_wt;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1

	//bishoy//
	Order* porder = new Order(OrderID, OrdType);
	porder->settotalMoney(OrdMoney);
	porder->setSize(OrdSize);
	porder->setArrTime(EventTime);
	porder->setStatus(WAIT);
	porder->settype(OrdType);
	porder->setVIP_WT(VIP_WT);

	if (OrdType == 0)
	{
		pRest->Addtonormalqueue(porder);
		pRest->original_normal_orders++;
	}
	else if (OrdType == 1)
		pRest->Addtoveganqueue(porder);
	else
		pRest->AddtoVIPqueue(porder);

	pRest->AddToordsqueueu(porder);
	
	
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2

	//Order* pOrd = new Order(OrderID,OrdType);
	//pRest->AddtoDemoQueue(pOrd);
}
