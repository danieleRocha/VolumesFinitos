
#ifndef FabricaDeMetodosParaSistemaLinear_h
#define FabricaDeMetodosParaSistemaLinear_h

#include "MetodoParaSistemaLinear.h"
#include "GaussSeidel.h"
#include "Thomas.h"



static class FabricaDeMetodosParaSistemaLinear
{
public:
	static MetodoParaSistemaLinear *CriarMetodo(int indice);
	
};


static int nMetodosParaSistemasLinearesDisponiveis = 2;
static char *nomeMetodosParaSistemasLineares[] = {"Gauss Seidel   [0]",
											      "Thomas         [1]:"};

#endif