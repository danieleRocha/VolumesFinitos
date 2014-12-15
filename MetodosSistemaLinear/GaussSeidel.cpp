#include "GaussSeidel.h"

bool GaussSeidel::Resolver(SistemaLinear sistema,string &mensagem) 
{
	bool calculou=true;
	
	int i = 0;
	int j = 0;
	int nIteracoes = 0;
	int niteracoesMaximo = 500;
	double erroMaximo = 100;

	double* delta;
	double* incognitaPassoAnterior;

	int n = sistema.dimensaoMatriz;
	double** a;
	double* x;
	double* b;
	double* erro;
	double* residuo;

#pragma region AlocacaoDeMemoria

	incognitaPassoAnterior = static_cast<double*>(malloc(n*sizeof(double)));
	delta= static_cast<double*>(malloc(n*sizeof(double)));
	x = static_cast<double*>(malloc(n*sizeof(double)));
	b = static_cast<double*>(malloc(n*sizeof(double)));
	a=static_cast<double**>(malloc(n*sizeof(double*)));
	erro=static_cast<double*>(malloc(n*sizeof(double)));
	residuo=static_cast<double*>(malloc(n*sizeof(double)));
	
	for (i=0; i<n; i++)
	{
		a[i]=static_cast<double*>(malloc(n*sizeof(double)));
	}

#pragma endregion 

#pragma region Alimentação dos dados

	mensagem = "";

	for (int i = 0; i < n; ++i)
	{
		x[i] = sistema.vetorCoeficientes[i];
		b[i] = sistema.vetorResposta[i];

		for (int j = 0; j < n; ++j)
		{
			a[i][j] = sistema.matrizDeCoeficientes[i][j];
		}
	}

#pragma endregion

#pragma region Calculo

#pragma region Verificação da dominância diagonal do sistema

	bool eDominante = VerificarDominaciaDiagonal(n,a);
	
	if(!eDominante)
		mensagem = "O sistema nao possui dominancia diagonal.";

#pragma endregion

calculo:

	while(erroMaximo>0.0001)
	{
		nIteracoes++;

		if(nIteracoes>niteracoesMaximo)
		{
			calculou = false;
			break;
		}

		erroMaximo = 0.0;

		//Solução por Gauss Seidel
		for(i = 0;i<n;i++)
		{
			double somaj = 0.0;
			delta[i] = 0.0;
			incognitaPassoAnterior[i] = x[i];

			for(j = 0;j<n;j++)
			{
				if(j!=i)
				{
					somaj = somaj + x[j]*a[i][j];
				}
			}

			if(a[i][i]!=0.0)
				x[i] = (b[i]-somaj)/a[i][i];
			else
				x[i] =0.0;
		}


		for(i = 0;i<n;i++)
		{
			if(x[i]!=0.0)
				delta[i] = 100*(incognitaPassoAnterior[i]-x[i])/x[i];
			else
				delta[i] =1.0;

			if(erroMaximo<fabs(delta[i]))
				erroMaximo = fabs(delta[i]);
		}
	}

#pragma endregion

#pragma region Passagem dos resultados

	for (int i = 0; i < n; ++i)
	{
		sistema.vetorCoeficientes[i] = x[i];
	}

#pragma endregion 

#pragma region LiberacaoMemoria

	free(static_cast<void*>(incognitaPassoAnterior));
	free(static_cast<void*>(delta));
	free(static_cast<void*>(x));
	free(static_cast<void*>(b));
	free(static_cast<void*>(erro));
	free(static_cast<void*>(residuo));
	for (i=0; i<n; i++)
	{
		free(static_cast<void*>(a[i]));
		
	}
	free(static_cast<void*>(a));

	#pragma endregion 

	return calculou;
}

bool GaussSeidel::VerificarDominaciaDiagonal(int ordemDaMatriz,double** coeficientesDaMatriz)
{
	bool eMaiorOuIgual = true;
	bool eEstritamenteMaior = false;
	for (int i = 0; i < ordemDaMatriz; ++i)
	{
		double somaForaDiagonalProncipal = 0.0;
		double elementoDiagonalPrincipal = abs(coeficientesDaMatriz[i][i]);

		for (int j = 0; j < ordemDaMatriz; ++j)
		{
			if(i!=j)
				somaForaDiagonalProncipal += abs(coeficientesDaMatriz[i][j]);
		}

		if(somaForaDiagonalProncipal>elementoDiagonalPrincipal)
		{
			eMaiorOuIgual = false;
		}
		if(elementoDiagonalPrincipal>somaForaDiagonalProncipal)
		{
			eEstritamenteMaior = true;
		}
	}

	if((eMaiorOuIgual)&&(eEstritamenteMaior))
		return true;

	return false;
}
