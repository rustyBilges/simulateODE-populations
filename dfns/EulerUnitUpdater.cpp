/*
 * EulerUnitUpdater.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#include "IUnitUpdater.h"
#include <cfloat>

EulerUnitUpdater::EulerUnitUpdater(){}

void EulerUnitUpdater::updateUnit_test(IDynamicalUnit *u, double totalCouplingInput){}

bool EulerUnitUpdater::updateUnit(IDynamicalUnit **allUnits, int nUnits, int unitToUpdate, double dt, int masterClock){

		double ARBITRARY_MIN = 0.01;//1e-20;//0.000001;
		double ARBITRARY_MAX = 1000;
		double ARBITRARY_DELTA = 1e-20;//0.0000001;

		double tempState = allUnits[unitToUpdate]->getHistory()->getHistoryTi(masterClock);

		double newState = tempState + (allUnits[unitToUpdate]->getXdot(allUnits, nUnits, masterClock))*dt;

		if (newState<=ARBITRARY_MIN){
			cerr << "extinction in species " << unitToUpdate << endl;
			return false;
		}
////		else if (newState[0]>= DBL_MAX){
		else if (newState>= ARBITRARY_MAX){
			cerr << "runaway population: species " << unitToUpdate << endl;
			return false;
		}
//		else if (newState==tempState){
//			cerr << "reached equilibrium, species " << unitToUpdate << endl;
//			return false;
//		}
		else if (newState>=tempState-ARBITRARY_DELTA && newState<=tempState+ARBITRARY_DELTA){
			cerr << "reached equilibrium, species " << unitToUpdate << endl;
			return false;
		}
		allUnits[unitToUpdate]->setState(newState);

		// store it in unit history and update the internal clock
		allUnits[unitToUpdate]->getHistory()->store(masterClock+1, allUnits[unitToUpdate]->getState());

		return true;
}

