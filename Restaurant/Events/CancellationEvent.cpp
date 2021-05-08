#include "CancellationEvent.h"

CancellationEvent::CancellationEvent(int eTime, int oID):Event(eTime,oID)
{
}

void CancellationEvent::Execute(Restaurant* pRest)
{
	
	Order* op;
	int id, firstid;

	pRest->peekfrontorderqueue(op);

	if (op->GetID() == OrderID && op->GetType()==TYPE_NRM && op->getStatus()==WAIT)
	{
		pRest->RemoveFromordqueue(op);
		pRest->original_normal_orders--;
		
	}
	else
	{
		pRest->RemoveFromordqueue(op);

		firstid = op->GetID();
		id = op->GetID();

		do
		{
			if (id != OrderID || op->GetType() != TYPE_NRM || op->getStatus() != WAIT)
			{
				pRest->AddToordsqueueu(op);
			}
			else
			{
				pRest->original_normal_orders--;
		    }
			pRest->peekfrontorderqueue(op);
			if (op->GetID() == firstid)
				break;
			pRest->RemoveFromordqueue(op);
			id = op->GetID();
		} while (firstid != id);

	}

	pRest->peekfrontnormqueue(op);

	if (op->GetID() == OrderID)
	{
		pRest->RemoveFromnormqueue(op);

	}
	else
	{

		Order* opp;
		int idd, firstidd;

		if (!pRest->RemoveFromnormqueue(opp))
			return;

		firstidd = opp->GetID();
		idd = opp->GetID();
		do
		{
			if (idd != OrderID)
			{
				pRest->Addtonormalqueue(opp);
			}
			pRest->peekfrontnormqueue(opp);
			if (opp->GetID() == firstidd)
				break;
			pRest->RemoveFromnormqueue(opp);
			idd = opp->GetID();
		} while (firstidd != idd);
	}


	

}
