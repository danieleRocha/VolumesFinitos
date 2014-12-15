#ifndef DifusaoComGeracao1D_h
#define DifusaoComGeracao1D_h

#include "Problema1D.h"


class DifusaoComGeracao1D :
	public Problema1D
{
private:
	
	double qK;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno() override;
	void IniciarVariavelNumerica() override;
	bool CalcularSolucaoAnalitica() override;

public:
	bool Resolver() override;

	class TermoDifusivoDifusaoComGeracao1D:
		public Difusividade
	{
	public:
		double Calcular(double fi) override;
	};

	class TermoFonteDifusaoComGeracao1D:
		public TermoFonte
	{
	public:
		double qK;
		double Calcular(double fi,double x) override;
		double Derivada(double fi,double x) override;
	};
	
	class FluxoMassicoDifusaoComGeracao1D:
		public FluxoMassico
	{
	public:
		double Calcular() override;
	};
};

#endif

