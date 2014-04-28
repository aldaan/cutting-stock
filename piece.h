#ifndef STOCK_H
#define STOCK_H

#include <utility>
#include <vector>
#include <set>

class Piece
{
	public:
		Piece();
		Piece(int width, int length, int value, int minimum, int maximum);
		std::set<std::pair<int, int> > getCutPoints(std::pair<int, int> corner) const;
		void setWidth(int width);
		void setLength(int length);
		int getWidth() const;
		int getLength() const;
		void setMaximum(int maximum);
		void setMinimum(int minimum);
		void setValue(int value);
		int getMaximum() const;
		int getMinimum() const;
		int getValue() const;
		friend bool operator<(const Piece &lhs, const Piece &rhs);

	private:
		int width;
		int length;
		int value;
		int minimum;
		int maximum;
};

#endif /* STOCK_H */
