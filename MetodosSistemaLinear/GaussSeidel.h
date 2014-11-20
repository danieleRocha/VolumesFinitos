
#ifndef GaussSeidel_h
#define GaussSeidel_h
#include "MetodoParaSistemaLinear.h"
#include "iostream"



class GaussSeidel :
	public MetodoParaSistemaLinear
{
public:
	bool Resolver(SistemaLinear sistema, string &mensagem) override;
private:
	bool VerificarDominaciaDiagonal(int ordemDaMatriz,double** coeficientesDaMatriz);
};

#endif 