#ifndef Problema1D_h
#define Problema1D_h

#include "Problema.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "math.h"
#include "CondicaoDeContorno.h"

using namespace std;


class Problema1D :
	public Problema
{
public:
	double x0,xL,L,fi0,fiL,dx;
	int numeroDeVolumes;
	int numeroDoMetodoParaSistemaLinear;
	int tipoDiscretizacaoTermoConvectivo;

	double* x;
	double* fiAnalitico;
	double* fiNumerico;
	double* peclet;
	double* desviosRelativos;
	double* desviosAbsolutos;

	
	CondicaoDeContorno* CondicaoDeContornoEsquerda;
	CondicaoDeContorno* CondicaoDeContornoDireita;

	bool Calcular();
	bool CalcularVariavelIndependenteEspacoX();
	
	bool CalcularSolucaoNumerica();
	bool CalcularDesvios();
	void ImprimirResultados();
	void AlocarMemoria();
	void LiberarMemoria();
};

#endif 