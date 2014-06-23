/*
 * NicheModel.h
 *
 *  Created on: 30 Apr 2014
 *      Author: rusty
 */

#ifndef NICHEMODEL_H_
#define NICHEMODEL_H_

#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

class NicheModel{

private:
	int unitCount;
	double connectance;
	double realisedConnectance;
	bool cannibals;

	vector <double> nicheValue;    //niche model: ni
	vector <double> rangeCentre;   //niche model: ci
	vector <double> rangeSize;     //niche model: ri
	vector <vector <double> > adjacency;               // currently the generated adj is binary, but use double to allow for weighting later on...

	double unifRand(double rmin, double rmax){
		// in [0,1]
		return ((double)rand()/(double)RAND_MAX)*(rmax-rmin) + rmin;
	}

public:

	NicheModel(int unitCount, double connectance=0.20, bool cannibals=false):unitCount(unitCount),connectance(connectance), realisedConnectance(0), cannibals(cannibals)
	{
		nicheValue.resize(unitCount,0);
		rangeCentre.resize(unitCount,0);
		rangeSize.resize(unitCount,0);

		adjacency.resize(unitCount, vector <double>(unitCount,0));
	}


	bool generate(){

		if (unitCount==2){
			adjacency.at(0).at(1) = -1;
			adjacency.at(1).at(0) =  1;
		}
		else{

			nicheValue.resize(unitCount,0);
			rangeCentre.resize(unitCount,0);
			rangeSize.resize(unitCount,0);

			adjacency.resize(unitCount, vector <double>(unitCount,0));

			assignNicheValues();
			assignRangeSizes();
			assignRangeCentres();

			fillAdjacency();

			vector <int> speciesToReplace;
	//		while (!checkNoIsolates(speciesToReplace)){
	//			replaceSpecies(speciesToReplace);
	//		}

	//		if (!checkNoIsolates(speciesToReplace)){return false;}
		}
			return true;
	}

	bool generate(string topology){

		adjacency.resize(unitCount, vector <double>(unitCount,0));
		if (unitCount!=3){
			cerr << "wrong number of species: niche model attewmpting to generate 3 species topology" << endl;
			return false;
		}

		cout << topology << endl;

		if (topology=="chain"){
			adjacency.at(0).at(1) = -1;
			adjacency.at(1).at(0) = 1;
			adjacency.at(1).at(2) = -1;
			adjacency.at(2).at(1) = 1;
		}
		else if (topology=="CE"){
			adjacency.at(0).at(1) = -1;
			adjacency.at(0).at(2) = -1;
			adjacency.at(1).at(0) = 1;
			adjacency.at(2).at(0) = 1;
		}
		else if (topology=="AC"){
			adjacency.at(0).at(2) = -1;
			adjacency.at(1).at(2) = -1;
			adjacency.at(2).at(0) = 1;
			adjacency.at(2).at(1) = 1;
		}
		else if (topology=="intra"){
			adjacency.at(0).at(1) = -1;
			adjacency.at(0).at(2) = -1;
			adjacency.at(1).at(2) = -1;
			adjacency.at(1).at(0) = 1;
			adjacency.at(2).at(0) = 1;
			adjacency.at(2).at(1) = 1;
		}
		return true;
	}


	bool generate(int checkCount){

		adjacency.resize(unitCount, vector <double>(unitCount,0));
		if (unitCount!=checkCount){
			cerr << "wrong number of species: niche model attempting to generate 2 species topology" << endl;
			return false;
		}

		adjacency.at(0).at(1) = -1;
		adjacency.at(1).at(0) = 1;
		return true;

		if (checkCount==4){

			// two non-interacting 2species systems:
			adjacency.at(0).at(1) = -1;
			adjacency.at(1).at(0) = 1;
			adjacency.at(2).at(3) = -1;
			adjacency.at(3).at(2) = 1;
////			// diamond topology:
//			adjacency.at(0).at(1) = -1;
//			adjacency.at(0).at(2) = -1;
//			adjacency.at(1).at(0) = 1;
//			adjacency.at(1).at(3) = -1;
//			adjacency.at(2).at(0) = 1;
//			adjacency.at(2).at(3) = -1;
//			adjacency.at(3).at(1) = 1;
//			adjacency.at(3).at(2) = 1;
//			// N-shaped topology:
//			adjacency.at(0).at(1) = -1;
//			adjacency.at(0).at(2) = -1;
//			adjacency.at(1).at(0) = 1;
//			adjacency.at(1).at(3) = -1;
//			adjacency.at(2).at(0) = 1;
//			adjacency.at(2).at(3) = -1;
		}
		return true;
	}


