#ifndef CalculadoraSistemaLinear2D_h
#define CalculadoraSistemaLinear2D_h

#include <cstdlib>
#include <math.h>
#include "SistemaLinear2D.h"
#include "MetodoParaSistemaLinear2D.h"
#include "LSOR.h"

class CalculadoraSistemaLinear2D
{
public:
	
	
	static bool ResolverSistemaLinear(int dimensaoMatriz, double** aP,double** aE,double** aW,
		double** aN,double** aS,double **b,double **resposta);
	
};

#endif
