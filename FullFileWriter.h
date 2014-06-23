/*
 * FullFileWriter.h
 *
 *  Created on: 19 Dec 2013
 *      Author: rusty
 */

#ifndef FULLFILEWRITER_H_
#define FULLFILEWRITER_H_

#include "IFileWriter.h"
#include "IDynamicalUnit.h"
#include "RM_Nsp_Parameters.h"

class FullFileWriter : public IFileWriter{

private:
	string suffix;

public:
	FullFileWriter(string preffix){
		this->suffix = preffix;
	};

	bool writeSimulationHistoryToFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, double dt){
		ofstream oFile;

		stringstream ID_ss, UC_ss;
		ID_ss << simulationID;
		UC_ss << unitCount;
		string fname = "sim"+ID_ss.str()+"_"+UC_ss.str()+"sp_"+suffix+".populationTimeseries";
		oFile.open(fname.c_str());

		for (int t=0; t<nTimesteps+1; t++){
			oFile << t*dt;
			for (int i=0; i< unitCount; i++){

				oFile  << ", " << units[i]->getHistory()->getHistoryTi(t);

			}
			oFile << endl;
		}
		oFile.close();
		return true;
	}

	void writeSimulationLogFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, double dt, string modelType, string responseType){
		ofstream oFile;

		stringstream ID_ss;
		ID_ss << simulationID;
		string fname = "sim"+ID_ss.str()+"_"+suffix+".logFile";
		oFile.open(fname.c_str());

		oFile << "**********************************************************************************************" << endl;
		oFile << "Simulation:    "    << simulationID << endl;
		oFile << "unitCount:     "    << unitCount    << endl;
		oFile << "timestepCount: "    << nTimesteps << endl;
		oFile << "dt:            "    << dt << endl;
		oFile << "**********************************************************************************************" << endl;
		oFile << "modelType:     " << modelType << "     (LV-> intrinsic growth exponential, RM-> logistic for basal species)" << endl;
		oFile << "responseType:  " << responseType << endl;
//		for (int i=0; i< unitCount; i++){
//			oFile << "Unit " << i << ":" << endl;
//			writeInitialState(i,units, oFile);
//			writeParameters(i,units, parametersPerUnit, oFile);
//		}
		oFile << "**********************************************************************************************" << endl;
//		writeNetwork(unitCount, network, oFile);

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

	void writeInitialState(int i,IDynamicalUnit** units, ofstream& oFile){

			oFile << "Initial State, " << units[i]->getHistory()->getHistoryTi(0) << endl;
	}

	void writeParameters(int i,IDynamicalUnit** units, int parametersPerUnit, ofstream& oFile){

		// could use this to write interaction function and intrinsic growth function??
	}

	bool saveParameters(int ID, RM_Nsp_Parameters* p){
		ofstream ofile;

		stringstream ID_ss;
		ID_ss << ID;
		string fname = "sim"+ID_ss.str()+"_"+suffix+".parameters";

		ofile.open(fname.c_str());
		if (!ofile){
			cerr<< "warning parameters could not be saved.." << endl;
			return false;
		}

		ofile << *p;

		ofile.close();
		return true;
	}



};


#endif /* FULLFILEWRITER_H_ */
