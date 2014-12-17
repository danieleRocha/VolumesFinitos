#ifndef CondicaoDeContornoDinamica_h
#define CondicaoDeContornoDinamica_h
#include "CondicaoDeContorno.h"


class CondicaoDeContornoDinamica:
	public CondicaoDeContorno
{
public:
	virtual void DefinirTipo(double x)=0;
	
};

#endif

