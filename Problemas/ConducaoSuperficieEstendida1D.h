#ifndef ConducaoSuperficieEstendida1D_h
#define ConducaoSuperficieEstendida1D_h

#include "Problema1D.h"

class ConducaoSuperficieEstendida1D :
	public Problema1D
{
private:
	int numeroDoTipoDeSuperficie;
	double h,D,w,t,P,k,Ac,TiInfinito,TBase;
	double  m2;
	
	bool SolicitarDadosDeEntrada();
	void ObterCondicoesIniciaisEDeContorno(double* fiAnalitico, double* fiNumerico) override;
	void IniciarVariavelNumerica(double* fiNumerico) override;
	bool CalcularSolucaoAnalitica(double* x, double* fiAnalitico) override;
	

public:
	bool Resolver() override;

	class TermoDifusivoConducaoSuperficieEstendida1D:
		public Difusividade
	{
	public:
		double Calcular(double fi) override;
	};

	class TermoFonteConducaoSuperficieEstendida1D:
		public TermoFonte
	{
	public:
		double m2;
		double Calcular(double fi) override;
		double Derivada(double fi) override;
	};
	
};

static int nSuperficiesDisponiveis = 2;
static char *nomeSuperficies[] = {"Pinos  [0]",
                                  "Aletas [1]"};

#endif
