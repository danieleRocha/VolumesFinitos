#ifndef PoluentesChamine1D_h
#define PoluentesChamine1D_h

#include "Problema1D.h"


class PoluentesChamine1D :
	public Problema1D
{
private:
	double difusividade;
	double fluxoMassico;
	double pecletFixado;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno() override;
	void IniciarVariavelNumerica() override;
	bool CalcularSolucaoAnalitica() override;

public:
	bool Resolver() override;

	class TermoDifusivoPoluentesChamine1D:
		public Difusividade
	{
	public:
		double difusividade;
		double Calcular(double fi) override;
	};

	class TermoFontePoluentesChamine1D:
		public TermoFonte
	{
	public:
		double Calcular(double fi,double x) override;
		double Derivada(double fi,double x) override;
	};
	
	class FluxoMassicoPoluentesChamine1D:
		public FluxoMassico
	{
	public:
		double fluxoMassico;
		double Calcular() override;
	};
};

#endif
