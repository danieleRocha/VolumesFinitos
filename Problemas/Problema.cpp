#include "Problema.h"


Problema::~Problema(void)
{
}

void Problema::DefinirArquivo()
{
	arquivo = ofstream("resultado.txt");
}

void Problema::ImprimirMensagemDeErro(int numeroDoErro)
{
	switch(numeroDoErro)
	{
	case 1:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço.\n\n";
		break;
	case 2:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a solução analítica.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a solução analítica.\n\n";
		break;
	case 3:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a solução numérica.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a solução numérica.\n\n";
		break;
	case 4:
		arquivo<<"Erro: \n"
			"Não foi possível calcular os desvios dos resultados numéricos em relação aos analíticos.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular os desvios dos resultados numéricos em relação aos analíticos.\n\n";
		break;
	case 5:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço x.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço x.\n\n";
		break;
	case 6:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço y.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a variável independente: espaço y.\n\n";
		break;
	case 7:
		arquivo<<"Erro: \n"
			"Não foi possível calcular a variável independente: tempo.\n\n";
		cout<<"Erro: \n"
			"Não foi possível calcular a variável independente: tempo.\n\n";
		break;
	default:
		arquivo<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		cout<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		break;
	}
}



