#include "Problema2D.h"

bool Problema2D::Calcular()
{
	ObterCondicoesIniciaisEDeContorno();

	bool calculou = CalcularSolucaoNumerica();
	if(!calculou)
	{
		ImprimirMensagemDeErro(3);
		return calculou;
	}

	return calculou;
}

bool Problema2D::IniciarCalculos()
{
	//Declaração de variáveis
	bool calculou = true;

	calculou = CalcularVariavelIndependenteEspacoX();
	if(!calculou)
	{
		ImprimirMensagemDeErro(5);
		return calculou;
	}

	calculou = CalcularVariavelIndependenteEspacoY();
	if(!calculou)
	{
		ImprimirMensagemDeErro(6);
		return calculou;
	}

	calculou = CalcularVariavelIndependenteTempo();
	if(!calculou)
	{
		ImprimirMensagemDeErro(7);
		return calculou;
	}

	IniciarVariavelNumerica();

	


	return calculou;
}

bool Problema2D::CalcularVariavelIndependenteTempo()
{
	tempo[0] = 0;
	
	for(int i = 1;i<nTempo;i++)
	{
		tempo[i] = tempo[i-1] + dt;
	}

	return true;
}

bool Problema2D::CalcularVariavelIndependenteEspacoY()
{
	y[0] = y0;
	y[numeroDeVolumesX+1] = yH;

	for(int i = 1;i<=numeroDeVolumesY;i++)
	{
		if(i==1)
			y[i] = y[i-1] + dy/2;
		else
			y[i] = y[i-1] + dy;
	}

	return true;
}

bool Problema2D::CalcularVariavelIndependenteEspacoX()
{
	x[0] = x0;
	x[numeroDeVolumesX+1] = xL;

	for(int i = 1;i<=numeroDeVolumesX;i++)
	{
		if(i==1)
			x[i] = x[i-1] + dx/2;
		else
			x[i] = x[i-1] + dx;
	}

	return true;
}

bool Problema2D::CalcularSolucaoNumerica()
{
	return VolumesFinitos::ResolverRegimePermanente2D(numeroDeVolumesX,dx,x,
		numeroDeVolumesY,dy,y,dt,fiNumerico,peclet,
		this->Difusividade,this->FluxoMassico,this->TermoFonte,
		this->CondicaoDeContornoEsquerda,this->CondicaoDeContornoDireita,
		this->CondicaoDeContornoInferior,this->CondicaoDeContornoSuperior,
		static_cast<VolumesFinitos::Discretizacao>(tipoDiscretizacaoTermoConvectivo));
}

void Problema2D::ImprimirResultados(double t)
{
	arquivo.precision(4);
	cout.precision(4);

	arquivo<<"\n\nResultados no tempo:"<<t<<" \n\n";
	cout<<"\n\nResultados no tempo:"<<t<<" \n\n";

	arquivo<<"z=[";

	for(int i=0;i<=numeroDeVolumesY+1;i++)
	{
		for(int j=0;j<=numeroDeVolumesX+1;j++)
		{
			arquivo<< fixed<< fiNumerico[i][j]<<"  ";
			cout<< fixed<< fiNumerico[i][j]<<"  ";
		}

		arquivo<<";\n";
		cout<< "\n";
	}

	arquivo<<"];";
}



void Problema2D::AlocarMemoria()
{
	const int nx = numeroDeVolumesX+2;
	const int ny = numeroDeVolumesY+2;
	
	x = static_cast<double*>(malloc(nx*sizeof(double)));
	y = static_cast<double*>(malloc(ny*sizeof(double)));
	tempo = static_cast<double*>(malloc(nTempo*sizeof(double)));

	fiAnalitico = static_cast<double**>(malloc(ny*sizeof(double)));
	fiNumerico = static_cast<double**>(malloc(ny*sizeof(double)));
	peclet = static_cast<double**>(malloc(ny*sizeof(double)));
	desviosRelativos = static_cast<double**>(malloc(ny*sizeof(double)));
	desviosAbsolutos = static_cast<double**>(malloc(ny*sizeof(double)));

	for (int i=0; i<ny; i++)
	{
		fiAnalitico[i]=static_cast<double*>(malloc(nx*sizeof(double)));
		fiNumerico[i]=static_cast<double*>(malloc(nx*sizeof(double)));
		peclet[i]=static_cast<double*>(malloc(nx*sizeof(double)));
		desviosRelativos[i]=static_cast<double*>(malloc(nx*sizeof(double)));
		desviosAbsolutos[i]=static_cast<double*>(malloc(nx*sizeof(double)));
	}

}

void Problema2D::LiberarMemoria()
{
	const int ny = numeroDeVolumesY+2;

	free(static_cast<void*>(x));
	free(static_cast<void*>(y));
	free(static_cast<void*>(tempo));

	for (int i=0; i<ny; i++)
	{
		free(static_cast<void*>(fiAnalitico[i]));
		free(static_cast<void*>(fiNumerico[i]));
		free(static_cast<void*>(peclet[i]));
		free(static_cast<void*>(desviosRelativos[i]));
		free(static_cast<void*>(desviosAbsolutos[i]));
				
	}

	free(static_cast<void*>(fiAnalitico));
	free(static_cast<void*>(fiNumerico));
	free(static_cast<void*>(peclet));
	free(static_cast<void*>(desviosRelativos));
	free(static_cast<void*>(desviosAbsolutos));

	arquivo.close();
}