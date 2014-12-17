#include "ConveccaoDifusao1D.h"

bool ConveccaoDifusao1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 1.0;//m
	
	TermoFonte->Linear = true;
	TermoFonte->Decrescente = false;

	x0 = 0.0;//m
	xL = L;//m
	fi0 = 1.0;
	fiL = 0.0;

	dx= (xL-x0)/(numeroDeVolumes); //Intervalo em x

	difusividade = 0.1; //Fixada
	fluxoMassico = 0.1;
	
	
	(static_cast<TermoDifusivoConveccaoDifusao1D*>(Difusividade))->difusividade = difusividade;
	(static_cast<FluxoMassicoConveccaoDifusao1D*>(FluxoMassico))->fluxoMassico = fluxoMassico;

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

bool ConveccaoDifusao1D::SolicitarDadosDeEntrada()
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

void ConveccaoDifusao1D::ObterCondicoesIniciaisEDeContorno()
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

void ConveccaoDifusao1D::IniciarVariavelNumerica()
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = 0.0;
	}
}

bool ConveccaoDifusao1D::CalcularSolucaoAnalitica()
{
	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = ((exp(fluxoMassico*x[i]/difusividade)-1)/(exp(fluxoMassico*L/difusividade)-1))*(fiL-fi0)+fi0;
	}

	return true;
}

double ConveccaoDifusao1D::TermoDifusivoConveccaoDifusao1D::Calcular(double fi)
{
	return difusividade;
}

double ConveccaoDifusao1D::TermoFonteConveccaoDifusao1D::Calcular(double fi,double x)
{
	return 0.0;
}

double ConveccaoDifusao1D::TermoFonteConveccaoDifusao1D::Derivada(double fi,double x)
{
	return 0.0;
}

double ConveccaoDifusao1D::FluxoMassicoConveccaoDifusao1D::Calcular()
{
	return fluxoMassico;
}