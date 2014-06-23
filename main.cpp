/*
 * main.cpp
 *
 *  Created on: 18 Jun 2014
 *      Author: rusty
 *
 *      TO DO:
 *      > change parameter usage and saving to be GENERAL for all types of units/simulation, including heterogenous units
 *      > change niche model usage and how it relates to connection matrix (e.g. two species->both basal, and pure LV sim->problem)
 *      > finish implementing "getInteractionStrength" and generalise to passive prey switching + intra-specific interactions
 *      > check getResponse! - was using wrong population density in the denominator?
 *      > change the first constructor to make it easier to run with randomised parameters and Niche model topology
 *      > create in trackers in a private function
 *      > TIDy!!
 *
 *      NOTES:
 *      > First order Euler is not accurate! either use v.small timestep, or implement RGKutta.
 *
 *      USAGE:
 *      > modelType (RM or LV) indicates the type of intrinsic growth function to use (for prey species)
 *      > responseType (see Simulation.cpp) indicates the type of functional response to use (currently units of homogeneous in response type)
 *      > try/catch when calling Simulation() constructor, this should avoid memory leaks from failed construction
 *      > tracking of biomass and interactions must be specified
 */

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>

// from current project
#include "IUnitUpdater.h"
#include "Simulation.h"
#include "IVariableTracker.h"
using namespace std;

bool randomSimulationTest(int simID, int unitCount, int timestepCount, double dt, string modelType, string responseType, unsigned int seed=0);
bool simulationFromParameterFileTest(string parametersFileName, unsigned int seed=0);

int main(){

	if(!randomSimulationTest(2,6,10000,0.01, "RM", "hollingII_noPassiveSwitching", time(NULL))){return 1;}
//	if(!simulationFromParameterFileTest("0_RM_Nsp.parameters")){return 1;}

	return 0;
}

bool randomSimulationTest(int simID, int unitCount, int timestepCount, double dt, string modelType, string responseType, unsigned int seed){
// generates random parameters until stable dynamics are achieved
// sensitive to conditions for extinction and equilibrium: PASS THESE ARE ARGUMENTS FOR FINE TUNING BY USER.
	srand(seed);
	bool finish = false;

	Simulation* sim;
	IUnitUpdater* updater = new EulerUnitUpdater();
	string historyPrefix = "TESTRAND";

	while (!finish){

		sim = new Simulation(simID, unitCount, timestepCount, dt, modelType, responseType);
		if (!sim->run(updater)){
			delete sim;
		}
		else {
			cout << "simulation run" << endl;
			sim->save(historyPrefix);
			finish = true;
		}
	}

	return true;
}

bool simulationFromParameterFileTest(string parametersFileName, unsigned int seed){

	srand(seed);

	Simulation* sim;
	IUnitUpdater* updater = new EulerUnitUpdater();
	string historyPrefix = "TEST";

	try
	{
		sim = new Simulation(0,2,10000,0.0001, "RM", "hollingII_noPassiveSwitching", parametersFileName);
	}
	catch(int e)
	{
		cerr << "error: could not create Simulation object. Code = " << e << endl;
		return false;
	}

	if (!sim->run(updater)){return false;}
	else {
		cout << "simulation run" << endl;
		sim->save(historyPrefix);
	}

	return true;
}

//int unitCount = 3;
//int parameterCount = 1;
//string response_type = "Holling_II";
//
//string topology = "AC";
////usage notes:
//// network is generated using the Niche Model, interaction strengths and parameters are randomised
//
//int timestepCount = 50000;
//double timestepSize = 0.01;
//int s = 0;
//
//stringstream UC_ss;
//UC_ss << unitCount;
//string historyPrefix = UC_ss.str()+"species_"+response_type;
//
//IUnitUpdater* updater = new EulerUnitUpdater();
//IFileWriter* writer = new FullFileWriter(historyPrefix);
//INetwork* network = new NullNetwork();
//
//NicheModel* model = new NicheModel(unitCount);
//
////	model->generate();
//model->generate(4);  // for a specified 4 species topology..
////	model->generate(topology);  // niche model can also be used to generate 3 species specified topologies
////	cout << *model;
//IDynamicalUnit** units = new IDynamicalUnit*[unitCount];
//
//while (s<parameterCount){
////	while (s<1000){
////	while(1){
//
//
//	RM_Nsp_Parameters* p = new RM_Nsp_Parameters(0, model->getAdjacency());       // generate parameters using Niche Model network
////		RM_Nsp_Parameters* p = new RM_Nsp_Parameters(40, 4, "40_RM_Nsp.parameters");  // read from parameter logfile
////		RM_Nsp_Parameters* p = new RM_Nsp_Parameters(s,unitCount);
////		test params:
////	    NB test params use different form for intrinsic growth of predator
////		RM_Nsp_Parameters* p = new RM_Nsp_Parameters(s);
////		cout << *p;
//
//	units = new IDynamicalUnit*[unitCount];
////		FunctionalResponse* response = new FunctionalResponse(response_type);
//
//	IFunctionalResponse* response = new HollingII_FunctionalResponse();
//
//	for (int i=0; i<unitCount; i++){
//		units[i] = new RM_Nsp_unit(i, timestepCount, p->couplingMatrix, p, response);
//	}
//
////		cout << "state0 = ";
////		units[0]->printState();
////		cout << endl << "state1 = ";
////		units[1]->printState();
//
//	Simulation* simulation = new Simulation(unitCount, timestepCount, timestepSize, units, updater, network, writer);
//	if (simulation->run()){
//		cout << s << endl;
//		if(!saveParameters(s, *p)){
//			cout << "warning could not save params" << endl;
//		}
//		if(!writer->writeSimulationHistoryToFile(s, unitCount, units, timestepCount, 1, timestepSize)){
//			cout << "WARNING COULD NOT SAVE HISTORY" << endl;
//		}
//		s = s + 1;
//	}
//	cout << "ok" << endl;
//	for (int i=0;i<unitCount;i++){delete units[i];}
//	cout << "ok" << endl;
//	delete response;
//	cout << "ok" << endl;
//	delete simulation;
//	cout << "ok" << endl;
//	delete p;
//	cout << "ok" << endl;
//
//	delete model;
//	cout << "ok" << endl;
//	model = new NicheModel(unitCount);
//	cout << "ok" << endl;
////		model->generate(topology);
////		while(!model->generate()){}
//	model->generate();
//	cout << "ok" << endl;
//
//}
//
//delete []units;
//

