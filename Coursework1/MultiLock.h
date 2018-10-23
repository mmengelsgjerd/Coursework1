#pragma once
//#include "LockDigit.h"
#include "Digits.h"

class MultiLock {


public:
	MultiLock();
	~MultiLock();
	void SetCN(Digits<int> arg, int i);
	void SetLN(Digits<int> arg, int i);
	void SetHN(Digits<int> arg, int i);

	void SetValidCN(bool arg, int i);
	void SetValidLN(bool arg, int i);
	void SetValidHN(bool arg, int i);

	bool IsValidCN(int i);
	bool IsValidLN(int i);
	bool IsValidHN(int i);
	
	bool Validate();
	bool SumToLeftLess();
	bool SumEven();

	Digits<int> GetCN(int i);
	Digits<int> GetLN(int i);
	Digits<int> GetHN(int i);

 private:
	 Digits<int> CN[5] = { Digits<int>(0,0,0,0) };
	 Digits<int> LN[5] = { Digits<int>(0,0,0,0) };
	 Digits<int> HN[5] = { Digits<int>(0,0,0,0) };
	 bool validCN[5];
	 bool validLN[5];
	 bool validHN[5];

};


MultiLock::MultiLock() {}
MultiLock::~MultiLock(){}

void MultiLock::SetCN(Digits<int> arg, int i){ CN[i] = arg; }
void MultiLock::SetLN(Digits<int> arg, int i) { LN[i] = arg; }
void MultiLock::SetHN(Digits<int> arg, int i) { HN[i] = arg; }

void MultiLock::SetValidCN(bool arg, int i) { validCN[i] = arg; }
void MultiLock::SetValidLN(bool arg, int i) { validLN[i] = arg; }
void MultiLock::SetValidHN(bool arg, int i) { validHN[i] = arg; }

bool MultiLock::IsValidCN(int i) { return validCN[i]; }
bool MultiLock::IsValidLN(int i) { return validLN[i]; }
bool MultiLock::IsValidHN(int i) { return validHN[i]; }


Digits<int> MultiLock::GetCN(int i) { return CN[i]; }
Digits<int> MultiLock::GetLN(int i) { return LN[i]; }
Digits<int> MultiLock::GetHN(int i) { return HN[i]; }

bool MultiLock::Validate()
{
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		if (!validCN[i]) count += 1;
		if (!validLN[i]) count += 1;
		if (!validHN[i]) count += 1;
	}
	if (count == 0 && SumToLeftLess() && SumEven()) return true;
	//if (count == 0) return true;
	else return false;
}

bool MultiLock::SumToLeftLess()
{
	if ((CN[0] < CN[1]) && (CN[1] < CN[2]) && (CN[2] < CN[3]) && (CN[3] < CN[4])) return true;
	else return false;
}

bool MultiLock::SumEven()
{
	bool a = false, b = false, c = false , d = false, e = false;
	if ((CN[0].GetA() + CN[0].GetB() + CN[0].GetC() + CN[0].GetD()) % 2 == 0) a = true;
	if ((CN[1].GetA() + CN[1].GetB() + CN[1].GetC() + CN[1].GetD()) % 2 == 0) b = true;
	if ((CN[2].GetA() + CN[2].GetB() + CN[2].GetC() + CN[2].GetD()) % 2 == 0) c = true;
	if ((CN[3].GetA() + CN[3].GetB() + CN[3].GetC() + CN[3].GetD()) % 2 == 0) d = true;
	if ((CN[4].GetA() + CN[4].GetB() + CN[4].GetC() + CN[4].GetD()) % 2 == 0) e = true;

	if (a && b && c && d && e) return true;
	else return false;

}