#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done

	int Size;//updated //
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	//update of phase two//
	int VIP_WT;//if a vip orde waited more than this time it is considered urgent order and must be surved even with busy or at-rest cooks//
	
	
	

public:
	Order(int ID, ORD_TYPE r_Type);
	virtual ~Order();

	int GetID();

	void settype(ORD_TYPE);
	ORD_TYPE GetType() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	
	//
	// TODO: Add More Member Functions As Needed
	//
		//marim
	double gettotalmoney();


	void settotalMoney(double oMony);

	//getters and setters//



	void setSize(int oSize);
	int getSize();

	void setArrTime(int );
	int getArrTime();

	void setFinishTime(int );
	int getFinishTime();
	
	void setServTime(int);
	int getSevTime() const;
	//updated phase two//
	void setVIP_WT(int vip_wt);
	int getVIP_WT();

	//mariam naser//
	void update_finish_TS(int time);
	bool is_promoted;
	int priorityofviporder();
};

#endif