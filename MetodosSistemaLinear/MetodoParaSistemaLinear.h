#ifndef MetodoParaSistemaLinear_h
#define MetodoParaSistemaLinear_h

#include <fstream>
#include "SistemaLinear.h"


using namespace std;

class MetodoParaSistemaLinear
{
public:
	
	virtual bool Resolver(SistemaLinear sistema,string &mensagem)=0;

	virtual ~MetodoParaSistemaLinear();
};

#endif 

