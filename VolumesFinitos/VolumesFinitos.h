#ifndef VolumesFinitos_h
#define VolumesFinitos_h

#include "Difusividade.h"
#include "TermoFonte.h"
#include "SistemaLinear.h"
#include "FabricaDeMetodosParaSistemaLinear.h"
#include "CondicaoDeContorno.h"

#include <cstdlib>
#include <math.h>

class VolumesFinitos
{

public:
	static double CalcularTauw(double dxmenos, double dxmais, double dx, double fiW,double fiP,
		Difusividade* difusividade);
	static double CalcularTaue(double dxmenos, double dxmais, double dx, double fiE,double fiP,
		Difusividade* difusividade);
	static void DefinirMalha(double* aP, double* aE, double* aW,double* b,int numeroDeVolumes,double dx,
		double* fi, double* fiInstanteAnterior,Difusividade* difusividade,TermoFonte* termoFonte,
		CondicaoDeContorno* condicaoDeContornoEsquerda,CondicaoDeContorno* condicaoDeContornoDireita);
	static void AjustarCondicoesDeContorno(int numeroDeVolumes, double dx, double* fi, Difusividade* difusividade,
		CondicaoDeContorno* condicaoDeContornoEsquerda, CondicaoDeContorno* condicaoDeContornoDireita);
	static bool ResolverSistemaLinear(double* fi,double* aP, double* aE, double* aW, double* b, int numeroDeVolumes,int numeroDoMetodoParaSistemaLinear);
	static bool ResolverRegimePermanente1D(int numeroDeVolumes,double dx,double* fi,int numeroDoMetodoParaSistemaLinear,
		Difusividade* difusividade,TermoFonte* termoFonte,CondicaoDeContorno* condicaoDeContornoEsquerda,
		CondicaoDeContorno* condicaoDeContornoDireita);
	
};

#endif