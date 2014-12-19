#ifndef VolumesFinitos_h
#define VolumesFinitos_h

#include "Difusividade.h"
#include "TermoFonte.h"
#include "FluxoMassico.h"
#include "FluxoMassicoVariavel.h"
#include "CalculadoraSistemaLinear1D.h"
#include "CalculadoraSistemaLinear2D.h"
#include "CondicaoDeContorno.h"
#include "CondicaoDeContornoDinamica.h"

#include <cstdlib>
#include <math.h>

class VolumesFinitos
{

public:
	
	enum Discretizacao
	{
		DiferencasCentrais,
		Upwind,
		QUICK,
		DiferencasCentraisDeferredCorrection
	};
		
	static bool ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* x,double* fi,double* peclet,int numeroDoMetodoParaSistemaLinear,
		Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,CondicaoDeContorno* condicaoDeContornoEsquerda,
		CondicaoDeContorno* condicaoDeContornoDireita, Discretizacao discretizacaoTermoConvectivo);

	
	static bool ResolverRegimePermanente2D(int numeroDeVolumesX, double dx, double* x, int numeroDeVolumesY, double dy, double* y,
		double dt, double** fiNumerico, double** peclet, Difusividade* difusividade, FluxoMassicoVariavel* fluxoMassico,
		TermoFonte* termoFonte, CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita,
		CondicaoDeContorno* condicaoDeContornoInferior, CondicaoDeContorno* condicaoDeContornoSuperior,
		VolumesFinitos::Discretizacao tipoDiscretizacaoTermoConvectivo);

private:

	//Região 1D

	static void AjustarCondicoesDeContorno(int numeroDeVolumes, double dx, double* fi, Difusividade* difusividade,
		CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita);
	
	static double CalcularTauw(double dxmenos, double dxmais, double dx, double fiW,double fiP,
		Difusividade* difusividade);
	static double CalcularTaue(double dxmenos, double dxmais, double dx, double fiE,double fiP,
		Difusividade* difusividade);
	
	static bool DefinirMalhaUpwind(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,
		double* x,double* fi,double* peclet, double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita);
	static bool DefinirMalhaDiferencasCentrais(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,
		double* x,double* fi,double* peclet, double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita);
	static bool DefinirMalhaQuick(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,
		double* x,double* fi,double* peclet, double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita);
	static bool DefinirMalhaDiferencasCentraisDeferredCorrection(double* aP, double* aE,double* aEE, double* aW,double* aWW,double* b,int numeroDeVolumes,double dx,
		double* x,double* fi,double* peclet, double* fiInstanteAnterior,Difusividade* difusividade,FluxoMassico* fluxoMassico,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita);
	
	//Região 2D

	static double CalcularTaus(double dymenos, double dymais, double dy, double fiS, double fiP, Difusividade* difusividade);
	static double CalcularTaun(double dymenos, double dymais, double dy, double fiN, double fiP, Difusividade* difusividade);

	static bool DefinirMalhaDiferencasCentrais(double** aP, double** aE, double** aW, double** aN, double** aS, double** b,
		int numeroDeVolumesX, double dx, double* x, int numeroDeVolumesY, double dy, double* y, double dt, double** fi,
		double** peclet, double** fiInstanteAnterior, Difusividade* difusividade, FluxoMassicoVariavel* fluxoMassico,
		TermoFonte* termoFonte, CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita,
		CondicaoDeContorno* condicaoDeContornoSuperior, CondicaoDeContorno* condicaoDeContornoInferior);
	static bool DefinirMalhaUpwind(double** aP, double** aE, double** aW, double** aN, double** aS, double** b, int numeroDeVolumesX,
		double dx, double* x, int numeroDeVolumesY, double dy, double* y, double dt, double** fi, double** peclet,
		double** fiInstanteAnterior, Difusividade* difusividade, FluxoMassicoVariavel* fluxoMassico, TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita, CondicaoDeContorno* condicaoDeContornoSuperior,
		CondicaoDeContorno* condicaoDeContornoInferior);

	static void AjustarCondicoesDeContorno(int numeroDeVolumesX, int numeroDeVolumesY,double* x, double dx, double dy, double** fi, 
		Difusividade* difusividade, CondicaoDeContorno* condicaoDeContornoEsquerda, 
		CondicaoDeContorno* condicaoDeContornoDireita, CondicaoDeContorno* condicaoDeContornoInferior, 
		CondicaoDeContorno* condicaoDeContornoSuperior);
	

};

#endif

static const int numeroDeMetodosDeDiscretizacao = 4;
static const char *nomeMetodosDeDiscretizacao[] = {"Diferenças Centrais                     [0]",
                                                   "Upwind                                  [1]",
                                                   "QUICK                                   [2]",
                                                   "Diferenças Centrais Deferred Correction [3]"};