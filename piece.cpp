#include "piece.h"

using namespace std;

Piece::Piece()
{

}

Piece::Piece(int width, int length, int value, int minimum, int maximum)
{
	this->width = width;
	this->length = length;
	this->value = value;
	this->minimum = minimum;
	this->maximum = maximum;
}

set<pair<int,int> > Piece::getCutPoints(std::pair<int,int> corner) const
{
	set<pair<int,int> > points;

	for (int i = get<0>(corner); i < get<0>(corner) + width; ++i)
	{
		for (int j = get<1>(corner); j < get<1>(corner) + length; ++j)
		{
			points.insert(pair<int,int>(i, j));
		}
	}

	return points;
}

void Piece::setWidth(int width)
{
	this->width = width;
}

void Piece::setLength(int length)
{
	this->length = length;
}

int Piece::getWidth() const
{
	return width;
}

int Piece::getLength() const
{
	return length;
}

void Piece::setMaximum(int maximum)
{
	this->maximum = maximum;
}

void Piece::setMinimum(int minimum)
{
	this->minimum = minimum;
}

void Piece::setValue(int value)
{
	this->value = value;
}

int Piece::getMaximum() const
{
	return maximum;
}

int Piece::getMinimum() const
{
	return minimum;
}

int Piece::getValue() const
{
	return value;
}

bool operator<(const Piece &lhs, const Piece &rhs)
{
	return lhs.width < rhs.width;
}