#ifndef Problema2D_h
#define Problema2D_h

#include "Problema.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "math.h"
#include "CondicaoDeContorno.h"
#include "CondicaoDeContornoDinamica.h"

using namespace std;


class Problema2D :
	public Problema
{
public:
	double x0,xL,L,dx;
	double y0,yH,H,dy;
	double fi00,fi0H,fiL0,fiLH;
	double dt;
	double fiInicial;
	int numeroDeVolumesX,numeroDeVolumesY;
	int nTempo;
	int tipoDiscretizacaoTermoConvectivo;

	double* x;
	double* y;
	double* tempo;
	double** fiAnalitico;
	double** fiNumerico;
	double** peclet;
	double** desviosRelativos;
	double** desviosAbsolutos;

	FluxoMassicoVariavel* FluxoMassico;

	CondicaoDeContorno* CondicaoDeContornoEsquerda;
	CondicaoDeContorno* CondicaoDeContornoDireita;
	CondicaoDeContorno* CondicaoDeContornoInferior;
	CondicaoDeContorno* CondicaoDeContornoSuperior;

	bool Calcular();
	bool IniciarCalculos();
	bool CalcularVariavelIndependenteEspacoX();
	bool CalcularVariavelIndependenteEspacoY();
	bool CalcularVariavelIndependenteTempo();
	bool CalcularSolucaoNumerica();
	void ImprimirResultados(double t);
	void AlocarMemoria();
	void LiberarMemoria();
};

#endif 
