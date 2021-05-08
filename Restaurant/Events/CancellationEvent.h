#pragma once
#include "..\Rest\Restaurant.h"
#include "..\Generic_DS\Queue.h"
#include "Event.h"
class CancellationEvent : public Event
{
public:

	CancellationEvent(int eTime, int oID);
	void Execute(Restaurant* pRest);
};

