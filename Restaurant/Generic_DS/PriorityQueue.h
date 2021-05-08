#pragma once
#include <iostream>
using namespace std;
#include "Node.h"
template <class T>
class PriorityQueue
{
private:
	int count;
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	void enqueue(const T& newEntry, int p);
	void dequeue(T& frntEntry);
	void print();
	T* toArray(int& count);
	int getcount()const;
	~PriorityQueue();
	bool peekFront(T& frntEntry)  const;
};


template <typename T>
T* PriorityQueue<T>::toArray(int& count)
{
	count = 0;

	if (!frontPtr)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}
template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	count = 0;

}

template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template <typename T>
void PriorityQueue<T>::enqueue(const T& newEntry, int p)
{
	count++;
	Node<T>* newNodePtr = new Node<T>;
	newNodePtr->setItem(newEntry);
	newNodePtr->setpriority(p);
	if (isEmpty())
	{
		frontPtr = backPtr = newNodePtr;
		backPtr->setNext(nullptr);
	}
	else if (frontPtr->getpriority() < p)
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
	}
	else if (frontPtr->getNext() == nullptr)
	{
		frontPtr->setNext(newNodePtr);
		newNodePtr->setNext(nullptr);
		return;
	}
	else
	{
		Node <T>* prev = frontPtr;
		Node <T>* curr = frontPtr->getNext();
		while (curr)
		{
			if (curr->getpriority() < p)
			{
				prev->setNext(newNodePtr);
				newNodePtr->setNext(curr);
				return;
			}
			else
			{
				prev = curr;
				curr = curr->getNext();
			}
			if (prev && prev->getNext() == nullptr)
			{
				prev->setNext(newNodePtr);
				newNodePtr->setNext(nullptr);
				return;
			}
		}

	}
}


template <typename T>
void PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	if (nodeToDeletePtr == backPtr)
		backPtr = nullptr;
	delete nodeToDeletePtr;
	count--;
}
template <typename T>
void PriorityQueue<T>::print()
{
	Node<T>* curr = frontPtr;
	while (curr)
	{
		cout << curr->getItem() << " ";
		curr = curr->getNext();
	}
	cout << endl;
}

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
}

template <typename T>
int PriorityQueue<T>::getcount()const
{
	return count;
}

template<class T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty()) {
		return false;
	}
	frntEntry = frontPtr->getItem();
	return true;
}

