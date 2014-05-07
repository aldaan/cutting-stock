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

vector<vector<int> > Piece::getCutPoints(int p, int q, int L, int W) const
{
	vector<vector<int> > points(L, vector<int>(W, 0));

	for (int i = p; i < p + length; ++i)
	{
		for (int j = q; j < q + width; ++j)
		{
			if (i < L and j < W)
			{
				points.at(i).at(j) = 1;
			}
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

bool compareWidth(const Piece &lhs, const Piece &rhs)
{
	return lhs.width < rhs.width;
}

bool compareLength(const Piece &lhs, const Piece &rhs)
{
	return lhs.length < rhs.length;
}