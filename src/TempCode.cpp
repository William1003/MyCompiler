#include"TempCode.h"

TempCode::TempCode()
{
	tempCode.open("tempCode.txt");
}

void TempCode::output()
{
}

string TempCode::genTempVar()
{
	static int n = 0;
	return "T" + to_string(n++);
}

void TempCode::outputToFile()
{
	for (int i = 0; i < quaternions.size(); i++)
	{
		tempCode << quaternions[i].toString() << endl;
	}
}

void TempCode::add(Quaternion quaternion)
{
	quaternions.push_back(quaternion);
}