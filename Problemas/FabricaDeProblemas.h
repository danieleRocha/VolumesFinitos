#ifndef FabricaDeProblemas_h
#define FabricaDeProblemas_h

#include "Problema.h"
#include "DifusaoComGeracao1D.h"


#include <stdlib.h>

class FabricaDeProblemas
{
public:
	static Problema *CriarProblema(int indice);
	
};

#endif 


static int nProblemasDisponiveis = 1;
static char *nomeProblemas[] = {"Difusão com geração 1D      [0]"};


