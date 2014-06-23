/*
 * NullNetwork.h
 *
 *  Created on: 12 Mar 2014
 *      Author: rusty
 */

#ifndef NULLNETWORK_H_
#define NULLNETWORK_H_

#include "INetwork.h"
#include <vector>
using namespace std;

class NullNetwork : public INetwork
// allows use of user specified network topology.
{

private:

public:
	~NullNetwork(){

	}

	NullNetwork();

	void     fillAdjacency(double p);
	double** getAdjacency();
	void     printAdjacency();

};

#endif /* NULLNETWORK_H_ */
