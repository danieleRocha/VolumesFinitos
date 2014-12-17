#ifndef ConveccaoDifusaoTransiente2D_h
#define ConveccaoDifusaoTransiente2D_h

#include "Problema2D.h"


class ConveccaoDifusaoTransiente2D :
	public Problema2D
{

private:
	double difusividade;
	double pecletFixado;

	double fiEntrada;
	double fluxoArestaDaEntrada;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno() override;
	void IniciarVariavelNumerica() override;
	bool CalcularSolucaoAnalitica() override;
	
public:
	bool Resolver() override;

	class TermoDifusivoConvecaoDifusaoTransiente2D:
		public Difusividade
	{
	public:
		double difusividade;
		double Calcular(double fi) override;
	};

	class TermoFonteConvecaoDifusaoTransiente2D:
		public TermoFonte
	{
	public:
		double Calcular(double fi,double x) override;
		double Derivada(double fi,double x) override;
	};
	
	class FluxoMassicoConvecaoDifusaoTransiente2D:
		public FluxoMassicoVariavel
	{
	public:
		double CalcularX(double x) override;
		double CalcularY(double y) override;
	};

	class CondicaoDeContornoDinamicaConvecaoDifusaoTransiente2D:
		public CondicaoDeContornoDinamica
	{
	public:
		double fluxoContorno;
		double fiContorno;
		void DefinirTipo(double x) override;
		
	};
};

#endif
