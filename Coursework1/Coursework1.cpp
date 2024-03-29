// Coursework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

#include "MultiLock.h"
#include "Set.h"

using namespace std;

const string key_file_name = "key.txt";
const string msf_file_name = "multi-safe file.txt";


Digits<int> UnlockHashFunctionRoot(Root ROOT, HashKey UHF) {return ROOT + UHF;}

Digits<int> UnlockHashFunction(Digits<int> HN, HashKey UHF) {return HN + UHF;}

Digits<int>  LockHashFunction(Digits<int> CN, HashKey LHF) {return CN + LHF;}

Digits<int> PassHashFunction(Digits<int> LN, HashKey PHF) {return LN + PHF;}

void CheckLocksMultiSafeFile(int i, vector<Set>& foundROOTS, vector<MultiLock>& locksHolder);

void CheckLocksKeyFile(int i, int& validSol, vector<Set>& goodROOTS, HashKey& UHF, HashKey& LHF, HashKey& PHF);

void ReadTextFile(ifstream& readFile, vector<Set>& findROOTS) throw (invalid_argument);

void ParseTextFile(string line, int w, int x, int y, int z, int& a, int& b, int& c, int& d);

void WriteMultiLockFile(ofstream& dataFile, vector<MultiLock>& locksHolder, string fileName);

void WriteKeyFile(ofstream& dataFile, vector<Set>& ROOTS, HashKey& UHF, HashKey& LHF, HashKey& PHF, string fileName);

void SetLockValues(MultiLock &locks, Root& ROOT, HashKey& UHF, HashKey& LHF, HashKey& PHF);


