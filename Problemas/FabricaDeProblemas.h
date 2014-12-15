#ifndef FabricaDeProblemas_h
#define FabricaDeProblemas_h

#include "Problema.h"
#include "DifusaoComGeracao1D.h"
#include "DifusaoComGeracaoEDifusividadeVariavel1D.h"
#include "ConducaoSuperficieEstendida1D.h"
#include "ConveccaoDifusao1D.h"
#include "PoluentesChamine1D.h"

#include <stdlib.h>

class FabricaDeProblemas
{
public:
	static Problema *CriarProblema(int indice);
	
};

#endif 


static int nProblemasDisponiveis = 5;
static char *nomeProblemas[] = {"Difusão com geração 1D                         [0]",
                                "Difusão com geração e difusividade variável 1D [1]",
                                "Condução em superfícies estendidas 1D          [2]",
                                "Convecção/Difusão 1D                           [3]",
                                "Concentração de poluentes em uma chaminé 1D    [4]"};


