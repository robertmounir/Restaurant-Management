#include "Cook.h"



Cook::Cook()
{
	ordersprepared = 0;
	assignTS = -1;
	finishTS = -1;
	ordersize = 0;
	is_injured = false;
	start_rest = true;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setspeed(int s)
{
	speed = s;
}

int Cook::getspeed() const
{
	return speed;
}

void Cook::setbreakduration(int Break)
{
	BreakDuration = Break;
}

int Cook::getbreakduration()
{
	return BreakDuration;
}

void Cook::setorderstoprepare(int orders)
{
	orderstoprepare = orders;
}

int Cook::getorderstoprepare()
{
	return orderstoprepare;
}

//phase two first test//
void Cook::setassignTS(int time)
{
	assignTS = time;
	finishTS = time + ceil(double(ordersize) / double(speed));
}

int Cook::getfinishTS() {
	return finishTS;
}

int Cook::getassignTS()
{
	return assignTS;
}
void Cook::SetOrdersPrepared(int num)
{
	ordersprepared = num;
}

void Cook::setordersize(int size)
{
	ordersize = size;
}

int Cook::getordersize()
{
	return ordersize;
}

int Cook::getordersprepared() const
{
	return ordersprepared;
}

void Cook::incrementordresprepared()
{
	ordersprepared++;
}

void Cook::update_finish_TS(int time)
{
	finishTS = 2 * finishTS - time;
}

void Cook::assign(Order* pOrd)
{
	ord = pOrd;
}

Order* Cook::getOrder()
{
	return ord;
}






