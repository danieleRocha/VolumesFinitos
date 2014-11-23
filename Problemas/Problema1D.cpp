#include "Problema1D.h"


bool Problema1D::Calcular(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
{
	//Declaração de variáveis
	bool calculou = true;

	calculou = CalcularVariavelIndependenteEspacoX(x);
	if(!calculou)
	{
		ImprimirMensagemDeErro(1);
		return calculou;
	}

	ObterCondicoesIniciaisEDeContorno(fiAnalitico,fiNumerico);

	IniciarVariavelNumerica(fiNumerico);

	calculou = CalcularSolucaoAnalitica(x,fiAnalitico);
	if(!calculou)
	{
		ImprimirMensagemDeErro(2);
		return calculou;
	}

	calculou = CalcularSolucaoNumerica(fiNumerico);
	if(!calculou)
	{
		ImprimirMensagemDeErro(3);
		return calculou;
	}

	calculou = CalcularDesvios(fiAnalitico,fiNumerico,desviosRelativos,desviosAbsolutos);
	if(!calculou)
	{
		ImprimirMensagemDeErro(4);
		return calculou;
	}

	return calculou;
}

void Problema1D::ImprimirMensagemDeErro(int numeroDoErro)
{
	switch(numeroDoErro)
	{
	case 1:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço.\n\n";
		break;
	case 2:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a solução analítica.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a solução analítica.\n\n";
		break;
	case 3:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a solução numérica.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a solução numérica.\n\n";
		break;
	case 4:
		arquivo<<"Erro: \n"
			"Não foi possível calcular os desvios dos resultados numéricos em relação aos analíticos.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular os desvios dos resultados numéricos em relação aos analíticos.\n\n";
		break;
	default:
		arquivo<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		cout<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		break;
	}
}

bool Problema1D::CalcularVariavelIndependenteEspacoX(double* x)
{
	x[0] = x0;
	x[numeroDeVolumes+1] = xL;

	for(int i = 1;i<=numeroDeVolumes;i++)
	{
		if(i==1)
			x[i] = x[i-1] + dx/2;
		else
			x[i] = x[i-1] + dx;
	}

	return true;
}



bool Problema1D::CalcularSolucaoNumerica(double* fiNumerico)
{
	return VolumesFinitos::ResolverRegimePermanente1D(numeroDeVolumes,dx,fiNumerico,
		numeroDoMetodoParaSistemaLinear,this->Difusividade,this->TermoFonte,this->CondicaoDeContornoEsquerda,
		this->CondicaoDeContornoDireita);
}

bool Problema1D::CalcularDesvios(double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
{
	for(int i=0;i<=numeroDeVolumes+1;i++)
	{
		desviosAbsolutos[i] = fiAnalitico[i]- fiNumerico[i];

		if(fabs(fiAnalitico[i]-0.0)>1e-12)
			desviosRelativos[i] = 100*(fiAnalitico[i]- fiNumerico[i])/fiAnalitico[i];
		else
			desviosRelativos[i] =0.0;
	}
	return true;
}

void Problema1D::ImprimirResultados(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
{
	arquivo.precision(3);
	cout.precision(3);

	arquivo<<"\n\nResultados: \n\n"
		"x        Solução Analítica   Solução Numérica     Desvio[%]     Desvio absoluto \n";
	cout<<"\n\nResultados: \n\n"
		"x        Solução Analítica   Solução Numérica     Desvio[%]     Desvio absoluto \n";

	for(int j=0;j<=numeroDeVolumes+1;j++)
	{
		

		arquivo<< fixed<< x[j]<<"        "<<fiAnalitico[j]<<"              "<<fiNumerico[j]<<"            "<<desviosRelativos[j]<<"            "<<desviosAbsolutos[j]<<"\n";
		cout<< fixed<< x[j]<<"        "<<fiAnalitico[j]<<"              "<<fiNumerico[j]<<"            "<<desviosRelativos[j]<<"            "<<desviosAbsolutos[j]<<"\n";
	}

}
