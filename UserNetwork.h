/*
 * UserNetwork.h
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef USERNETWORK_H_
#define USERNETWORK_H_


#include "INetwork.h"
#include <vector>
using namespace std;

class UserNetwork : public INetwork
// allows use of user specified network topology.
{

private:
	int      nNodes;
	double** adjacency;
public:
	~UserNetwork(){
		for (int i=0;i<nNodes;i++){
			delete[] adjacency[i];
		}
		delete[] adjacency;
	}

	UserNetwork(int n, double** userConnections);  // double because fileReader reads doubles.

	UserNetwork(int n, vector<vector<double> >adjacency);  // double because fileReader reads doubles.

	void     fillAdjacency(double p);
	double** getAdjacency();
	void     printAdjacency();

};





#endif /* USERNETWORK_H_ */
