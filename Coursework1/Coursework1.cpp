// Coursework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include "MultiLock.h"
#include <string>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include "Root.h"
#include "HashKey.h"

using namespace std;

const string key_file_name = "key.txt";
const string msf_file_name = "multi-safe file.txt";

/*LockDigit UnlockHashFunction(LockDigit ROOT, LockDigit UHF)
{
	return ROOT + UHF;
}

LockDigit LockHashFunction(LockDigit CN, LockDigit LHF)
{
	return CN + LHF;
}

LockDigit PassHashFunction(LockDigit LN, LockDigit PHF)
{
	return LN + PHF;
}
*/
Digits UnlockHashFunctionRoot(Root ROOT, HashKey UHF)
{
	return ROOT + UHF;
}

Digits UnlockHashFunction(Digits HN, HashKey UHF)
{
	return HN + UHF;
}

Digits LockHashFunction(Digits CN, HashKey LHF)
{
	return CN + LHF;
}

Digits PassHashFunction(Digits LN, HashKey PHF)
{
	return LN + PHF;
}

/*
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
}*/

//void Write5Locks(int i, Number UHF, Number LHF, Number PHF, ofstream& dataFile);
//void WriteReadLocks(bool write, int i, int& validSol, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile);
//void Read5Locks(int i, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile);
void CheckLocks(bool write, int i, int& validSol, vector<Root>& goodROOTS, vector<MultiLock>& locksHolder, Root& ROOT, HashKey UHF, HashKey LHF, HashKey PHF, ofstream& dataFile);

void ReadTextFile(ifstream& readFile, vector<Root>& ROOTS, HashKey UHF, HashKey LHF, HashKey PHF) throw (invalid_argument);

void ParseTextFile(string line, int w, int x, int y, int z, int& a, int& b, int& c, int& d);

bool SumLeftLess(MultiLock locks[5]);

void WriteFile(ofstream& dataFile, vector<Root> ROOTS, vector<MultiLock>& locksHolder, HashKey UHF, HashKey LHF, HashKey PHF, string fileName, bool keyFile);

