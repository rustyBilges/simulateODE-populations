/*
 * Simulation.cpp
 *
 *  Created on: 19 Dec 2013
 *      Author: rusty
 */

#include "Simulation.h"

Simulation::~Simulation(){

	for (int i=0;i<unitCount;i++){delete units[i];}
	delete []units;
	delete defaultResponse;
	delete model;
	delete parameters;
	delete biomassTracker;
	delete interactionTracker;

}

//Simulation::Simulation(int simID, int unitCount, int nTimesteps, double dt, string modelType, string responseType):modelType(modelType),responseType(responseType),simID(simID){
//
//	this->unitCount = unitCount;
//	this->timestepCount = nTimesteps;
//	this->dt = dt;
//
//	createFunctionalResponse(responseType);
//
//	model = new NicheModel(unitCount);
//	model->generate();
////	model->generate(4);  // for a specified 4 species topology..
////	model->generate("chain");
//
//	parameters = new RM_Nsp_Parameters(simID, model->getAdjacency());
//
//	units = new IDynamicalUnit*[unitCount];
//
//	for (int i=0; i<unitCount; i++){
//		units[i] = new RM_Nsp_unit(i, timestepCount, parameters->couplingMatrix, parameters, defaultResponse, modelType);
//	}
//
//	this->trackBiomass = false;
//	this->trackInteractions = false;
//	biomassTracker = new NullTracker();
//	interactionTracker = new NullTracker();
//}

Simulation::Simulation(int simID, int unitCount, int nTimesteps, double dt, string modelType, string responseType, string parameterFilename, bool trackBiomass, bool trackInteractions):modelType(modelType),responseType(responseType),simID(simID){

	this->unitCount = unitCount;
	this->timestepCount = nTimesteps;
	this->dt = dt;

	createFunctionalResponse(responseType);

	model = new NicheModel(unitCount);  // RENAME!!
	if (parameterFilename==""){
		model->generate();
	//	model->generate(4);  // for a specified 4 species topology..
	//	model->generate("chain");

		parameters = new RM_Nsp_Parameters(simID, model->getAdjacency());
	}
	else{
		parameters = new RM_Nsp_Parameters(simID, unitCount, parameterFilename);  // NOT SAFE: READING PARAMETERS FILE CAN FAIL: ADD EXCEPTION HANDLING
	}

	units = new IDynamicalUnit*[unitCount];

	for (int i=0; i<unitCount; i++){
		units[i] = new RM_Nsp_unit(i, timestepCount, parameters->couplingMatrix, parameters, defaultResponse, modelType);
	}

	this->trackBiomass = trackBiomass;
	this->trackInteractions = trackInteractions;

	// put this in a private method!
	if (trackBiomass){
		biomassTracker = new BiomassTracker(timestepCount, unitCount, dt, parameters, units);
	}
	else {
		biomassTracker = new NullTracker();
	}
	if (trackInteractions){
		interactionTracker = new InteractionTracker(timestepCount, unitCount, dt, parameters, units);
	}
	else{
		biomassTracker = new NullTracker();
	}

}


void Simulation::printStates(){

		for (int i=0; i< unitCount; i++){
				units[i]->printState();
		}
		cout<< "***************************************************" << endl;
	}

bool Simulation::step(int masterClock, IUnitUpdater* updater){
		for (int i=0; i< unitCount; i++){

			if (!updater->updateUnit(units, unitCount, i, dt, masterClock)){
				return false;
			}

		}
		biomassTracker->store(masterClock, defaultResponse);
		interactionTracker->store(masterClock, defaultResponse);
		return true;
	}

bool Simulation::run(IUnitUpdater* updater){

		for (int masterClock=0; masterClock< timestepCount; masterClock++){
			if(!step(masterClock, updater)){
				return false;
			}
		}
		return true;

	}

bool Simulation::save(string suffix){

		IFileWriter* writer = new FullFileWriter(suffix);
		writer->writeSimulationHistoryToFile(simID, unitCount, units, timestepCount, dt);
		writer->writeSimulationLogFile(simID, unitCount, units, timestepCount, dt, modelType, responseType);

		writer->saveParameters(simID, parameters);
		biomassTracker->save(suffix, simID);
		interactionTracker->save(suffix, simID);
		return true;
	}

void Simulation::createFunctionalResponse(string responseType){

	if (responseType=="linear"){
		defaultResponse = new Linear_FunctionalResponse();
	}
	else if (responseType=="hollingII"){
		defaultResponse = new HollingII_FunctionalResponse;
	}
	else if (responseType=="hollingIII"){
		defaultResponse = new HollingIII_FunctionalResponse;
	}
	else if (responseType=="hollingII_noPassiveSwitching"){
		defaultResponse = new HollingII_FunctionalResponse_NoPassiveSwitching;
	}
	else{
		throw 69;
	}

}

bool Simulation::getSimulationResults(vector<vector<double> >& populationTimeseries){

	if ((int)populationTimeseries.size()!=timestepCount+1 || (int)populationTimeseries.at(0).size()!=unitCount+1){
		cerr << "population timeseries vector wrong size, cannot obtain simulation results. exiting.." << endl;
		return false;
	}

	for (int t=0; t<=timestepCount; t++){
		populationTimeseries.at(t).at(0) = t*dt;
		for (int i=1; i<=unitCount; i++){
			populationTimeseries.at(t).at(i) = units[i]->getHistory()->getHistoryTi(t);
		}
	}

	return true;
}