	vector <vector <double> > getAdjacency(){
		return adjacency;
	}


	friend std::ostream& operator<<(std::ostream &os, const NicheModel &model);

private:
	//methods used by generate
	bool assignNicheValues(){

		for (int i=0; i<unitCount; i++){
			nicheValue.at(i) = unifRand(0,1);
		}

		return true;
	}

	bool assignRangeSizes(){

		double y = 0;
		double beta = -1 + 1/(2*connectance);

		for (int i=0; i<unitCount; i++){

			y = unifRand(0,1);
			rangeSize.at(i) = nicheValue.at(i)*(1-pow((1-y),(1/beta)));

		}

		return true;
	}

	bool assignRangeCentres(){

		for (int i=0; i<unitCount; i++){
			if (cannibals){
				rangeCentre.at(i) = unifRand(rangeSize.at(i)/2, nicheValue.at(i));
			}
			else{
				rangeCentre.at(i) = unifRand(rangeSize.at(i)/2, nicheValue.at(i)-rangeSize.at(i)/2);
			}
		}

		return true;
	}

	bool fillAdjacency(){

		adjacency.resize(unitCount, vector <double>(unitCount,0));
		double rangeMin = 0;
		double rangeMax = 0;

		for (int i=0; i<unitCount; i++){

			rangeMax = rangeCentre.at(i) + (rangeSize.at(i)/2);
			rangeMin = rangeCentre.at(i) - (rangeSize.at(i)/2);

			for (int j=0; j<unitCount; j++){

				if (nicheValue.at(j)>rangeMin && nicheValue.at(j)<rangeMax){
					if (adjacency.at(i).at(j)!=0){cout << "warning, overwriting adjacancy element" << endl;}
					if (adjacency.at(j).at(i)!=0){cout << "warning, overwriting adjacancy element" << endl;}
					adjacency.at(i).at(j) = 1;
					adjacency.at(j).at(i) = -1;

				}
			}

		}
		calculateConnectance();
		return true;
	}

	bool calculateConnectance(){
		realisedConnectance = 0;
		for (int i=0; i<unitCount; i++){
			for (int j = i; j<unitCount; j++){
				if(adjacency.at(i).at(j)!=0){realisedConnectance++;}
			}
		}
		realisedConnectance = realisedConnectance/(pow(unitCount,2));
		return true;
	}

	bool checkNoIsolates(vector <int>& speciesToReplace){
		// true -> no isolated species
		int rowSum = 0;
		for (int i=0; i<unitCount; i++){
			rowSum = 0;
			for (int j = i; j<unitCount; j++){
				if(adjacency.at(i).at(j)!=0){rowSum++;}
			}
			if (rowSum==0){speciesToReplace.push_back(i);}
		}

		if (speciesToReplace.size()!=0){return false;}

		return true;
	}

	bool checkNoIdentical(vector <int>& speciesToReplace){

		//true-> no identical species
		// write this!!

		return true;
	}

	bool replaceSpecies(vector <int>& speciesToreplace){

		for (unsigned int i=0; i<speciesToreplace.size(); i++){
			nicheValue.at(speciesToreplace.at(i)) = unifRand(0,1);
		}

		double y = 0;
		double beta = -1 + 1/(2*connectance);

		for (unsigned int i=0; i<speciesToreplace.size(); i++){

			y = unifRand(0,1);
			rangeSize.at(speciesToreplace.at(i)) = nicheValue.at(speciesToreplace.at(i))*(1-pow((1-y),(1/beta)));

		}

		for (unsigned int i=0; i<speciesToreplace.size(); i++){
			if (cannibals){
				rangeCentre.at(speciesToreplace.at(i)) = unifRand(rangeSize.at(speciesToreplace.at(i))/2, nicheValue.at(speciesToreplace.at(i)));
			}
			else{
				rangeCentre.at(speciesToreplace.at(i)) = unifRand(rangeSize.at(speciesToreplace.at(i))/2, nicheValue.at(speciesToreplace.at(i))-rangeSize.at(speciesToreplace.at(i))/2);
			}
		}
		fillAdjacency();
		speciesToreplace.clear();
		return true;
	}


};

inline std::ostream &operator << (std::ostream &f, const NicheModel &model)
{
    f << "Niche Model adjacency" << endl;
    for (int i=0; i<model.unitCount; i++){
    	for (int j=0; j<model.unitCount; j++){
    		f << model.adjacency.at(i).at(j) << ", ";
    	}
    	f << endl;
    }
    f << endl << endl;
    f << "Desired connectance = " << model.connectance << endl;
    f << "Realised connectance = " << model.realisedConnectance << endl;
    return f;
}



#endif /* NICHEMODEL_H_ */
