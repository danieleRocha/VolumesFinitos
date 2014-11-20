#include "FabricaDeProblemas.h"



Problema* FabricaDeProblemas::CriarProblema(int indice)
{
	switch (indice)
	{
	case 0:
		{
			DifusaoComGeracao1D* difusao = new DifusaoComGeracao1D();
			difusao->Difusividade = new DifusaoComGeracao1D::TermoDifusivoDifusaoComGeracao1D();
			difusao->TermoFonte = new DifusaoComGeracao1D::TermoFonteDifusaoComGeracao1D();
			
			return difusao;
		}
	default:
		return NULL;
		
	}
}
