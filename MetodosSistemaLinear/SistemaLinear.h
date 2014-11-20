#ifndef SistemaLinear_h
#define SistemaLinear_h

#include <cstdlib>

class SistemaLinear
{
public:
	
	int dimensaoMatriz;
	double** matrizDeCoeficientes;
	double* vetorResposta;
	double* vetorCoeficientes;
	
	SistemaLinear(int dimensaoMatriz, double** matrizDeCoeficientes,double *vetorResposta);
	~SistemaLinear();
};

#endif 

