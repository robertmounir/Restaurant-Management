#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int OrdSize;//updated//
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	//updated phase two//
	int VIP_WT;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType);
	//Add more constructors if needed
	ArrivalEvent(int eTime,int oID,ORD_TYPE oType,double oMoney,int oSize,int Vip_wt);
	
	virtual void Execute(Restaurant *pRest);	//override execute function

};

#endif