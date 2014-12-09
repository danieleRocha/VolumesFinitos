#include "VolumesFinitos.h"


bool VolumesFinitos::ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* fi,double* peclet,int numeroDoMetodoParaSistemaLinear,
												Difusividade* difusividade,FluxoMassico* fluxoMassico, TermoFonte* termoFonte,
												CondicaoDeContorno* condicaoDeContornoEsquerda,
												CondicaoDeContorno* condicaoDeContornoDireita,
												Discretizacao discretizacaoTermoConvectivo)
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

		calculou = DefinirMalha(aP,aE,aW,b,numeroDeVolumes,dx,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
			condicaoDeContornoEsquerda,condicaoDeContornoDireita,discretizacaoTermoConvectivo);

		if(!calculou) goto liberarMemoria;

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



///Só serve para malha uniforme
bool VolumesFinitos::DefinirMalha(double* aP, double* aE, double* aW,double* b,int numeroDeVolumes,double dx, double* fi,double* peclet,
								  double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
								  CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita,
								  Discretizacao discretizacaoTermoConvectivo)
{
	double dxmais = dx/2;
	double dxmenos = dx/2;
	double fluxo = fluxoMassico->Calcular();

#pragma region ContrucaoMalha

	for(int i=0;i<numeroDeVolumes;i++)
	{
		double taue;
		double tauw;
		double difusiv = difusividade->Calcular(fi[i+1]);

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
		aP[i] = 0.0;

		switch (discretizacaoTermoConvectivo)
		{
		case DiferencasCentrais: 
			aW[i] = aW[i] + fluxo*dxmais/dx;
			aE[i] = aE[i] - fluxo*dxmenos/dx;
			break;
		case Upwind: 
			aW[i] = aW[i] + max(0.0,fluxo);
			aE[i] = aE[i] + max(0.0, -fluxo);
			break;
		case QUICK: 
			break;

		}

		if(difusiv!=0.0)
			peclet[i+1] = (fluxo*dx)/difusiv;
		else
			peclet[i+1] = 0.0;

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
			aP[i] = aP[i]+ fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo) //Validado
			{
				aP[i] = aP[i] + (tauw/dxmais);
				b[i] = b[i] + (tauw/dxmais)*condicaoDeContornoEsquerda->fi;

				switch (discretizacaoTermoConvectivo)
				{
				case DiferencasCentrais: 
					b[i] = b[i] + (fluxo)*condicaoDeContornoEsquerda->fi;
					break;
				case Upwind: 
					aP[i] = aP[i] - min(fluxo,0.0);
					b[i] = b[i] + (max(0.0,fluxo))*condicaoDeContornoEsquerda->fi;
					break;
				case QUICK: 
					break;

				}
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)//Todo:Validar
			{
				aP[i] = aP[i] - fluxo;
				b[i] = b[i] + (1)*condicaoDeContornoEsquerda->fluxo;


				switch (discretizacaoTermoConvectivo)
				{
				case DiferencasCentrais: 
					b[i] = b[i] + (fluxo/(tauw/dxmais))*condicaoDeContornoEsquerda->fluxo;
					break;
				case Upwind: 
					b[i] = b[i] + ((max(fluxo,0.0))/(tauw/dxmais))*condicaoDeContornoEsquerda->fluxo;
					break;
				case QUICK: 
					break;

				}

			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceitoTipo)
			{
				//Todo: Implementar mudança quando se considera o termo convectivo e os diferentes métodos

				b[i] = b[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa*condicaoDeContornoEsquerda->fiInfinito)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

				aP[i] = aP[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

			}

		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;
			aP[i] = aP[i] -fluxo;

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) //Validado
			{
				aP[i] = aP[i]  + (taue/dxmenos);
				b[i] = b[i] + (taue/dxmenos)*condicaoDeContornoDireita->fi;

				switch (discretizacaoTermoConvectivo)
				{
				case DiferencasCentrais: 
					b[i] = b[i] + (-fluxo)*condicaoDeContornoDireita->fi;
					break;
				case Upwind: 
					aP[i] = aP[i] - min(0.0, -fluxo);
					b[i] = b[i] + (max(0.0,-fluxo))*condicaoDeContornoDireita->fi;
					break;
				case QUICK: 
					break;

				}
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)//Todo:Validar
			{
				aP[i] = aP[i] + fluxo;
				b[i] = b[i] + (-1)*condicaoDeContornoDireita->fluxo;

				switch (discretizacaoTermoConvectivo)
				{
				case DiferencasCentrais: 
					b[i] = b[i] + (fluxo/(taue/dxmenos))*condicaoDeContornoDireita->fluxo;
					break;
				case Upwind: 
					b[i] = b[i] + (-(max(-fluxo,0.0))/(taue/dxmenos))*condicaoDeContornoDireita->fluxo;
					break;
				case QUICK: 
					break;

				}
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceitoTipo)
			{
				//Todo: Implementar mudança quando se considera o termo convectivo e os diferentes métodos

				b[i] = b[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));

				aP[i] = aP[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));
			}

		}

		aP[i] = aP[i] + aE[i]+aW[i];

		/*if(aP[i]>0)
		{
		if(aE[i]<0) return false;
		if(aW[i]<0) return false;
		}
		else
		{
		if(aE[i]>0) return false;
		if(aW[i]>0) return false;
		}*/
	}



#pragma endregion

	return true;

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