#pragma once
#include "..\Restaurant\Generic_DS\BagADT.h"
#include "..\Restaurant\Generic_DS\Node.h"


template<class T>
class LinkedBag :public BagADT<T> {
	Node<T>* Head;
	Node<T>* prev;
	Node<T>* Back;
public:
	LinkedBag() {
		Head = Back = prev = nullptr;
	}


	void Add(T Entry) {
		if (Head == nullptr) {
			Head = new Node<T>(Entry);
			Back = Head;
			return;
		}
		Node<T>* Temp = new Node<T>(Entry);
		Back->setNext(Temp);
		prev = Back;
		Back = Temp;
	}


	void Remove(T Entry) {

	}
};