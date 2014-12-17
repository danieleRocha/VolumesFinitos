#include "FabricaDeMetodosParaSistemaLinear.h"

MetodoParaSistemaLinear1D* FabricaDeMetodosParaSistemaLinear::CriarMetodo(int indice)
{
	switch (indice)
	{
	case 0:
		return new GaussSeidel();
	case 1:
		return new Thomas();
	default:
		return NULL;
		
	}
}