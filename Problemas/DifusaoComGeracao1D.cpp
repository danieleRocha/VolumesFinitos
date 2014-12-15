#include "DifusaoComGeracao1D.h"

bool DifusaoComGeracao1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 1.0;//m
	qK =  5.0;//K/m²

	//Passa dados para o objeto TermoFonte
	(static_cast<TermoFonteDifusaoComGeracao1D*>(TermoFonte))->qK = qK;
	TermoFonte->Linear = true;
	TermoFonte->Decrescente = false;
	tipoDiscretizacaoTermoConvectivo = 0;

	x0 = 0.0;//m
	xL = L;//m
	fi0 = 0.0;
	fiL = 0.0;

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

bool DifusaoComGeracao1D::SolicitarDadosDeEntrada()
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

void DifusaoComGeracao1D::ObterCondicoesIniciaisEDeContorno()
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

void DifusaoComGeracao1D::IniciarVariavelNumerica()
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = 0.0;
	}
}

bool DifusaoComGeracao1D::CalcularSolucaoAnalitica()
{
	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = -qK*x[i]*x[i]/2 + 2.5*x[i];
	}

	return true;
}

double DifusaoComGeracao1D::TermoDifusivoDifusaoComGeracao1D::Calcular(double fi)
{
	return 1.0;
}

double DifusaoComGeracao1D::TermoFonteDifusaoComGeracao1D::Calcular(double fi,double x)
{
	return qK;
}

double DifusaoComGeracao1D::TermoFonteDifusaoComGeracao1D::Derivada(double fi,double x)
{
	return 0;
}

double DifusaoComGeracao1D::FluxoMassicoDifusaoComGeracao1D::Calcular()
{
	return 0.0;
}