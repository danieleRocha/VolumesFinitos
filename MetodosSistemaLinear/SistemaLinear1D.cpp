#include "SistemaLinear1D.h"


SistemaLinear1D::SistemaLinear1D(int dimensaoMatriz, double** matrizDeCoeficientes,double *vetorResposta)
{
	this->dimensaoMatriz = dimensaoMatriz;
	this->matrizDeCoeficientes = matrizDeCoeficientes;
	this->vetorResposta = vetorResposta;
	this->vetorCoeficientes = new double[dimensaoMatriz];
		
	for (int i = 0; i < this->dimensaoMatriz; ++i)
	{
		this->vetorCoeficientes[i]=1.0;
	}
}

SistemaLinear1D::SistemaLinear1D(int dimensaoMatriz, double** matrizDeCoeficientes, double* vetorResposta, double* vetorCoeficientes)
{
	this->dimensaoMatriz = dimensaoMatriz;
	this->matrizDeCoeficientes = matrizDeCoeficientes;
	this->vetorResposta = vetorResposta;
	this->vetorCoeficientes = vetorCoeficientes;
		
	for (int i = 0; i < this->dimensaoMatriz; ++i)
	{
		this->vetorCoeficientes[i]=1.0;
	}
}

SistemaLinear1D::~SistemaLinear1D()
{
	
}
