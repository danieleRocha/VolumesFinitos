#ifndef LSOR_h
#define LSOR_h

#include "MetodoParaSistemaLinear2D.h"
#include <istream>

class LSOR :
	public MetodoParaSistemaLinear2D
{
private:
	int ordem;
	double* b;
	double* r;
	double** A;
	double** incognitasPassoAnterior;
	double** delta;
	double* dThomas;
	double* bThomas;

public:
	LSOR(int ordem);
	~LSOR();
	bool Resolver(SistemaLinear2D sistema) override;
	
};

#endif 

