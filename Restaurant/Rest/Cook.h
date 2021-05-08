#pragma once
#include"Order.h"
#include "..\Defs.h"
#include <math.h>

#pragma once
class Cook
{
	Order* ord;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;//dishes it can prepare in one clock tick (in one timestep)
	int BreakDuration;
	int orderstoprepare;// number of orders a cook must prepare to take a brake//
	//phase two first test//
	int assignTS;
	int finishTS;
	int ordersprepared;//nuber of dishes a cook has prepared //
	int ordersize;
	//upadated of phase two//
	int cookhealth;
	int restperiod;//time an injured cook should wait until he/she recovers,after this period the cook can join the availabe cook again//
	

public:
	
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE) ;
	void setspeed(int s);
	int getspeed()const;
	void setbreakduration(int );
	int getbreakduration();
	void setorderstoprepare(int dishes);
	int getorderstoprepare();
	//phase two first test//
	void setassignTS(int time);
	int getfinishTS();
	int getassignTS();
	void setordersize(int size);
	int getordersize();
	int getordersprepared() const;
	void incrementordresprepared();

	//mariam naser modification//
	void update_finish_TS(int time);

	void SetOrdersPrepared(int num);
	
	bool is_injured; 
	bool start_rest;

	void assign(Order* pOrd);
	Order* getOrder();
};