int main()
{

	
	HashKey UHF, LHF, PHF;
	//HashKey UHF = HashKey(1, 1, 1, 1);
	//HashKey LHF = HashKey(1, 1, 1, 1);
	//HashKey PHF = HashKey(-1, -1, -1, -1);
	Root ROOT;
	//Root ROOT2;
	//Root ROOT3;
	//Root ROOT4;
	//Root ROOT5;

	//LockDigit UHF, LHF, PHF, ROOT;
	ofstream dataFile;
	ifstream readFile;

	cout << "How many valid solutions do you need? ";
	int validSolWanted;
	cin >> validSolWanted;
	cout << endl;
	int validSol = 0;

	clock_t start;
	start = clock();

	vector<Root> goodROOTS;
	vector<MultiLock> locks;

	int i = 0;
	while (validSol < validSolWanted)
	{
		CheckLocks(true, i, validSol, goodROOTS, locks, ROOT, UHF, LHF, PHF, dataFile);
		i += 1;
	}

	WriteFile(dataFile, goodROOTS, locks, UHF, LHF, PHF, key_file_name, true);

	cout << "Iterations:" << i << "  " << endl <<(clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	
	start = clock();
	
	vector<Root> ROOTS;
	try {
		ReadTextFile(readFile, ROOTS, UHF, LHF, PHF);
	}
	catch(const invalid_argument& ifn)
	{
		cout << "Unable to read data: " << ifn.what() << endl;
		exit(1);
	}

	for (int i = 0; i < (int)ROOTS.size(); i++)
	{
		try {
			CheckLocks(false, i, validSol, goodROOTS, locks, ROOTS[i], UHF, LHF, PHF, dataFile);
		}
		catch (const invalid_argument& ifn)
		{
			cout << "Unable to read data: " << ifn.what() << endl;
			exit(1);
		}

	}
	WriteFile(dataFile, ROOTS, locks, UHF, LHF, PHF, msf_file_name, false);
	
	cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	
	return 0;
    
}

/*void WriteReadLocks(bool write, int i, int& validSol, Number ROOT, Number UHF, Number LHF, Number PHF, ofstream& dataFile) throw (invalid_argument)
{

	srand(i * 2); //7 Generates valid CN. No srand() Generates non valid CN.
	//srand((time(NULL))*i); //7 Generates valid CN. No srand() Generates non valid CN.

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
		if (Validate(validCN, validLN, validHN))// && (SumLeftLess(locks)))
		//if (SumLeftLess(locks))
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
*/

void ReadTextFile(ifstream& readFile, vector<Root>& ROOTS, HashKey UHF, HashKey LHF, HashKey PHF) throw (invalid_argument)
{
	readFile.open(key_file_name);
	if (readFile.fail()) throw invalid_argument("no file exists" + key_file_name);
	if (readFile.is_open())
	{
		string line;
		int lineNumber = 0;
		int a, b, c, d;
		while (getline(readFile, line))
		{
			
			if (line.find("ROOT") != string::npos)
			{
				//ParseTextFile(line, 4, 5, 6, 7, a, b, c, d);
				ROOTS.push_back(Root(((int)line[5]-48), ((int)line[6] - 48), ((int)line[7] - 48), ((int)line[8] - 48)));

			}
			switch (lineNumber)
			{
			case 2:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				UHF = HashKey(a, b, c, d);
				break;
			case 3:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				LHF = HashKey(a, b, c, d);
				break;
			case 4:
				ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
				PHF = HashKey(a, b, c, d);
			}
			lineNumber += 1;

		}

	}
	readFile.close();
}

void ParseTextFile(string line, int w, int x,int y, int z, int& a, int& b, int& c, int& d)
{ 

	if (line[w] == '+') a = (int)(line[w + 1] - 48);
	else (int)((line[w + 1] - 48)*-1);

	if (line[x] == '+') a = (int)(line[x + 1] - 48);
	else (int)((line[x + 1] - 48)*-1);

	if (line[y] == '+') a = (int)(line[y + 1] - 48);
	else (int)((line[y + 1] - 48)*-1);

	if (line[z] == '+') a = (int)(line[z + 1] - 48);
	else (int)((line[z + 1] - 48)*-1);
	
}

void WriteFile(ofstream& dataFile, vector<Root> ROOTS, vector<MultiLock>& locksHolder, HashKey UHF, HashKey LHF, HashKey PHF, string fileName, bool keyFile)
{
	
	dataFile.open(fileName.c_str(), std::ios_base::app);
	dataFile.clear();
	if (dataFile.fail()) throw invalid_argument("no file exists" + key_file_name);

	if (keyFile)
	{
		dataFile << "NS " << ROOTS.size() << endl;
		for (int i = 0; i < (int)ROOTS.size(); i++) {

			dataFile << "ROOT " << ROOTS[i].GetA() << ROOTS[i].GetB() << ROOTS[i].GetC() << ROOTS[i].GetD() << endl;
			dataFile << "UHF " << UHF << endl;
			dataFile << "LHF " << LHF << endl;
			dataFile << "PHF " << PHF << endl;
		}
	}
	else
	{
		for (int i = 0; i < (int)locksHolder.size(); i++)
		{
			dataFile << "NS" << i << " VALID" << endl;
			for (int j = 0; j < 5; j++)
			{
				dataFile << "CN" << j << " " << locksHolder[i].GetCN(j).GetA() << locksHolder[i].GetCN(j).GetB() << locksHolder[i].GetCN(j).GetC() << locksHolder[i].GetCN(j).GetD() 
					<< ", LN" << j << " " << locksHolder[i].GetLN(j).GetA() << locksHolder[i].GetLN(j).GetB() << locksHolder[i].GetLN(j).GetC() << locksHolder[i].GetLN(j).GetD() 
					<< ", HN" << j << " " << locksHolder[i].GetHN(j).GetA() << locksHolder[i].GetHN(j).GetB() << locksHolder[i].GetHN(j).GetC() << locksHolder[i].GetHN(j).GetD() << endl;
			}
		}
	}
	dataFile.close();
	
}

void CheckLocks(bool key, int i, int& validSol, vector<Root>& goodROOTS, vector<MultiLock>& locksHolder, Root& ROOT, HashKey UHF, HashKey LHF, HashKey PHF, ofstream& dataFile)
{

	srand(i * 2);
	//srand((time(NULL))*i);

	if (key)
	{
		ROOT = Root();
	}

	MultiLock locks;
	
	locks.SetCN(UnlockHashFunctionRoot(ROOT, UHF), 0);
	locks.SetValidCN(locks.GetCN(0).ValidCN(), 0);

	locks.SetLN(LockHashFunction(locks.GetCN(0), LHF), 0);
	locks.SetValidLN(locks.GetLN(0) == locks.GetCN(0), 0);

	locks.SetHN(PassHashFunction(locks.GetLN(0), PHF), 0);
	locks.SetValidHN((locks.GetHN(0) == ROOT), 0);


	for (int i = 1; i < 5; i++)
	{
		
		locks.SetCN(UnlockHashFunction(locks.GetHN(i - 1), UHF), i);
		locks.SetValidCN(locks.GetCN(i).ValidCN(), i);

		locks.SetLN(LockHashFunction(locks.GetCN(i), LHF), i);
		locks.SetValidLN((locks.GetLN(i) == locks.GetCN(i)), i);

		locks.SetHN(PassHashFunction(locks.GetLN(i), PHF), i);
		locks.SetValidHN((locks.GetHN(i)) == (locks.GetHN(i-1)), i);

	}
	
	
	if (key)
	{
		if (i > 50000)
		{
			UHF = HashKey();
			LHF = HashKey();
			PHF = HashKey();
		}
		if (locks.Validate())// && (SumLeftLess(locks)))
		//if (SumLeftLess(locks))
		{
			validSol += 1;
			goodROOTS.push_back(ROOT);
		}
	}
	else
	{
		locksHolder.push_back(locks);
	}
}

