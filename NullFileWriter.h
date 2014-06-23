/*
 * NullFileWriter.h
 *
 *  Created on: 25 Feb 2014
 *      Author: rusty
 */

#ifndef NULLFILEWRITER_H_
#define NULLFILEWRITER_H_


#include "IFileWriter.h"
#include "IDynamicalUnit.h"

#include <vector>
using namespace std;

class NullFileWriter : public IFileWriter{

private:



public:
	NullFileWriter(){

	}

	void writeSimulationHistoryToFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, int unitDimension, double dt){
	}

	void writeSimulationLogFile(int simulationID, int unitCount, IDynamicalUnit** units, int nTimesteps, int unitDimension, double dt, int parametersPerUnit, INetwork* network){
	}

	void writeNetwork(int unitCount, INetwork* network, ofstream& oFile){
	}

	void writeInitialState(int i,IDynamicalUnit** units, int unitDimension, ofstream& oFile){
	}

	void writeUnitParameters(int i,IDynamicalUnit** units, int parametersPerUnit, ofstream& oFile){
	}


};




#endif /* NULLFILEWRITER_H_ */
