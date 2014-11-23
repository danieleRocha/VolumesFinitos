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
	ofstream arquivo;
	CondicaoDeContorno* CondicaoDeContornoEsquerda;
	CondicaoDeContorno* CondicaoDeContornoDireita;

	bool Calcular(double* x, double* fiAnalitico, double* fiNumerico,double* desviosRelativos,double* desviosAbsolutos);
	void ImprimirMensagemDeErro(int numeroDoErro);
	bool CalcularVariavelIndependenteEspacoX(double* x);
	virtual bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico)=0;
	virtual void IniciarVariavelNumerica(double* fiNumerico)=0;
	virtual void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico)=0;
	bool CalcularSolucaoNumerica(double* fiNumerico);
	bool CalcularDesvios(double* fiAnalitico, double* fiNumerico,double* desviosRelativos,double* desviosAbsolutos);
	void ImprimirResultados(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos,double* desviosAbsolutos);
};

#endif 