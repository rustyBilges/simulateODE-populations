/*
 * UnitHistory.h
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef UNITHISTORY_H_
#define UNITHISTORY_H_

#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

class UnitHistory {

private:
//	double **history;
	int length;
	vector <double> history;

public:
	 UnitHistory(int tSteps);
	~UnitHistory();

	void   store(int tic, double state);
	double getHistoryTi(int Ti);
	void   printHistory();  // change to overload <<
};



#endif /* UNITHISTORY_H_ */
