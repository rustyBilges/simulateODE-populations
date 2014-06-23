/*
 * RM_Nsp_unit.h
 *
 *  Created on: 28 Apr 2014
 *      Author: rusty
 */

#ifndef RM_NSP_UNIT_H_
#define RM_NSP_UNIT_H_


#include "IDynamicalUnit.h"
#include "RM_Nsp_Parameters.h"
#include "IFunctionalResponse.h"
#include "IIntrinsicGrowthFunction.h"
#include <vector>
using namespace std;

class RM_Nsp_unit : public IDynamicalUnit
{
public:
	RM_Nsp_Parameters* parameters;
	IFunctionalResponse* response; //!
private:
	string  unitType;
	int          unitID, nParams, nTimesteps;
	double state;
	UnitHistory* history;

	IIntrinsicGrowthFunction* intrinsicGrowthFunction;

	vector <vector<double > >& connectionMatrix;

public:

	~RM_Nsp_unit(){
		delete history;
	}
	//depricated constructor:
	RM_Nsp_unit(int ID, int tsteps, vector <vector <double > >& connectionMatrix, RM_Nsp_Parameters* parameters, string type = "Holling_II");
	RM_Nsp_unit(int ID, int tsteps, vector <vector <double > >& connectionMatrix, RM_Nsp_Parameters* parameters, IFunctionalResponse* response, string modelType="RM");

	void setInitialState(double st);
	void setState(double st);
	void randomiseState(double sMin, double sMax);

	double  getState();
	int     getDim();
	void    printState();

	double getXdot(IDynamicalUnit **allUnits, int nUnits, int masterClock);


	UnitHistory* getHistory();
	string getUnitType();

private:

	double unifRand(double rMin, double rMax);

	bool checkIfBasalSpecies(){

		for (unsigned int i=0; i<connectionMatrix.size(); i++){
			if (connectionMatrix.at(unitID).at(i)>0){
				return false;
			}
		}
		// otherwise it must be a basal species..(primary producer)
		return true;
	}

	int countInteractions(){
		int count=0;
		for (unsigned int i=0; i<connectionMatrix.size(); i++){
			if(connectionMatrix.at(unitID).at(i)!=0){
				count++;
			}
		}
		return count;
	}


};




#endif /* RM_NSP_UNIT_H_ */
