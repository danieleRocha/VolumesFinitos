#ifndef CalculadoraSistemaLinear1D_h
#define CalculadoraSistemaLinear1D_h

#include <cstdlib>
#include <math.h>
#include "SistemaLinear1D.h"
#include "MetodoParaSistemaLinear1D.h"
#include "FabricaDeMetodosParaSistemaLinear.h"

class CalculadoraSistemaLinear1D
{
public:
	static bool ResolverSistemaLinear(double* resposta,double* aP, double* aE,double* aEE, double* aW, double* aWW, double* b, int n,int numeroDoMetodoParaSistemaLinear1D);
	static bool ResolverSistemaLinear(double* resposta, double** matrizCoeficientes, double* b, int n, int numeroDoMetodoParaSistemaLinear1D);
};

#endif

