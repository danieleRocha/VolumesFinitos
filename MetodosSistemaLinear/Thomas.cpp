#include "Thomas.h"


bool Thomas::Resolver(SistemaLinear sistema, string &mensagem)
{
	bool calculou=true;

	int i = 0;
	double* d;
	double* l;
	double* u;
	double* b;

#pragma region AlocacaoDeMemoria

	d = static_cast<double*>(malloc(sistema.dimensaoMatriz*sizeof(double)));
	l= static_cast<double*>(malloc(sistema.dimensaoMatriz*sizeof(double)));
	u = static_cast<double*>(malloc(sistema.dimensaoMatriz*sizeof(double)));
	b= static_cast<double*>(malloc(sistema.dimensaoMatriz*sizeof(double)));

#pragma endregion 

	mensagem = "";

	d[0] = sistema.matrizDeCoeficientes[0][0];
	b[0] = sistema.vetorResposta[0];

	for(i = 1;i<sistema.dimensaoMatriz;i++)
	{
		d[i] = sistema.matrizDeCoeficientes[i][i] - (sistema.matrizDeCoeficientes[i][i-1]*sistema.matrizDeCoeficientes[i-1][i])/d[i-1];
		b[i] = sistema.vetorResposta[i] - (sistema.matrizDeCoeficientes[i][i-1]*b[i-1])/d[i-1];
	}

	sistema.vetorCoeficientes[sistema.dimensaoMatriz-1] = b[sistema.dimensaoMatriz-1]/d[sistema.dimensaoMatriz-1];

	for(i = sistema.dimensaoMatriz-2;i>=0;i--)
	{
		sistema.vetorCoeficientes[i] = (b[i]-(sistema.matrizDeCoeficientes[i][i+1]*sistema.vetorCoeficientes[i+1]))/d[i];
	}

#pragma region LiberacaoMemoria

	free(static_cast<void*>(d));
	free(static_cast<void*>(l));
	free(static_cast<void*>(u));
	free(static_cast<void*>(b));

#pragma endregion 

	return calculou;
}