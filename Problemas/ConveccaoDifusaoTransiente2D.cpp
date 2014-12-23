#include "ConveccaoDifusaoTransiente2D.h"

bool ConveccaoDifusaoTransiente2D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 6.0;//m
	H = 2.0;//m
	fiInicial = 100.0;

	TermoFonte->Linear = true;
	TermoFonte->Decrescente = false;

	x0 = 0.0;//m
	xL = L;//m
	y0 = 0.0;//m
	yH = H;//m
	fi00 = fiInicial;
	fi0H =fiInicial;
	fiL0 = fiInicial;
	fiLH = fiInicial;

	dx= (xL-x0)/(numeroDeVolumesX); //Intervalo em x
	dy= (yH-y0)/(numeroDeVolumesY); //Intervalo em x

	difusividade = 1.0e-6; //Fixada
	fluxoArestaDaEntrada = 0.0;
	fiEntrada = 200.0;
	
	(static_cast<TermoDifusivoConvecaoDifusaoTransiente2D*>(Difusividade))->difusividade = difusividade;
	

	DefinirArquivo();
	AlocarMemoria();

	calculou = IniciarCalculos();
	if(!calculou) goto liberarMemoria;

	//ImprimirResultados(tempo[0]);

	for (int i = 1; i < nTempo; i++)
	{
		cout<<"Tempo = "<<tempo[i]<"calculando...";
		calculou = Calcular();
		if(!calculou) goto liberarMemoria;

		
	}
	
	ImprimirResultados(tempo[nTempo-1]);

	//arquivo<<"\nCalculos finalizados com sucesso.\n\n";
	cout<<"\nCalculos finalizados com sucesso.\n\n";

liberarMemoria:
	LiberarMemoria();

	return calculou;
}

bool ConveccaoDifusaoTransiente2D::SolicitarDadosDeEntrada()
{
	bool errou = false;

	cout<<"\n\nDigite o número de volumes em x: ";
	cin >> numeroDeVolumesX;
	cout<<"\n\nDigite o número de volumes em y: ";
	cin >> numeroDeVolumesY;
	cout<<"\n\nDigite o valor do passo de tempo: ";
	cin >> dt;
	cout<<"\n\nDigite o valor do número de passos de tempo: ";
	cin >> nTempo;
	cout<<"\n\nDigite o número do tipo de discretização do termo convectivo( ";
	for(int i=0;i<numeroDeMetodosDeDiscretizacao;i++)
	{
		cout<<"\n"<<nomeMetodosDeDiscretizacao[i];
	}
	cout<<"):";
	cin>> tipoDiscretizacaoTermoConvectivo;

	if((tipoDiscretizacaoTermoConvectivo<0)||(tipoDiscretizacaoTermoConvectivo>(numeroDeMetodosDeDiscretizacao-1)))
	{
		cout<<"Atenção: \n"
			"Número do método inválido. O número do método de discretização do termo convectivo deve estar entre 0 e "<<(numeroDeMetodosDeDiscretizacao-1);
		errou = true;
	}

	return !errou;
}

void ConveccaoDifusaoTransiente2D::ObterCondicoesIniciaisEDeContorno()
{
	
	for(int j = 0;j<=numeroDeVolumesX;j++)
	{
		if(((x[j]>2.5)||(fabs(x[j]-2.5)<1e-5))&&((x[j]<3.5)||(fabs(x[j]-3.5)<1e-5)))
		{
			fiNumerico[0][j] = fiEntrada;
		}
	}

	peclet[0][0] = 0.0;
	peclet[numeroDeVolumesY+1][0] = 0.0;
	peclet[0][numeroDeVolumesX+1]= 0.0;
	peclet[numeroDeVolumesY+1][numeroDeVolumesX+1]= 0.0;

	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::segundoTipo;
	CondicaoDeContornoEsquerda->fluxo = 0.0;

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::segundoTipo;
	CondicaoDeContornoDireita->fluxo = 0.0;

	CondicaoDeContornoSuperior = new CondicaoDeContorno();
	CondicaoDeContornoSuperior->tipo= CondicaoDeContorno::segundoTipo;
	CondicaoDeContornoSuperior->fluxo = 0.0;

	CondicaoDeContornoInferior = new CondicaoDeContornoDinamicaConvecaoDifusaoTransiente2D();
	CondicaoDeContornoInferior->dinamica= true;

	(static_cast<CondicaoDeContornoDinamicaConvecaoDifusaoTransiente2D*>(CondicaoDeContornoInferior))->fluxoContorno = fluxoArestaDaEntrada;
	(static_cast<CondicaoDeContornoDinamicaConvecaoDifusaoTransiente2D*>(CondicaoDeContornoInferior))->fiContorno = fiEntrada;
	
}

void ConveccaoDifusaoTransiente2D::IniciarVariavelNumerica()
{
	//Condição inicial do problema
	for (int i = 0; i <= numeroDeVolumesY+1; i++)
	{
		for (int j = 0; j <= numeroDeVolumesX+1; j++)
		{
			fiNumerico[i][j] = fiInicial;
		}
	}
}

bool ConveccaoDifusaoTransiente2D::CalcularSolucaoAnalitica()
{
	return false;
}

double ConveccaoDifusaoTransiente2D::TermoDifusivoConvecaoDifusaoTransiente2D::Calcular(double fi)
{
	return difusividade;
}

double ConveccaoDifusaoTransiente2D::TermoFonteConvecaoDifusaoTransiente2D::Calcular(double fi,double x)
{
	return 0.0;
}

double ConveccaoDifusaoTransiente2D::TermoFonteConvecaoDifusaoTransiente2D::Derivada(double fi,double x)
{
	return 0.0;
}

double ConveccaoDifusaoTransiente2D::FluxoMassicoConvecaoDifusaoTransiente2D::CalcularX(double x)
{
	return sin(x);
}

double ConveccaoDifusaoTransiente2D::FluxoMassicoConvecaoDifusaoTransiente2D::CalcularY(double y)
{
	return exp(-y);
}

void ConveccaoDifusaoTransiente2D::CondicaoDeContornoDinamicaConvecaoDifusaoTransiente2D::DefinirTipo(double x)
{
	if(((x>2.5)||(fabs(x-2.5)<1e-5))&&((x<3.5)||(fabs(x-3.5)<1e-5)))
	{
		this->tipo = CondicaoDeContorno::primeiroTipo;
		this->fi = fiContorno;
	}
	else
	{
		this->tipo = CondicaoDeContorno::segundoTipo;
		this->fluxo = fluxoContorno;
	}
}