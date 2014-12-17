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
			difusao->FluxoMassico = new DifusaoComGeracao1D::FluxoMassicoDifusaoComGeracao1D();
			return difusao;
		}
	case 1:
		{
			DifusaoComGeracaoEDifusividadeVariavel1D* difusao = new DifusaoComGeracaoEDifusividadeVariavel1D();
			difusao->Difusividade = new DifusaoComGeracaoEDifusividadeVariavel1D::TermoDifusivoDifusaoComGeracaoEDifusividadeVariavel1D();
			difusao->TermoFonte = new DifusaoComGeracaoEDifusividadeVariavel1D::TermoFonteDifusaoComGeracaoEDifusividadeVariavel1D();
			difusao->FluxoMassico=new DifusaoComGeracaoEDifusividadeVariavel1D::FluxoMassicoDifusaoComGeracaoEDifusividadeVariavel1D();
			return difusao;
		}
	case 2:
		{
			ConducaoSuperficieEstendida1D* conducao = new ConducaoSuperficieEstendida1D();
			conducao->Difusividade = new ConducaoSuperficieEstendida1D::TermoDifusivoConducaoSuperficieEstendida1D();
			conducao->TermoFonte = new ConducaoSuperficieEstendida1D::TermoFonteConducaoSuperficieEstendida1D();
			conducao->FluxoMassico = new ConducaoSuperficieEstendida1D::FluxoMassicoConducaoSuperficieEstendida1D();
			return conducao;
		}
	case 3:
		{
			ConveccaoDifusao1D* conveccaoDifusao = new ConveccaoDifusao1D();
			conveccaoDifusao->Difusividade = new ConveccaoDifusao1D::TermoDifusivoConveccaoDifusao1D();
			conveccaoDifusao->TermoFonte = new ConveccaoDifusao1D::TermoFonteConveccaoDifusao1D();
			conveccaoDifusao->FluxoMassico = new ConveccaoDifusao1D::FluxoMassicoConveccaoDifusao1D();
			return conveccaoDifusao;
		}
	case 4:
		{
			PoluentesChamine1D* poluentes = new PoluentesChamine1D();
			poluentes->Difusividade = new PoluentesChamine1D::TermoDifusivoPoluentesChamine1D();
			poluentes->TermoFonte = new PoluentesChamine1D::TermoFontePoluentesChamine1D();
			poluentes->FluxoMassico = new PoluentesChamine1D::FluxoMassicoPoluentesChamine1D();
			return poluentes;
		}
	case 5:
		{
			ConveccaoDifusaoTransiente2D* conveccaoDifusao = new ConveccaoDifusaoTransiente2D();
			conveccaoDifusao->Difusividade = new ConveccaoDifusaoTransiente2D::TermoDifusivoConvecaoDifusaoTransiente2D();
			conveccaoDifusao->TermoFonte = new ConveccaoDifusaoTransiente2D::TermoFonteConvecaoDifusaoTransiente2D();
			conveccaoDifusao->FluxoMassico = new ConveccaoDifusaoTransiente2D::FluxoMassicoConvecaoDifusaoTransiente2D();
			return conveccaoDifusao;
		}
	default:
		return NULL;
		
	}
}
