/*
 * NoisyEulerUnitUpdater.cpp
 *
 *  Created on: 14 Apr 2014
 *      Author: rusty
 */


#include "IUnitUpdater.h"
#include <cfloat>

NoisyEulerUnitUpdater::NoisyEulerUnitUpdater(double variance){
	this->variance = variance;
}

void NoisyEulerUnitUpdater::updateUnit_test(IDynamicalUnit *u, double totalCouplingInput){}

bool NoisyEulerUnitUpdater::updateUnit(IDynamicalUnit **allUnits, int nUnits, int unitToUpdate, double dt, int masterClock){

		double ARBITRARY_MIN = 0.1;//1e-20;
		double ARBITRARY_MAX = 10000;
		double ARBITRARY_DELTA = 0.000001;


		//double *temp = allUnits[unitToUpdate] -> getState();  // this was a relic!
		double tempState = allUnits[unitToUpdate]->getHistory()->getHistoryTi(masterClock);

		double newState = tempState + (allUnits[unitToUpdate]->getXdot(allUnits, nUnits, masterClock))*dt;
		// additive noise:
		newState += dt*sqrt(variance)*AWGN_generator();

		// instead of quitting under extinction, implement reflective boundary conditions:
		if (newState<0){
			newState= - newState;
		}

		else if (newState>= ARBITRARY_MAX){
			cerr << "runaway population: species " << unitToUpdate << endl;
			return false;
		}

		// this checks for equilbrium being reached. CHANGE TO COMPARE TIME REACHED AGAINST MASTERCLOCK.
//		else if (newState>=tempState-ARBITRARY_DELTA && newState<=tempState+ARBITRARY_DELTA){
//			cerr << "reached equilibrium, species " << unitToUpdate << endl;
//			return false;
//		}
		allUnits[unitToUpdate]->setState(newState);

		// store it in unit history and update the internal clock
		allUnits[unitToUpdate]->getHistory()->store(masterClock+1, allUnits[unitToUpdate]->getState());

		return true;
}




