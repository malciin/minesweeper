#pragma once
#include <stdexcept>

class Tile
{
	// Value of title. Represent number of mines around
	// or -1, if that tile is a mine.
	int value;
	int status;
	int additional;
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
		WRONG_MARK = 1,
		CORRECT_MARK
	};

	bool isMine() const
	{
		return value == -1;
	}

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