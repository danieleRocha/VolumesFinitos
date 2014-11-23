#include "ConducaoPino1D.h"

bool ConducaoPino1D::Resolver()
{
	bool calculou = true;

	calculou =  SolicitarDadosDeEntrada();
	if(!calculou) return calculou;

	L = 0.05; //[m] Comprimento do pino
	h =  5.0; //[W/(m�K)] Coeficiente de convec��o
	D = 0.01; //[m] Di�metro do pino
	k = 10.0; //[W/(mK)] Condutividade t�rmica do pino
	TiInfinito = 293.0;//[K] Temperatura do ambiente
	TBase = 373.0;//[K] Temperatura da base do pino

	P=M_PI*D;//m Per�metro do pino
	Ac = M_PI*D*D/4;// m� �rea da se��o transversal do pino

	m2 = (h*P)/(k*Ac); //Par�metro admensional
	
	//Passa dados para o objeto TermoFonte
	(static_cast<TermoFonteConducaoPino1D*>(TermoFonte))->m2 = m2;

	x0 = 0.0;//m
	xL = L;//m
	fi0 = TBase-TiInfinito;
	
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

bool ConducaoPino1D::SolicitarDadosDeEntrada()
{
	bool errou = false;

	cout<<"\n\nDigite o n�mero de volumes: ";
	cin >> numeroDeVolumes;
	cout<<"Digite o n�mero do m�todo para solu��o de sistemas lineares(";

	for(int i=0;i<nMetodosParaSistemasLinearesDisponiveis;i++)
	{
		cout<<"\n"<<nomeMetodosParaSistemasLineares[i];
	}
	cout<<"):";
	cin >>numeroDoMetodoParaSistemaLinear;

	if((numeroDoMetodoParaSistemaLinear<0)||(numeroDoMetodoParaSistemaLinear>(nMetodosParaSistemasLinearesDisponiveis-1)))
	{
		cout<<"Aten��o: \n"
			"N�mero do m�todo inv�lido. O n�mero do m�todo para solu��o de sistemas lineares deve estar entre 0 e "<<(nMetodosParaSistemasLinearesDisponiveis-1);
		errou = true;
	}

	return !errou;
}

void ConducaoPino1D::ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico)
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiNumerico[numeroDeVolumes+1] = 62.125;
	
	CondicaoDeContornoEsquerda = new CondicaoDeContorno();
	CondicaoDeContornoEsquerda->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoEsquerda->fi = fi0;

	/*CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::primeiroTipo;
	CondicaoDeContornoDireita->fi = 62.125;*/

	CondicaoDeContornoDireita = new CondicaoDeContorno();
	CondicaoDeContornoDireita->tipo= CondicaoDeContorno::terceitoTipo;
	CondicaoDeContornoDireita->alfa = h/k;
	CondicaoDeContornoDireita->fiInfinito = 0.0; //Para c�lculo adimensional -->fi = (T - Tinfinito)

}

void ConducaoPino1D::IniciarVariavelNumerica(double* fiNumerico)
{
	for (int i = 1; i <= numeroDeVolumes; i++)
	{
		fiNumerico[i] = fi0;
	}
}

bool ConducaoPino1D::CalcularSolucaoAnalitica(double* x, double* fiAnalitico)
{
	double m = pow(m2,0.5);

	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = ((cosh(m*(L-x[i]))+(h/(m*k))*sinh(m*(L-x[i])))/
			(cosh(m*L)+(h/(m*k))*sinh(m*L)))*fi0;
	}

	return true;
}

double ConducaoPino1D::TermoDifusivoConducaoPino1D::Calcular(double fi)
{
	return 1.0;
}

double ConducaoPino1D::TermoFonteConducaoPino1D::Calcular(double fi)
{
	return -m2*fi;
}

