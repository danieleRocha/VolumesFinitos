#include "SistemaLinear2D.h"


SistemaLinear2D::SistemaLinear2D(int dimensaoMatriz, double** aP,double** aE,double** aW,
		double** aN,double** aS,double **b,double **resposta)
{
	this->dimensaoMatriz = dimensaoMatriz;
	this->aP = aP;
	this->aE = aE;
	this->aW = aW;
	this->aN = aN;
	this->aS = aS;
	this->b = b;
	this->resposta = resposta;
}


SistemaLinear2D::~SistemaLinear2D(void)
{
}
