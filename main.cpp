#include <iostream>
#include <fstream>
#include <cmath>
#include "piece.h"

using namespace std;

void readInput(Piece& stock, vector<Piece>& patterns);
void writeOutput(Piece& stock, vector<Piece>& patterns);
void reductionOverlappingPieces(Piece& stock, vector<Piece>& patterns);
void reductionFreeArea(Piece& stock, vector<Piece>& patterns);
//double lagrangeanUpperBound(Piece& stock, vector<Piece>& patterns);
void generateLP(Piece& stock, vector<Piece>& patterns);

int main(int argc, char **argv)
{
	Piece stock;
	vector<Piece> patterns;

	readInput(stock, patterns);
	reductionOverlappingPieces(stock, patterns);
	reductionFreeArea(stock, patterns);
	writeOutput(stock, patterns);
	generateLP(stock, patterns);

	return 0;
}

void readInput(Piece& stock, vector<Piece>& patterns)
{

	ifstream input("Stock_input_data.txt");
	int w, l, p, min, max;

	input >> w >> l;
	stock.setWidth(w);
	stock.setLength(l);

	while (input >> w >> l >> p >> min >> max)
	{
		patterns.push_back(Piece(w, l, p, min, max));
	}

	input.close();
}

void writeOutput(Piece& stock, vector<Piece>& patterns)
{
	ofstream output("Stock_output.txt");

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		output << patterns.at(i).getWidth() << " ";
		output << patterns.at(i).getLength() << " ";
		output << patterns.at(i).getValue() << " ";
		output << patterns.at(i).getMinimum() << " ";
		output << patterns.at(i).getMaximum() << "\n";
	}

	output.close();
}

void reductionOverlappingPieces(Piece& stock, vector<Piece>& patterns)
{
	int L0 = stock.getLength();
	int W0 = stock.getWidth();

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		for (unsigned int j = 0; j < patterns.size(); ++j)
		{
			if (i != j)
			{
				int Li = patterns.at(i).getLength();
				int Lj = patterns.at(j).getLength();
				int Wi = patterns.at(i).getWidth();
				int Wj = patterns.at(j).getWidth();
				int Pj = patterns.at(j).getMinimum();
				int Qi = patterns.at(i).getMaximum();

				int reduce = floor(L0 / Li) * floor((W0 - ceil(Pj / floor(L0 / Lj)) * Wj) / Wi);
				int q = (Qi <= reduce ? Qi : reduce);

				patterns.at(i).setMaximum(q);
			}
		}
	}

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		for (unsigned int j = 0; j < patterns.size(); ++j)
		{
			if (i != j)
			{
				int Wi = patterns.at(i).getWidth();
				int Wj = patterns.at(j).getWidth();
				int Li = patterns.at(i).getLength();
				int Lj = patterns.at(j).getLength();
				int Pj = patterns.at(j).getMinimum();
				int Qi = patterns.at(i).getMaximum();

				int reduce = floor(W0 / Wi) * floor((L0 - ceil(Pj / floor(W0 / Wj)) * Lj) / Li);
				int q = (Qi <= reduce ? Qi : reduce);

				patterns.at(i).setMaximum(q);
			}
		}
	}
}

void reductionFreeArea(Piece& stock, vector<Piece>& patterns)
{
	int L0 = stock.getLength();
	int W0 = stock.getWidth();
	int area = 0;

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		area += patterns.at(i).getMinimum() * patterns.at(i).getLength() * patterns.at(i).getWidth();
	}

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		int Wi = patterns.at(i).getWidth();
		int Li = patterns.at(i).getLength();
		int Pi = patterns.at(i).getMinimum();
		int Qi = patterns.at(i).getMaximum();

		int reduce = Pi + floor((L0 * W0 - area)/(Li * Wi));
		int q = (Qi <= reduce ? Qi : reduce);

		patterns.at(i).setMaximum(q);
	}
}

/*double lagrangeanUpperBound(Piece& stock, vector<Piece>& patterns)
{
	int m = patterns.size();
	int L = stock.getLength();
	int W = stock.getWidth();

	vector<vector<vector<int> > > V(m, vector<vector<int> >(L, vector<int>(W)));

	for (int i = 0; i < m; ++i)
	{
		for (int p = 0; p < L; ++p)
		{
			for (int q = 0; q < W; ++q)
			{



				V.at(i).at(p).at(q) = ;
			}
		}
	}
}*/

void generateLP(Piece& stock, vector<Piece>& patterns)
{
	int m = patterns.size();
	int L = stock.getLength();
	int W = stock.getWidth();

	ofstream output("Stock_LP.lp");

	output << "Maximize" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (int p = 0; p < L; ++p)
		{
			for (int q = 0; q < W; ++q)
			{
				output << " + " << patterns.at(i).getValue() << " x" << i << "_" << p << "_" << q;
			}

			output << endl;
		}

		output << endl;
	}

	output << endl << endl << "Subject to" << endl;

	/*for (int i = 0; i < m; ++i)
	{
		for (int p = 0; p < L; ++p)
		{
			for (int q = 0; q < W; ++q)
			{
				for (int r = 0; r < L; ++r)
				{
					for (int s = 0; s < W; ++s)
					{

					}
				}
			}
		}
	}*/

	for (int i = 0; i < m; ++i)
	{
		output << patterns.at(i).getMinimum() << " <= ";

		for (int p = 0; p < L; ++p)
		{
			for (int q = 0; q < W; ++q)
			{
				output << " + x" << i << "_" << p << "_" << q;
			}

			output << endl;
		}

		output << " <= " << patterns.at(i).getMaximum() << endl;
	}

	output << endl << "Binaries" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (int p = 0; p < L; ++p)
		{
			for (int q = 0; q < W; ++q)
			{
				output << "x" << i << "_" << p << "_" << q << " ";
			}

			output << endl;
		}

		output << endl;
	}

	output << endl << "End" << endl;
}