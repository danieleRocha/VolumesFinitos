#include "LSOR.h"


bool LSOR::Resolver(SistemaLinear2D sistema, string& mensagem)
{
	#pragma region Declaração dos dados

	bool calculou=true;
		
	double* b;
	double* r;
	double** A;
	double** incognitasPassoAnterior;
	double** delta;

	int ordem = sistema.dimensaoMatriz;

	double erroMaximo = 100;
	double deltaPermitido = 0.1; //%
	int iteracao = 1;
	int numeroMaximoIteracoes = 100;
	double relaxacao = 1.3;

	#pragma endregion

	#pragma region AlocacaoDeMemoria

	incognitasPassoAnterior=static_cast<double**>(malloc(ordem*sizeof(double*)));
	delta=static_cast<double**>(malloc(ordem*sizeof(double*)));
	A = static_cast<double**>(malloc(ordem*sizeof(double*)));
	b = static_cast<double*>(malloc(ordem*sizeof(double)));
	r = static_cast<double*>(malloc(ordem*sizeof(double))); 

	for (int i=0; i<ordem; i++)
	{
		incognitasPassoAnterior[i]=static_cast<double*>(malloc(ordem*sizeof(double)));
		delta[i]=static_cast<double*>(malloc(ordem*sizeof(double)));
		A[i]=static_cast<double*>(malloc(ordem*sizeof(double)));
	}
	#pragma endregion 

	#pragma region Calculo

	//Iteraçoes do Metodo LSOR
	while((erroMaximo>deltaPermitido)&&(iteracao<numeroMaximoIteracoes))
	{
		erroMaximo = 0.0;
		for(int i = 0;i<ordem;i++)
		{
			for(int j = 0;j<ordem;j++)
			{
				incognitasPassoAnterior[i][j] = sistema.resposta[i][j];
			}
		}

		//Varredura no eixo y, com i crescente
		//z representa uma linha da matriz original
		for(int z = 0;z<ordem;z++) 
		{
			//Variaveis que irao compor o sistema a ser resolvido pelo metodo de Thomas

			for(int i = 0;i<ordem;i++)
			{
				for(int j = 0;j<ordem;j++)
				{
					A[i][j] = 0;
				}
			}        

			for(int i = 0;i<ordem;i++)
			{
				A[i][i]=sistema.aP[z][i]; //Diagonal Principal

				if(i<(ordem-1))
					A[i][i+1] = -sistema.aE[z][i]; //Lado direito
				if(i>0)
					A[i][i-1] = -sistema.aW[z][i]; //Lado direito               

				b[i] = sistema.b[z][i];

				if(z<(ordem-1))
				{
					b[i] = b[i] + sistema.resposta[z+1][i]*sistema.aN[z][i];
				}
				if(z>0)
				{
					b[i] = b[i] + sistema.resposta[z-1][i]*sistema.aS[z][i];
				}
			}

			        /*cout<<"Matriz A \n";
			          
			        for(int i=0;i<ordem;i++)
			        {
			        
			            printf("%6.4f  %6.4f  %6.4f \n",
			                    A[i][0],A[i][1],A[i][2]);
			        
			        }

			        printf("Vetor b \n");
			          
			        for(int i=0;i<ordem;i++)
			        {
			        
			            printf("%6.4f\n",
			                    b[i]);
			        
			        }*/

			//Calcula o sistema Linear por Thomas
			calculou = CalculadoraSistemaLinear1D::ResolverSistemaLinear(r,A,b,ordem,1);

			if(!calculou) goto liberarMemoria;

			//        printf("Incognitas \n");
			//          
			//        for(i=0;i<ordem;i++)
			//        {
			//            printf("%6.4f\n",
			//                    r[i]);
			//        }

			//Atualiza os valores
			for(int i = 0;i<ordem;i++)//Atualizaçao das incognitas
			{
				sistema.resposta[z][i] = r[i];
			}

			//Aplicação da relaxação
			for(int i = 0;i<ordem;i++)
			{
				sistema.resposta[z][i] = incognitasPassoAnterior[z][i] + relaxacao*(sistema.resposta[z][i]-incognitasPassoAnterior[z][i]);
			}
		}

		        /*printf("\nRespostas iteracao %i LSOR \n",iteracao);
		        
		        for(int i=ordem-1;i>=0;i--)
		        {
		            for(int j = 0;j<ordem;j++)
		            {
		                printf("%6.4f  ", sistema.resposta[i][j]);
		            }
		            printf("\n");
		        }*/

		for(int i = 0;i<ordem;i++)
		{
			for(int j = 0;j<ordem;j++)
			{
				delta[i][j] = 100*(incognitasPassoAnterior[i][j]-sistema.resposta[i][j])/sistema.resposta[i][j];

				if(erroMaximo<fabs(delta[i][j]))
					erroMaximo = fabs(delta[i][j]);
			}
		}

		iteracao = iteracao + 1;
	}

	if(iteracao>numeroMaximoIteracoes)
		calculou=false;

#pragma endregion

liberarMemoria:

	#pragma region LiberacaoMemoria

	for (int i=0; i<ordem; i++)
	{
		free(static_cast<void*>(A[i]));
		free(static_cast<void*>(incognitasPassoAnterior[i]));
		free(static_cast<void*>(delta[i]));
	}
	free(static_cast<void*>(A));
	free(static_cast<void*>(b));
	free(static_cast<void*>(r));
	free(static_cast<void*>(incognitasPassoAnterior));
	free(static_cast<void*>(delta));

	#pragma endregion


	return calculou;
}

