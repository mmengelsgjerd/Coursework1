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

int main()
{

	Number ROOT = Number((rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0), (rand() % 9 + 0));
	Number UHF = Number();
	Number LHF = Number();
	Number PHF = Number();

	Number CN = UnlockHashFunction(ROOT, UHF);
	cout << (CN.IllegalCN() ? "CN is illegal." : "CN is perfectly fine") << endl;
	
	Number LN = LockHashFunction(CN, LHF);
	Number HN = PassHashFunction(LN, PHF);

	cout << "After being hashed these are the following ROOT, CN, LN and HN value!" << endl << endl;
	cout << endl << "ROOT = " << ROOT << "\t (" << UHF << ")" << endl;
	cout << endl << "CN = " << CN << "\t (" << LHF << ")" << endl;
	cout << endl << "LN = " << LN << "\t (" << PHF << ")" << endl;
	cout << endl << "HN = " << HN << endl;

	return 0;
    
}




