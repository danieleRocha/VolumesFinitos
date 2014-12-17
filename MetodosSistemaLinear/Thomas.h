#ifndef Thomas_h
#define Thomas_h

#include "MetodoParaSistemaLinear1D.h"
#include <istream>

class Thomas :
	public MetodoParaSistemaLinear1D
{
public:
	bool Resolver(SistemaLinear1D sistema, string &mensagem) override;
};

#endif 

