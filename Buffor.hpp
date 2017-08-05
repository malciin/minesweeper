#pragma once
#include <exception>
#include <stdexcept>
#include <memory>

template <typename Type>
class Buffor
{
	//std::unique_ptr<Type> buffor; WRONG (!) explanation: https://drive.google.com/file/d/0B84Fb42SRsObUksxSVZ4YmRDcWs/view?usp=drivesdk
	Type * buffor;				// main buffor table
	unsigned int indexIn;		// input index
	unsigned int indexOut;		// output index
	int queued;					// actually queued in buffor
	int maxSize;				// max buffor size
	bool overwriting;			// enable looping buffor (can lost data) - default false
public:
	Buffor(unsigned int size);	// default constructor

	Buffor(Buffor & copy);	// copy constructor that'll copy only queued items
							// for ex. we have in first buffor three items at pos 34, 35, 36 in table,
							// and this copy will deep copy only 34, 35, 36 items and copied buffor will have 
							// indexOut = 0, indexIn = 3

	Buffor(Buffor && asgn);	// assignment constructor

	~Buffor();				// default destructor

	void push(Type item);	// add item to buffor O(1)

	Type pop();				// remove item from buffor and return it O(1)

	// Inline methods
	inline void drop();		// erase whole buffor O(1)
	inline bool empty();	// true if buffor is empty

	inline void enableOverwritting();	// enable overwritting (usefull for example for storage last typed commands for command line like class
										// where you must remember only latest used commands and you could overwritting oldest command with the new one)

	inline void disableOverwritting();	// New buffor always have set overwritting flag to false
	inline int getSize() const;			// return size of buffor
	inline int getMaxSize() const;		// return max size

	Type & operator[](unsigned int Index) const;	
};

/*
	Methods implementations
*/

template <typename Type>
Buffor<Type>::Buffor(unsigned int size)
{
	buffor = new Type[size];
	maxSize = size;
	indexIn = 0;
	indexOut = 0;
	queued = 0;
	overwriting = false;
}

template <typename Type>
Buffor<Type>::Buffor(Buffor & copy)
{
	delete[] buffor;
	buffor = new Type[copy.maxSize];
	maxSize = copy.maxSize;
	queued = copy.queued;

	int i = 0;
	int copyIndex = copy.indexOut;
	for (; i < queued; i++, copyIndex++)
	{
		if (copyIndex == copy.maxSize)
			copyIndex = 0;
		buffor[i] = copy.buffor[copyIndex];
	}
	indexOut = 0;
	indexIn = queued;
	overwriting = copy.overwriting;
}

template <typename Type>
Buffor<Type>::Buffor(Buffor && asgn)
{
	if (buffor != nullptr)
		delete[] buffor;
	buffor = asgn;
	asgn.buffor = nullptr;
	maxSize = asgn.maxSize;
	indexIn = asgn.indexIn;
	indexOut = asgn.indexOut;
	queued = asgn.queued;
	overwriting = asgn.overwriting;
}

template <typename Type>
Buffor<Type>::~Buffor()
{
	delete[] buffor;
}

template <typename Type>
void Buffor<Type>::push(Type item)
{
	if (queued < maxSize || overwriting)
	{
		buffor[indexIn] = item;
		indexIn++;
		if (indexIn == maxSize)
			indexIn = 0;

		if (queued != maxSize)
			queued++;
	}
	else
		throw std::out_of_range("Queue full. Cannot add more elements!");
}

template <typename Type>
Type Buffor<Type>::pop()
{
	if (queued > 0)
	{
		Type * poped = &buffor[indexOut];
		indexOut++;
		if (indexOut == maxSize)
			indexOut = 0;
		queued--;
		return *poped;
	}
	else
		throw std::out_of_range("Queue empty. Cannot pop more elements!");
}

template <typename Type>
inline void Buffor<Type>::drop()
{
	indexIn = 0;
	indexOut = 0;
	queued = 0;
}

template <typename Type>
inline void Buffor<Type>::enableOverwritting()
{
	overwriting = true;
}
template <typename Type>
inline void Buffor<Type>::disableOverwritting()
{
	overwriting = false;
}

template <typename Type>
inline bool Buffor<Type>::empty()
{
	return queued > 0 ? false : true;
}

template <typename Type>
inline int Buffor<Type>::getSize() const
{
	return queued;
}

template <typename Type>
inline int Buffor<Type>::getMaxSize() const
{
	return maxSize;
}

template <typename Type>
Type & Buffor<Type>::operator[](unsigned int Index) const
{
	if (Index >= maxSize)
		throw std::out_of_range("Exceeded range");
	int index = Index + indexOut;
	if (index >= queued)
		throw std::out_of_range("Buffor doesn't have that much elements!");

	if (index >= maxSize)
		index -= maxSize;


	return buffor[index];
}