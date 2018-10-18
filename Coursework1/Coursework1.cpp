// Coursework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include "Lock.h"
#include <string>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <stdlib.h>

using namespace std;

const string key_file_name = "key.txt";
const string msf_file_name = "multi-safe file.txt";

Number UnlockHashFunction(Number ROOT, Number UHF)
{
	return ROOT + UHF;
}

Number LockHashFunction(Number CN, Number LHF)
{
	return CN + LHF;
}

Number PassHashFunction(Number LN, Number PHF)
{
	return LN + PHF;
}


bool Validate(bool CN[5], bool LN[5], bool HN[5])
{
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		if (!CN[i]) count += 1;
		if (!LN[i]) count += 1;
		if (!HN[i]) count += 1;
	}
	if (count > 0) return false;
	else return true;
}

//void Write5Locks(int i, Number UHF, Number LHF, Number PHF, ofstream& dataFile);
void WriteReadLocks(bool write, int i, int& validSol, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile);

//void Read5Locks(int i, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile);

void ReadTextFile(ifstream& readFile, vector<Number>& ROOTS, Number UHF, Number LHF, Number PHF);

void ParseTextFile(string line, int w, int x, int y, int z, int& a, int& b, int& c, int& d);

int main()
{

	Number UHF, LHF, PHF, ROOT;
	ofstream dataFile;
	ifstream readFile;

	cout << "How many valid solutions do you need? ";
	int validSolWanted;
	cin >> validSolWanted;
	cout << endl;
	int validSol = 0;

	auto start = chrono::steady_clock::now();

	dataFile.open(key_file_name.c_str(), std::ios_base::app);
	dataFile << "NS             " << endl;
	dataFile.close();

	int i = 0;
	while (validSol < validSolWanted)
	{
		try {
			WriteReadLocks(true, i, validSol, ROOT, UHF, LHF, PHF, dataFile);
		}
		catch (const invalid_argument& ifn)
		{
			cout << "Unable to read data: " << ifn.what() << endl;
			exit(1);
		}
		i += 1;
	}

	dataFile.open(key_file_name.c_str(), std::ios_base::in);
	dataFile << "NS " << validSol;
	dataFile.close();
	auto end = chrono::steady_clock::now();

	cout << "Iterations:" << i << "  " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
	
	start = chrono::steady_clock::now();
	
	vector<Number> ROOTS;
	ReadTextFile(readFile, ROOTS, UHF, LHF, PHF);

	for (int i = 0; i < ROOTS.size(); i++)
	{
		try {
			WriteReadLocks(false, i, validSol, ROOTS[i], UHF, LHF, PHF, dataFile);
		}
		catch (const invalid_argument& ifn)
		{
			cout << "Unable to read data: " << ifn.what() << endl;
			exit(1);
		}
		//Read5Locks(i, ROOTS[i], UHF, LHF, PHF, dataFile);
	}
	
	end = chrono::steady_clock::now();

	cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
	

	return 0;
    
}

void WriteReadLocks(bool write, int i, int& validSol, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile) throw (invalid_argument)
{

	//srand(i * 2); //7 Generates valid CN. No srand() Generates non valid CN.
	srand(time(NULL)*i); //7 Generates valid CN. No srand() Generates non valid CN.

	if (write)
	{
		ROOT = Number((rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0));
	}

	Lock locks[5];

	bool validCN[5];
	bool validLN[5];
	bool validHN[5];

	//cout << "Iteration #" << i << endl;

	//cout << "******* Lock 0 *******" << endl;
	locks[0].SetCN(UnlockHashFunction(ROOT, UHF));
	validCN[0] = locks[0].GetCN().ValidCN();
	//cout << (validCN ? "CN is valid. " : "CN is not valid. ");

	locks[0].SetLN(LockHashFunction(locks[0].GetCN(), LHF));
	validLN[0] = (locks[0].GetLN() == locks[0].GetCN());
	//cout << (validLN ? "LN is valid. " : "LN is not valid. ");

	locks[0].SetHN(PassHashFunction(locks[0].GetLN(), PHF));
	validHN[0] = (locks[0].GetHN() == ROOT);
	//cout << (validHN ? "HN is valid. " : "HN is not valid. ") << endl << endl;

	for (int i = 1; i < 5; i++)
	{
		//cout << "******* Lock " << i << " *******" << endl;
		locks[i].SetCN(UnlockHashFunction(locks[i - 1].GetHN(), UHF));
		validCN[i] = locks[i].GetCN().ValidCN();
		//cout << (validCN ? "CN is valid. " : "CN is not valid. ");

		locks[i].SetLN(LockHashFunction(locks[i].GetCN(), LHF));
		validLN[i] = (locks[i].GetLN() == locks[i].GetCN());
		//cout << (validLN ? "LN is valid. " : "LN is not valid. ");

		locks[i].SetHN(PassHashFunction(locks[i].GetLN(), PHF));
		validHN[i] = (locks[i].GetHN() == locks[i - 1].GetHN());
		//cout << (validHN ? "HN is valid. " : "HN is not valid. ") << endl << endl;


	}
	//bool valid = Validate(validCN, validLN, validHN);
	if (write)
	{
		if (Validate(validCN, validLN, validHN))
		{
			dataFile.open(key_file_name.c_str(), std::ios_base::app);
			if (dataFile.fail()) throw invalid_argument("no file exists" + key_file_name);
			dataFile << "ROOT " << ROOT << endl;
			dataFile << "UHF " << UHF << endl;
			dataFile << "LHF " << LHF << endl;
			dataFile << "PHF " << PHF << endl;
			
			dataFile.close();
			validSol += 1;
		}
	}
	else 
	{
		dataFile.open(msf_file_name.c_str(), std::ios_base::app);
		if (dataFile.fail()) throw invalid_argument("no file exists" + key_file_name);
		dataFile << "NS" << i << ((Validate(validCN, validLN, validHN)) ? " VALID" : " NOT VALID") << endl;

		for (int i = 0; i < 5; i++)
		{
			dataFile << "CN" << i << " " << locks[i].GetCN().GetA() << locks[i].GetCN().GetB() << locks[i].GetCN().GetC() << locks[i].GetCN().GetD() << ", LN" << i << " " << locks[i].GetLN().GetA() << locks[i].GetLN().GetB() << locks[i].GetLN().GetC() << locks[i].GetLN().GetD() << ", HN" << i << " " << locks[i].GetHN().GetA() << locks[i].GetHN().GetB() << locks[i].GetHN().GetC() << locks[i].GetHN().GetD() << endl;
		}
		dataFile.close();
	}
}


