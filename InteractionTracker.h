/*
 * interactionTracker.h
 *
 *  Created on: 19 Jun 2014
 *      Author: rusty
 */

#ifndef INTERACTIONTRACKER_H_
#define INTERACTIONTRACKER_H_

#include <vector>
using namespace std;

class InteractionTracker{

private:
	vector <vector<vector<double> > >interactionTimeseries;      // this uses the Interaction Matrix metric for interaction strength


public:
	~InteractionTracker(){}
	InteractionTracker(){}
	InteractionTracker(int timestepCount, int unitCount){

		interactionTimeseries.resize(timestepCount,vector<vector <double> >(unitCount,vector<double>(unitCount,0)));   // stores ALL interactions (including intra-specific)

	}

	void storeInteraction(int t, int i, int j, double interaction){
		interactionTimeseries.at(t).at(i).at(j) = interaction;
	}



};



#endif /* INTERACTIONTRACKER_H_ */
