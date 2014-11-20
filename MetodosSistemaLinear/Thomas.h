#ifndef Thomas_h
#define Thomas_h

#include "MetodoParaSistemaLinear.h"
#include <istream>

class Thomas :
	public MetodoParaSistemaLinear
{
public:
	bool Resolver(SistemaLinear sistema, string &mensagem) override;
};

#endif 

