#ifndef FluxoMassicoVariavel_h
#define FluxoMassicoVariavel_h
#include "FluxoMassico.h"


class FluxoMassicoVariavel:
	public FluxoMassico
{
public:
	virtual	double CalcularX(double x) = 0;
	virtual	double CalcularY(double y) = 0;
	
};

#endif

