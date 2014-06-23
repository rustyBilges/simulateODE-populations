/*
 * UnitUpdater.h
 *
 *  Created on: 17 Dec 2013
 *      Author: rusty
 */

#ifndef IUNITUPDATER_H_
#define IUNITUPDATER_H_

#include "IDynamicalUnit.h"

// for the Gaussian noise generator:
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926536

class IUnitUpdater{

private:

public:

	virtual ~IUnitUpdater(){}
	virtual bool updateUnit(IDynamicalUnit **allUnits, int nUnits, int unitToUpdate, double dt, int masterClock)=0;

	virtual void updateUnit_test(IDynamicalUnit *u, double totalCouplingInput)=0;

};

class NoisyEulerUnitUpdater : public IUnitUpdater
{

private:
	double variance;

	double AWGN_generator()
	{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

	  double temp1;
	  double temp2;
	  double result;
	  int p;

	  p = 1;

	  while( p > 0 )
	  {
	        temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
	                                                       integer between 0 and  RAND_MAX,
	                                                       which is defined in stdlib.h.
	                                                   */

	    if ( temp2 == 0 )
	    {// temp2 is >= (RAND_MAX / 2)
	      p = 1;
	    }// end if
	    else
	    {// temp2 is < (RAND_MAX / 2)
	       p = -1;
	    }// end else

	  }// end while()

	  temp1 = cos( ( 2.0 * (double)PI ) * rand() / ( (double)RAND_MAX ) );
	  result = sqrt( -2.0 * log( temp2 ) ) * temp1;

	  return result;        // return the generated random sample to the caller

	}// end AWGN_generator()

public:
	NoisyEulerUnitUpdater(double variance);

	void updateUnit_test(IDynamicalUnit *u, double totalCouplingInput);

	bool updateUnit(IDynamicalUnit **allUnits, int nUnits, int unitToUpdate, double dt, int masterClock);

};

class EulerUnitUpdater : public IUnitUpdater
{

public:
	EulerUnitUpdater();

	void updateUnit_test(IDynamicalUnit *u, double totalCouplingInput);

	bool updateUnit(IDynamicalUnit **allUnits, int nUnits, int unitToUpdate, double dt, int masterClock);

};

#endif /* IUNITUPDATER_H_ */

