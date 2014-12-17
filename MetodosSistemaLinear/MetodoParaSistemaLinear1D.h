#ifndef MetodoParaSistemaLinear1D_h
#define MetodoParaSistemaLinear1D_h

#include <fstream>
#include "SistemaLinear1D.h"


using namespace std;

class MetodoParaSistemaLinear1D
{
public:
	
	virtual bool Resolver(SistemaLinear1D sistema,string &mensagem)=0;

	virtual ~MetodoParaSistemaLinear1D();
};

#endif 

