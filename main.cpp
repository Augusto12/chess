#include <iostream>
#include <algorithm>
using namespace std;


void imprimir_tabuleiro(char vetor[8][9])
{
		for (int i=0; i<8; i++)
	{
		for (int j=0; j<8; j++)
			cout << vetor[i][j];
		cout << endl;
	}
}
/*void mover_peca(char vetor[8][9])
{
	int l1, c1, l2, c2, t;
	cout << "Escolha uma peça: " << endl;
	cin >> l1 >> c1;
	cout << "Escolha a nova posição da peça: " << endl;
	cin >> l2 >> c2;
	t=vetor[l1][c1];
	vetor[l1][c1]=vetor[l2][c2];
	vetor[l2][c2]=t;
	imprimir_tabuleiro(vetor);
}*/
void mover_peao(char vetor[8][9])
{
	int l1, c1, l2, c2, t;
	cout << "Escolha um peao: " << endl;
	cin >> l1 >> c1;
	cout << "Escolha a nova posição da peça: " << endl;
	cin >> l2 >> c2;
	if (vetor[l1][c1]=='P')
	{
		if (l1==1 && (vetor[l1+1][c1+1]==' ' || vetor[l1+1][c1-1]==' ') && l2<=l1+2 && l2>l1 && c2==c1)
		{
			std::swap(vetor[l1][c1], vetor[l2][c2]);
			imprimir_tabuleiro(vetor);
		}
		if (l1!=1 && (vetor[l1+1][c1+1]==' ' || vetor[l1+1][c1-1]==' ') && l2==l1+1 && c2==c1)
		{
			t=vetor[l1][c1];
			vetor[l1][c1]=vetor[l2][c2];
			vetor[l2][c2]=t;
			imprimir_tabuleiro(vetor);
		}
		/*if((l2>l1+1 && l1!=1) || c2!=c1)
		cout << "Posicao invalida" << endl;*/
		if (c2!=c1 && (c2<c1+2 || c2>c1-2) && (vetor[l1+1][c1+1]!=' ' || vetor[l1+1][c1-1]!=' '))		
		{
			t=vetor[l1][c1];
			vetor[l1][c1]=vetor[l2][c2];
			vetor[l2][c2]=t;
			imprimir_tabuleiro(vetor);
		}
		else
			cout << "Posicao invalida" << endl;
	}
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
	while (1)
	{
		cout << "Deseja mover uma peça?" << endl;
		cin >> a;
		if (a=='s')
			mover_peao(tabuleiro);
		else
			break;
	}
	return 0;
}