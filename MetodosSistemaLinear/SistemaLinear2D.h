#ifndef SistemaLinear2D_h
#define SistemaLinear2D_h

#include <cstdlib>

class SistemaLinear2D
{
public:
	
	int dimensaoMatriz;
	double** aP;
	double** aE;
	double** aW;
	double** aN;
	double** aS;
	double** b;
	double** resposta;
	
	SistemaLinear2D(int dimensaoMatriz, double** aP,double** aE,double** aW,
		double** aN,double** aS,double **b,double **resposta);
	~SistemaLinear2D();
};

#endif 

