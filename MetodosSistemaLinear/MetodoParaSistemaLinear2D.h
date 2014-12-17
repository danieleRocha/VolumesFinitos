#ifndef MetodoParaSistemaLinear2D_h
#define MetodoParaSistemaLinear2D_h

#include <fstream>
#include "SistemaLinear2D.h"
#include "CalculadoraSistemaLinear1D.h"

using namespace std;

class MetodoParaSistemaLinear2D
{
public:
	
	virtual bool Resolver(SistemaLinear2D sistema,string &mensagem)=0;

	virtual ~MetodoParaSistemaLinear2D();
};

#endif 

