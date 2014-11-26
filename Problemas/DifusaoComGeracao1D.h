#ifndef DifusaoComGeracao1D_h
#define DifusaoComGeracao1D_h

#include "Problema1D.h"


class DifusaoComGeracao1D :
	public Problema1D
{
private:
	
	double qK;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico) override;
	void IniciarVariavelNumerica(double* fiNumerico) override;
	bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico) override;

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
		double Calcular(double fi) override;
		double Derivada(double fi) override;
	};
	
};

#endif

