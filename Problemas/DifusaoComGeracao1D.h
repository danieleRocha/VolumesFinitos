#ifndef DifusaoComGeracao1D_h
#define DifusaoComGeracao1D_h

#include "Problema.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "math.h"

using namespace std;


class DifusaoComGeracao1D :
	public Problema
{
private:
	double x0,xL,L,fi0,fiL,dx;
	double qK;
	int numeroDeVolumes;
	int numeroDoMetodoParaSistemaLinear;
	ofstream arquivo;

	bool SolicitarDadosDeEntrada();
	bool Calcular(double* x, double* fiAnalitico, double* fiNumerico,double* desviosRelativos,double* desviosAbsolutos);
	void ImprimirMensagemDeErro(int numeroDoErro);
	bool CalcularVariavelIndependenteEspacoX(double* x);
	void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico);
	bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico);
	bool CalcularSolucaoNumerica(double* x, double* fiNumerico);
	bool CalcularDesvios(double* fiAnalitico, double* fiNumerico,double* desviosRelativos,double* desviosAbsolutos);
	void ImprimirResultados(double* x, double* fiAnalitico, double* fiNumerico, double* desviosRelativos,double* desviosAbsolutos);

public:
	bool Resolver() override;

	class TermoDifusivoDifusaoComGeracao1D:
		public Difusividade
	{
	public:
		double Calcular(double fi) override;
	};

	class TermoFonteDifusaoComGeracao1D:
		public TermoFonte
	{
	public:
		double qK;
		double Calcular(double fi) override;
	};
	

};

#endif

