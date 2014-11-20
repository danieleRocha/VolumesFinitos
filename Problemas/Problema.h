#ifndef Problema_h
#define Problema_h

#include <iostream>
#include "Difusividade.h"
#include "TermoFonte.h"
#include "VolumesFinitos.h"

using namespace std;

class Problema
{

public:
	TermoFonte* TermoFonte;
	Difusividade* Difusividade;
	virtual bool Resolver()=0;
	virtual ~Problema();
	
};

#endif

