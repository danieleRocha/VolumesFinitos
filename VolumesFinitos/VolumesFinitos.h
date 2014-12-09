#ifndef VolumesFinitos_h
#define VolumesFinitos_h

#include "Difusividade.h"
#include "TermoFonte.h"
#include "FluxoMassico.h"
#include "SistemaLinear.h"
#include "FabricaDeMetodosParaSistemaLinear.h"
#include "CondicaoDeContorno.h"

#include <cstdlib>
#include <math.h>

class VolumesFinitos
{

public:
	
	enum Discretizacao
	{
		DiferencasCentrais,
		Upwind,
		QUICK
	};

	static double CalcularTauw(double dxmenos, double dxmais, double dx, double fiW,double fiP,
		Difusividade* difusividade);
	static double CalcularTaue(double dxmenos, double dxmais, double dx, double fiE,double fiP,
		Difusividade* difusividade);
	static bool DefinirMalha(double* aP, double* aE, double* aW,double* b,int numeroDeVolumes,double dx,
		double* fi,double* peclet, double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita,
		Discretizacao discretizacaoTermoConvectivo);
	static void AjustarCondicoesDeContorno(int numeroDeVolumes, double dx, double* fi, Difusividade* difusividade,
		CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita);
	static bool ResolverSistemaLinear(double* fi,double* aP, double* aE, double* aW, double* b, int numeroDeVolumes,int numeroDoMetodoParaSistemaLinear);
	static bool ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* fi,double* peclet,int numeroDoMetodoParaSistemaLinear,
		Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,CondicaoDeContorno* condicaoDeContornoEsquerda,
		CondicaoDeContorno* condicaoDeContornoDireita, Discretizacao discretizacaoTermoConvectivo);
	
};

#endif

static const int numeroDeMetodosDeDiscretizacao = 3;
static const char *nomeMetodosDeDiscretizacao[] = {"Diferenças Centrais [0]",
                                             "Upwind              [1]",
                                             "QUICK               [2]"};