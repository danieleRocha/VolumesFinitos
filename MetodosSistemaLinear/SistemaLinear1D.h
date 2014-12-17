#ifndef SistemaLinear1D_h
#define SistemaLinear1D_h

#include <cstdlib>

class SistemaLinear1D
{
public:
	
	int dimensaoMatriz;
	double** matrizDeCoeficientes;
	double* vetorResposta;
	double* vetorCoeficientes;
	
	SistemaLinear1D(int dimensaoMatriz, double** matrizDeCoeficientes,double *vetorResposta);
	~SistemaLinear1D();
};

#endif 

