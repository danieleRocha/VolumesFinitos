#include "CalculadoraSistemaLinear1D.h"


bool CalculadoraSistemaLinear1D::ResolverSistemaLinear(double* resposta, double* aP, double* aE, double* aEE, double* aW, double* aWW, double* b, int n, int numeroDoMetodoParaSistemaLinear1D)
{
	bool calculou;
	double** matrizCoeficientes;

#pragma region AlocacaoDeMemoria

	matrizCoeficientes=static_cast<double**>(malloc(n*sizeof(double*)));

	for (int i=0; i<n; i++)
	{
		matrizCoeficientes[i]=static_cast<double*>(malloc(n*sizeof(double)));
	}

#pragma endregion 

	//cout.precision(4);

	//cout<<"\n\nMatriz: \n\n";

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if(i==j)
				matrizCoeficientes[i][j] = aP[i];
			else if(j==(i-1))
				matrizCoeficientes[i][j] = -aW[i];
			else if(j==(i-2))
				matrizCoeficientes[i][j] = -aWW[i];
			else if(j==(i+1))
				matrizCoeficientes[i][j] = -aE[i];
			else if(j==(i+2))
				matrizCoeficientes[i][j] = -aEE[i];
			else
				matrizCoeficientes[i][j] = 0.0;

			//cout<<"  "<<matrizCoeficientes[i][j];
		}
		//cout<<"          "<<b[i];
		//cout<<"\n";
	}

	SistemaLinear1D* sistema = new SistemaLinear1D(n,matrizCoeficientes,b);

	MetodoParaSistemaLinear1D* metodo =  FabricaDeMetodosParaSistemaLinear::CriarMetodo(numeroDoMetodoParaSistemaLinear1D);

	string mensagem = "";

	calculou = metodo->Resolver(*sistema,mensagem);

	if(calculou)
	{
		for (int i = 0; i < n; i++)
		{
			resposta[i] = sistema->vetorCoeficientes[i];
		}
	}

#pragma region LiberacaoMemoria

	for (int i=0; i<n; i++)
	{
		free(static_cast<void*>(matrizCoeficientes[i]));

	}
	free(static_cast<void*>(matrizCoeficientes));

#pragma endregion 


	return calculou;
}

bool CalculadoraSistemaLinear1D::ResolverSistemaLinear(double* resposta, double** matrizCoeficientes, double* b, int n, int numeroDoMetodoParaSistemaLinear1D)
{
	bool calculou;
		
	SistemaLinear1D* sistema = new SistemaLinear1D(n,matrizCoeficientes,b);

	MetodoParaSistemaLinear1D* metodo =  FabricaDeMetodosParaSistemaLinear::CriarMetodo(numeroDoMetodoParaSistemaLinear1D);

	string mensagem = "";

	calculou = metodo->Resolver(*sistema,mensagem);

	if(calculou)
	{
		for (int i = 0; i < n; i++)
		{
			resposta[i] = sistema->vetorCoeficientes[i];
		}
	}

	return calculou;
}