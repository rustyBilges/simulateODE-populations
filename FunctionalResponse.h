/*
 * FunctionalResponse.h
 *
 *  Created on: 7 May 2014
 *      Author: rusty
 */

#ifndef FUNCTIONALRESPONSE_H_
#define FUNCTIONALRESPONSE_H_

#include <fstream>
#include <math.h>
using namespace std;

class FunctionalResponse{

private:
	string type;  // Linear, Holling I,II,III. Other?
	string topology;  // currently used to distinguish between CE and AC in 3 species simulations

public:
	FunctionalResponse(string _type):type(_type),topology("NA"){} // topology not required for 2 species simulations
	FunctionalResponse(string _type, string _topology):type(_type),topology(_topology){}

//	friend RM_Nsp_unit;
	// return double based on types. needs access to units[i], parameters
	// this class should also be responsible for evaluating simulation IM elements, since this is based on analytic derivation of the functional response

	double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

		double response = 0;

		if (type=="Holling_II"){
			return Holling_II_response(unitID, j, unitCount, connectionMatrix, allUnits, masterClock, parameters);
		}
		else if (type=="Holling_III"){
			return Holling_III_response(unitID, j, unitCount, connectionMatrix, allUnits, masterClock, parameters);
		}
		else if (type=="Holling_II_no_passive_switching"){
			return Holling_II_response_no_passive_switching(unitID, j, unitCount, connectionMatrix, allUnits, masterClock, parameters);
		}
		else {
			cout << "WARNING REPSONSE TYPE UNKNOWN: SPEICES INTERACTION = 0" << endl;
		}