int main()
{
	srand(time(NULL));
	HashKey UHF, LHF, PHF;
	Root ROOT;

	ofstream dataFile;
	ifstream readFile;

	cout << "How many valid solutions do you need? ";
	int validSolWanted;
	cin >> validSolWanted;
	cout << endl;
	int validSol = 0;

	clock_t start;
	start = clock();

	vector<Set> goodROOTS;
	MultiLock locks;
	vector<MultiLock> locksHolder;

	//Find solutions
	int i = 0;
	while (validSol < validSolWanted)
	{
		CheckLocksKeyFile(i, validSol, goodROOTS, UHF, LHF, PHF);
		i += 1;
		if (i > 10000)
		{
			UHF = HashKey();
			LHF = HashKey();
			PHF = HashKey();
		}
	}

	//Write solutions key file
	try {
		WriteKeyFile(dataFile, goodROOTS, UHF, LHF, PHF, key_file_name);
	}
	catch (const invalid_argument& ifn)
	{
		cout << "Unable to read data: " << ifn.what() << endl;
		exit(1);
	}

	cout << "Iterations:" << i << "  " << endl <<(clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	
	//Read roots from key file
	start = clock();
	vector<Root> ROOTS;
	
	vector<Set> foundROOTS;

	try {
		ReadTextFile(readFile, foundROOTS);
	}
	catch(const invalid_argument& ifn)
	{
		cout << "Unable to read data: " << ifn.what() << endl;
		exit(1);
	}

	//Check roots from key file
	for (int i = 0; i < (int)foundROOTS.size(); i++)
	{
		try {
			CheckLocksMultiSafeFile(i, foundROOTS, locksHolder);
		}
		catch (const invalid_argument& ifn)
		{
			cout << "Unable to read data: " << ifn.what() << endl;
			exit(1);
		}

	}

	//Write multi-safe file
	try {
		WriteMultiLockFile(dataFile, locksHolder, msf_file_name);
	}
	catch (const invalid_argument& ifn)
	{
		cout << "Unable to read data: " << ifn.what() << endl;
		exit(1);
	}
	
	
	cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	
	return 0;
    
}

void ReadTextFile(ifstream& readFile, vector<Set>& findROOTS) throw (invalid_argument)
{
	readFile.open(key_file_name);
	if (readFile.fail()) throw invalid_argument("no file exists" + key_file_name);
	if (readFile.is_open())
	{
		Root ROOT = Root(0, 0, 0, 0);
		HashKey UHF = HashKey(0, 0, 0, 0);
		HashKey LHF = HashKey(0, 0, 0, 0);
		HashKey PHF = HashKey(0, 0, 0, 0);

		string line;
		int lineNumber = 0;
		int a, b, c, d;
		while (getline(readFile, line))
		{
			if (lineNumber != 0)
			{	
				if (lineNumber % 4 == 1)
				{
					//ROOTS.push_back(Root(((int)line[5] - 48), ((int)line[6] - 48), ((int)line[7] - 48), ((int)line[8] - 48)));
					ROOT = Root(((int)line[5] - 48), ((int)line[6] - 48), ((int)line[7] - 48), ((int)line[8] - 48));
				}

				else if (lineNumber % 4 == 2)
				{
					ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
					UHF = HashKey(a, b, c, d);
				}

				else if (lineNumber % 4 == 3)
				{
					ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
					LHF = HashKey(a, b, c, d);
				}

				else
				{
					ParseTextFile(line, 4, 7, 10, 13, a, b, c, d);
					PHF = HashKey(a, b, c, d);
				}
				
				/*if (line.find("ROOT") != string::npos)
				{
					//ParseTextFile(line, 4, 5, 6, 7, a, b, c, d);
					ROOTS.push_back(Root(((int)line[5]-48), ((int)line[6] - 48), ((int)line[7] - 48), ((int)line[8] - 48)));

				}
				switch (lineNumber)
				{
				case (lineNumber % 4 == 2):
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
				}*/
				

			}
			if (lineNumber % 4 == 0 && lineNumber != 0)
			{
				findROOTS.push_back(Set(UHF, LHF, PHF, ROOT));
				
			}
			lineNumber += 1;
		}

	}
	readFile.close();
}

void ParseTextFile(string line, int w, int x,int y, int z, int& a, int& b, int& c, int& d)
{ 

	if (line[w] == '+') a = (int)(line[w + 1] - 48);
	else a = (int)((line[w + 1] - 48)*-1);

	if (line[x] == '+') b = (int)(line[x + 1] - 48);
	else b = (int)((line[x + 1] - 48)*-1);

	if (line[y] == '+') c = (int)(line[y + 1] - 48);
	else c = (int)((line[y + 1] - 48)*-1);

	if (line[z] == '+') d = (int)(line[z + 1] - 48);
	else d = (int)((line[z + 1] - 48)*-1);
	
}

void WriteKeyFile(ofstream& dataFile, vector<Set>& ROOTS, HashKey& UHF, HashKey& LHF, HashKey& PHF, string fileName) throw (invalid_argument)
{
	dataFile.open(fileName.c_str());//, std::ios_base::app);
	if (dataFile.fail()) throw invalid_argument("no file exists" + key_file_name);

	dataFile << "NS " << ROOTS.size() << endl;
	for (int i = 0; i < (int)ROOTS.size(); i++) {
		Root CurrentRoot = ROOTS[i].GetROOT();
			 
		dataFile << "ROOT " << CurrentRoot.GetA() << CurrentRoot.GetB() << CurrentRoot.GetC() << CurrentRoot.GetD() << "\n";
		dataFile << "UHF " << ROOTS[i].GetUHF() << "\n";
		dataFile << "LHF " << ROOTS[i].GetLHF() << "\n";
		dataFile << "PHF " << ROOTS[i].GetPHF() << "\n";
	}
	dataFile.close();
}

void WriteMultiLockFile(ofstream& dataFile, vector<MultiLock>& locksHolder, string fileName) throw (invalid_argument)
{
	dataFile.open(fileName.c_str());//, std::ios_base::app);
	if (dataFile.fail()) throw invalid_argument("no file exists" + key_file_name);

	for (int i = 0; i < (int)locksHolder.size(); i++)
	{
		dataFile << "NS" << i << " VALID" << "\n";
		for (int j = 0; j < 5; j++)
		{
			dataFile << "CN" << j << " " << locksHolder[i].GetCN(j).GetA() << locksHolder[i].GetCN(j).GetB() << locksHolder[i].GetCN(j).GetC() << locksHolder[i].GetCN(j).GetD()
				<< ", LN" << j << " " << locksHolder[i].GetLN(j).GetA() << locksHolder[i].GetLN(j).GetB() << locksHolder[i].GetLN(j).GetC() << locksHolder[i].GetLN(j).GetD()
				<< ", HN" << j << " " << locksHolder[i].GetHN(j).GetA() << locksHolder[i].GetHN(j).GetB() << locksHolder[i].GetHN(j).GetC() << locksHolder[i].GetHN(j).GetD() << "\n";;
		}
	}
	
	dataFile.close();

}

void CheckLocksMultiSafeFile(int i, vector<Set>& foundROOTS, vector<MultiLock>& locksHolder)
{
	MultiLock locks;
	
	Root ROOT = foundROOTS[i].GetROOT();
	HashKey UHF = foundROOTS[i].GetUHF();
	HashKey LHF = foundROOTS[i].GetLHF();
	HashKey PHF = foundROOTS[i].GetPHF();

	SetLockValues(locks, ROOT, UHF, LHF, PHF);

	if (locks.Validate()) locksHolder.push_back(locks);
}

void CheckLocksKeyFile(int i, int& validSol, vector<Set>& goodROOTS, HashKey& UHF, HashKey& LHF, HashKey& PHF)
{

	Root ROOT;
	MultiLock locks;

	SetLockValues(locks, ROOT, UHF, LHF, PHF);

	if (locks.Validate())
	{
		validSol += 1;
		goodROOTS.push_back(Set(UHF, LHF, PHF, ROOT));
	}
	
}

void SetLockValues(MultiLock &locks, Root& ROOT, HashKey& UHF, HashKey& LHF, HashKey& PHF)
{
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
		locks.SetValidHN((locks.GetHN(i)) == (locks.GetHN(i - 1)), i);
	}
}


