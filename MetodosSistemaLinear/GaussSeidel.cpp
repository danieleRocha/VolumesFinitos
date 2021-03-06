#include "GaussSeidel.h"

bool GaussSeidel::Resolver(SistemaLinear1D sistema) 
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

	incognitaPassoAnterior =new double[n];
	delta=new double[n];
	x =new double[n];
	b =new double[n];
	a=new double*[n];
	erro=new double[n];
	residuo=new double[n];
	
	for (i=0; i<n; i++)
	{
		a[i]=new double[n];
	}

#pragma endregion 

#pragma region Alimenta��o dos dados

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

#pragma region Verifica��o da domin�ncia diagonal do sistema

	bool eDominante = VerificarDominaciaDiagonal(n,a);
	
	/*if(!eDominante)
		mensagem = "O sistema nao possui dominancia diagonal.";
*/
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

		//Solu��o por Gauss Seidel
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

	delete [] incognitaPassoAnterior;
	delete [] delta;
	delete [] x;
	delete [] b;
	delete [] erro;
	delete [] residuo;
	
	for (i=0; i<n; i++)
	{
		delete [] a[i];
	}
	delete [] a;

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
