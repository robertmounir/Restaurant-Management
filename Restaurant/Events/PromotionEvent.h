#pragma once

#include "Event.h"
#include "../Rest/Restaurant.h"
class PromotionEvent :public Event
{
	double ExMoney;	//Total money is to be paied for promption
public:
	PromotionEvent(int eTime, int oID, int exmoney);
	virtual void Execute(Restaurant* pRest);	//override execute function
};