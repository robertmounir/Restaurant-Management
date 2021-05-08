#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	is_promoted = false;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


void Order::settype(ORD_TYPE oType)
{
	type = oType;
}

ORD_TYPE Order::GetType() const
{
	return type;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::settotalMoney(double oMony)
{
	totalMoney = oMony;
}

double Order::gettotalmoney()
{
	return totalMoney;
}

void Order::setSize(int oSize)
{
	Size = oSize;
}

int Order::getSize()
{
	return Size;
}

void Order::setArrTime(int time) {
	ArrTime = time;
}

int Order::getArrTime()
{
	return ArrTime;
}

void Order::setFinishTime(int time)
{
	FinishTime = time;
}

int Order::getFinishTime()
{
	return FinishTime;
}

void Order::setServTime(int time) {
	ServTime = time;
}

int Order::getSevTime() const
{
	return ServTime;
}

//updated phase two//
void Order::setVIP_WT(int vip_wt)
{
	VIP_WT = vip_wt;
}

int Order::getVIP_WT()
{
	return VIP_WT;
}

void Order::update_finish_TS(int time)
{
	this->FinishTime = 2 * FinishTime - time;
}

int Order::priorityofviporder()
{
	return (5 * totalMoney + 3 * ArrTime - 4 * Size);
}
