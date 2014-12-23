#include "CalculadoraSistemaLinear2D.h"



bool CalculadoraSistemaLinear2D::ResolverSistemaLinear(int dimensaoMatriz, double** aP, double** aE, double** aW, double** aN, double** aS, double** b, double** resposta)
{
	bool calculou;

	SistemaLinear2D* sistema= new SistemaLinear2D(dimensaoMatriz,aP,aE,aW,aN,aS,b,resposta);

	MetodoParaSistemaLinear2D* metodo =  new LSOR(sistema->dimensaoMatriz);

	calculou = metodo->Resolver(*sistema);

	cout.precision(4);

	/*cout<<"\n\nMatriz: \n\n";

	cout<<"\naP             aE               aW              aN            aS             b";*/
	if(calculou)
	{
		for (int i = 0; i < dimensaoMatriz; i++)
		{
			for (int j = 0; j < dimensaoMatriz; j++)
			{
				resposta[i][j] = sistema->resposta[i][j];

				//cout<<"\n"<<aP[i][j]<<"     "<<aE[i][j]<<"     "<<aW[i][j]<<"     "<<aN[i][j]<<"    "<<aS[i][j]<<"    "<<b[i][j]<<"   ";
			}
		}
	}

    delete metodo;
	delete sistema;

	return calculou;
}
