#ifndef CondicaoDeContorno_h
#define CondicaoDeContorno_h



class CondicaoDeContorno
{
public:
	enum Tipo
	{
		primeiroTipo,
		segundoTipo,
		terceitoTipo
	};

	virtual ~CondicaoDeContorno();
	Tipo tipo;
	double fi;
	double fluxo;
	double fiInfinito;
	double alfa;
};

#endif

