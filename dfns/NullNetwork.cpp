/*
 * NullNetwork.cpp
 *
 *  Created on: 12 Mar 2014
 *      Author: rusty
 */

#include "NullNetwork.h"

NullNetwork::NullNetwork(){}

void NullNetwork::fillAdjacency(double p){}
double** NullNetwork::getAdjacency(){
	double temp = 0.0;
	double** null = new double*[1];
	null[0] = &temp;
	return null;}
void NullNetwork::printAdjacency(){}








