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
	case 1:
		{
			DifusaoComGeracaoEDifusividadeVariavel1D* difusao = new DifusaoComGeracaoEDifusividadeVariavel1D();
			difusao->Difusividade = new DifusaoComGeracaoEDifusividadeVariavel1D::TermoDifusivoDifusaoComGeracaoEDifusividadeVariavel1D();
			difusao->TermoFonte = new DifusaoComGeracaoEDifusividadeVariavel1D::TermoFonteDifusaoComGeracaoEDifusividadeVariavel1D();
			
			return difusao;
		}
	case 2:
		{
			ConducaoPino1D* conducao = new ConducaoPino1D();
			conducao->Difusividade = new ConducaoPino1D::TermoDifusivoConducaoPino1D();
			conducao->TermoFonte = new ConducaoPino1D::TermoFonteConducaoPino1D();
			
			return conducao;
		}
	default:
		return NULL;
		
	}
}
