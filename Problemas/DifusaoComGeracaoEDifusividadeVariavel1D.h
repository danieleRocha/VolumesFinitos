#ifndef DifusaoComGeracaoEDifusividadeVariavel1D_h
#define DifusaoComGeracaoEDifusividadeVariavel1D_h

#include "Problema1D.h"

class DifusaoComGeracaoEDifusividadeVariavel1D :
	public Problema1D
{
private:
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico) override;
	void IniciarVariavelNumerica(double* fiNumerico) override;
	bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico) override;
	
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
		double Calcular(double fi) override;
		double Derivada(double fi) override;
	};
	

};

#endif

