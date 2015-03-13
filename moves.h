#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

enum Cor
{
    BRANCO,
    PRETO
};


void imprimir_tabuleiro(char vetor[8][9])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
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

bool mover_torre(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
    Cor cor = cor_peca(tabuleiro, l1, c1);
    if (c1 != c2 && l1 != l2)
        return false;

    if (c1 == c2 && l1 != l2)
    {
        if (l2 > l1)
            for(int i = l1 + 1; i < l2; i++)
                if (tabuleiro[i][c1] != ' ')
                    return false;
        else
            for(int i = l1 - 1; i > l2; i--)
                if (tabuleiro[i][c1] != ' ')
                    return false;
    }
    else if (l1 == l2 && c1 != c2)
    {
        if (c2 > c1)
            for(int i = c1 + 1; i < c2; i++)
                if (tabuleiro[l1][i] != ' ')
                    return false;
        else
            for(int i = c1 - 1; i > c2; i--)
                if (tabuleiro[l1][i] != ' ')
                    return false;
    }

    if (tabuleiro[l2][c2] == ' ' || cor != cor_peca(tabuleiro, l2, c2))
    {
        tabuleiro[l2][c2] = tabuleiro[l1][c1];
        tabuleiro[l1][c1] = ' ';
        return true;
    }
    return false;
}

bool mover_cavalo(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
    Cor cor = cor_peca(tabuleiro, l1, c1);
    if (abs(l2 - l1) > 3 || abs(c2 - c1) > 3)
        return false;
    if ((abs(l2 - l1) == 2 && abs(c2 - c1) == 1) || (abs(l2 -l1) == 1 && abs(c2 - c1) == 2) && (tabuleiro[l2][c2] == ' ' ||
        cor != cor_peca(tabuleiro, l2, c2)))
    {
        tabuleiro[l2][c2] = tabuleiro[l1][c1];
        tabuleiro[l1][c1] = ' ';
        return true;
    }
    return false;
}

bool mover_bispo(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
    Cor cor = cor_peca(tabuleiro, l1, c1);
    if (abs(l2 - l1) != abs(c2 - c1))
        return false;
    if (l2 > l1)
    {
        if (c2 > c1)
          for(int i = l1 + 1; i < l2; i++)
              if (tabuleiro[i][c1 + i - l1] != ' ')
                  return false;
        else
          for(int i = l1 + 1; i < l2; i++)
              if (tabuleiro[i][c1 - i + l1] != ' ')
                  return false;
    }
    else
    {
        if (c2 > c1)
          for(int i = l1 - 1; i > l2; i--)
              if (tabuleiro[i][c1 - i + l1] != ' ')
                  return false;
        else
          for(int i = l1 - 1; i > l2; i--)
              if (tabuleiro[i][c1 + i - l1] != ' ')
                  return false;
    }

    if (tabuleiro[l2][c2] == ' ' || cor != cor_peca(tabuleiro, l2, c2))
    {
        tabuleiro[l2][c2] = tabuleiro[l1][c1];
        tabuleiro[l1][c1] = ' ';
        return true;
    }
    return false;
}

bool mover_rainha(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
    if (l1 == l2 || c1 == c2)
        return mover_torre(tabuleiro, l1, c1, l2, c2);
    if (abs(l2 - l1) == abs(c2 - c1))
        return mover_bispo(tabuleiro, l1, c1, l2, c2);
    else
        return false;
}

bool mover_rei(char tabuleiro[8][9], int l1, int c1, int l2, int c2)
{
    if (abs(l2 - l1) > 1 || abs(c2 - c1) > 1 || (l1 == l2 && c2 == c1))
        return false;
    if (abs(l2 - l1) <= 1 && abs(c2 - c1) <= 1 && (tabuleiro[l2][c2] == ' ' || cor_peca(tabuleiro, l1, c1) != cor_peca(tabuleiro, l2, c2)))
    {
        tabuleiro[l2][c2] = tabuleiro[l1][c1];
        tabuleiro[l1][c1] = ' ';
        return true;
    }
    return false;
}
//posicao_rei é um vetor em que as posições 0 e 1 correspondem as coordenadas da linha e da coluna do rei branco, respectivamente, e as
//posições 2 e 3 correspondem as coordenadas do rei preto.
/*bool xeque(char tabuleiro[8][9], int posicao_rei[4])
{
    if(cor_peca == BRANCO && )
}*/
bool escolher_destino(char tabuleiro[8][9], int l1, int c1, int l2, int c2, bool movido[6])
{
    char peca = tabuleiro[l1][c1];
    if (peca == 'p' || peca == 'P')
        return mover_peao(tabuleiro, l1, c1, l2, c2);
    if (peca == 'r' || peca == 'R')
    {
        bool m = mover_torre(tabuleiro, l1, c1, l2, c2);
        if (m)
        {
            if (l1 == 0 && c1 == 0)
                movido[3] = true;
            else if (l1 == 0 && c1 == 7)
                movido[5] = true;
            if (l1 == 7 && c1 == 0)
                movido[0] = true;
            else if (l1 == 7 && c1 == 7)
                movido[2] = true;
        }
        return m;
    }
    if (peca == 'n' || peca == 'N')
        return mover_cavalo(tabuleiro, l1, c1, l2, c2);
    if (peca == 'b' || peca == 'B')
        return mover_bispo(tabuleiro, l1, c1, l2, c2);
    if (peca == 'q' || peca == 'Q')
        return mover_rainha(tabuleiro, l1, c1, l2, c2);
    if (peca == 'k' || peca == 'K')
    {
        bool m1 = mover_rei(tabuleiro, l1, c1, l2, c2);
        if(m1)
        {
            if (l1 == 7 && c1 == 4)
                movido[1] = true;
            if (l1 == 0 && c1 == 4)
                movido[4] = true;
        }
        //Para o vetor movido, os índices 0, 1, 2, 3, 4 e 5 denotam respectivamente as peças nas posições (7,0), (7,4), (7,7), (0,0), (0,4) e (0,7)
        if (abs(c2 - c1) == 2 && l2 == l1)
        {
            if (c2 > c1 && !movido[cor_peca(tabuleiro, l1, c1) == BRANCO ? 1 : 4] && !movido[cor_peca(tabuleiro, l1, c1 + 3) == BRANCO ? 2 : 5]
                && (tabuleiro[l1][c1 + 1] == ' ' && tabuleiro[l1][c1 + 2] == ' '))
            {
                swap(tabuleiro[l1][c1], tabuleiro[l2][c2]);
                swap(tabuleiro[l1][c1 + 3], tabuleiro[l1][c2 - 1]);
                return true;
            }

            if (c2 < c1 && !movido[cor_peca(tabuleiro, l1, c1) == BRANCO ? 1 : 4] && !movido[cor_peca(tabuleiro, l1, c1 - 4) == BRANCO ? 0 : 3]
                && (tabuleiro[l1][c1-1] == ' ' && tabuleiro[l1][c1-2] == ' ' && tabuleiro[l1][c1-3] == ' '))
            {
                swap(tabuleiro[l1][c1], tabuleiro[l2][c2]);
                swap(tabuleiro[l1][c1 - 4], tabuleiro[l1][c2 + 1]);
                return true;
            }
        }
        else
            return mover_rei(tabuleiro, l1, c1, l2, c2);
    }
}
