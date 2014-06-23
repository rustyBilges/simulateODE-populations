/*
 * randomSimulator.h
 *
 *  Created on: 23 Jun 2014
 *      Author: rusty
 */

#ifndef RANDOMSIMULATOR_H_
#define RANDOMSIMULATOR_H_

#include <string.h>
using namespace std;

class RandomSimulator{

private:
	int simID, unitCount, timestepCount;
	double dt;
	string modelType, responseType;
	unsigned int seed;

	Simulation* sim;
	IUnitUpdater* updater;

public:
	RandomSimulator(int simID, int unitCount, int timestepCount, double dt, string modelType, string responseType, unsigned int seed=0):simID(simID),unitCount(unitCount), timestepCount(timestepCount), dt(dt), modelType(modelType), responseType(responseType), seed(seed){

		sim = new Simulation(simID, unitCount, timestepCount, dt, modelType, responseType);
		updater = new EulerUnitUpdater();
	}

	bool runUntilStable(){
		srand(seed);
		bool finish = false;

		while (!finish){

			sim = new Simulation(simID, unitCount, timestepCount, dt, modelType, responseType);
			if (!sim->run(updater)){
				delete sim;
			}
			else {
//				cout << "simulation run" << endl;
				finish = true;
			}
		}

		return true;
	}

	bool save(string saveSuffix=""){
		sim->save(saveSuffix);
		return true;
	}



};


#endif /* RANDOMSIMULATOR_H_ */
