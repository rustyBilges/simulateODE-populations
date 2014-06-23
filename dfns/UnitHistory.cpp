/*
 * UnitHistory.cpp
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#include "UnitHistory.h"

UnitHistory::~UnitHistory(){
}

UnitHistory::UnitHistory(int tSteps):length(tSteps){
	history.resize(length, 0);
}

void UnitHistory::store(int tic, double state){
		history.at(tic) = state;
}

double UnitHistory::getHistoryTi(int Ti){
		return history.at(Ti);
	}

void UnitHistory::printHistory(){

}


