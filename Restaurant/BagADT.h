#pragma once

template<class T>
class BagADT {
public:
	virtual void Add(T Entry) = 0;
	virtual bool Remove(T Entry) = 0;
	virtual bool isEmoty(T Entry) = 0;
	virtual bool peek(T Entry) = 0;
};