/*
 * RM_Nsp_unit.cpp
 *
 *  Created on: 28 Apr 2014
 *      Author: rusty
 */


#include "RM_Nsp_unit.h"

RM_Nsp_unit::RM_Nsp_unit(int ID, int tsteps, vector <vector< double > >& connectionMatrix, RM_Nsp_Parameters* parameters, string type):unitID(ID),nTimesteps(tsteps), connectionMatrix(connectionMatrix){

		if (checkIfBasalSpecies()){
			unitType="RM_prey";
			nParams = 2 + countInteractions();

			intrinsicGrowthFunction = new ExponentialGrowthFn(parameters->growthRates.at(ID));
//			intrinsicGrowthFunction = new LogisticGrowthFn(parameters->growthRates.at(ID), parameters->carryingCapacities.at(ID));
		}
		else {
			unitType="RM_predator";
			nParams = 1 + countInteractions();

			intrinsicGrowthFunction = new ExponentialGrowthFn(parameters->growthRates.at(ID));
		} // does not distinguish between top and intermediate predators

		history = new UnitHistory(nTimesteps+1);  // gives space for all timesteps + intial state.

		this->parameters = parameters;

//		this->response = new FunctionalResponse(type); //!
		// !!!!!
		response = new HollingII_FunctionalResponse();

//		//setParams(defaultParams);
//		setState(defaultState);
		randomiseState(0, parameters->carryingCapacities.at(0));
		history->store(0,state);

	}


RM_Nsp_unit::RM_Nsp_unit(int ID, int tsteps, vector <vector< double > >& connectionMatrix, RM_Nsp_Parameters* parameters, IFunctionalResponse* response, string modelType):unitID(ID),nTimesteps(tsteps), connectionMatrix(connectionMatrix){

		if (checkIfBasalSpecies()){
			unitType="RM_prey";
			nParams = 2 + countInteractions();
			if (modelType=="LV"){
				intrinsicGrowthFunction = new ExponentialGrowthFn(parameters->growthRates.at(ID));
			}
			else if (modelType=="RM"){
				intrinsicGrowthFunction = new LogisticGrowthFn(parameters->growthRates.at(ID), parameters->carryingCapacities.at(ID));
			}
		}
		else {
			unitType="RM_predator";
			nParams = 1 + countInteractions();

			intrinsicGrowthFunction = new ExponentialGrowthFn(parameters->growthRates.at(ID));
		} // does not distinguish between top and intermediate predators

		history = new UnitHistory(nTimesteps+1);  // gives space for all timesteps + intial state.

		this->parameters = parameters;

		this->response = response; //!

		randomiseState(0, parameters->carryingCapacities.at(0));
		history->store(0,state);

	}


void RM_Nsp_unit::setInitialState(double st){
	state = st;
	history->store(0,state);
}

void RM_Nsp_unit::setState(double st){
	state = st;
}

void RM_Nsp_unit::randomiseState(double sMin, double sMax){
		state = unifRand(sMin,sMax);
}

double RM_Nsp_unit::getState(){
	return state;
}
void RM_Nsp_unit::printState(){

	cout << state << ", ";
}

double RM_Nsp_unit::getXdot(IDynamicalUnit **allUnits, int nUnits, int masterClock){

	double xi = history->getHistoryTi(masterClock);
	double xdot = intrinsicGrowthFunction->growthRate(xi);

	//do coupling dynamics:
	for (int j=0; j<nUnits; j++){
		if (connectionMatrix.at(unitID).at(j)!=0){
			xdot += response->getResponse(unitID, j, nUnits, connectionMatrix, allUnits, masterClock, parameters);
		}
	}
	return xdot;
}

UnitHistory* RM_Nsp_unit::getHistory(){return history;}

string RM_Nsp_unit::getUnitType(){
	return unitType;
}

double RM_Nsp_unit::unifRand(double rMin, double rMax){
	return ((double) rand() / (RAND_MAX)) * (rMax-rMin) + rMin;
}








