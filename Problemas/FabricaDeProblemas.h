#ifndef FabricaDeProblemas_h
#define FabricaDeProblemas_h

#include "Problema.h"
#include "DifusaoComGeracao1D.h"
#include "DifusaoComGeracaoEDifusividadeVariavel1D.h"
#include "ConducaoPino1D.h"

#include <stdlib.h>

class FabricaDeProblemas
{
public:
	static Problema *CriarProblema(int indice);
	
};

#endif 


static int nProblemasDisponiveis = 3;
static char *nomeProblemas[] = {"Difusão com geração 1D                         [0]",
                                "Difusão com geração e difusividade variável 1D [1]",
                                "Condução em um pino 1D                         [2]"};


