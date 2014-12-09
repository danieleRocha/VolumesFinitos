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

	ofstream arquivo;
	CondicaoDeContorno* CondicaoDeContornoEsquerda;
	CondicaoDeContorno* CondicaoDeContornoDireita;

	bool Calcular();
	void ImprimirMensagemDeErro(int numeroDoErro);
	bool CalcularVariavelIndependenteEspacoX();
	virtual bool CalcularSolucaoAnalitica()=0;
	virtual void IniciarVariavelNumerica()=0;
	virtual void ObterCondicoesIniciaisEDeContorno()=0;
	bool CalcularSolucaoNumerica();
	bool CalcularDesvios();
	void ImprimirResultados();
	void AlocarMemoria();
	void DefinirArquivo();
	void LiberarMemoria();
};

#endif 