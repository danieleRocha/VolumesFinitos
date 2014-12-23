#ifndef Thomas_h
#define Thomas_h

#include "MetodoParaSistemaLinear1D.h"
#include <istream>

class Thomas :
	public MetodoParaSistemaLinear1D
{
public:
	bool Resolver(SistemaLinear1D sistema) override;
	bool Resolver(SistemaLinear1D sistema,double* dThomas,double* bThomas);
};

#endif 

