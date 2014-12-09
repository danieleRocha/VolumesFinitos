#include "Problema1D.h"


bool Problema1D::Calcular()
{
	//Declara��o de vari�veis
	bool calculou = true;

	calculou = CalcularVariavelIndependenteEspacoX();
	if(!calculou)
	{
		ImprimirMensagemDeErro(1);
		return calculou;
	}

	ObterCondicoesIniciaisEDeContorno();

	IniciarVariavelNumerica();

	calculou = CalcularSolucaoAnalitica();
	if(!calculou)
	{
		ImprimirMensagemDeErro(2);
		return calculou;
	}

	calculou = CalcularSolucaoNumerica();
	if(!calculou)
	{
		ImprimirMensagemDeErro(3);
		return calculou;
	}

	calculou = CalcularDesvios();
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
			"N�o foi poss�vel calcular a vari�vel independente: espa�o.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o.\n\n";
		break;
	case 2:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o anal�tica.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o anal�tica.\n\n";
		break;
	case 3:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o num�rica.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o num�rica.\n\n";
		break;
	case 4:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular os desvios dos resultados num�ricos em rela��o aos anal�ticos.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular os desvios dos resultados num�ricos em rela��o aos anal�ticos.\n\n";
		break;
	default:
		arquivo<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		cout<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		break;
	}
}

bool Problema1D::CalcularVariavelIndependenteEspacoX()
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

bool Problema1D::CalcularSolucaoNumerica()
{
	return VolumesFinitos::ResolverRegimePermanente1D(numeroDeVolumes,dx,fiNumerico,peclet,
		numeroDoMetodoParaSistemaLinear,this->Difusividade,this->FluxoMassico,this->TermoFonte,
		this->CondicaoDeContornoEsquerda,this->CondicaoDeContornoDireita,static_cast<VolumesFinitos::Discretizacao>(tipoDiscretizacaoTermoConvectivo));
}

bool Problema1D::CalcularDesvios()
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

void Problema1D::ImprimirResultados()
{
	arquivo.precision(4);
	cout.precision(4);

	arquivo<<"\n\nResultados: \n\n"
		"x       Peclet  Solu��o Anal�tica  Solu��o Num�rica  Desvio[%]  Desvio absoluto \n";
	cout<<"\n\nResultados: \n\n"
		"x       Peclet  Solu��o Anal�tica  Solu��o Num�rica  Desvio[%]  Desvio absoluto \n";

	for(int j=0;j<=numeroDeVolumes+1;j++)
	{
		

		arquivo<< fixed<< x[j]<<"   "<<peclet[j]<<"         "<<fiAnalitico[j]<<"             "<<fiNumerico[j]<<"       "<<desviosRelativos[j]<<"       "<<desviosAbsolutos[j]<<"\n";
		cout<< fixed<< x[j]<<"   "<<peclet[j]<<"         "<<fiAnalitico[j]<<"             "<<fiNumerico[j]<<"       "<<desviosRelativos[j]<<"       "<<desviosAbsolutos[j]<<"\n";
	}

}

void Problema1D::AlocarMemoria()
{
	const int n = numeroDeVolumes+2;
	
	x = static_cast<double*>(malloc(n*sizeof(double)));
	fiAnalitico = static_cast<double*>(malloc(n*sizeof(double)));
	fiNumerico = static_cast<double*>(malloc(n*sizeof(double)));
	peclet = static_cast<double*>(malloc(n*sizeof(double)));
	desviosRelativos = static_cast<double*>(malloc(n*sizeof(double)));
	desviosAbsolutos = static_cast<double*>(malloc(n*sizeof(double)));
}

void Problema1D::DefinirArquivo()
{
	arquivo = ofstream("resultado.txt");
}

void Problema1D::LiberarMemoria()
{
	free(static_cast<void*>(x));
	free(static_cast<void*>(fiAnalitico));
	free(static_cast<void*>(fiNumerico));
	free(static_cast<void*>(peclet));
	free(static_cast<void*>(desviosRelativos));
	free(static_cast<void*>(desviosAbsolutos));

	arquivo.close();
}