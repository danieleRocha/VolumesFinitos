#include "DifusaoComGeracaoEDifusividadeVariavel1D.h"

bool DifusaoComGeracaoEDifusividadeVariavel1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	x0 = 0.0;//m
	xL = 1.0;//m
	fi0 = 1.0;
	fiL = 1.0;

	dx= (xL-x0)/(numeroDeVolumes); //Intervalo em x

	arquivo = ofstream("resultado.txt");
	const int n = numeroDeVolumes+2;
	double* x;
	double* fiAnalitico;
	double* fiNumerico;
	double* desviosRelativos;
	double* desviosAbsolutos;

#pragma region AlocacaoDeMemoria

	x = static_cast<double*>(malloc(n*sizeof(double)));
	fiAnalitico = static_cast<double*>(malloc(n*sizeof(double)));
	fiNumerico = static_cast<double*>(malloc(n*sizeof(double)));
	desviosRelativos = static_cast<double*>(malloc(n*sizeof(double)));
	desviosAbsolutos = static_cast<double*>(malloc(n*sizeof(double)));

#pragma endregion 

	calculou = Calcular(x,fiAnalitico,fiNumerico,desviosRelativos,desviosAbsolutos);
	if(!calculou) return calculou;

	ImprimirResultados(x,fiAnalitico,fiNumerico,desviosRelativos,desviosAbsolutos);

	arquivo<<"\nCalculos finalizados com sucesso.\n\n";
	cout<<"\nCalculos finalizados com sucesso.\n\n";

#pragma region LiberacaoMemoria

	free(static_cast<void*>(x));
	free(static_cast<void*>(fiAnalitico));
	free(static_cast<void*>(fiNumerico));
	free(static_cast<void*>(desviosRelativos));
	free(static_cast<void*>(desviosAbsolutos));

	arquivo.close();

#pragma endregion 

	return calculou;
}

bool DifusaoComGeracaoEDifusividadeVariavel1D::SolicitarDadosDeEntrada()
{
	bool errou = false;

	cout<<"\n\nDigite o número de volumes: ";
	cin >> numeroDeVolumes;
	cout<<"Digite o número do método para solução de sistemas lineares(";

	for(int i=0;i<nMetodosParaSistemasLinearesDisponiveis;i++)
	{
		cout<<"\n"<<nomeMetodosParaSistemasLineares[i];
	}
	cout<<"):";
	cin >>numeroDoMetodoParaSistemaLinear;

	if((numeroDoMetodoParaSistemaLinear<0)||(numeroDoMetodoParaSistemaLinear>(nMetodosParaSistemasLinearesDisponiveis-1)))
	{
		cout<<"Atenção: \n"
			"Número do método inválido. O número do método para solução de sistemas lineares deve estar entre 0 e "<<(nMetodosParaSistemasLinearesDisponiveis-1);
		errou = true;
	}

	return !errou;
}

void DifusaoComGeracaoEDifusividadeVariavel1D::ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico)
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiAnalitico[numeroDeVolumes+1] = fiL;
	fiNumerico[numeroDeVolumes+1] = fiL;

	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoEsquerda->fi = fi0;

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoDireita->fi = fiL;
}

void DifusaoComGeracaoEDifusividadeVariavel1D::IniciarVariavelNumerica(double* fiNumerico)
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = 1.0;
	}
}

bool DifusaoComGeracaoEDifusividadeVariavel1D::CalcularSolucaoAnalitica(double* x, double* fiAnalitico)
{
	for(int i=1;i<=numeroDeVolumes;i++)
	{
		fiAnalitico[i] = 0.0;
	}

	return true;
}

double DifusaoComGeracaoEDifusividadeVariavel1D::TermoDifusivoDifusaoComGeracaoEDifusividadeVariavel1D::Calcular(double fi)
{
	double difusividade = 10*exp(-1.0/(0.1+fi));
	return difusividade;
}

double DifusaoComGeracaoEDifusividadeVariavel1D::TermoFonteDifusaoComGeracaoEDifusividadeVariavel1D::Calcular(double fi)
{
	double termoFonte = -( 1 + fi*fi);
	return termoFonte;
}

