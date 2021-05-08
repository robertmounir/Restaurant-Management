#include "PromotionEvent.h"
#include"Event.h"
#include "../Rest/Restaurant.h"
PromotionEvent::PromotionEvent(int eTime, int oID, int exmoney) :Event(eTime, oID)
{
	if (exmoney >= 0)    ExMoney = exmoney;
	else ExMoney = 0;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	//This function should promote the normal order to be VIP order in 2 cases : the client paied extra money or normal order has been waiting more than  NP time steps

        	Order* pOrder;

		if (!pRest->peekfrontnormqueue(pOrder))  

			return;  // if empty waiting list => return

   		if ( ! pOrder->is_promoted) // to make sure that an order can not be promoted twice (because of autopromotion)
		{

			if (pOrder && pOrder->GetID() == OrderID && pOrder->GetType() == TYPE_NRM && pOrder->getStatus() == WAIT) // if only one waiting order in the list
			{
				pRest->RemoveFromnormqueue(pOrder);
				pRest->AddtoVIPqueue(pOrder);
				pOrder->is_promoted = true;
				pOrder->settype(TYPE_VIP);
				pOrder->settotalMoney(pOrder->gettotalmoney() + ExMoney);

				return;
			}

			pRest->RemoveFromnormqueue(pOrder);

			int fixed_id;
			int changed_id;
			if (pOrder)
			{
				 fixed_id = pOrder->GetID();
				 changed_id = pOrder->GetID();
			}
			
			do
			{
				if (changed_id != OrderID) // Check if the order's -that has the turn- id not equal ID of the promoted order then add it to the normal orders' queue again
				pRest->Addtonormalqueue(pOrder);

				pRest->peekfrontnormqueue(pOrder);
				if (pOrder->GetID() == fixed_id)
					return;
	            
				if ( pOrder->GetID() == OrderID) // Check if we have reached the required order 
				{
					pRest->RemoveFromnormqueue(pOrder); // Remove it from the normal orders' queue
					pRest->AddtoVIPqueue(pOrder); // Then add it to the VIP orders' queue
					pOrder->is_promoted = true;
					pOrder->settype(TYPE_VIP);
					pOrder->settotalMoney(pOrder->gettotalmoney() + ExMoney);
					
				
				}
				else  // If it's not the required order then remove it from the normal queue
				{
					pRest->RemoveFromnormqueue(pOrder);
				}
					
				changed_id = pOrder->GetID(); // Store in the changed id the value if the next order's id

			} while (fixed_id != changed_id);
			
			
	
		}	
}