/*
 * VectorFileWriter.h
 *
 *  Created on: 1 Feb 2014
 *      Author: rusty
 */

#ifndef VECTORFILEWRITER_H_
#define VECTORFILEWRITER_H_


#include "IFileWriter.h"
#include "IDynamicalUnit.h"

#include <vector>
using namespace std;

class VectorFileWriter : public IFileWriter{

private:
	vector<vector<double> >* timeseries;


public:
	VectorFileWriter(vector<vector<double> >* test){

		this->timeseries = test;

	}

	void writeSimulationHistoryToFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, int unitDimension, double dt){

//		(*timeseries).at(0).at(2) = 15.6;

		for (int t=0; t<nTimesteps+1; t++){
			(*timeseries).at(0).at(t) = t*dt;
			for (int i=0; i< unitCount; i++){
				for (int d=0; d<unitDimension; d++){
					(*timeseries).at(i+1).at(t) = units[i]->getHistory()->getHistoryTi(d, t);
				}
			}

		}

	}

	void writeSimulationLogFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, int unitDimension, double dt, int parametersPerUnit, INetwork* network){
		ofstream oFile;
		//const char* fname;
		char fname[50];
		sprintf(fname, "simulation%d_log.txt", simulationID);
		//itoa(simulationID,simID,10);
		//fname = strcat(simID,"simulation.txt");
		oFile.open(fname);

		oFile << "Simulation: " << simulationID << endl;
		oFile << "unitCount:  " << unitCount    << endl;
		oFile << "unitType:   " << units[0]->getUnitType();
		oFile << "nTimesteps: " << nTimesteps << endl;
		oFile << "dt:         " << dt << endl;
		oFile << "***********************************************" << endl;
		for (int i=0; i< unitCount; i++){
			oFile << "Unit " << i << ":" << endl;
			writeInitialState(i,units, unitDimension, oFile);
			writeUnitParameters(i,units, parametersPerUnit, oFile);
		}
		oFile << "***********************************************" << endl;
		writeNetwork(unitCount, network, oFile);

		oFile.close();
	}

	void writeNetwork(int unitCount, INetwork* network, ofstream& oFile){
		oFile << "Connections: " << endl;
		double** tempAdjacecny = network->getAdjacency();
		for (int i=0;i<unitCount;i++){
			for (int j=0;j<unitCount;j++){
				oFile << "    " << tempAdjacecny[i][j];
			}
			oFile << endl;
		}
	}

	void writeInitialState(int i,IDynamicalUnit** units, int unitDimension, ofstream& oFile){
		for (int d=0; d< unitDimension; d++){
			oFile << "Initial State";
			oFile << ", " << units[i]->getHistory()->getHistoryTi(d,0);
		}
		oFile << endl;
	}

	void writeUnitParameters(int i,IDynamicalUnit** units, int parametersPerUnit, ofstream& oFile){
		for (int p=0; p< parametersPerUnit; p++){
			oFile << "Parameters: ";
			oFile << ", " << units[i]->getParamI(p);
		}
		oFile << endl;
	}


};



#endif /* VECTORFILEWRITER_H_ */
