/*
 * DynamicalUnit.h
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef IDYNAMICALUNIT_H_
#define IDYNAMICALUNIT_H_

#include <stdlib.h>
#include <iostream>
#include "UnitHistory.h"
#include "INetwork.h"
using namespace std;

class IDynamicalUnit{
	/*
	 * dynamicalUnit interface header

	 * This is an interface class for dynamical units of dimension D, with Np parameters.
	 *
	 * The member variables: state and parameters are double arrays or size 1xD and 1xNp respectively.
	 * The coupling function is defined as a function of the parameters. It takes argument systemState
	 * which defines the entire state of the networked dynamical system.
	 *
	 * There is also a function to compute the total coupling input to an instantiated unit, based on the
	 * connections defined in the network. This perhaps introduces unnecessary dependencies, since the unit
	 * has to be passed the object of type network. Perhaps this method should be included in unitUpdater class?
	 *
	 * There are functions to randomise the unit states and parameters.
	 */


public:
	virtual ~IDynamicalUnit(){}

	virtual void    setInitialState(double state) = 0;
	virtual void    setState(double state) = 0;
	virtual double  getState() = 0;
	virtual void    printState() = 0;

	virtual double  getXdot(IDynamicalUnit **allUnits, int nUnits, int masterClock)=0;

	virtual void randomiseState(double sMin, double sMax)=0;

	virtual UnitHistory* getHistory()=0;
	virtual string getUnitType()=0;
};





#endif /* IDYNAMICALUNIT_H_ */
