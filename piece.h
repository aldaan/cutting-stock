#ifndef STOCK_H
#define STOCK_H

#include <utility>
#include <vector>

class Piece
{
	public:
		Piece();
		Piece(int width, int length, int value, int minimum, int maximum);
		bool isCutPoint(int r, int s, int p, int q) const;
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
		friend bool compareWidth(const Piece &lhs, const Piece &rhs);
		friend bool compareLength(const Piece &lhs, const Piece &rhs);

	private:
		int width;
		int length;
		int value;
		int minimum;
		int maximum;
};

bool compareWidth(const Piece &lhs, const Piece &rhs);
bool compareLength(const Piece &lhs, const Piece &rhs);

#endif /* STOCK_H */
