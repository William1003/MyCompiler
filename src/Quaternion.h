#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include<string>
#include"constValue.h"
#include<vector>

using namespace std;

class Quaternion
{
private:
	quaternion::QuaternionOpType op;
	string oper1;
	string oper2;
	string dest;

public:
	Quaternion(quaternion::QuaternionOpType op, string oper1, string oper2, string dest) : 
		op(op), oper1(oper1), oper2(oper2), dest(dest) {}
	string toString();
};

#endif // !_QUATERNION_H_
