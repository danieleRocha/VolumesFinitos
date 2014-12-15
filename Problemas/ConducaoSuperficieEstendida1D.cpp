#include "ConducaoSuperficieEstendida1D.h"

bool ConducaoSuperficieEstendida1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 0.05; //[m] Comprimento da superfície estendida
	h =  5.0; //[W/(m²K)] Coeficiente de convecção
	D = 0.01; //[m] Diâmetro do pino
	w =0.01;//[m] Largura da aleta
	t =0.01;//[m] Altura da aleta
	k = 10.0; //[W/(mK)] Condutividade térmica do material
	TiInfinito = 293.0;//[K] Temperatura do ambiente
	TBase = 373.0;//[K] Temperatura da base da superfície estendida

	if(numeroDoTipoDeSuperficie==0)
	{
		P=M_PI*D;//m Perímetro do pino
		Ac = M_PI*D*D/4;// m² Área da seção transversal do pino
	}
	else
	{
		P=2*w+2*t;//m Perímetro da aleta
		Ac = w*t;// m² Área da seção transversal da aleta
	}

	m2 = (h*P)/(k*Ac); //Parâmetro admensional
	
	//Passa dados para o objeto TermoFonte
	(static_cast<TermoFonteConducaoSuperficieEstendida1D*>(TermoFonte))->m2 = m2;
	TermoFonte->Linear = false;
	TermoFonte->Decrescente = true;
	tipoDiscretizacaoTermoConvectivo = 0;

	x0 = 0.0;//m
	xL = L;//m
	fi0 = TBase-TiInfinito;
	
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

bool ConducaoSuperficieEstendida1D::SolicitarDadosDeEntrada()
{
	bool errou = false;

	cout<<"Digite o número do tipo da superfície estendida(";

	for(int i=0;i<nSuperficiesDisponiveis;i++)
	{
		cout<<"\n"<<nomeSuperficies[i];
	}
	cout<<"):";
	cin >>numeroDoTipoDeSuperficie;
	cout<<"\n\nDigite o número de volumes: ";
	cin >> numeroDeVolumes;
	cout<<"Digite o número do método para solução de sistemas lineares(";

	for(int i=0;i<nMetodosParaSistemasLinearesDisponiveis;i++)
	{
		cout<<"\n"<<nomeMetodosParaSistemasLineares[i];
	}
	cout<<"):";
	cin >>numeroDoMetodoParaSistemaLinear;

	if((numeroDoTipoDeSuperficie<0)||(numeroDoTipoDeSuperficie>(nSuperficiesDisponiveis-1)))
	{
		cout<<"Atenção: \n"
			"Número do tipo de superfície inválido. O número do do tipo de superfície deve estar entre 0 e "<<(nSuperficiesDisponiveis-1);
		errou = true;
	}

	if((numeroDoMetodoParaSistemaLinear<0)||(numeroDoMetodoParaSistemaLinear>(nMetodosParaSistemasLinearesDisponiveis-1)))
	{
		cout<<"Atenção: \n"
			"Número do método inválido. O número do método para solução de sistemas lineares deve estar entre 0 e "<<(nMetodosParaSistemasLinearesDisponiveis-1);
		errou = true;
	}

	return !errou;
}

void ConducaoSuperficieEstendida1D::ObterCondicoesIniciaisEDeContorno()
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiNumerico[numeroDeVolumes+1] = 62.125;
	peclet[0] = 0.0;
	peclet[numeroDeVolumes+1]= 0.0;
	
	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoEsquerda->fi = fi0;

	/*CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoDireita->fi = 62.125;*/

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::terceiroTipo;
	CondicaoDeContornoDireita->alfa = h/k;
	CondicaoDeContornoDireita->fiInfinito = 0.0; //Para cálculo adimensional -->fi = (T - Tinfinito)

}

void ConducaoSuperficieEstendida1D::IniciarVariavelNumerica()
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = fi0;
	}
}

bool ConducaoSuperficieEstendida1D::CalcularSolucaoAnalitica()
{
	double m = pow(m2,0.5);

	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = ((cosh(m*(L-x[i]))+(h/(m*k))*sinh(m*(L-x[i])))/
			(cosh(m*L)+(h/(m*k))*sinh(m*L)))*fi0;
	}

	return true;
}

double ConducaoSuperficieEstendida1D::TermoDifusivoConducaoSuperficieEstendida1D::Calcular(double fi)
{
	return 1.0;
}

double ConducaoSuperficieEstendida1D::TermoFonteConducaoSuperficieEstendida1D::Calcular(double fi,double x)
{
	return -m2*fi;
}

double ConducaoSuperficieEstendida1D::TermoFonteConducaoSuperficieEstendida1D::Derivada(double fi,double x)
{
	return -m2;
}

double ConducaoSuperficieEstendida1D::FluxoMassicoConducaoSuperficieEstendida1D::Calcular()
{
	return 0.0;
}