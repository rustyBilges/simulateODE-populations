/*
 * IVariableTracker.h
 *
 *  Created on: 19 Jun 2014
 *      Author: rusty
 */

#ifndef IVARIABLETRACKER_H_
#define IVARIABLETRACKER_H_

#include <vector>
using namespace std;

class IVariableTracker{

public:
	virtual ~IVariableTracker(){}
	virtual void store(int masterClock, IFunctionalResponse* response) = 0;
	virtual bool save(string suffix, int simID) = 0;
	virtual bool returnSeries(vector<vector <vector<double> > >& series) = 0;
};


class BiomassTracker:public IVariableTracker
{

private:
	int unitCount, timestepCount;
	double dt;
	RM_Nsp_Parameters* parameters;
	IDynamicalUnit** units;

	vector <vector<vector<double> > >biomassTimeseries;

public:
	~BiomassTracker(){}
	BiomassTracker(int timestepCount, int unitCount, double dt, RM_Nsp_Parameters* parameters, IDynamicalUnit** units){

		this->unitCount         = unitCount;
		this->timestepCount     = timestepCount;
		this-> dt               = dt;
		this->parameters        = parameters;
		this->units             = units;

		biomassTimeseries.resize(timestepCount+1,vector<vector <double> >(unitCount,vector<double>(unitCount,0)));       // only stores Biomass flows between species which are directly interacting in the model

	}

	void store(int masterClock, IFunctionalResponse* response){
		for (int i=0;i<unitCount;i++){
			for (int j=0; j<unitCount;j++){
				biomassTimeseries.at(masterClock).at(i).at(j) = response->getResponse(i,j,unitCount,parameters->couplingMatrix,units, masterClock, parameters);
			}
		}
	}

	bool save(string suffix, int simID){

		stringstream ID_ss;
		ID_ss << simID;
		string fname = "sim"+ID_ss.str()+"_"+suffix+".biomassTimeseries";
		ofstream ofile;
		ofile.open(fname.c_str());
		if (!ofile){return false;}

		print(ofile);

		ofile.close();

		return true;
	}


	void print(ofstream& os){

		for (int t=0; t<=timestepCount; t++){
			os << t*dt << ", ";
			for (int i=0;i<unitCount;i++){
				for (int j=0; j<unitCount;j++){
					os << biomassTimeseries.at(t).at(i).at(j) << ", ";
				}
			}
			os << endl;
		}

	}

	bool returnSeries(vector<vector <vector<double> > >& series){

		try
		{
			series = biomassTimeseries;
		}
		catch(int e)
		{
			return false;
		}
		return true;
	}
};

class InteractionTracker:public IVariableTracker
{

private:
	int unitCount, timestepCount;
	double dt;
	RM_Nsp_Parameters* parameters;
	IDynamicalUnit** units;

	vector <vector<vector<double> > >interactionTimeseries;

public:
	~InteractionTracker(){}
	InteractionTracker(int timestepCount, int unitCount, double dt, RM_Nsp_Parameters* parameters, IDynamicalUnit** units){

		this->unitCount         = unitCount;
		this->timestepCount     = timestepCount;
		this-> dt               = dt;
		this->parameters        = parameters;
		this->units             = units;

		interactionTimeseries.resize(timestepCount+1,vector<vector <double> >(unitCount,vector<double>(unitCount,0)));       // only stores Biomass flows between species which are directly interacting in the model

	}

	void store(int masterClock, IFunctionalResponse* response){
		for (int i=0;i<unitCount;i++){
			for (int j=0; j<unitCount;j++){
				interactionTimeseries.at(masterClock).at(i).at(j) = response->getInteractionStrength(i,j,unitCount,parameters->couplingMatrix,units, masterClock, parameters);
			}
		}
	}

	bool save(string suffix, int simID){

		stringstream ID_ss;
		ID_ss << simID;
		string fname = "sim"+ID_ss.str()+"_"+suffix+".interactionTimeseries";
		ofstream ofile;
		ofile.open(fname.c_str());
		if (!ofile){return false;}

		print(ofile);

		ofile.close();

		return true;
	}

	void print(ofstream& os){

		for (int t=0; t<=timestepCount; t++){
			os << t*dt << ", ";
			for (int i=0;i<unitCount;i++){
				for (int j=0; j<unitCount;j++){
					os << interactionTimeseries.at(t).at(i).at(j) << ", ";
				}
			}
			os << endl;
		}

	}

	bool returnSeries(vector<vector <vector<double> > >& series){

		try
		{
			series = interactionTimeseries;
		}
		catch(int e)
		{
			return false;
		}
		return true;
	}
};


class NullTracker:public IVariableTracker
{

private:

public:
	~NullTracker(){}
	NullTracker(){

	}

	void store(int masterClock, IFunctionalResponse* response){}

	bool save(string suffix, int simID){return true;}

	bool returnSeries(vector<vector <vector<double> > >& series){

		return true;
	}

};

#endif /* IVARIABLETRACKER_H_ */
