#ifndef LSOR_h
#define LSOR_h

#include "MetodoParaSistemaLinear2D.h"
#include <istream>

class LSOR :
	public MetodoParaSistemaLinear2D
{
public:
	bool Resolver(SistemaLinear2D sistema, string &mensagem) override;
	
};

#endif 

