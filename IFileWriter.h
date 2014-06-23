/*
 * IFileWriter.h
 *
 *  Created on: 19 Dec 2013
 *      Author: rusty
 */

#ifndef IFILEWRITER_H_
#define IFILEWRITER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "RM_Nsp_Parameters.h"
using namespace std;

class IFileWriter{
	/*
		 * INetwork interface header

		 * Extend this class to help build different networks?
		 * e.g. method: setij(int i, int j) etc.
		 *
		 */

public:
	virtual bool writeSimulationHistoryToFile(int simulationID, int unitCount, IDynamicalUnit** untis, int nTimesteps, double dt) = 0;
	virtual void writeSimulationLogFile(int simulationID, int unitCount, IDynamicalUnit** untis, int nTimesteps, double dt, string modelType, string responseType) = 0;
	virtual bool saveParameters(int ID, RM_Nsp_Parameters* p) = 0;
	virtual      ~IFileWriter(){}
};




#endif /* IFILEWRITER_H_ */
