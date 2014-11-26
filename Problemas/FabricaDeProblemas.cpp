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
			ConducaoSuperficieEstendida1D* conducao = new ConducaoSuperficieEstendida1D();
			conducao->Difusividade = new ConducaoSuperficieEstendida1D::TermoDifusivoConducaoSuperficieEstendida1D();
			conducao->TermoFonte = new ConducaoSuperficieEstendida1D::TermoFonteConducaoSuperficieEstendida1D();
			
			return conducao;
		}
	default:
		return NULL;
		
	}
}
