#include "VolumesFinitos.h"


bool VolumesFinitos::ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* fi,int numeroDoMetodoParaSistemaLinear,
												Difusividade* difusividade, TermoFonte* termoFonte,
												CondicaoDeContorno* condicaoDeContornoEsquerda,
												CondicaoDeContorno* condicaoDeContornoDireita)
{
	bool calculou = true;

	//Parâmetros das Iterações do Coeficiente de Difusão
	double* fiInstanteAnterior;
	double* fiIteracaoAnterior;
	double* delta;
	double erroMaximo = 100;
	double deltaPermitido = 0.01; //%
	int iteracao = 1;
	int numeroMaximoIteracoes = 100;

	//Parâmetros da malha
	double* aP;
	double* aE;
	double* aW;
	double* b;

#pragma region AlocacaoDeMemoria

	fiInstanteAnterior = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	fiIteracaoAnterior = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	delta = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aP = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aE = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aW = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	b = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));

#pragma endregion 

#pragma region Calculo

	for(int i = 0;i<numeroDeVolumes;i++)
	{
		fiInstanteAnterior[i] = fi[i+1];
	}

	//Iteraçoes do cálculo iterativo da difusividade e termo fonte (se houver)
	while((erroMaximo>deltaPermitido)&&(iteracao<numeroMaximoIteracoes))
	{
		erroMaximo = 0.0;
		for(int i = 0;i<numeroDeVolumes;i++)
		{
			fiIteracaoAnterior[i] = fi[i+1];
		}

		DefinirMalha(aP,aE,aW,b,numeroDeVolumes,dx,fi,fiInstanteAnterior,difusividade,termoFonte,
			condicaoDeContornoEsquerda,condicaoDeContornoDireita);

		calculou = ResolverSistemaLinear(fi,aP,aE,aW,b,numeroDeVolumes,numeroDoMetodoParaSistemaLinear);

		AjustarCondicoesDeContorno(numeroDeVolumes,dx,fi,difusividade,condicaoDeContornoEsquerda,condicaoDeContornoDireita);

		//houveErro = (*metodosBloco[0])(numeroDePontos,aP,aE,aW,aN,aS,c,incognitas);

		if(!calculou) goto liberarMemoria;

		for(int i = 0;i<numeroDeVolumes;i++)
		{
			delta[i] = 100*(fiIteracaoAnterior[i]-fi[i+1])/fi[i+1];

			if(erroMaximo<fabs(delta[i]))
				erroMaximo = fabs(delta[i]);
		}

		iteracao = iteracao + 1;
	}

	if(iteracao>numeroMaximoIteracoes)
	{
		calculou = false;
		goto liberarMemoria;
	}

#pragma endregion 

liberarMemoria:


#pragma region LiberacaoMemoria

	free(static_cast<void*>(fiInstanteAnterior));
	free(static_cast<void*>(fiIteracaoAnterior));
	free(static_cast<void*>(delta));
	free(static_cast<void*>(aP));
	free(static_cast<void*>(aE));
	free(static_cast<void*>(aW));
	free(static_cast<void*>(b));

#pragma endregion

	return calculou;
}


void VolumesFinitos::DefinirMalha(double* aP, double* aE, double* aW,double* b,int numeroDeVolumes,double dx, double* fi,
								  double* fiInstanteAnterior,Difusividade* difusividade,TermoFonte* termoFonte,
								  CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;

#pragma region ContrucaoMalha

	for(int i=0;i<numeroDeVolumes;i++)
	{
		double taue;
		double tauw;

		if(i==0)//Condição de contorno à esquerda
			tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[i+1],difusividade); 
		else
			tauw = CalcularTauw(dxmenos,dxmais,dx,fi[i],fi[i+1],difusividade); 

		if(i==(numeroDeVolumes-1))//Condição de contorno à direita
			taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[i+1],difusividade);
		else
			taue = CalcularTaue(dxmenos,dxmais,dx,fi[i+2],fi[i+1],difusividade);

		aW[i] = tauw/dx;
		aE[i] = taue/dx;
		aP[i] = 0;

		if(termoFonte->Linear)
		{
			b[i] = termoFonte->Calcular(fi[i+1])*dx;
		}
		else //Para termo fonte linearizado(S(fi)=Sc+Sp*fip)
		{
			//decrescente com fi --> Sp=dS/dfi|fi* e Sc = S(fi*)-fi*dS/dfi|fi*
			//...então b=b+(S(fi)-(fi*)*(dS/dfi|fi*))*dx e aP = aP - dx*(dS/dfi|fi*)
			if(termoFonte->Decrescente)
			{
				b[i] = (termoFonte->Calcular(fi[i+1])-termoFonte->Derivada(fi[i+1])*fi[i+1])*dx;
				aP[i] = aP[i] - (termoFonte->Derivada(fi[i+1]))*dx;
			}
			else //crescente com fi --> Sp=0 e Sc = S(fi*)...então b=b+Sc*dx
			{
				b[i] = termoFonte->Calcular(fi[i+1])*dx;
			}
		}
		
		if(i==0) //Condição de contorno à esquerda
		{
			aW[i] = 0;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo)
			{
				aP[i] = aP[i] + (tauw/dxmais);
				b[i] = b[i] + ((tauw*condicaoDeContornoEsquerda->fi)/dxmais);
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				b[i] = b[i] + condicaoDeContornoEsquerda->fluxo;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceitoTipo)
			{
				b[i] = b[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa*condicaoDeContornoEsquerda->fiInfinito)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

				aP[i] = aP[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

			}

		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo)
			{
				aP[i] = aP[i] + (taue/dxmenos);
				b[i] = b[i] + ((taue*condicaoDeContornoDireita->fi)/dxmenos);
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)
			{
				b[i] = b[i] - condicaoDeContornoDireita->fluxo;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceitoTipo)
			{
				b[i] = b[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));

				aP[i] = aP[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));
			}

		}

		aP[i] = aP[i] + aE[i]+aW[i];
	}

