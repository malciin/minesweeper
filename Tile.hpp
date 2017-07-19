#pragma once
#include <stdexcept>
#include <iostream>
class Tile
{
	
	int value;			// Value of tile. Represent number of mines around
						// or -1, if that tile is a mine.
	int status;			// Status of tile. Represent if a tile is hidden, marked or revealed
	int additional;		// Addtional status of tile. Represent if marked is correct or wrong 
						// (obviously if tile is marked)
public:
	Tile()
	{
		value = 0;
		status = 0;
		additional = 0;
	}
	enum Value
	{
		BORDER = -2,
		MINE = -1
	};
	enum Status
	{
		HIDDEN = 1,
		REVEALED,
		MARKED
	};
	enum MarkStatus
	{
		NOTHING,
		WRONG_MARK,
		CORRECT_MARK
	};

	void setValue(int value)
	{
		if (value < -2 || value > 8)
			throw std::invalid_argument("Value must be from range [-2; 8]");
		this->value = value;
	}
	void setStatus(Status status)
	{
		this->status = status;
	}
	void setMarkStatus(MarkStatus status)
	{
		additional = status;
	}

	Status getStatus() const
	{
		return static_cast<Status>(status);
	}
	MarkStatus getMarkStatus() const
	{
		return static_cast<MarkStatus>(additional);
	}
	int getValue()
	{
		return value;
	}

	operator int()
	{
		return value;
	}
};