#include "LSOR.h"


LSOR::LSOR(int ordem)
{
	#pragma region AlocacaoDeMemoria

	dThomas = new double[ordem];
	bThomas= new double[ordem];

	incognitasPassoAnterior=new double*[ordem];
	delta=new double*[ordem];
	A = new double*[ordem];
	b = new double[ordem];
	r =  new double[ordem];
	

	for (int i=0; i<ordem; i++)
	{
		incognitasPassoAnterior[i]=new double[ordem];
		delta[i]=new double[ordem];
		A[i]=new double[ordem];
	}
	#pragma endregion 
}

LSOR::~LSOR()
{
	#pragma region LiberacaoMemoria

	delete [] dThomas;
	delete [] bThomas;
	
	for (int i=0; i<ordem; i++)
	{
		delete [] A[i];
		delete [] incognitasPassoAnterior[i];
		delete [] delta[i];
	}
	delete [] A;
	delete [] b;
	delete [] r;
	delete [] incognitasPassoAnterior;
	delete [] delta;
		
	#pragma endregion
}

bool LSOR::Resolver(SistemaLinear2D sistema)
{
	#pragma region Declaração dos dados

	bool calculou=true;
		
	/*double* b;
	double* r;
	double** A;
	double** incognitasPassoAnterior;
	double** delta;*/

	ordem = sistema.dimensaoMatriz;

	double erroMaximo = 100;
	double deltaPermitido = 0.1; //%
	int iteracao = 1;
	int numeroMaximoIteracoes = 100;
	double relaxacao = 1.3;

	/*double* dThomas;
	double* bThomas;*/

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
			calculou = CalculadoraSistemaLinear1D::ResolverSistemaLinear(r,A,b,dThomas,bThomas,ordem);

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

	


	return calculou;
}

