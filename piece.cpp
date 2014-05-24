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

bool Piece::isCutPoint(int r, int s, int p, int q) const
{
	return p <= r and r < p + length and q <= s and s < q + width;
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

bool compareWidth(const Piece &lhs, const Piece &rhs)
{
	return lhs.width < rhs.width;
}

bool compareLength(const Piece &lhs, const Piece &rhs)
{
	return lhs.length < rhs.length;
}