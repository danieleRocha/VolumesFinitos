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

	x0 = 0.0;//m
	xL = L;//m
	fi0 = 0.0;
	fiL = 0.0;

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

bool DifusaoComGeracao1D::Calcular(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
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

	calculou = CalcularSolucaoAnalitica(x,fiAnalitico);
	if(!calculou)
	{
		ImprimirMensagemDeErro(2);
		return calculou;
	}

	calculou = CalcularSolucaoNumerica(x,fiNumerico);
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

void DifusaoComGeracao1D::ImprimirMensagemDeErro(int numeroDoErro)
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

bool DifusaoComGeracao1D::CalcularVariavelIndependenteEspacoX(double* x)
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

void DifusaoComGeracao1D::ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico)
{
	fiAnalitico[0] = fi0;
	fiNumerico[0] = fi0;
	fiAnalitico[numeroDeVolumes+1] = fiL;
	fiNumerico[numeroDeVolumes+1] = fiL;
}

bool DifusaoComGeracao1D::CalcularSolucaoAnalitica(double* x, double* fiAnalitico)
{
	for(int i=1;i<=numeroDeVolumes+1;i++)
	{
		fiAnalitico[i] = -qK*x[i]*x[i]/2 + 2.5*x[i];
	}

	return true;
}

bool DifusaoComGeracao1D::CalcularSolucaoNumerica(double* x, double* fiNumerico)
{
	return VolumesFinitos::ResolverRegimePermanente1D(numeroDeVolumes,dx,fiNumerico,numeroDoMetodoParaSistemaLinear,this->Difusividade,this->TermoFonte);
}

bool DifusaoComGeracao1D::CalcularDesvios(double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
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

void DifusaoComGeracao1D::ImprimirResultados(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos, double* desviosAbsolutos)
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

double DifusaoComGeracao1D::TermoDifusivoDifusaoComGeracao1D::Calcular(double fi)
{
	return 1.0;
}

double DifusaoComGeracao1D::TermoFonteDifusaoComGeracao1D::Calcular(double fi)
{
	return qK;
}