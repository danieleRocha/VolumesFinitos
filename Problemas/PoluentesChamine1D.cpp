#include "PoluentesChamine1D.h"

bool PoluentesChamine1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 6.0;//m
	
	TermoFonte->Linear = true;
	TermoFonte->Decrescente = false;

	x0 = 0.0;//m
	xL = L;//m
	fi0 = 0.0;
	fiL = 2.0;

	dx= (xL-x0)/(numeroDeVolumes); //Intervalo em x

	difusividade =2.0; //Fixada
	fluxoMassico = -3.0;

	(static_cast<TermoDifusivoPoluentesChamine1D*>(Difusividade))->difusividade = difusividade;
	(static_cast<FluxoMassicoPoluentesChamine1D*>(FluxoMassico))->fluxoMassico = fluxoMassico;

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

bool PoluentesChamine1D::SolicitarDadosDeEntrada()
{
	bool errou = false;

	cout<<"\n\nDigite o número de volumes: ";
	cin >> numeroDeVolumes;
	cout<<"\n\nDigite o número do tipo de discretização do termo convectivo( ";
	for(int i=0;i<numeroDeMetodosDeDiscretizacao;i++)
	{
		cout<<"\n"<<nomeMetodosDeDiscretizacao[i];
	}
	cout<<"):";
	cin>> tipoDiscretizacaoTermoConvectivo;

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

	if((tipoDiscretizacaoTermoConvectivo<0)||(tipoDiscretizacaoTermoConvectivo>(numeroDeMetodosDeDiscretizacao-1)))
	{
		cout<<"Atenção: \n"
			"Número do método inválido. O número do método de discretização do termo convectivo deve estar entre 0 e "<<(numeroDeMetodosDeDiscretizacao-1);
		errou = true;
	}

	return !errou;
}

void PoluentesChamine1D::ObterCondicoesIniciaisEDeContorno()
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiAnalitico[numeroDeVolumes+1] = fiL;
	fiNumerico[numeroDeVolumes+1] = fiL;
	peclet[0] = 0.0;
	peclet[numeroDeVolumes+1]= 0.0;

	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::segundoTipo;
	CondicaoDeContornoEsquerda->fluxo = -2.0;

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoDireita->fi = fiL;

}

void PoluentesChamine1D::IniciarVariavelNumerica()
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = 0.0;
	}
}

bool PoluentesChamine1D::CalcularSolucaoAnalitica()
{
	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = 0.0;
	}

	return true;
}

double PoluentesChamine1D::TermoDifusivoPoluentesChamine1D::Calcular(double fi)
{
	return difusividade;
}

double PoluentesChamine1D::TermoFontePoluentesChamine1D::Calcular(double fi,double x)
{
	double valor = x*cos(M_PI*x);
	return valor;
}

double PoluentesChamine1D::TermoFontePoluentesChamine1D::Derivada(double fi,double x)
{
	return 0.0;
}

double PoluentesChamine1D::FluxoMassicoPoluentesChamine1D::Calcular()
{
	return fluxoMassico;
}