#pragma endregion

}

void VolumesFinitos::AjustarCondicoesDeContorno(int numeroDeVolumes,double dx, double* fi,
												Difusividade* difusividade,
												CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;

	if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
	{
		double tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[1],difusividade); 
		fi[0] = (condicaoDeContornoEsquerda->fluxo*dxmais)/tauw+fi[1];
	}
	else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceitoTipo)
	{
		double tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[1],difusividade); 

		double fiw = ((tauw/dxmais)*fi[1]+condicaoDeContornoEsquerda->alfa*condicaoDeContornoEsquerda->fiInfinito)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

		fi[0] = fiw;
	}

	if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)
	{
		double taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[numeroDeVolumes],difusividade);
		fi[numeroDeVolumes+1] = fi[numeroDeVolumes]-(condicaoDeContornoDireita->fluxo*dxmenos)/taue;
	}
	else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceitoTipo)
	{
		double taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[numeroDeVolumes],difusividade);

		double fie = ((taue/dxmenos)*fi[numeroDeVolumes]+condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
			(condicaoDeContornoDireita->alfa+(taue/dxmenos));

		fi[numeroDeVolumes+1] = fie;
	}

}

bool VolumesFinitos::ResolverSistemaLinear(double* fi,double* aP, double* aE, double* aW,
										   double* b, int numeroDeVolumes,int numeroDoMetodoParaSistemaLinear)
{
	bool calculou;
	double** matrizCoeficientes;

#pragma region AlocacaoDeMemoria

	matrizCoeficientes=static_cast<double**>(malloc(numeroDeVolumes*sizeof(double*)));

	for (int i=0; i<numeroDeVolumes; i++)
	{
		matrizCoeficientes[i]=static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	}

#pragma endregion 

	for (int i = 0; i < numeroDeVolumes; i++)
	{
		for (int j = 0; j < numeroDeVolumes; j++)
		{
			if(i==j)
				matrizCoeficientes[i][j] = aP[i];
			else if(j==(i-1))
				matrizCoeficientes[i][j] = -aW[i];
			else if(j==(i+1))
				matrizCoeficientes[i][j] = -aE[i];
			else
				matrizCoeficientes[i][j] = 0.0;
		}
	}

	SistemaLinear* sistema = new SistemaLinear(numeroDeVolumes,matrizCoeficientes,b);

	MetodoParaSistemaLinear* metodo =  FabricaDeMetodosParaSistemaLinear::CriarMetodo(numeroDoMetodoParaSistemaLinear);

	string mensagem = "";

	calculou = metodo->Resolver(*sistema,mensagem);

	if(calculou)
	{
		for (int i = 0; i < numeroDeVolumes; i++)
		{
			fi[i+1] = sistema->vetorCoeficientes[i];
		}
	}

#pragma region LiberacaoMemoria

	for (int i=0; i<numeroDeVolumes; i++)
	{
		free(static_cast<void*>(matrizCoeficientes[i]));

	}
	free(static_cast<void*>(matrizCoeficientes));

#pragma endregion 


	return calculou;
}

double VolumesFinitos::CalcularTauw(double dxmenos, double dxmais, double dx, double fiW,double fiP,
									Difusividade* difusividade)
{
	double tauw = 0.0;
	double tauW = difusividade->Calcular(fiW);
	double tauP =  difusividade->Calcular(fiP);

	if((tauW!=0.0)&&(tauP!=0.0))
		tauw = 1/((dxmenos/dx)/tauW + (dxmais/dx)/tauP);

	return tauw;
}

double VolumesFinitos::CalcularTaue(double dxmenos, double dxmais, double dx, double fiE,double fiP,
									Difusividade* difusividade)
{
	double taue = 0.0;
	double tauE= difusividade->Calcular(fiE);
	double tauP = difusividade->Calcular(fiP);

	if((tauE!=0.0)&&(tauP!=0.0))
		taue = 1/((dxmenos/dx)/tauP + (dxmais/dx)/tauE);

	return taue;
}