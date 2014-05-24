#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <set>
#include "piece.h"

using namespace std;

void readInput(Piece& stock, vector<Piece>& patterns, string filename);
void writeOutput(Piece& stock, vector<Piece>& patterns, string filename);
void reductionOverlappingPieces(Piece& stock, vector<Piece>& patterns);
void reductionFreeArea(Piece& stock, vector<Piece>& patterns);
void generateNormalPatterns(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths);
void generateLP(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths, string filename);

int main(int argc, char* argv[])
{
	Piece stock;
	vector<Piece> patterns;
	set<int> lengths;
	set<int> widths;
	string file;

	if(0 == argc)
	{
		file = "Stock_input_data";
	}
	else
	{
		file = argv[1];
	}

	readInput(stock, patterns, file);
	reductionOverlappingPieces(stock, patterns);
	reductionFreeArea(stock, patterns);
	writeOutput(stock, patterns, file);
	generateNormalPatterns(stock, patterns, lengths, widths);
	generateLP(stock, patterns, lengths, widths, file);

	return 0;
}

void readInput(Piece& stock, vector<Piece>& patterns, string filename)
{
	ifstream input(filename + ".txt");
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

void writeOutput(Piece& stock, vector<Piece>& patterns, string filename)
{
	ofstream output(filename + "_modified.txt");

	output << stock.getWidth() << " " << stock.getLength() << endl;

	for (unsigned int i = 0; i < patterns.size(); ++i)
	{
		output << patterns.at(i).getWidth() << " ";
		output << patterns.at(i).getLength() << " ";
		output << patterns.at(i).getValue() << " ";
		output << patterns.at(i).getMinimum() << " ";
		output << patterns.at(i).getMaximum() << endl;
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
	int W0 = stock.getWidth();
	int L0 = stock.getLength();
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
			int Li = patterns.at(i).getLength();

			if (lengths.find(x - Li) != lengths.end())
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
			int Wi = patterns.at(i).getWidth();

			if (widths.find(x - Wi) != widths.end())
			{
				widths.insert(x);
				found = true;
			}

			++i;
		}
	}
}

void generateLP(Piece& stock, vector<Piece>& patterns, set<int>& lengths, set<int>& widths, string filename)
{
	int m = patterns.size();
	int L = stock.getLength();
	int W = stock.getWidth();

	ofstream output(filename + "_LP.lp");

	output << "Maximize" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				output << (0 == i and lengths.begin() == p and widths.begin() == q? "" : " + ") << patterns.at(i).getValue() << " x" << i << "_" << *p << "_" << *q;
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
						if (patterns.at(i).isCutPoint(*r, *s, *p, *q))
						{
							output << " + x" << i << "_" << *p << "_" << *q;
						}
					}
				}
				output << endl;
			}

			output << " <= 1" << endl;
		}
	}

	for (int i = 0; i < m; ++i)
	{
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

	output << endl << "Bounds" << endl;

	for (int i = 0; i < m; ++i)
	{
		for (set<int>::iterator p = lengths.begin(); p != lengths.end(); ++p)
		{
			for (set<int>::iterator q = widths.begin(); q != widths.end(); ++q)
			{
				if (*p + patterns.at(i).getLength() > L or *q + patterns.at(i).getWidth() > W)
				{
					output << "x" << i << "_" << *p << "_" << *q << " = 0" << endl;
				}
			}
		}
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