#ifndef Problema_h
#define Problema_h

#include <iostream>
#include "Difusividade.h"
#include "TermoFonte.h"
#include "FluxoMassico.h"
#include "FluxoMassicoVariavel.h"
#include "VolumesFinitos.h"

using namespace std;

class Problema
{

public:

	ofstream arquivo;

	TermoFonte* TermoFonte;
	Difusividade* Difusividade;
	FluxoMassico* FluxoMassico;
	virtual bool Resolver()=0;
	virtual ~Problema();
		
	void DefinirArquivo();
	void ImprimirMensagemDeErro(int numeroDoErro);
	virtual void IniciarVariavelNumerica()=0;
	virtual void ObterCondicoesIniciaisEDeContorno()=0;
	virtual bool CalcularSolucaoAnalitica()=0;
};

#endif

