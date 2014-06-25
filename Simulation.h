/*
 * Simulation.h
 *
 *  Created on: 18 Dec 2013
 *      Author: rusty
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "IUnitUpdater.h"
#include "IFileWriter.h"
#include "FullFileWriter.h"  // not needed if combine header files
#include "IFunctionalResponse.h"
#include "NicheModel.h"
#include "RM_Nsp_Parameters.h"
#include "IDynamicalUnit.h"
#include "RM_Nsp_unit.h"
#include "IVariableTracker.h"

class Simulation{

private:
	// simulation information (for model construction + reference)
	string modelType, responseType;

	//simulation parameters
	int simID, unitCount, timestepCount;
	double dt;

	// simulation objects
	IFunctionalResponse* defaultResponse;
	IDynamicalUnit** units;
	NicheModel* model;
	RM_Nsp_Parameters* parameters;

	//determine additional calculations to perform and save
	bool trackBiomass;
	bool trackInteractions;
	IVariableTracker* biomassTracker, *interactionTracker;

public:
	~Simulation();

//	Simulation(int simID=0, int unitCount=2, int nTimesteps=1000, double dt=0.01, string modelType = "LV", string responseType="linear");  // defaults to pure GLV simulation
	Simulation(int simID=0, int unitCount=2, int nTimesteps=1000, double dt=0.01, string modelType = "LV", string responseType="linear", string parameterFilename="", bool trackBiomass=true, bool trackInteractions=true);
//	Simulation(int unitCount, int nTimesteps, double dt, IDynamicalUnit** units, IUnitUpdater* updater, INetwork* network, IFileWriter* fileWriter);

	void printStates();

	bool run(IUnitUpdater* updater);
	bool save(string suffix="");

	// ohter results we want to know about?
	bool getSimulationResults(vector<vector<double> >& populationTimeseries);
	bool getSimulationResults(vector<vector<double> >& populationTimeseries, vector<vector< vector<double> > >& interactionTimeseries);
//	bool getSimulationResults(vector<vector<double> >& populationTimeseries, vector<vector< vector<double> > >& interactionTimeseries, vector<vector< vector<double> > >& biomassTimeseriesvector);

private:
	bool step(int masterClock, IUnitUpdater* updater);
	void createFunctionalResponse(string responseType);
};


#endif /* SIMULATION_H_ */
