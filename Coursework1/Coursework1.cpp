// Coursework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Number.h"

using namespace std;

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

void RunOneIteration(int i)
{
	srand(i); //Generates a valid CN.
	Number ROOT = Number((rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0));
	Number UHF, LHF, PHF;

	Number CN = UnlockHashFunction(ROOT, UHF);
	bool cont = CN.ValidCN();
	//cout << (cont ? "CN is valid." : "CN is not valid.") << endl;
	if (cont)
	{
		cout << "CN is valid. Iteration continued" << endl << endl;
		Number LN = LockHashFunction(CN, LHF);
		Number HN = PassHashFunction(LN, PHF);

		cout << "After being hashed these are the following ROOT, CN, LN and HN value!" << endl << endl;
		cout << endl << "ROOT = " << ROOT << "\t (" << UHF << ")" << endl;
		cout << endl << "CN = " << CN << "\t (" << LHF << ")" << endl;
		cout << endl << "LN = " << LN << "\t (" << PHF << ")" << endl;
		cout << endl << "HN = " << HN << endl;
	}
	else
	{
		cout << "CN is not valid. Iteration terminated" << endl << endl;
		return;
	}
}

int main()
{

	cout << "How many iterations do you want to run?";
	int iterations;
	cin >> iterations;

	for (int i = 0; i < iterations; i++)
	{
		RunOneIteration(i);
	}



	return 0;
    
}