		return response;
	}

	bool evaluateSimulationIM(RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount, vector <double>& IM){

		// only works for Holling II and III:
		if(!evaluateSimulationIM_Holling(parameters, units, unitCount, timestepCount, IM)){return false;}


		return true;
	}

	bool saveInteractionFunctions(int unitCount, int timestepCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** units, RM_Nsp_Parameters* parameters){
		// this saves the interaction functions by evaluating the simulation IM elemtents of the course of the simulation
		// i.e. it assumes that we are interested in the whole simulation rather than just the sampling period
		stringstream ID_ss;
		ID_ss << parameters->ID;
		string fname = type+"_interaction_function_params"+ID_ss.str()+".fn";
		ofstream ofile;
		ofile.open(fname.c_str());

		if(!save_Holling_interaction(ofile, parameters, units, unitCount, timestepCount, connectionMatrix)){ofile.close(); return false;}

		ofile.close();
		return true;
	}

	bool saveFunctionalResponse(int unitCount, int timestepCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** units, RM_Nsp_Parameters* parameters){
		// this saves the interaction functions by evaluating the simulation IM elemtents of the course of the simulation
		// i.e. it assumes that we are interested in the whole simulation rather than just the sampling period
		stringstream ID_ss;
		ID_ss << parameters->ID;
		string fname = type+"_FUNCTIONALRESPONSE_params"+ID_ss.str()+".fn";
		ofstream ofile;
		ofile.open(fname.c_str());

		if(!save_Holling_functional(ofile, parameters, units, unitCount, timestepCount, connectionMatrix)){ofile.close(); return false;}

		ofile.close();
		return true;
	}



	bool saveIM_timseries(RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount, double timestepSize){
		// this to calculate the Interaction Matrix (with elements averaged over the inference sampling period)
		// current implementation for RM 2species with Holling Type2 and linear predator mortality. Need to add more
		// for other functional response forms.

		ofstream ofile;
		ofile.open("IM_timseries.ts");

		// assign parameters for ease of use, this asusmes that prey is species0
		double r0 = parameters->growthRates.at(0);
	//	double r1 = parameters->growthRates.at(1);
		double K0 = parameters->carryingCapacities.at(0);
		double Ks = parameters->saturationConstants.at(1);
		double a01 = parameters->couplingMatrix.at(0).at(1);
		double a10 = parameters->couplingMatrix.at(1).at(0);

		double x0 = 0;
		double x1 = 0;

		for (int t=0; t<timestepCount; t++){

			x0 = units[0]->getHistory()->getHistoryTi(t);
			x1 = units[1]->getHistory()->getHistoryTi(t);

			ofile << timestepSize*t << ", ";

			if (type=="Holling_II"){
				ofile << -(r0/K0)-x1*a01/pow(Ks + x0,2) << ", ";
				ofile << a01/(Ks + x0)<< ", ";
				ofile << a10/(Ks + x0) -x0*a10/pow(Ks + x0,2)<< ", ";
				ofile << 0<< endl;
			}
			else if (type=="Holling_III"){

				double alpha = 2; // default

				ofile << 0 << ", ";                        // it doesn't really but we are not interested in self-interactions for now
				ofile << (pow(x0,alpha-1)*a01)/(pow(Ks,alpha) + pow(x0,alpha)) << ", ";
				ofile << alpha*a10*pow(x0,alpha-1)/(pow(Ks,alpha) + pow(x0,alpha)) -alpha*pow(x0,alpha+1)*a10/pow(pow(Ks,alpha) + pow(x0,alpha),2) << ", ";
				ofile << 0 << endl;;
			}

		}

		ofile.close();
		return true;
	}


	bool saveBiomassFlows(RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount, double timestepSize){
		// this to calculate the Interaction Matrix (with elements averaged over the inference sampling period)
		// current implementation for RM 2species with Holling Type2 and linear predator mortality. Need to add more
		// for other functional response forms.

		ofstream ofile;
		ofile.open("Biomass_timseries.ts");

		// assign parameters for ease of use, this asusmes that prey is species0
		double r0 = parameters->growthRates.at(0);
	//	double r1 = parameters->growthRates.at(1);
		double K0 = parameters->carryingCapacities.at(0);
		double Ks = parameters->saturationConstants.at(1);
		double a01 = parameters->couplingMatrix.at(0).at(1);
		double a10 = parameters->couplingMatrix.at(1).at(0);

		double x0 = 0;
		double x1 = 0;

		for (int t=0; t<timestepCount; t++){

			x0 = units[0]->getHistory()->getHistoryTi(t);
			x1 = units[1]->getHistory()->getHistoryTi(t);

			ofile << timestepSize*t << ", ";

			if (type=="Holling_II"){
//				ofile << -(r0/K0)-x1*a01/pow(Ks + x0,2) << ", ";
				ofile << a01*x0*x1/(Ks + x0)<< ", ";
				ofile << a10*x0*x1/(Ks + x0) <<endl;
			}
			else if (type=="Holling_III"){

				double alpha = 2; // default
				ofile << (pow(x0,alpha)*a01*x1)/(pow(Ks,alpha) + pow(x0,alpha)) << ", ";
				ofile << alpha*a10*pow(x0,alpha)*x1/(pow(Ks,alpha) + pow(x0,alpha)) << endl;
			}

		}

		ofile.close();
		return true;
	}



