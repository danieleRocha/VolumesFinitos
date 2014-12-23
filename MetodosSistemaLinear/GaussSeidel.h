
#ifndef GaussSeidel_h
#define GaussSeidel_h
#include "MetodoParaSistemaLinear1D.h"
#include "iostream"



class GaussSeidel :
	public MetodoParaSistemaLinear1D
{
public:
	bool Resolver(SistemaLinear1D sistema) override;
private:
	bool VerificarDominaciaDiagonal(int ordemDaMatriz,double** coeficientesDaMatriz);
};

#endif 