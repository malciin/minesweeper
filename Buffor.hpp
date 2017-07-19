#pragma once
#include <exception>
#include <stdexcept>
#include <memory>
template <typename Type>
class Buffor
{
	//std::unique_ptr<Type> buffor; (!) WRONG: http://i.imgur.com/8SowNdB.png
	Type * buffor;				// main buffor tab
	unsigned int indexIn;		// input index
	unsigned int indexOut;		// output index
	int queued;					// actually queued in buffor
	int maxSize;				// max buffor size
	bool overwriting;			// enable looping buffor (can lost data)
public:
	inline void enableOverwritting()
	{
		overwriting = true;
	}
	inline void disableOverwritting()
	{
		overwriting = false;
	}
	// default constructor
	Buffor(unsigned int size)	
	{
		buffor = new Type[size];
		maxSize = size;
		indexIn = 0;
		indexOut = 0;
		queued = 0;
	}

	// copy constructor that'll copy only queued items
	// for ex. we have in buffor one three items at pos 34, 35, 36
	// and this copy assgn will deep copy only 34, 35, 36 items and copied buffor will have 
	// indexOut = 0, indexIn = 3
	Buffor(Buffor & copy)
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
	}

	// assignment constructor
	Buffor(Buffor && asgn)
	{
		buffor = asgn;
		asgn.buffor = nullptr;
		maxSize = asgn.maxSize;
		indexIn = asgn.indexIn;
		indexOut = asgn.indexOut;
		queued = asgn.queued;
	}

	// default destructor
	~Buffor()
	{
	}

	// add item to buffor O(1)
	void push(Type item)
	{
		if (queued < maxSize || overwriting)
		{
			buffor[indexIn] = item;
			indexIn++;
			if (indexIn == maxSize)
				indexIn = 0;

			if (queued!=maxSize)
				queued++;
		}
		else
			throw std::out_of_range("Queue full. Cannot add more elements!");
	}

	// remove item from buffor and return it O(1)
	Type pop()
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

	// remove whole buffor O(1)
	inline void drop()
	{
		indexIn = 0;
		indexOut = 0;
		queued = 0;
	}

	// return true if empty
	inline bool empty()
	{
		return queued > 0 ? false : true;
	}

	inline int getSize() const
	{
		return queued;
	}

	inline int getMaxSize() const
	{
		return maxSize;
	}

	// return nth element O(1)

	Type & operator[](unsigned int Index) const
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
	
};