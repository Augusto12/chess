#include <iostream>
#include <algorithm>
using namespace std;

enum Cor
{
	BRANCO,
	PRETO
};


void imprimir_tabuleiro(char vetor[8][9])
{
	for (int i=0; i<8; i++)
	{
		for (int j=0; j<8; j++)
			cout << vetor[i][j];
		cout << endl;
	}
}


Cor cor_peca(char tabuleiro[8][9], int l, int c)
{
	//TODO: fazer de um jeito mais eficiente
	char peca = tabuleiro[l][c];
	if (peca == 'K' || peca == 'Q' || peca == 'B' || peca == 'N' || peca == 'R' || peca == 'P')
		return PRETO;
	else
		return BRANCO;
}

bool selecionar_peca(char tabuleiro[8][9], int l, int c, Cor corjogador)
{
	if(tabuleiro[l][c] == ' ' || corjogador != cor_peca(tabuleiro, l, c))
		return false;
	return true;
}



bool mover_peao(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
	Cor cor = cor_peca(tabuleiro, l1, c1);
	bool move = false;
	if (c1 == c2)
	{
		if ((l2-l1 == (cor == BRANCO ? -1 : 1) && tabuleiro[l2][c2] == ' ') ||
			(l2-l1 == (cor == BRANCO ? -2 : 2) && tabuleiro[l2][c2] == ' ' && tabuleiro[l2 + (cor == BRANCO ? 1 : -1)][c2] == ' '))
			move = true;
	}
	else if (c1-c2 == 1 || c2-c1 == 1)
	{
		//TODO: implementar funçao se é inimigo
		if (l2-l1 == (cor == BRANCO ? -1 : 1) && tabuleiro[l2][c2] != ' ' && cor != cor_peca(tabuleiro, l2, c2))
			move = true;
	}

	if (move)
	{
		tabuleiro[l2][c2] = tabuleiro[l1][c1];
		tabuleiro[l1][c1] = ' ';
		return true;
	}
	
	return false;

}


bool escolher_destino(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
	char peca = tabuleiro[l1][c1];
	if (peca == 'p' || peca == 'P')
		return mover_peao(tabuleiro, l1, c1, l2, c2);
	return false;
}

int main()
{
	char a;
	char tabuleiro[8][9] = {"RNBQKBNR",
							"PPPPPPPP",
							"        ",
							"        ",
							"        ",
							"        ",
							"pppppppp",
							"rnbqkbnr"};
	Cor corjogador = BRANCO;
	while (1)
	{
		imprimir_tabuleiro(tabuleiro);
		
		int l1, c1, l2, c2;
		cout << "Escolha um peao: " << endl;
		cin >> l1 >> c1;
		
		if (!selecionar_peca(tabuleiro, l1, c1, corjogador))
		{	
			cout << "Peça inválida" << endl;
			continue;
		}

		cout << "Escolha a nova posição da peça: " << endl;
		cin >> l2 >> c2;
		if(!escolher_destino(tabuleiro, l1, c1, l2, c2))
		{		
			cout << "Destino inválido" << endl;
			continue;
		}


	}

	return 0;
}