void ReadTextFile(ifstream& readFile, vector<Number>& ROOTS, Number UHF, Number LHF, Number PHF)
{
	readFile.open(key_file_name);
	if (readFile.is_open())
	{
		string line;
		int lineNumber = 0;
		int a, b, c, d;
		while (getline(readFile, line))
		{
			lineNumber += 1;
			if (line.find("ROOT") != string::npos)
			{
				ParseTextFile(line, 5, 8, 11, 14, a, b, c, d);
				ROOTS.push_back(Number(a, b, c, d));

			}
			switch (lineNumber)
			{
			case 2:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				UHF = Number(a, b, c, d);
				break;
			case 3:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				LHF = Number(a, b, c, d);
				break;
			case 4:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				PHF = Number(a, b, c, d);
			}


		}

	}
	readFile.close();
}

void ParseTextFile(string line, int w, int x,int y, int z, int& a, int& b, int& c, int& d)
{
	switch (line[w])
	{
	case '+': a = (int)line[w+1] - 48; break;
	case '-': a = (int)(line[w+1] - 48)*-1; break;
	}
	switch (line[x])
	{
	case '+': b = (int)line[x+1] - 48; break;
	case '-': b = (int)(line[x+1] - 48)*-1; break;
	}
	switch (line[y])
	{
	case '+': c = (int)line[y+1] - 48; break;
	case '-': c = (int)(line[y+1] - 48)*-1; break;
	}
	switch (line[z])
	{
	case '+': d = (int)line[z+1] - 48; break;
	case '-': d = (int)(line[z+1] - 48)*-1; break;
	}
}


/*void Read5Locks(int i, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile)
{
	//srand(i * 2); //7 Generates valid CN. No srand() Generates non valid CN.

	//Number ROOT = Number((rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0));

	Lock locks[5];

	bool validCN[5];
	bool validLN[5];
	bool validHN[5];

	//cout << "Iteration #" << i << endl;

	//cout << "******* Lock 0 *******" << endl;
	locks[0].SetCN(UnlockHashFunction(ROOT, UHF));
	validCN[0] = locks[0].GetCN().ValidCN();
	//cout << (validCN ? "CN is valid. " : "CN is not valid. ");

	locks[0].SetLN(LockHashFunction(locks[0].GetCN(), LHF));
	validLN[0] = (locks[0].GetLN() == locks[0].GetCN());
	//cout << (validLN ? "LN is valid. " : "LN is not valid. ");

	locks[0].SetHN(PassHashFunction(locks[0].GetLN(), PHF));
	validHN[0] = (locks[0].GetHN() == ROOT);
	//cout << (validHN ? "HN is valid. " : "HN is not valid. ") << endl << endl;

	for (int i = 1; i < 5; i++)
	{
		//cout << "******* Lock " << i << " *******" << endl;
		locks[i].SetCN(UnlockHashFunction(locks[i - 1].GetHN(), UHF));
		validCN[i] = locks[i].GetCN().ValidCN();
		//cout << (validCN ? "CN is valid. " : "CN is not valid. ");

		locks[i].SetLN(LockHashFunction(locks[i].GetCN(), LHF));
		validLN[i] = (locks[i].GetLN() == locks[i].GetCN());
		//cout << (validLN ? "LN is valid. " : "LN is not valid. ");

		locks[i].SetHN(PassHashFunction(locks[i].GetLN(), PHF));
		validHN[i] = (locks[i].GetHN() == locks[i - 1].GetHN());
		//cout << (validHN ? "HN is valid. " : "HN is not valid. ") << endl << endl;


	}
	//bool valid = Validate(validCN, validLN, validHN);
	dataFile.open(msf_file_name.c_str(), std::ios_base::app);
	dataFile << "NS" << i << ((Validate(validCN, validLN, validHN)) ? " VALID" : " NOT VALID") << endl;

	for (int i = 0; i < 5; i++)
	{
		dataFile << "CN" << i << " " << locks[i].GetCN().GetA() << locks[i].GetCN().GetB() << locks[i].GetCN().GetC() << locks[i].GetCN().GetD() << ", LN" << i << " " << locks[i].GetLN().GetA() << locks[i].GetLN().GetB() << locks[i].GetLN().GetC() << locks[i].GetLN().GetD() << ", HN" << i << " " << locks[i].GetHN().GetA() << locks[i].GetHN().GetB() << locks[i].GetHN().GetC() << locks[i].GetHN().GetD() << endl;
	}
	dataFile.close();
	//cout << (valid ? "VALID!!" : "Not valid") << endl;
}*/