private:

	double Holling_II_response(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
		// unitID is the unit calling the response function
		// j is the species to calculate coupling with
		double Ks = 0;
		double aij = connectionMatrix.at(unitID).at(j);
		double xi  = allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
		double xj  = allUnits[j]->getHistory()->getHistoryTi(masterClock);

		double denominator = 0;
		if (aij==0){
			return 0;
		}
		else if(aij>0){
			// species i is the predator..
			Ks = parameters->saturationConstants.at(unitID);
			denominator += Ks;
			for (int i=0; i<unitCount; i++){
				if(connectionMatrix.at(unitID).at(i)>0){
					denominator += allUnits[i]->getHistory()->getHistoryTi(masterClock);
				}
			}
		}
		else {
			// species i is being predated by species j
			Ks = parameters->saturationConstants.at(j);
			denominator += Ks;
			for (int i=0; i<unitCount; i++){
				if(connectionMatrix.at(j).at(i)>0){
					denominator += allUnits[i]->getHistory()->getHistoryTi(masterClock);
				}
			}
		}

		return (aij*xi*xj)/(denominator);
//		return (xi*xj)/(xi+Ks);

	}

	double Holling_II_response_no_passive_switching(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
		// unitID is the unit calling the response function
		// j is the species to calculate coupling with

		// this version does not include passive prey switching, such that the denominator is simple and there ar eonly pairwise interactions..
		double Ks = 0;
		double aij = connectionMatrix.at(unitID).at(j);
		double xi  = allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
		double xj  = allUnits[j]->getHistory()->getHistoryTi(masterClock);

		double denominator = 0;
		if (aij==0){
			return 0;
		}
		else if(aij>0){
			// species i is the predator..
			Ks = parameters->saturationConstants.at(unitID);
			denominator += allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
			denominator += Ks;
		}
		else {
			// species i is being predated by species j
			Ks = parameters->saturationConstants.at(j);
			denominator += Ks;
			denominator += allUnits[j]->getHistory()->getHistoryTi(masterClock);
		}

		return (aij*xi*xj)/(denominator);
//		return (xi*xj)/(xi+Ks);

	}


	double Holling_III_response(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

			double alpha = 2; // Holling type3 has an exponent which we set as default=2.

			// unitID is the unit calling the response function
			// j is the species to calculate coupling with
			double Ks = 0;
			double aij = connectionMatrix.at(unitID).at(j);
			double xi  = allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
			double xj  = allUnits[j]->getHistory()->getHistoryTi(masterClock);

			double denominator = 0;
			if (aij==0){
				return 0;
			}
			else if(aij>0){
				// species i is the predator..
				Ks = parameters->saturationConstants.at(unitID);
				denominator += pow(Ks,alpha);
				for (int i=0; i<unitCount; i++){
					if(connectionMatrix.at(unitID).at(i)>0){
						denominator += pow(allUnits[i]->getHistory()->getHistoryTi(masterClock), alpha);
					}
				}
			}
			else {
				// species i is being predated by species j
				Ks = parameters->saturationConstants.at(j);
				denominator +=pow( Ks, alpha);
				for (int i=0; i<unitCount; i++){
					if(connectionMatrix.at(j).at(i)>0){
						denominator += pow(allUnits[i]->getHistory()->getHistoryTi(masterClock),alpha);
					}
				}
			}
			if(aij>0){
				return (aij*pow(xj,alpha)*xi)/(denominator);
			}
			else{
				return (aij*pow(xi,alpha)*xj)/(denominator);
			}

		}

	bool evaluateSimulationIM_Holling(RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount, vector <double>& IM){
		// this to calculate the Interaction Matrix (with elements averaged over the inference sampling period)
		// current implementation for RM 2species with Holling Type2 and linear predator mortality. Need to add more
		// for other functional response forms.

		// IM must be passed in with zero elements
		// make sure the same parameters are passed in as are stored by the units!!
		if (IM.size()!=(unsigned int)unitCount*unitCount){
			cerr << "wrong number of elements in IM" << endl;
			return false;
		}
		// assign parameters for ease of use, this asusmes that prey is species0
		double r0 = parameters->growthRates.at(0);
	//	double r1 = parameters->growthRates.at(1);
		double K0 = parameters->carryingCapacities.at(0);
		double Ks = parameters->saturationConstants.at(1);
		double a01 = parameters->couplingMatrix.at(0).at(1);
		double a10 = parameters->couplingMatrix.at(1).at(0);

		double x0 = 0;
		double x1 = 0;

		for (int t=0; t<timestepCount; t++){

			x0 = units[0]->getHistory()->getHistoryTi(t);
			x1 = units[1]->getHistory()->getHistoryTi(t);

			if (type=="Holling_II"){
				IM.at(0) += -(r0/K0)-x1*a01/pow(Ks + x0,2);
				IM.at(1) += a01/(Ks + x0);
				IM.at(2) += a10/(Ks + x0) -x0*a10/pow(Ks + x0,2);
				IM.at(3) += 0;
			}
			else if (type=="Holling_III"){

				double alpha = 2; // default

				IM.at(0) += 0;                        // it doesn't really but we are not interested in self-interactions for now
				IM.at(1) += (pow(x0,alpha-1)*a01)/(pow(Ks,alpha) + pow(x0,alpha));
				IM.at(2) += alpha*a10*pow(x0,alpha-1)/(pow(Ks,alpha) + pow(x0,alpha)) -alpha*pow(x0,alpha+1)*a10/pow(pow(Ks,alpha) + pow(x0,alpha),2);  //right yeah??
				IM.at(3) += 0;
			}

		}

		for (unsigned int i =0; i<IM.size(); i++){
			IM.at(i) = IM.at(i)/timestepCount;      // take the mean value over the sampling period
		}

		return true;
	}

	bool save_Holling_interaction(ofstream& ofile, RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount,vector <vector <double> >& connectionMatrix){
		// save the interactions as a function of prey density
		double prey_min = 0;
		double prey_max = 0;

		if (unitCount==2){

			prey_max =parameters->carryingCapacities.at(0);
			// generate and save functional response:
			double Ks = parameters->saturationConstants.at(1);
			double a01 = parameters->couplingMatrix.at(0).at(1);
			double a10 = parameters->couplingMatrix.at(1).at(0);

			if (type=="Holling_II"){
				for (double x0=prey_min; x0<prey_max; x0+=0.1){
					ofile << x0 << ",  " << a01/(Ks + x0) << ",  " <<  a10/(Ks + x0) -x0*a10/pow(Ks + x0,2) << endl;
				}
			}
			else if (type=="Holling_III"){

				double alpha = 2;

				for (double x0=prey_min; x0<prey_max; x0+=0.1){
					ofile << x0 << ",  " << (pow(x0,alpha-1)*a01)/(pow(Ks,alpha) + pow(x0,alpha)) << ",  " << alpha*a10*pow(x0,alpha-1)/(pow(Ks,alpha) + pow(x0,alpha)) -alpha*pow(x0,alpha+1)*a10/pow(pow(Ks,alpha) + pow(x0,alpha),2) << endl;
				}
			}
		}
		else if(unitCount==3){
			// now we assume HOlling II with CE topology. NEED TO GENERALISE THIS...

			if (topology=="CE"){
				if(!checkIfBasalSpecies(0, connectionMatrix)){
					cerr << "method save_Holling_interaction of class FunctionalResponse assumes that species O is the primary producer in a 3species CE topology" << endl;
					return false;
				}
				prey_max =parameters->carryingCapacities.at(0);
				double Ks1 = parameters->saturationConstants.at(1);
				double Ks2 = parameters->saturationConstants.at(2);  // there are two predators
				double a01 = parameters->couplingMatrix.at(0).at(1);
				double a02 = parameters->couplingMatrix.at(0).at(2);
				double a10 = parameters->couplingMatrix.at(1).at(0);
				double a20 = parameters->couplingMatrix.at(2).at(0);

				// columns to output: preyX | A01 | A02 | A10 | A12=0 | A20 | A21=0
				if (type=="Holling_II"){
					for (double x0=prey_min; x0<prey_max; x0+=0.1){
						ofile << x0 << ",  " << a01/(Ks1 + x0) << ",  " <<  a02/(Ks2 + x0) << ", " <<  a10/(Ks1 + x0) -x0*a10/pow(Ks1 + x0,2) << ", " << 0 << ", " << a20/(Ks2 + x0) -x0*a20/pow(Ks2 + x0,2) << ", " << 0 << endl;
					}
				}
			}
			else if (topology=="AC"){
				// this needs working on because the function is actually a surface: (plot it for now at half carrying capacity of prey2)

				if(!checkIfBasalSpecies(0, connectionMatrix) || !checkIfBasalSpecies(1, connectionMatrix)){
					cerr << "method save_Holling_interaction of class FunctionalResponse assumes that species O and 1 are primary producers in a 3species AC topology" << endl;
					return false;
				}

				prey_max =parameters->carryingCapacities.at(0);
				double x1 = parameters->carryingCapacities.at(1)/2;  // prey2_half_capacity
				double x2 = parameters->saturationConstants.at(1)*4; // abitrary predator population level

				double Ks = parameters->saturationConstants.at(1);

//				double a01 = parameters->couplingMatrix.at(0).at(1);
				double a02 = parameters->couplingMatrix.at(0).at(2);
//				double a10 = parameters->couplingMatrix.at(1).at(0);
				double a12 = parameters->couplingMatrix.at(1).at(2);
				double a20 = parameters->couplingMatrix.at(2).at(0);
				double a21 = parameters->couplingMatrix.at(2).at(1);

				// columns to output: preyX | A01 | A02 | A10 | A12 | A20 | A21
				if (type=="Holling_II"){
					for (double x0=prey_min; x0<prey_max; x0+=0.1){
						ofile << x0 << ",  " << -a02*x2/(Ks + x0 + x1) << ",  " <<  a02/(Ks + x0 + x1) << ", " <<  -a12*x2/pow(Ks + x0 + x1,2) << ", " << a12/(Ks + x0 + x1) << ", " << a20/(Ks + x0 + x1) -x0*a20/pow(Ks + x0 + x1,2) -x1*a21/pow(Ks + x0 + x1,2)  << ", " << a21/(Ks + x0 + x1) -x0*a20/pow(Ks + x0 + x1,2) -x1*a21/pow(Ks + x0 + x1,2) << endl;
					}
				}
			}



		}
		return true;
	}

	bool save_Holling_functional(ofstream& ofile, RM_Nsp_Parameters* parameters, IDynamicalUnit** units, int unitCount, int timestepCount,vector <vector <double> >& connectionMatrix){
			// save the interactions as a function of prey density
			double prey_min = 0;
			double prey_max = 0;

			if (unitCount!=2){
				cerr << "only implemented functional saving for 2 species" << endl;
				return false;
			}

			prey_max =parameters->carryingCapacities.at(0);
			// generate and save functional response:
			double Ks = parameters->saturationConstants.at(1);
			double a01 = parameters->couplingMatrix.at(0).at(1);
			double a10 = parameters->couplingMatrix.at(1).at(0);

			if (type=="Holling_II"){
				for (double x0=prey_min; x0<prey_max; x0+=0.1){
					ofile << x0 << ",  " << a01*x0/(Ks + x0) << ",  " <<  a10*x0/(Ks + x0) << endl;
				}
			}
			else if (type=="Holling_III"){

				double alpha = 2;

				for (double x0=prey_min; x0<prey_max; x0+=0.1){
					ofile << x0 << ",  " << (pow(x0,alpha)*a01)/(pow(Ks,alpha) + pow(x0,alpha)) << ",  " << alpha*a10*pow(x0,alpha)/(pow(Ks,alpha) + pow(x0,alpha))  << endl;
				}
			}

			return true;
		}


private:
	bool checkIfBasalSpecies(int unitID, vector <vector<double> >& connectionMatrix){

		for (unsigned int i=0; i<connectionMatrix.size(); i++){
			if (connectionMatrix.at(unitID).at(i)>0){
				return false;
			}
		}
		// otherwise it must be a basal species..(primary producer)
		return true;
	}
};


#endif /* FUNCTIONALRESPONSE_H_ */
