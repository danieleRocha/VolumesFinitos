#include "VolumesFinitos.h"



bool VolumesFinitos::ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* x,double* fi,double* peclet,int numeroDoMetodoParaSistemaLinear,
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
	double* aEE;
	double* aW;
	double* aWW;
	double* b;
	double* resposta;

#pragma region AlocacaoDeMemoria

	fiInstanteAnterior = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	fiIteracaoAnterior = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	delta = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aP = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aE = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aEE = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aW = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	aWW = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	b = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));
	resposta = static_cast<double*>(malloc(numeroDeVolumes*sizeof(double)));

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

		switch (discretizacaoTermoConvectivo)
		{
		case DiferencasCentrais: 
			calculou = DefinirMalhaDiferencasCentrais(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
				condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;
		case Upwind: 
			calculou = DefinirMalhaUpwind(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
				condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;
		case QUICK: 
			calculou = DefinirMalhaQuick(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
				condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;
		case DiferencasCentraisDeferredCorrection: 
			calculou = DefinirMalhaDiferencasCentraisDeferredCorrection(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
				condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;
		default: 
			return false;
		}


		if(!calculou) goto liberarMemoria;

		calculou = CalculadoraSistemaLinear1D::ResolverSistemaLinear(resposta,aP,aE,aEE,aW,aWW,b,numeroDeVolumes,numeroDoMetodoParaSistemaLinear);

		if(!calculou) goto liberarMemoria;

		for (int i = 0; i < numeroDeVolumes; i++)
		{
			fi[i+1] = resposta[i];
		}

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
	free(static_cast<void*>(aEE));
	free(static_cast<void*>(aW));
	free(static_cast<void*>(aWW));
	free(static_cast<void*>(b));
	free(static_cast<void*>(resposta));

#pragma endregion

	return calculou;
}

bool VolumesFinitos::ResolverRegimePermanente2D(int numeroDeVolumesX, double dx, double* x, int numeroDeVolumesY, double dy,
												double* y, double dt, double** fi, double** peclet,
												Difusividade* difusividade, FluxoMassicoVariavel* fluxoMassico, TermoFonte* termoFonte, 
												CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita, 
												CondicaoDeContorno* condicaoDeContornoInferior,CondicaoDeContorno* condicaoDeContornoSuperior,
												Discretizacao discretizacaoTermoConvectivo)
{
	bool calculou = true;

	//Parâmetros das Iterações do Coeficiente de Difusão
	double** fiInstanteAnterior;
	double** fiIteracaoAnterior;
	double** delta;
	double erroMaximo = 100;
	double deltaPermitido = 0.01; //%
	int iteracao = 1;
	int numeroMaximoIteracoes = 100;

	//Parâmetros da malha
	double** aP;
	double** aE;
	double** aW;
	double** aN;
	double** aS;
	double** b;
	double** resposta;

#pragma region AlocacaoDeMemoria

	fiInstanteAnterior = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	fiIteracaoAnterior = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	delta = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aP = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aE = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aW = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aN = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aS = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	b = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	resposta = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));

	for (int i=0; i<numeroDeVolumesY; i++)
	{
		fiInstanteAnterior[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		fiIteracaoAnterior[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		delta[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aP[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aE[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aW[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aN[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aS[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		b[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		resposta[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
	}

#pragma endregion 

#pragma region Calculo

	for(int i = 0;i<numeroDeVolumesY;i++)
	{
		for(int j = 0;j<numeroDeVolumesX;j++)
		{
			fiInstanteAnterior[i][j] = fi[i+1][j+1];
		}
	}

	//Iteraçoes do cálculo iterativo da difusividade e termo fonte (se houver)
	while((erroMaximo>deltaPermitido)&&(iteracao<numeroMaximoIteracoes))
	{
		erroMaximo = 0.0;
		for(int i = 0;i<numeroDeVolumesY;i++)
		{
			for(int j = 0;j<numeroDeVolumesX;j++)
			{
				fiIteracaoAnterior[i][j] = fi[i+1][j+1];
				resposta[i][j] = fi[i+1][j+1];
			}
		}

		switch (discretizacaoTermoConvectivo)
		{
		case DiferencasCentrais: 
			calculou = DefinirMalhaDiferencasCentrais(aP,aE,aW,aN,aS,b,numeroDeVolumesX,dx,x,numeroDeVolumesY,dy,y,dt,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
				condicaoDeContornoEsquerda,condicaoDeContornoDireita,condicaoDeContornoSuperior,condicaoDeContornoInferior);
			break;
			/*case Upwind: 
			calculou = DefinirMalhaUpwind(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
			condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;
			case DiferencasCentraisDeferredCorrection: 
			calculou = DefinirMalhaDiferencasCentraisDeferredCorrection(aP,aE,aEE,aW,aWW,b,numeroDeVolumes,dx,x,fi,peclet,fiInstanteAnterior,difusividade,fluxoMassico,termoFonte,
			condicaoDeContornoEsquerda,condicaoDeContornoDireita);
			break;*/
		default: 
			return false;
		}


		if(!calculou) goto liberarMemoria;

		//Só está funcionando para mesmo número de volumes em x e y!!!!!!!!!
		calculou = CalculadoraSistemaLinear2D::ResolverSistemaLinear(numeroDeVolumesX,aP,aE,aW,aN,aS,b,resposta);

		if(!calculou) goto liberarMemoria;

		for (int i = 0; i < numeroDeVolumesY; i++)
		{
			for (int j = 0; j < numeroDeVolumesX; j++)
			{
				fi[i+1][j+1] = resposta[i][j];
			}
		}

		AjustarCondicoesDeContorno(numeroDeVolumesX,numeroDeVolumesY,x,dx,dy,fi,difusividade,condicaoDeContornoEsquerda,
			condicaoDeContornoDireita,condicaoDeContornoInferior,condicaoDeContornoSuperior);

		//houveErro = (*metodosBloco[0])(numeroDePontos,aP,aE,aW,aN,aS,c,incognitas);

		if(!calculou) goto liberarMemoria;

		for(int i = 0;i<numeroDeVolumesY;i++)
		{
			for(int j = 0;j<numeroDeVolumesX;j++)
			{
				delta[i][j] = 100*(fiIteracaoAnterior[i][j]-fi[i+1][j+1])/fi[i+1][j+1];

				if(erroMaximo<fabs(delta[i][j]))
					erroMaximo = fabs(delta[i][j]);
			}
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

	fiInstanteAnterior = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	fiIteracaoAnterior = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	delta = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aP = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aE = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aW = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aN = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	aS = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	b = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));
	resposta = static_cast<double**>(malloc(numeroDeVolumesY*sizeof(double)));


	for (int i=0; i<numeroDeVolumesY; i++)
	{
		fiInstanteAnterior[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		fiIteracaoAnterior[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		delta[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aP[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aE[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aW[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aN[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		aS[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		b[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
		resposta[i]=static_cast<double*>(malloc(numeroDeVolumesX*sizeof(double)));
	}

#pragma endregion

	return calculou;
}

bool VolumesFinitos::DefinirMalhaDiferencasCentrais(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,double* x, double* fi,double* peclet,
													double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
													CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
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

		aW[i] = tauw/dx + fluxo*dxmais/dx;
		aE[i] = taue/dx - fluxo*dxmenos/dx;
		aP[i] = 0.0; 
		aEE[i] = 0.0;
		aWW[i] = 0.0;

		if(difusiv!=0.0)
			peclet[i+1] = (fluxo*dx)/difusiv;
		else
			peclet[i+1] = 0.0;

		if(termoFonte->Linear)
		{
			b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
		}
		else //Para termo fonte linearizado(S(fi)=Sc+Sp*fip)
		{
			//decrescente com fi --> Sp=dS/dfi|fi* e Sc = S(fi*)-fi*dS/dfi|fi*
			//...então b=b+(S(fi)-(fi*)*(dS/dfi|fi*))*dx e aP = aP - dx*(dS/dfi|fi*)
			if(termoFonte->Decrescente)
			{
				b[i] = (termoFonte->Calcular(fi[i+1],x[i+1])-termoFonte->Derivada(fi[i+1],x[i+1])*fi[i+1])*dx;
				aP[i] = aP[i] - (termoFonte->Derivada(fi[i+1],x[i+1]))*dx;
			}
			else //crescente com fi --> Sp=0 e Sc = S(fi*)...então b=b+Sc*dx
			{
				b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
			}
		}

		if(i==0) //Condição de contorno à esquerda
		{
			aW[i] = 0;
			aP[i] = aP[i]+ 2*fluxo*(dxmais/dx);

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo) //Validado para convecção
			{
				aP[i] = aP[i] + (tauw/dxmais);
				b[i] = b[i] + (tauw/dxmais + fluxo)*condicaoDeContornoEsquerda->fi;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo) //Validado para convecção
			{
				aP[i] = aP[i] - fluxo;
				b[i] = b[i] + (1+fluxo/(tauw/dxmais))*condicaoDeContornoEsquerda->fluxo;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				//Todo: Implementar mudança quando se considera o termo convectivo

				b[i] = b[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa*condicaoDeContornoEsquerda->fiInfinito)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

				aP[i] = aP[i] + ((tauw/dxmais)*condicaoDeContornoEsquerda->alfa)/
					(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

			}

		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;
			aP[i] = aP[i]- 2*fluxo*(dxmenos/dx);

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) //Validado para convecção
			{
				aP[i] = aP[i] + (taue/dxmenos);
				b[i] = b[i] + (taue/dxmenos-fluxo)*condicaoDeContornoDireita->fi;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo) //Validado para convecção
			{
				aP[i] = aP[i] + fluxo;
				b[i] = b[i] + (fluxo/(taue/dxmenos)-1)*condicaoDeContornoDireita->fluxo;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
			{
				//Todo: Implementar mudança quando se considera o termo convectivo

				b[i] = b[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));

				aP[i] = aP[i] + ((taue/dxmenos)*condicaoDeContornoDireita->alfa)/
					(condicaoDeContornoDireita->alfa+(taue/dxmenos));
			}

		}
		else
		{
			aP[i] = aP[i] + 2*fluxo*(dxmais/dx) - 2*fluxo*(dxmenos/dx);
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

bool VolumesFinitos::DefinirMalhaDiferencasCentrais(double** aP, double** aE, double** aW, double** aN, double** aS, 
													double** b, int numeroDeVolumesX, double dx, double* x, 
													int numeroDeVolumesY, double dy, double* y, double dt, double** fi, 
													double** peclet, double** fiInstanteAnterior, Difusividade* difusividade,
													FluxoMassicoVariavel* fluxoMassico, TermoFonte* termoFonte,
													CondicaoDeContorno* condicaoDeContornoEsquerda,
													CondicaoDeContorno* condicaoDeContornoDireita, CondicaoDeContorno* condicaoDeContornoSuperior, 
													CondicaoDeContorno* condicaoDeContornoInferior)
{
	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;
	double dymais = dy/2;
	double dymenos = dy/2;


#pragma region ContrucaoMalha

	for(int i=0;i<numeroDeVolumesY;i++)
	{
		double yn = y[i+2];
		double ys = x[i];

		double fluxoN = fluxoMassico->CalcularY(yn);
		double fluxoS = fluxoMassico->CalcularY(ys);

		for(int j=0;j<numeroDeVolumesX;j++)
		{
			double xe = x[j+2];
			double xw = x[j];

			double fluxoE = fluxoMassico->CalcularX(xe);
			double fluxoW = fluxoMassico->CalcularX(xw);

			double taue;
			double tauw;
			double taun;
			double taus;

			double aEstrela;
			double fiPEstrela;
			double sp;

			double difusiv = difusividade->Calcular(fi[i+1][j+1]);

			if(j==0)//Condição de contorno à esquerda
				tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[i+1][j+1],difusividade); 
			else
				tauw = CalcularTauw(dxmenos,dxmais,dx,fi[i+1][j],fi[i+1][j+1],difusividade); 

			if(j==(numeroDeVolumesX-1))//Condição de contorno à direita
				taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[i+1][j+1],difusividade);
			else
				taue = CalcularTaue(dxmenos,dxmais,dx,fi[i+1][j+2],fi[i+1][j+1],difusividade);

			if(i==0)//Condição de contorno à inferior
				taus = CalcularTaus(dymenos,dymais,dy,condicaoDeContornoInferior->fi,fi[i+1][j+1],difusividade); 
			else
				taus = CalcularTaus(dymenos,dymais,dy,fi[i][j+1],fi[i+1][j+1],difusividade); 

			if(i==(numeroDeVolumesY-1))//Condição de contorno superior
				taun = CalcularTaun(dymenos,dymais,dy,condicaoDeContornoSuperior->fi,fi[i+1][j+1],difusividade);
			else
				taun = CalcularTaun(dymenos,dymais,dy,fi[i+2][j+1],fi[i+1][j+1],difusividade);


			aE[i][j] = taue*dy/dx - fluxoE*dy*dxmenos/dx;
			aW[i][j] = tauw*dy/dx + fluxoW*dy*dxmais/dx;
			aN[i][j] = taun*dx/dy - fluxoN*dx*dymenos/dy;
			aS[i][j] = taus*dx/dy + fluxoS*dx*dymais/dy;

			sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
				-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
				+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
				-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

			aEstrela = dx*dy/dt;
			fiPEstrela = fi[i+1][j+1];

			b[i][j] = aEstrela*fiPEstrela;

			aP[i][j] = 0.0; 

			if(termoFonte->Linear)
			{
				b[i][j] = b[i][j]+ termoFonte->Calcular(fi[i+1][j+1],x[j+1])*dx;
			}
			else
			{
				//Não implementado
				return false;
			}

			if((i==0)&&(j==0))//Canto esquerdo inferior
			{
				aW[i][j] = 0;
				aS[i][j] = 0;

				sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
					-fluxoW*dy
					+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
					-fluxoS*dx;

				if(condicaoDeContornoInferior->dinamica)
				{
					static_cast<CondicaoDeContornoDinamica*>(condicaoDeContornoInferior)->DefinirTipo(x[j]);
				}

				if((condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)&&
					(condicaoDeContornoInferior->tipo==CondicaoDeContorno::segundoTipo))
				{
					b[i][j] = b[i][j] + (fluxoW*dy/(tauw/dxmais) + dy)*condicaoDeContornoEsquerda->fluxo
						+(fluxoS*dx/(taus/dymais) + dx )*condicaoDeContornoInferior->fluxo;
				}
				else
				{
					//Não implementado
					return false;
				}
			}
			else if((i==(numeroDeVolumesY-1))&&(j==0))//Canto esquerdo superior
			{
				aW[i][j] = 0;
				aN[i][j] = 0;

				sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
					-fluxoW*dy
					+fluxoN*dx
					-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

				if((condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)&&
					(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::segundoTipo))
				{
					b[i][j] = b[i][j] + (fluxoW*dy/(tauw/dxmais) + dy)*condicaoDeContornoEsquerda->fluxo
						+( fluxoN*dx/(taun/dymenos) - dx )*condicaoDeContornoSuperior->fluxo;
				}
				else
				{
					//Não implementado
					return false;
				}
			}
			else if((j==(numeroDeVolumesX-1))&&(i==0))//canto direito inferior
			{
				aE[i][j] = 0;
				aS[i][j] = 0;

				sp = fluxoE*dy 
					-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
					+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
					-fluxoS*dx;

				if(condicaoDeContornoInferior->dinamica)
				{
					static_cast<CondicaoDeContornoDinamica*>(condicaoDeContornoInferior)->DefinirTipo(x[j]);
				}

				if((condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)&&
					(condicaoDeContornoInferior->tipo==CondicaoDeContorno::segundoTipo))
				{
					b[i][j] = b[i][j] + ( fluxoE*dy/(taue/dxmenos) - dy )*condicaoDeContornoDireita->fluxo
						+(fluxoS*dx/(taus/dymais) + dx )*condicaoDeContornoInferior->fluxo;
				}
				else
				{
					//Não implementado
					return false;
				}
			}
			else if((j==(numeroDeVolumesX-1))&&(i==(numeroDeVolumesY-1)))//canto direito superior
			{
				aE[i][j] = 0;
				aN[i][j] = 0;

				sp = fluxoE*dy 
					-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
					+fluxoN*dx
					-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

				if((condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)&&
					(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::segundoTipo))
				{
					b[i][j] = b[i][j] + ( fluxoE*dy/(taue/dxmenos) - dy )*condicaoDeContornoDireita->fluxo
						+ ( fluxoN*dx/(taun/dymenos) - dx )*condicaoDeContornoSuperior->fluxo;
				}
				else
				{
					//Não implementado
					return false;
				}
			}
			else if(j==0) //Condição de contorno à esquerda
			{
				aW[i][j] = 0;
				sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
					-fluxoW*dy
					+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
					-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

				if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo)
				{
					//Não implementado
					return false;
				}
				else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo) 
				{
					b[i][j] = b[i][j] + (fluxoW*dy/(tauw/dxmais) + dy)*condicaoDeContornoEsquerda->fluxo;
				}
				else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
				{
					//Não implementado
					return false;

				}

			}
			else if(j==(numeroDeVolumesX-1))//Condição de contorno à direita
			{
				aE[i][j] = 0;

				sp = fluxoE*dy 
					-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
					+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
					-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

				if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) 
				{
					//Não implementado
					return false;
				}
				else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo) 
				{
					b[i][j] = b[i][j] + ( fluxoE*dy/(taue/dxmenos) - dy )*condicaoDeContornoDireita->fluxo;
				}
				else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
				{
					//Não implementado
					return false;
				}

			}
			else if(i==0) //Condição de contorno inferior
			{
				aS[i][j] = 0;

				if(condicaoDeContornoInferior->dinamica)
				{
					static_cast<CondicaoDeContornoDinamica*>(condicaoDeContornoInferior)->DefinirTipo(x[j+1]);
				}

				if(condicaoDeContornoInferior->tipo==CondicaoDeContorno::primeiroTipo)
				{
					b[i][j] = b[i][j] + ( fluxoS*dx + dx*(taus/dymais))*condicaoDeContornoInferior->fi;

					sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
						-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
						+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
						+(taus/dxmais)*dx;

				}
				else if(condicaoDeContornoInferior->tipo==CondicaoDeContorno::segundoTipo) 
				{
					b[i][j] = b[i][j] + (fluxoS*dx/(taus/dymais) + dx )*condicaoDeContornoInferior->fluxo;

					sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
						-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
						+fluxoN*dx*dymais/dy + fluxoN*dx*dymenos/dy
						-fluxoS*dx;

				}
				else if(condicaoDeContornoInferior->tipo==CondicaoDeContorno::terceiroTipo)
				{
					//Não implementado
					return false;

				}

			}
			else if(i==(numeroDeVolumesY-1))//Condição de contorno superior
			{
				aN[i][j] = 0;

				sp = fluxoE*dy*dxmais/dx + fluxoE*dy*dxmenos/dx 
					-fluxoW*dy*dxmenos/dx -fluxoW*dy*dxmais/dx
					+fluxoN*dx
					-fluxoS*dx*dymenos/dy -fluxoS*dx*dymais/dy;

				if(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::primeiroTipo)
				{
					//Não implementado
					return false;
				}
				else if(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::segundoTipo) 
				{
					b[i][j] = b[i][j] + ( fluxoN*dx/(taun/dymenos) - dx )*condicaoDeContornoSuperior->fluxo;
				}
				else if(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::terceiroTipo)
				{
					//Não implementado
					return false;
				}

			}

			aP[i][j] = aP[i][j] + aEstrela + aE[i][j] +aW[i][j] + aN[i][j] + aS[i][j] + sp;

			if(aP[i]>0)
			{
				if(aE[i]<0) return false;
				if(aW[i]<0) return false;
				if(aS[i]<0) return false;
				if(aN[i]<0) return false;
			}
			else
			{
				if(aE[i]>0) return false;
				if(aW[i]>0) return false;
				if(aS[i]>0) return false;
				if(aN[i]>0) return false;
			}
		}
	}


#pragma endregion

	return true;
}

bool VolumesFinitos::DefinirMalhaUpwind(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,double* x, double* fi,double* peclet,
										double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
										CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
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

		aW[i] = tauw/dx + max(0.0,fluxo);
		aE[i] = taue/dx + max(0.0, -fluxo);
		aP[i] = 0.0; 
		aEE[i] = 0.0;
		aWW[i] = 0.0;

		if(difusiv!=0.0)
			peclet[i+1] = (fluxo*dx)/difusiv;
		else
			peclet[i+1] = 0.0;

		if(termoFonte->Linear)
		{
			b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
		}
		else //Para termo fonte linearizado(S(fi)=Sc+Sp*fip)
		{
			//decrescente com fi --> Sp=dS/dfi|fi* e Sc = S(fi*)-fi*dS/dfi|fi*
			//...então b=b+(S(fi)-(fi*)*(dS/dfi|fi*))*dx e aP = aP - dx*(dS/dfi|fi*)
			if(termoFonte->Decrescente)
			{
				b[i] = (termoFonte->Calcular(fi[i+1],x[i+1])-termoFonte->Derivada(fi[i+1],x[i+1])*fi[i+1])*dx;
				aP[i] = aP[i] - (termoFonte->Derivada(fi[i+1],x[i+1]))*dx;
			}
			else //crescente com fi --> Sp=0 e Sc = S(fi*)...então b=b+Sc*dx
			{
				b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
			}
		}

		if(i==0) //Condição de contorno à esquerda
		{
			aW[i] = 0;
			aP[i] = aP[i]+ fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo) 
			{
				aP[i] = aP[i] + (tauw/dxmais) - min(fluxo,0.0);
				b[i] = b[i] + (tauw/dxmais + max(0.0,fluxo))*condicaoDeContornoEsquerda->fi;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				aP[i] = aP[i] - fluxo;
				b[i] = b[i] + (1+ (max(fluxo,0.0))/(tauw/dxmais))*condicaoDeContornoEsquerda->fluxo;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;
			aP[i] = aP[i] -fluxo;

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) 
			{
				aP[i] = aP[i] + (taue/dxmenos) - min(0.0, -fluxo);
				b[i] = b[i] + (taue/dxmenos + max(0.0,-fluxo))*condicaoDeContornoDireita->fi;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)
			{
				aP[i] = aP[i] + fluxo;
				b[i] = b[i] + (-(max(-fluxo,0.0))/(taue/dxmenos)-1)*condicaoDeContornoDireita->fluxo;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}

		aP[i] = aP[i] + aE[i]+aW[i];

	}



#pragma endregion

	return true;

}

bool VolumesFinitos::DefinirMalhaQuick(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,double* x, double* fi,double* peclet,
									   double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
									   CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;
	double fluxo = fluxoMassico->Calcular();
	int alfaE = 1;
	int alfaW = 1;

	if(fluxo<0)
	{
		alfaE = 0;
		alfaW = 0;
	}

#pragma region ContrucaoMalha

	for(int i=0;i<numeroDeVolumes;i++)
	{
		double taue;
		double tauw;
		double Sp = 0.0;

		double difusiv = difusividade->Calcular(fi[i+1]);

		if(i==0)//Condição de contorno à esquerda
			tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[i+1],difusividade); 
		else
			tauw = CalcularTauw(dxmenos,dxmais,dx,fi[i],fi[i+1],difusividade); 

		if(i==(numeroDeVolumes-1))//Condição de contorno à direita
			taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[i+1],difusividade);
		else
			taue = CalcularTaue(dxmenos,dxmais,dx,fi[i+2],fi[i+1],difusividade);


		aW[i] = tauw/dx + (6.0/8.0)*alfaW*fluxo + (1.0/8.0)*alfaE*fluxo + (3.0/8.0)*(1-alfaW)*fluxo;
		aE[i] = taue/dx + -(3.0/8.0)*alfaE*fluxo  -(6.0/8.0)*(1-alfaE)*fluxo - (1.0/8.0)*(1-alfaW)*fluxo;
		aWW[i] = -(1.0/8.0)*alfaW*fluxo;
		aEE[i] = (1.0/8.0)*(1-alfaE)*fluxo;
		aP[i] = 0.0; 


		if(difusiv!=0.0)
			peclet[i+1] = (fluxo*dx)/difusiv;
		else
			peclet[i+1] = 0.0;

		if(termoFonte->Linear)
		{
			b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
		}
		else //Para termo fonte linearizado(S(fi)=Sc+Sp*fip)
		{
			//decrescente com fi --> Sp=dS/dfi|fi* e Sc = S(fi*)-fi*dS/dfi|fi*
			//...então b=b+(S(fi)-(fi*)*(dS/dfi|fi*))*dx e aP = aP - dx*(dS/dfi|fi*)
			if(termoFonte->Decrescente)
			{
				b[i] = (termoFonte->Calcular(fi[i+1],x[i+1])-termoFonte->Derivada(fi[i+1],x[i+1])*fi[i+1])*dx;
				aP[i] = aP[i] - (termoFonte->Derivada(fi[i+1],x[i+1]))*dx;
			}
			else //crescente com fi --> Sp=0 e Sc = S(fi*)...então b=b+Sc*dx
			{
				b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
			}
		}

		if(i==0) //Condição de contorno à esquerda
		{
			aW[i] = 0;
			aWW[i] = 0.0;
			aEE[i] = (1-alfaE)*(fluxo/8.0);

			aE[i] = taue/dx + -(3.0/8.0)*fluxo -(1-alfaE)*(3.0/8.0)*fluxo + (1.0/3.0)*(tauw/dx);
			Sp = (8.0/3.0)*(tauw/dx)+alfaE*(2.0/8.0)*fluxo+fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo)
			{
				b[i] = b[i] + ((8.0/3.0)*(tauw/dx) + alfaE*(2.0/8.0)*fluxo + fluxo)*condicaoDeContornoEsquerda->fi;
			}
			//Todo: Implementar condições abaixo
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				return false;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}
		else if((i==1)&&(fluxo>0))
		{
			aWW[i] = 0.0;

			aW[i] = tauw/dx + (7.0/8.0)*fluxo + (1.0/8.0)*fluxo;
			aE[i] = taue/dx + -(3.0/8.0)*fluxo;

			Sp = -(2.0/8.0)*fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo)
			{
				b[i] = b[i] + (-(2.0/8.0)*fluxo)*condicaoDeContornoEsquerda->fi;
			}
			//Todo: Implementar condições abaixo
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				return false;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}
		}
		else if((i==(numeroDeVolumes-2))&&(fluxo<0))
		{
			aWW[i] = 0.0;
			aEE[i] = 0.0;
			aW[i] = tauw/dx + (3.0/8.0)*fluxo;
			aE[i] = taue/dx + -(7.0/8.0)*fluxo -(1.0/8.0)*fluxo;

			Sp = (2.0/8.0)*fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo)
			{
				b[i] = b[i] + ((2.0/8.0)*fluxo)*condicaoDeContornoDireita->fi;
			}
			//Todo: Implementar condições abaixo
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				return false;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}
		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;
			aEE[i] = 0;

			aWW[i] = -(1.0/8.0)*alfaW*fluxo;
			aW[i] = tauw/dx + (3.0/8.0)*fluxo + alfaW*(3.0/8.0)*fluxo + (1.0/3.0)*(taue/dx);
			Sp = (8.0/3.0)*(taue/dx) - fluxo -(1-alfaW)*(2.0/8.0)*fluxo;

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) 
			{
				b[i] = b[i] + ((8.0/3.0)*(taue/dx) - fluxo -(1-alfaW)*(2.0/8.0)*fluxo)*condicaoDeContornoDireita->fi;
			}
			//Todo: Implementar condições abaixo
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				return false;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}

		aP[i] = aP[i] + aE[i]+aW[i]+aEE[i]+aWW[i]+fluxo-fluxo+Sp;

	}



#pragma endregion

	return true;

}

bool VolumesFinitos::DefinirMalhaDiferencasCentraisDeferredCorrection(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,double* x, double* fi,double* peclet,
																	  double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
																	  CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita)
{

	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;
	double fluxo = fluxoMassico->Calcular();
	double beta = 0.5;

#pragma region ContrucaoMalha

	for(int i=0;i<numeroDeVolumes;i++)
	{
		double taue;
		double tauw;
		double difusiv = difusividade->Calcular(fi[i+1]);
		double Suds = 0;
		double Scds = 0;
		double Scorr; 

		if(i==0)//Condição de contorno à esquerda
			tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[i+1],difusividade); 
		else
			tauw = CalcularTauw(dxmenos,dxmais,dx,fi[i],fi[i+1],difusividade); 

		if(i==(numeroDeVolumes-1))//Condição de contorno à direita
			taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[i+1],difusividade);
		else
			taue = CalcularTaue(dxmenos,dxmais,dx,fi[i+2],fi[i+1],difusividade);

		aW[i] = tauw/dx + max(0.0,fluxo);
		aE[i] = taue/dx + max(0.0, -fluxo);
		aP[i] = 0.0; 
		aEE[i] = 0.0;
		aWW[i] = 0.0;

		if(difusiv!=0.0)
			peclet[i+1] = (fluxo*dx)/difusiv;
		else
			peclet[i+1] = 0.0;

		if(termoFonte->Linear)
		{
			b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
		}
		else //Para termo fonte linearizado(S(fi)=Sc+Sp*fip)
		{
			//decrescente com fi --> Sp=dS/dfi|fi* e Sc = S(fi*)-fi*dS/dfi|fi*
			//...então b=b+(S(fi)-(fi*)*(dS/dfi|fi*))*dx e aP = aP - dx*(dS/dfi|fi*)
			if(termoFonte->Decrescente)
			{
				b[i] = (termoFonte->Calcular(fi[i+1],x[i+1])-termoFonte->Derivada(fi[i+1],x[i+1])*fi[i+1])*dx;
				aP[i] = aP[i] - (termoFonte->Derivada(fi[i+1],x[i+1]))*dx;
			}
			else //crescente com fi --> Sp=0 e Sc = S(fi*)...então b=b+Sc*dx
			{
				b[i] = termoFonte->Calcular(fi[i+1],x[i+1])*dx;
			}
		}

		if(i==0) //Condição de contorno à esquerda
		{
			aW[i] = 0;
			aP[i] = aP[i]+ fluxo;

			if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::primeiroTipo) 
			{
				aP[i] = aP[i] + (tauw/dxmais) - min(fluxo,0.0);
				b[i] = b[i] + (tauw/dxmais + max(0.0,fluxo))*condicaoDeContornoEsquerda->fi;

				Suds = fluxo*fi[i+2]-fluxo*fi[i+1];
				Scds = fluxo*((dxmenos/dx)*fi[i+2]+(dxmais/dx)*fi[i+1])-fluxo*condicaoDeContornoEsquerda->fi;
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
			{
				aP[i] = aP[i] - fluxo;
				b[i] = b[i] + (1+ (max(fluxo,0.0))/(tauw/dxmais))*condicaoDeContornoEsquerda->fluxo;

				Suds = fluxo*fi[i+2]-fluxo*fi[i+1];
				Scds = fluxo*((dxmenos/dx)*fi[i+2]+(dxmais/dx)*fi[i+1])-
					fluxo*(fi[i+1]+condicaoDeContornoEsquerda->fluxo/(tauw/dxmais));
			}
			else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}
		else if(i==(numeroDeVolumes-1))//Condição de contorno à direita
		{
			aE[i] = 0;
			aP[i] = aP[i] -fluxo;

			if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::primeiroTipo) 
			{
				aP[i] = aP[i] + (taue/dxmenos) - min(0.0, -fluxo);
				b[i] = b[i] + (taue/dxmenos + max(0.0,-fluxo))*condicaoDeContornoDireita->fi;

				Suds = fluxo*condicaoDeContornoDireita->fi-fluxo*fi[i+1];
				Scds = fluxo*condicaoDeContornoDireita->fi -fluxo*((dxmenos/dx)*fi[i+1]+(dxmais/dx)*fi[i]);
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)
			{
				return false;
			}
			else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
			{
				return false;
			}

		}
		else //Nós centrais
		{
			Suds = fluxo*fi[i+2]-fluxo*fi[i+1];
			Scds = fluxo*((dxmenos/dx)*fi[i+2]+(dxmais/dx)*fi[i+1])-fluxo*((dxmais/dx)*fi[i]+(dxmenos/dx)*fi[i+1]);
		}

		Scorr = beta*(Scds-Suds);
		b[i]= b[i] - Scorr;
		aP[i] = aP[i] + aE[i]+aW[i];

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
	else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
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
	else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
	{
		double taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[numeroDeVolumes],difusividade);

		double fie = ((taue/dxmenos)*fi[numeroDeVolumes]+condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
			(condicaoDeContornoDireita->alfa+(taue/dxmenos));

		fi[numeroDeVolumes+1] = fie;
	}

}

void VolumesFinitos::AjustarCondicoesDeContorno(int numeroDeVolumesX,int numeroDeVolumesY,double* x,double dx,double dy,double** fi,
												Difusividade* difusividade,
												CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita,
												CondicaoDeContorno* condicaoDeContornoInferior,CondicaoDeContorno* condicaoDeContornoSuperior)
{

	//Malha uniforme
	double dxmais = dx/2;
	double dxmenos = dx/2;
	double dymais = dy/2;
	double dymenos = dy/2;

	if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::segundoTipo)
	{
		for (int j = 0; j < numeroDeVolumesY+2; j++)
		{
			double tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[j][1],difusividade); 
			fi[j][0] = (condicaoDeContornoEsquerda->fluxo*dxmais)/tauw+fi[j][1];
		}

	}
	else if(condicaoDeContornoEsquerda->tipo==CondicaoDeContorno::terceiroTipo)
	{
		for (int j = 0; j < numeroDeVolumesY+2; j++)
		{
			double tauw = CalcularTauw(dxmenos,dxmais,dx,condicaoDeContornoEsquerda->fi,fi[j][1],difusividade); 
			double fiw = ((tauw/dxmais)*fi[j][1]+condicaoDeContornoEsquerda->alfa*condicaoDeContornoEsquerda->fiInfinito)/
				(condicaoDeContornoEsquerda->alfa+(tauw/dxmais));

			fi[j][0] = fiw;
		}
	}

	if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::segundoTipo)
	{
		for (int j = 0; j < numeroDeVolumesY+2; j++)
		{
			double taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[j][numeroDeVolumesX],difusividade);
			fi[j][numeroDeVolumesX+1] = fi[j][numeroDeVolumesX]-(condicaoDeContornoDireita->fluxo*dxmenos)/taue;
		}
	}
	else if(condicaoDeContornoDireita->tipo==CondicaoDeContorno::terceiroTipo)
	{
		for (int j = 0; j < numeroDeVolumesY+2; j++)
		{
			double taue = CalcularTaue(dxmenos,dxmais,dx,condicaoDeContornoDireita->fi,fi[j][numeroDeVolumesX],difusividade);
			double fie = ((taue/dxmenos)*fi[j][numeroDeVolumesX]+condicaoDeContornoDireita->alfa*condicaoDeContornoDireita->fiInfinito)/
				(condicaoDeContornoDireita->alfa+(taue/dxmenos));

			fi[j][numeroDeVolumesX+1] = fie;
		}
	}

	if(condicaoDeContornoInferior->dinamica)
	{
		for (int j = 0; j < numeroDeVolumesX+2; j++)
		{
			static_cast<CondicaoDeContornoDinamica*>(condicaoDeContornoInferior)->DefinirTipo(x[j]);

			if(condicaoDeContornoInferior->tipo==CondicaoDeContorno::segundoTipo)
			{
				double taus = CalcularTaus(dymenos,dymais,dy,condicaoDeContornoInferior->fi,fi[1][j],difusividade); 
				fi[0][j] = (condicaoDeContornoInferior->fluxo*dymais)/taus+fi[1][j];
			}
			else if(condicaoDeContornoInferior->tipo==CondicaoDeContorno::terceiroTipo)
			{

				double taus = CalcularTaus(dymenos,dymais,dy,condicaoDeContornoInferior->fi,fi[1][j],difusividade); 
				double fis = ((taus/dymais)*fi[1][j]+condicaoDeContornoInferior->alfa*condicaoDeContornoInferior->fiInfinito)/
					(condicaoDeContornoInferior->alfa+(taus/dymais));

				fi[0][j] = fis;

			}
		}


	}


	if(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::segundoTipo)
	{
		for (int j = 0; j < numeroDeVolumesX+2; j++)
		{
			double taun = CalcularTaun(dymenos,dymais,dy,condicaoDeContornoSuperior->fi,fi[numeroDeVolumesY][j],difusividade);
			fi[numeroDeVolumesY+1][j] = fi[numeroDeVolumesY][j]-(condicaoDeContornoSuperior->fluxo*dymenos)/taun;
		}
	}
	else if(condicaoDeContornoSuperior->tipo==CondicaoDeContorno::terceiroTipo)
	{
		for (int j = 0; j < numeroDeVolumesX+2; j++)
		{
			double taun = CalcularTaun(dymenos,dymais,dx,condicaoDeContornoSuperior->fi,fi[numeroDeVolumesY][j],difusividade);
			double fin = ((taun/dymenos)*fi[numeroDeVolumesY][j]+condicaoDeContornoSuperior->alfa*condicaoDeContornoSuperior->fiInfinito)/
				(condicaoDeContornoSuperior->alfa+(taun/dymenos));

			fi[numeroDeVolumesY+1][j] = fin;
		}
	}

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

double VolumesFinitos::CalcularTaus(double dymenos, double dymais, double dy, double fiS,double fiP,
									Difusividade* difusividade)
{
	double taus = 0.0;
	double tauS = difusividade->Calcular(fiS);
	double tauP =  difusividade->Calcular(fiP);

	if((tauS!=0.0)&&(tauP!=0.0))
		taus = 1/((dymenos/dy)/tauS + (dymais/dy)/tauP);

	return taus;
}

double VolumesFinitos::CalcularTaun(double dymenos, double dymais, double dy, double fiN,double fiP,
									Difusividade* difusividade)
{
	double taun = 0.0;
	double tauN= difusividade->Calcular(fiN);
	double tauP = difusividade->Calcular(fiP);

	if((tauN!=0.0)&&(tauP!=0.0))
		taun = 1/((dymenos/dy)/tauP + (dymais/dy)/tauN);

	return taun;
}

