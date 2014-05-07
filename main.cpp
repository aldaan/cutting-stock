#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <set>
#include "piece.h"

using namespace std;

void readInput(Piece& stock, vector<Piece>& patterns, string file);
void writeOutput(Piece& stock, vector<Piece>& patterns);
void reductionOverlappingPieces(Piece& stock, vector<Piece>& patterns);
void reductionFreeArea(Piece& stock, vector<Piece>& patterns);
void generateNormalPatterns(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths);
void generateLP(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths);

int main(int argc, char* argv[])
{
	Piece stock;
	vector<Piece> patterns;
	set<int> lengths;
	set<int> widths;

	readInput(stock, patterns, argv[1]);
	reductionOverlappingPieces(stock, patterns);
	reductionFreeArea(stock, patterns);
	writeOutput(stock, patterns);
	generateNormalPatterns(stock, patterns, lengths, widths);
	generateLP(stock, patterns, lengths, widths);

	return 0;
}

void readInput(Piece& stock, vector<Piece>& patterns, string file)
{
	ifstream input;

	if (not file.empty())
	{
		input.open(file);
	}
	else
	{
		input.open("Stock_input_data.txt");
	}

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

void generateNormalPatterns(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths)
{
	int m = patterns.size();
	int L0 = stock.getLength();
	int W0 = stock.getWidth();
	Piece minL = *min_element(patterns.begin(), patterns.end(), compareLength);
	Piece minW = *min_element(patterns.begin(), patterns.end(), compareWidth);

	lengths.insert(0);
	widths.insert(0);

	for (int x = 0; x <= L0 - minL.getLength(); ++x)
	{
		int i = 0;
		bool found = false;

		while (i < m and not found)
		{
			if (lengths.find(x - patterns.at(i).getLength()) != lengths.end())
			{
				lengths.insert(x);
				found = true;
			}

			++i;
		}
	}

	for (int x = 0; x <= W0 - minW.getWidth(); ++x)
	{
		int i = 0;
		bool found = false;

		while (i < m and not found)
		{
			if (widths.find(x - patterns.at(i).getWidth()) != widths.end())
			{
				widths.insert(x);
				found = true;
			}

			++i;
		}
	}
}

void generateLP(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths)
{
	int m = patterns.size();
	int L = stock.getLength();
	int W = stock.getWidth();

	ofstream output("Stock_LP.lp");

	output << "Maximize" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				output << " + " << patterns.at(i).getValue() << " x" << i << "_" << *p << "_" << *q;
			}

			output << endl;
		}

		output << endl;
	}

	output << "Subject to" << endl;

	for (set<int>::iterator r = lengths.begin(); r != lengths.end(); ++r)
	{
		for (set<int>::iterator s = widths.begin(); s != widths.end(); ++s)
		{
			for (int i = 0; i < m; ++i)
			{
				for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
				{
					for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
					{
						if (0 < patterns.at(i).getCutPoints(*p, *q, L, W).at(*r).at(*s))
						{
							output << " + x" << i << "_" << *p << "_" << *q;
						}
					}
				}
			}

			output << " <= 1" << endl;;
		}
	}

	for (int i = 0; i < m; ++i)
	{
		output << endl;

		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				output << " + x" << i << "_" << *p << "_" << *q;
			}

			output << endl;
		}

		output << " >= " << patterns.at(i).getMinimum() << endl << endl;

		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				output << " + x" << i << "_" << *p << "_" << *q;
			}

			output << endl;
		}

		output << " <= " << patterns.at(i).getMaximum() << endl;
	}

	output << endl << "Binaries" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				output << "x" << i << "_" << *p << "_" << *q << " ";
			}

			output << endl;
		}

		output << endl;
	}

	output << "End" << endl;
	output.close();
}