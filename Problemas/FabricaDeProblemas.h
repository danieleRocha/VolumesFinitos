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
static char *nomeProblemas[] = {"Difus�o com gera��o 1D                         [0]",
                                "Difus�o com gera��o e difusividade vari�vel 1D [1]",
                                "Condu��o em um pino 1D                         [2]"};


