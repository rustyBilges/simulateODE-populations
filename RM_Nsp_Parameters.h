/*
 * RM_Nsp_Parameters.h
 *
 *  Created on: 28 Apr 2014
 *      Author: rusty
 */

#ifndef RM_NSP_PARAMETERS_H_
#define RM_NSP_PARAMETERS_H_


#include <iostream>
//#include <armadillo>    // NEEDED WHY??!!
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "IDynamicalUnit.h"
using namespace std;

class RM_Nsp_Parameters
{

public:

	int ID;  // each parameter set to have a unique ID
	int unitCount;
	int basalCount;

	vector <double> growthRates;
	vector <double> carryingCapacities;
	vector <double> saturationConstants;

	vector <vector <double > > couplingMatrix;  // A

private:

	bool checkIfBasalSpecies(int unitID){

		for (unsigned int i=0; i<couplingMatrix.size(); i++){
			if (couplingMatrix.at(unitID).at(i)>0){
				return false;
			}
		}
		// otherwise it must be a basal species..(primary producer)
		return true;
	}


public:
//	RM_Nsp_Parameters(int ID, double* parameters){}
//	RM_Nsp_Parameters(int ID, string suffix);{}
	RM_Nsp_Parameters(int ID, int unitCount, string fname): ID(ID),unitCount(unitCount),basalCount(0)
	{
		// READS PARAMETERS FROM A FILE (NEW FORMAT)
		ifstream file;
		file.open(fname.c_str());
		if(!file.is_open()){
			cerr << "warning, parameter file not open..DO NOT TRUST THESE RESULTS.." << endl;

		}

		couplingMatrix.resize(unitCount, vector<double>(unitCount,0));

		int lineNumber = 1;
		string temp;
		double tempD;
		string dummy; // to eat commas!

		while(getline(file, temp, '\n')){
			istringstream ss(temp);
//			cout << ss.str() << endl;
			if (lineNumber==3){
					while(ss){
						ss >> tempD >> dummy;
						growthRates.push_back(tempD);
					}
			}
			else if (lineNumber==5){
				while(ss){
					ss >> tempD >> dummy;
					carryingCapacities.push_back(tempD);
				}
			}
			else if (lineNumber==7){
				while(ss){
					ss >> tempD >> dummy;
					saturationConstants.push_back(tempD);
				}
			}
			else if (lineNumber>8 && lineNumber<9+unitCount){
				int columnCount=0;
				while(ss){
					ss >> tempD >> dummy;
					if (columnCount<unitCount){
						couplingMatrix.at(lineNumber-9).at(columnCount) = tempD;
					}
					columnCount++;
//					couplingMatrix.at(lineNumber-8).push_back(tempD);
				}
			}

			lineNumber++;
		}
		file.close();

	}

	RM_Nsp_Parameters(int ID, int unitCount, int rmax=1, int kmax=100, int a01min=-1, int Ksmax=10, int rmin = -1, int a10max=1){
//	RM_Nsp_Parameters(int ID, int unitCount, int rmax=5, int kmax=1000, int a01min=-1, int Ksmax=1, int rmin = -0.1, int a10max=1){

		this->ID = ID;
		this->unitCount = unitCount;
		this->basalCount  = 0;

		couplingMatrix.resize(unitCount, vector <double> (unitCount, 0));
		growthRates.resize(unitCount, 0);
		carryingCapacities.resize(unitCount, 0);
		saturationConstants.resize(unitCount, 0);

		if (unitCount==2){
			// default coupling matrix, 2 species for now:

			couplingMatrix.at(0).at(0) = 0;
			couplingMatrix.at(0).at(1) = a01min*((double)rand()/RAND_MAX);
			couplingMatrix.at(1).at(0) = a10max*((double)rand()/RAND_MAX);
			couplingMatrix.at(1).at(1) = 0;

		}
		else if (unitCount==4){
			//diamond network:

			couplingMatrix.at(0).at(1) = a01min*((double)rand()/RAND_MAX);
			couplingMatrix.at(0).at(2) = a01min*((double)rand()/RAND_MAX);
			couplingMatrix.at(1).at(0) = a10max*((double)rand()/RAND_MAX);
			couplingMatrix.at(1).at(3) = a01min*((double)rand()/RAND_MAX);
			couplingMatrix.at(2).at(0) = a10max*((double)rand()/RAND_MAX);
			couplingMatrix.at(2).at(3) = a01min*((double)rand()/RAND_MAX);
			couplingMatrix.at(3).at(1) = a10max*((double)rand()/RAND_MAX);
			couplingMatrix.at(3).at(2) = a10max*((double)rand()/RAND_MAX);
		}

		for (int i=0; i<unitCount; i++){
			if(checkIfBasalSpecies(i)){
				basalCount++;
				growthRates.at(i) = (rmax*((double)rand()/RAND_MAX));
				carryingCapacities.at(i) = (kmax*((double)rand()/RAND_MAX));
			}
			else {
				growthRates.at(i) = (rmin*((double)rand()/RAND_MAX));
				saturationConstants.at(i) = (Ksmax*((double)rand()/RAND_MAX));
			}
		}


	}

