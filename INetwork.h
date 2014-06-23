/*
 * INetwork.h
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef INETWORK_H_
#define INETWORK_H_

#include <stdlib.h>
#include <iostream>
using namespace std;

class INetwork{

public:
	virtual void fillAdjacency(double p) = 0;
	virtual double** getAdjacency() = 0;
	virtual void printAdjacency() = 0;
	virtual ~INetwork(){}
};



#endif /* INETWORK_H_ */
