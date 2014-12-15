#ifndef TermoFonte_h
#define TermoFonte_h



class TermoFonte
{
public:
	bool Linear;
	bool Decrescente;
	virtual ~TermoFonte();
	virtual	double Calcular(double fi,double x) = 0;
	virtual	double Derivada(double fi,double x) = 0;

};

#endif

