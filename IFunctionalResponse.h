/*
 * IFunctionalResponse.h
 *
 *  Created on: 18 Jun 2014
 *      Author: rusty
 */

#ifndef IFUNCTIONALRESPONSE_H_
#define IFUNCTIONALRESPONSE_H_

#include <math.h>
#include "RM_Nsp_Parameters.h"
#include "IDynamicalUnit.h"
using namespace std;

class IFunctionalResponse{

public:

	virtual double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters) = 0;
	virtual double getInteractionStrength(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters)=0;
	virtual ~IFunctionalResponse(){}

};

class HollingII_FunctionalResponse: public IFunctionalResponse
{
private:

public:

	HollingII_FunctionalResponse(){}
	~HollingII_FunctionalResponse(){}

	double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

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

	double getInteractionStrength(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
		return 0;
	}
};

class HollingII_FunctionalResponse_NoPassiveSwitching: public IFunctionalResponse
{
private:

public:

	HollingII_FunctionalResponse_NoPassiveSwitching(){}
	~HollingII_FunctionalResponse_NoPassiveSwitching(){}

	double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
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
			denominator += allUnits[j]->getHistory()->getHistoryTi(masterClock);   // CHECK THIS!!
			denominator += Ks;
		}
		else {
			// species i is being predated by species j
			Ks = parameters->saturationConstants.at(j);
			denominator += Ks;
			denominator += allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
		}

		return (aij*xi*xj)/(denominator);
//		return (xi*xj)/(xi+Ks);

	}

	double getInteractionStrength(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

		double Ks = 0;
		double aij = connectionMatrix.at(unitID).at(j);
		double x0 = 0;

		if (aij==0){
			return 0;  // ignoring intra-specific interactions for now
		}
		else if(aij>0){
			// species i is the predator..
			x0  = allUnits[j]->getHistory()->getHistoryTi(masterClock);
			Ks = parameters->saturationConstants.at(unitID);
			return aij/(x0+Ks) - aij/pow((x0+Ks),2);
		}
		else {
			// species i is being predated by species j
			x0  = allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
			Ks = parameters->saturationConstants.at(j);
			return aij/(x0+Ks);
		}

	}

};

class HollingIII_FunctionalResponse: public IFunctionalResponse
{
private:
	double alpha;   // Holling type3 has an exponent which we set as default=2.
public:
	HollingIII_FunctionalResponse(double _alpha=2):alpha(_alpha){}
	~HollingIII_FunctionalResponse(){}

	double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

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

	double getInteractionStrength(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
		return 0;
	}
};

class Linear_FunctionalResponse: public IFunctionalResponse
{
private:

public:

	Linear_FunctionalResponse(){}
	~Linear_FunctionalResponse(){}

	double getResponse(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){

		// unitID is the unit calling the response function
		// j is the species to calculate coupling with
		double aij = connectionMatrix.at(unitID).at(j);
		double xi  = allUnits[unitID]->getHistory()->getHistoryTi(masterClock);
		double xj  = allUnits[j]->getHistory()->getHistoryTi(masterClock);

		return (aij*xi*xj);



	}

	double getInteractionStrength(int unitID, int j, int unitCount, vector <vector <double> >& connectionMatrix, IDynamicalUnit** allUnits, int masterClock, RM_Nsp_Parameters* parameters){
		return connectionMatrix.at(unitID).at(j);
	}

};


#endif /* IFUNCTIONALRESPONSE_H_ */
