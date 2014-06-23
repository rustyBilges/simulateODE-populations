/*
 * Timeseries.h
 *
 *  Created on: 25 Feb 2014
 *      Author: rusty
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
using namespace std;

class Timeseries{
// as declared also in GeneralisedSimulationState.h
public:
	int length;
	int width;
	double stepLength;

	vector<vector<double> > timeseries;

	~Timeseries(){
		for (int i=0; i<width;i++){
			timeseries.at(i).erase(timeseries.at(i).begin(), timeseries.at(i).end());
		}
		timeseries.erase(timeseries.begin(), timeseries.end());
	}

	Timeseries(int length, int width, double stepLength){
		this->length = length;
		this->width = width;
		this-> stepLength = stepLength;

		// initialise to zero.. (beware length of simulation output+1)
		for (int i=0; i<width;i++){
			timeseries.push_back(vector<double> ());

			for (int j=0;j<length;j++){
				timeseries[i].push_back(0);
			}
		}
	}

	Timeseries(Timeseries* other){
		this->length = other->length;
		this->width = other->width;
		this->stepLength = other->stepLength;

		this-> timeseries = other->timeseries;
	}

	void copyTimeseries(Timeseries* other){
		if (this->length==other->length && this->width==other->width){
			for (int i=0;i<width;i++){
				for (int j=0;j<length;j++){
					this->timeseries.at(i).at(j) = other->timeseries.at(i).at(j);
				}
			}
		}

	}

};



#endif /* TIMESERIES_H_ */
