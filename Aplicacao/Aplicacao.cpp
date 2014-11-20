// Aplicacao.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <stdlib.h>

#define Aplicacao_cpp

#include "stdafx.h"
#include "Problema.h"
#include "FabricaDeProblemas.h"
#include <fstream>

using namespace std;

int _tmain()
{
	inicio:

	bool calculou = true;
	int numeroDoProblema;

	cout<<"\n\n\nQual problema voc� deseja resolver?(";

	for(int i=0;i<nProblemasDisponiveis;i++)
	{
		cout<<"\n"<<nomeProblemas[i];
	}
	cout<<"):";
	cin >>numeroDoProblema;


	if((numeroDoProblema<0)||(numeroDoProblema>(nProblemasDisponiveis-1)))
	{
		cout<<"Aten��o: \n"
			"N�mero do problema inv�lido. O n�mero do problema deve estar entre 0 e "<<(nProblemasDisponiveis-1);
		calculou = false;
	}

	if(!calculou)
	{
		goto inicio;
	}

	Problema* problema = FabricaDeProblemas::CriarProblema(numeroDoProblema);

	calculou = problema->Resolver();

	goto inicio;
}
