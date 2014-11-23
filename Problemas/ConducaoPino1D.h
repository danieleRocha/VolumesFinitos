#ifndef ConducaoPino1D_h
#define ConducaoPino1D_h

#include "Problema1D.h"

class ConducaoPino1D :
	public Problema1D
{
private:
	
	double h,D,P,k,Ac,TiInfinito,TBase;
	double  m2;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico) override;
	void IniciarVariavelNumerica(double* fiNumerico) override;
	bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico) override;
	

public:
	bool Resolver() override;

	class TermoDifusivoConducaoPino1D:
		public Difusividade
	{
	public:
		double Calcular(double fi) override;
	};

	class TermoFonteConducaoPino1D:
		public TermoFonte
	{
	public:
		double m2;
		double Calcular(double fi) override;
	};
	
};

#endif

