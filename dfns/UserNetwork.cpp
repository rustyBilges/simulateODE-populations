/*
 * UserNetwork.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

/*
 * UserNetwork.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#include "UserNetwork.h"

UserNetwork::UserNetwork(int n, double** userConnections):nNodes(n){
		adjacency = new double*[nNodes];
		for (int i=0;i<nNodes;i++){
			adjacency[i] = new double[nNodes];
		}
		for (int i=0;i<nNodes;i++){
			for (int j=0;j<nNodes;j++){
				adjacency[i][j] = userConnections[i][j];
			}
		}
	}

UserNetwork::UserNetwork(int n, vector<vector<double> >adjacency):nNodes(n){
	this->adjacency = new double*[nNodes];
	for (int i=0;i<nNodes;i++){
		this->adjacency[i] = new double[nNodes];
	}
	for (int i=0;i<nNodes;i++){
		for (int j=0;j<nNodes;j++){
			this->adjacency[i][j] = adjacency.at(i).at(j);
		}
	}
}
void UserNetwork::fillAdjacency(double p){}
double** UserNetwork::getAdjacency(){return adjacency;}
void UserNetwork::printAdjacency(){
		for (int i=0;i<nNodes;i++){
			for (int j=0;j<nNodes;j++){
				cout << adjacency[i][j] << "                ";
			}
			cout << endl;
		}
	}








