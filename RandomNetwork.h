/*
 *
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef RANDOMNETWORK_H_
#define RANDOMNETWORK_H_

#include "INetwork.h"

class RandomNetwork : public INetwork
{

private:
	int      nNodes;
	double** adjacency;
public:
	RandomNetwork(int n, double p);

	~RandomNetwork(){
		for (int i=0; i<nNodes; i++){
			delete[] adjacency[i];
		}
		delete[] adjacency;
	}

	void     fillAdjacency(double p);
	double** getAdjacency();
	void     printAdjacency();


	double   selectEntry(int i, int j, double p);
	double   unifRand();

};




#endif /* RANDOMNETWORK_H_ */
