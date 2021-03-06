#ifndef DifusaoComGeracaoEDifusividadeVariavel1D_h
#define DifusaoComGeracaoEDifusividadeVariavel1D_h

#include "Problema1D.h"

class DifusaoComGeracaoEDifusividadeVariavel1D :
	public Problema1D
{
private:
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno() override;
	void IniciarVariavelNumerica() override;
	bool CalcularSolucaoAnalitica() override;
	
public:
	bool Resolver() override;

	class TermoDifusivoDifusaoComGeracaoEDifusividadeVariavel1D:
		public Difusividade
	{
	public:
		double Calcular(double fi) override;
	};

	class TermoFonteDifusaoComGeracaoEDifusividadeVariavel1D:
		public TermoFonte
	{
	public:
		double Calcular(double fi,double x) override;
		double Derivada(double fi,double x) override;
	};
	
	class FluxoMassicoDifusaoComGeracaoEDifusividadeVariavel1D:
		public FluxoMassico
	{
	public:
		double Calcular() override;
	};
};

#endif

