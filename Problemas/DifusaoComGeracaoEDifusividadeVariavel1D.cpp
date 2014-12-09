#include "DifusaoComGeracaoEDifusividadeVariavel1D.h"

bool DifusaoComGeracaoEDifusividadeVariavel1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	TermoFonte->Linear = false;
	TermoFonte->Decrescente = true;

	x0 = 0.0;//m
	xL = 1.0;//m
	fi0 = 1.0;
	fiL = 1.0;

	dx= (xL-x0)/(numeroDeVolumes); //Intervalo em x

	DefinirArquivo();
	AlocarMemoria();
	
	calculou = Calcular();
	if(!calculou) return calculou;

	ImprimirResultados();

	arquivo<<"\nCalculos finalizados com sucesso.\n\n";
	cout<<"\nCalculos finalizados com sucesso.\n\n";

    LiberarMemoria();

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

void DifusaoComGeracaoEDifusividadeVariavel1D::ObterCondicoesIniciaisEDeContorno()
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiAnalitico[numeroDeVolumes+1] = fiL;
	fiNumerico[numeroDeVolumes+1] = fiL;
	peclet[0] = 0.0;
	peclet[numeroDeVolumes+1]= 0.0;

	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoEsquerda->fi = fi0;

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoDireita->fi = fiL;
}

void DifusaoComGeracaoEDifusividadeVariavel1D::IniciarVariavelNumerica()
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = 1.0;
	}
}

bool DifusaoComGeracaoEDifusividadeVariavel1D::CalcularSolucaoAnalitica()
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

double DifusaoComGeracaoEDifusividadeVariavel1D::TermoFonteDifusaoComGeracaoEDifusividadeVariavel1D::Derivada(double fi)
{
	return -2*fi;
}

double DifusaoComGeracaoEDifusividadeVariavel1D::FluxoMassicoDifusaoComGeracaoEDifusividadeVariavel1D::Calcular()
{
	return 0.0;
}