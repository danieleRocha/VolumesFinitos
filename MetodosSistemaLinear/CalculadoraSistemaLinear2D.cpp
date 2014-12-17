#include "CalculadoraSistemaLinear2D.h"



bool CalculadoraSistemaLinear2D::ResolverSistemaLinear(int dimensaoMatriz, double** aP, double** aE, double** aW, double** aN, double** aS, double** b, double** resposta)
{
	bool calculou;

	SistemaLinear2D* sistema = new SistemaLinear2D(dimensaoMatriz,aP,aE,aW,aN,aS,b,resposta);

	MetodoParaSistemaLinear2D* metodo =  new LSOR();

	string mensagem = "";

	calculou = metodo->Resolver(*sistema,mensagem);

	if(calculou)
	{
		for (int i = 0; i < dimensaoMatriz; i++)
		{
			for (int j = 0; j < dimensaoMatriz; j++)
			{
				resposta[i][j] = sistema->resposta[i][j];
			}
		}
	}

	return calculou;
}