/*
 * IIntrinsicGrowthFunction.h
 *
 *  Created on: 18 Jun 2014
 *      Author: rusty
 */

#ifndef IINTRINSICGROWTHFUNCTION_H_
#define IINTRINSICGROWTHFUNCTION_H_


#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

class IIntrinsicGrowthFunction{

public:
	virtual double growthRate(double x) = 0;
	virtual ~IIntrinsicGrowthFunction(){}
};

class ExponentialGrowthFn: public IIntrinsicGrowthFunction
{

private:
	double r0;
	double exp;

public:
	~ExponentialGrowthFn(){

	}

	ExponentialGrowthFn(double _r0, double _exp=1):r0(_r0), exp(_exp){}

	double growthRate(double x0){
		return pow(x0,exp)*r0;
	}

};

class LogisticGrowthFn: public IIntrinsicGrowthFunction
{

private:
	double r0;
	double carryingCapacity;

public:
	~LogisticGrowthFn(){

	}

	LogisticGrowthFn(double _r0, double _carryingCapacity):r0(_r0),carryingCapacity(_carryingCapacity){}

	double growthRate(double x0){
		return x0*r0*(1-(x0/carryingCapacity));
	}

};



#endif /* IINTRINSICGROWTHFUNCTION_H_ */
