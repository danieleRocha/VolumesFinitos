
#ifndef FabricaDeMetodosParaSistemaLinear_h
#define FabricaDeMetodosParaSistemaLinear_h

#include "MetodoParaSistemaLinear1D.h"
#include "GaussSeidel.h"
#include "Thomas.h"



static class FabricaDeMetodosParaSistemaLinear
{
public:
	static MetodoParaSistemaLinear1D *CriarMetodo(int indice);
	
};


static int nMetodosParaSistemasLinearesDisponiveis = 2;
static char *nomeMetodosParaSistemasLineares[] = {"Gauss Seidel   [0]",
											      "Thomas         [1]:"};

#endif