/*
 * RandomNetwork.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#include "RandomNetwork.h"

RandomNetwork::RandomNetwork(int n, double p):nNodes(n){
		adjacency = new double*[nNodes];
		for (int i=0;i<nNodes;i++){
			adjacency[i] = new double[nNodes];
		}
		fillAdjacency(p);
	}
void RandomNetwork::fillAdjacency(double p){
		for (int i=0;i<nNodes;i++){
			for (int j=0;j<nNodes;j++){
				adjacency[i][j] = selectEntry(i,j, p);
			}
		}
	}
double** RandomNetwork::getAdjacency(){return adjacency;}
void RandomNetwork::printAdjacency(){
		for (int i=0;i<nNodes;i++){
			for (int j=0;j<nNodes;j++){
				cout << adjacency[i][j] << "                ";
			}
			cout << endl;
		}
	}


double RandomNetwork::selectEntry(int i, int j, double p){
		if (i != j){
			if (unifRand()<=(1-p)){return 0.0;}
			else {return unifRand();}
		}
		else {return 0.0;}
	}
double RandomNetwork::unifRand(){return (double)rand()/(double)RAND_MAX;}




