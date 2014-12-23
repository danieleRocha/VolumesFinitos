#include "Thomas.h"


bool Thomas::Resolver(SistemaLinear1D sistema)
{
	bool calculou=true;

	double* d;
	double* b;

#pragma region AlocacaoDeMemoria

	d = new double[sistema.dimensaoMatriz];
	b= new double[sistema.dimensaoMatriz];

#pragma endregion 
	
	d[0] = sistema.matrizDeCoeficientes[0][0];
	b[0] = sistema.vetorResposta[0];

	for(int i = 1;i<sistema.dimensaoMatriz;i++)
	{
		d[i] = sistema.matrizDeCoeficientes[i][i] - (sistema.matrizDeCoeficientes[i][i-1]*sistema.matrizDeCoeficientes[i-1][i])/d[i-1];
		b[i] = sistema.vetorResposta[i] - (sistema.matrizDeCoeficientes[i][i-1]*b[i-1])/d[i-1];
	}

	sistema.vetorCoeficientes[sistema.dimensaoMatriz-1] = b[sistema.dimensaoMatriz-1]/d[sistema.dimensaoMatriz-1];

	for(int i = sistema.dimensaoMatriz-2;i>=0;i--)
	{
		sistema.vetorCoeficientes[i] = (b[i]-(sistema.matrizDeCoeficientes[i][i+1]*sistema.vetorCoeficientes[i+1]))/d[i];
	}

#pragma region LiberacaoMemoria

	delete [] d;
	delete [] b;
		
#pragma endregion 

	return calculou;
}

bool Thomas::Resolver(SistemaLinear1D sistema, double* dThomas, double* bThomas)
{
	bool calculou=true;

	dThomas[0] = sistema.matrizDeCoeficientes[0][0];
	bThomas[0] = sistema.vetorResposta[0];

	for(int i = 1;i<sistema.dimensaoMatriz;i++)
	{
		dThomas[i] = sistema.matrizDeCoeficientes[i][i] - (sistema.matrizDeCoeficientes[i][i-1]*sistema.matrizDeCoeficientes[i-1][i])/dThomas[i-1];
		bThomas[i] = sistema.vetorResposta[i] - (sistema.matrizDeCoeficientes[i][i-1]*bThomas[i-1])/dThomas[i-1];
	}

	sistema.vetorCoeficientes[sistema.dimensaoMatriz-1] = bThomas[sistema.dimensaoMatriz-1]/dThomas[sistema.dimensaoMatriz-1];

	for(int i = sistema.dimensaoMatriz-2;i>=0;i--)
	{
		sistema.vetorCoeficientes[i] = (bThomas[i]-(sistema.matrizDeCoeficientes[i][i+1]*sistema.vetorCoeficientes[i+1]))/dThomas[i];
	}


	return calculou;
}