	RM_Nsp_Parameters(int ID, vector< vector<double> > adjacency, int rmax=3, int kmax=100, double a01min=-1, int Ksmax=10, double rmin = -1, double a10max=1){
	//	RM_Nsp_Parameters(int ID, int unitCount, int rmax=5, int kmax=1000, int a01min=-1, int Ksmax=1, int rmin = -0.1, int a10max=1){

			this->ID = ID;
			this->unitCount = adjacency.size();
			this->basalCount  = 0;

			couplingMatrix.resize(unitCount, vector <double> (unitCount, 0));
			growthRates.resize(unitCount, 0);
			carryingCapacities.resize(unitCount, 0);
			saturationConstants.resize(unitCount, 0);


			for (int i=0; i<unitCount; i++){
				for (int j=0; j<unitCount; j++){
					if (adjacency.at(i).at(j)>0){
//						couplingMatrix.at(i).at(j) = 0.5*a10max*((double)rand()/RAND_MAX); //!!!
						couplingMatrix.at(i).at(j) = a10max*((double)rand()/RAND_MAX);
					}
					else if (adjacency.at(i).at(j)<0){
						couplingMatrix.at(i).at(j) = a01min*((double)rand()/RAND_MAX);
					}
				}
			}

			for (int i=0; i<unitCount; i++){
				if(checkIfBasalSpecies(i)){
					basalCount++;
					growthRates.at(i) = (rmax*((double)rand()/RAND_MAX));
					carryingCapacities.at(i) = (kmax*((double)rand()/RAND_MAX));
				}
				else {
					growthRates.at(i) = (rmin*((double)rand()/RAND_MAX));
					saturationConstants.at(i) = (Ksmax*((double)rand()/RAND_MAX));
				}
			}


		}


	RM_Nsp_Parameters(int ID){

		// test parameters: unstable spiral
		this->ID = ID;
		this->unitCount = 2;
		this->basalCount = 1;

		// default coupling matrix, 2 species for now:
		//UNSTABLE SPIRAL -> RUNAWAY POPULATION UNLESS START NEAR STATIONARY POINT
		couplingMatrix.resize(unitCount, vector <double> (unitCount, 0));
		couplingMatrix.at(0).at(0) = 0;
		couplingMatrix.at(0).at(1) = -0.254906;
		couplingMatrix.at(1).at(0) = 0.623052;
		couplingMatrix.at(1).at(1) = 0;

		growthRates.resize(unitCount, 0);
		carryingCapacities.resize(unitCount, 0);
		saturationConstants.resize(unitCount, 0);

		growthRates.at(0) = 0.23117;
		growthRates.at(1) = -0.230395;
		carryingCapacities.at(0) = 26.3078;
		saturationConstants.at(1) = 6.81433;

		// STABLE SPIRAL (NUMBER9)
//		couplingMatrix.at(0).at(0) = 0;
//		couplingMatrix.at(0).at(1) = -0.189394;
//		couplingMatrix.at(1).at(0) = 0.141317;
//		couplingMatrix.at(1).at(1) = 0;
//
//		growthRates.resize(unitCount, 0);
//		carryingCapacities.resize(unitCount, 0);
//		saturationConstants.resize(unitCount, 0);
//
//		growthRates.at(0) = 0.284182;
//		growthRates.at(1) = -0.0613354;
//		carryingCapacities.at(0) = 21.476;
//		saturationConstants.at(1) = 8.59469;

	}

	~RM_Nsp_Parameters(){}


	void printParameters(){}
	void about(){}
	bool findEquilibrium(){return true;}
	bool calculateJacobianElements(){return true;}
	bool classifyStability(){return true;}

	bool classifyParameterSet(){

		if(!findEquilibrium())          {return false;}
		if(!calculateJacobianElements()){return false;}
		if(!classifyStability())        {return false;}

		return true;
	}

	bool saveParameters(){
//		ofstream ofile;
//		string fname = ID+"_RM_Nsp.parameters";
//		ofile.open(fname.c_str());
//		if (!ofile){
//			cerr<< "warning parameters could not be saved.." << endl;
//			return false;
//		}
//
//		ofile << this;  // does this work??!!
//
//		ofile.close();
		return true;
	}

	bool initialiseUnits(IDynamicalUnit** units);

};


inline std::ostream &operator << (std::ostream &f, const RM_Nsp_Parameters &params)
{
    f << "Model parameters for this general ODE simulation." << endl;
    f << "growth rates:" << endl;
    for (int i=0; i<params.unitCount; i++){
    	f << params.growthRates.at(i) << ", ";
    }
    f << endl << "carrying capacities:" << endl;
    for (int i=0; i<params.unitCount; i++){
    	f << params.carryingCapacities.at(i) << ", ";
    }
    f << endl << "saturation constants:" << endl;
    for (int i=0; i<params.unitCount; i++){
    	f << params.saturationConstants.at(i) << ", ";
    }
    f << endl << "coupling matrix (A):" << endl;
    for (int i=0; i<params.unitCount; i++){
    	for (int j=0; j<params.unitCount; j++){
    	f << params.couplingMatrix.at(i).at(j) << ", ";
    	}
    	f << endl;
    }
    f << endl << endl;
    return f;
}



#endif /* RM_NSP_PARAMETERS_H_ */

