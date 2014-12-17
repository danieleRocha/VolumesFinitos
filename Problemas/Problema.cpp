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
			"N�o foi poss�vel calcular a vari�vel independente: espa�o.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o.\n\n";
		break;
	case 2:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o anal�tica.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o anal�tica.\n\n";
		break;
	case 3:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o num�rica.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a solu��o num�rica.\n\n";
		break;
	case 4:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular os desvios dos resultados num�ricos em rela��o aos anal�ticos.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular os desvios dos resultados num�ricos em rela��o aos anal�ticos.\n\n";
		break;
	case 5:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o x.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o x.\n\n";
		break;
	case 6:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o y.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: espa�o y.\n\n";
		break;
	case 7:
		arquivo<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: tempo.\n\n";
		cout<<"Erro: \n"
			"N�o foi poss�vel calcular a vari�vel independente: tempo.\n\n";
		break;
	default:
		arquivo<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		cout<<"Erro: \n"
			"Houve um erro no sistema.\n\n";
		break;
	}
}



