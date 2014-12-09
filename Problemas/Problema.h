#ifndef Problema_h
#define Problema_h

#include <iostream>
#include "Difusividade.h"
#include "TermoFonte.h"
#include "FluxoMassico.h"
#include "VolumesFinitos.h"

using namespace std;

class Problema
{

public:
	TermoFonte* TermoFonte;
	Difusividade* Difusividade;
	FluxoMassico* FluxoMassico;
	virtual bool Resolver()=0;
	virtual ~Problema();
	
};

#endif

