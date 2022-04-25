#ifndef typeCalc_h
#define typeCalc_h 1

#include <iostream>

class TypeCalculations {
public:
	TypeCalculations();
	~TypeCalculations();
	static int GetTypeCalc() {return typeCalc;}
	static void SetType(int t) {typeCalc = t;};
	void TypeMessenger();
private:
	static int typeCalc;
};
#endif
