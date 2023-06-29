#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

char player[2][32];
int v[3][3];
int op;
int locLiber = 0;
int numarMutare = 0;
int* CheckWinOrLose();


void Clear()
{
	system("cls"); 
}
void PrintTable(bool clearConsole=false)
{
	if(clearConsole)
		Clear();
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Legenda:\n_ este loc liber\n");
	printf("X este %s \n", player[0]);
	printf("0 este %s \n\n", player[1]);
	locLiber = 0;

	for (int i = 0; i < 3; ++i)
	{
		printf(" | ");
		for (int j = 0; j < 3; ++j)
			if (v[i][j] == 0)
			{
				locLiber++;
				printf("_ | ");
			}
			else if (v[i][j] == 1)
				printf("X | ");
			else
				printf("0 | ");

		printf("\n -------------\n");
	}
}
// 0 = nimeni nu a castigat || 1 = avem un castigator/egal deci oprim jocul
int CheckWin()
{
	int valLinie,valColoana,valDP,valDS,x;
	for (x = 0; x < 2; ++x)
	{
		valDP = valDS = 0;
		for (int i = 0; i < 3; ++i)
		{
			valLinie = valColoana = 0;
			for (int j = 0; j < 3; ++j)
			{
				//Verificam pe linie
				if (v[i][j] == x + 1)
					++valLinie;
				//Verificam pe coloana
				if (v[j][i] == x + 1)
					++valColoana;
				//Verificam DP
				if (i == j && v[i][j] == x + 1)
					++valDP;
				//Verificam DS
				if (i + j == 2 && v[i][j] == x + 1)
					++valDS;
			}
			if (valLinie == 3|| valColoana == 3)
				goto winner;
		}
		if (valDP == 3|| valDS == 3)
			goto winner;
	}

	if (locLiber == 0)
		goto winner;

	return 0;
	winner:
		if (locLiber == 0)
			printf("\n\nJocul s-a terminat!!!Egalitate");
		else
			printf("\n\nJocul s-a terminat!!!Castigator este %s", player[x]);
		std::this_thread::sleep_for(std::chrono::milliseconds(3500));
		return 1;
}

void ReadCoord(int *x, int *y)
{
	do
	{
		scanf("%d %d", x, y);

		if(0 >= *x || 0 >= *y || *x > 3 || *y > 3)
		{
			printf("\nTe rog sa scrii niste coordonate valide![ x: 1/2/3   y: 1/2/3 ]\n");
			continue;
		}

		*x -= 1;
		*y -= 1;

		if (v[*x][*y] != 0)
		{
			printf("\nTe rog scrie alte coordonate deoarece pozitia aleasa este ocupata!\n");
			continue;
		}
		break;
	} while (1 == 1);
}

void PvP(int whosTurn)
{
	PrintTable(true);
	if (CheckWin())
		return;

	int x, y;
	printf("Este randul lui %s, scrie te rog coord(x y)[ex: 1 1]: \n", player[whosTurn]);
	ReadCoord(&x, &y);
	v[x][y] = whosTurn + 1;

	PvP(!whosTurn);
}

void RandomMove()
{
	int *pozitiiLibere[9], nr = 0;

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (v[i][j] == 0)
				pozitiiLibere[nr++] = &v[i][j];

	*pozitiiLibere[rand() % nr] = 2;
}

void PvAIEasy(int whosTurn)
{
	PrintTable(true);
	if (CheckWin())
		return;

	if (whosTurn == 0)
	{
		int x, y;
		printf("Scrie te rog coord(x y)[ex: 1 1]: \n");
		ReadCoord(&x, &y);
		v[x][y] = 1;
	}
	else
	{
		RandomMove();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		PrintTable(true);
		printf("Calculator a facut o miscare\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}

	PvAIEasy(!whosTurn);
}

void PvAIHard(int whosTurn)
{
	PrintTable(true);
	if (CheckWin())
		return;

	if (whosTurn == 0)
	{
		int x, y;
		printf("Scrie te rog coord(x y)[ex: 1 1]: \n");
		ReadCoord(&x, &y);
		v[x][y] = 1;
	}
	else
	{
		switch (numarMutare)
		{
			case 0:
			{
				if (v[1][1] == 0)
					v[1][1] = 2;
				else
					v[0][0] = 2;
				break;
			}
			default:
			{
				int *poz = CheckWinOrLose();
				if (poz == NULL)
					RandomMove();
				else
				{
					*poz = 2;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		PrintTable(true);
		printf("Calculator a facut o miscare\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		numarMutare++;
	}

	PvAIHard(!whosTurn);
}

int main()
{
	time_t t;
	srand((unsigned)time(&t));

	do
	{
		Clear();
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				v[i][j] = 0;

		printf("Optiuni  de joc:\n1. Player VS Player\n2. Player VS PC(Easy)\n3. Player VS PC(Hard to imposible)\n\n0. Iesire\n\n");
		scanf("%d", &op);
		switch (op)
		{
			case 1:
			{
				Clear();
				printf("\nNume jucator 1: \n");
				scanf("%s", player[0]);
				printf("\nNume jucator 2: \n");
				scanf("%s", player[1]);

				PvP(0);
				break;
			}
			case 2:
			{
				Clear();
				printf("\nNume jucator: \n");
				scanf("%s", player[0]);
				strcpy(player[1], "Calculator(Easy)");

				PvAIEasy(0);
				break;
			}
			case 3:
			{
				Clear();
				printf("\nNume jucator: \n");
				scanf("%s", player[0]);
				strcpy(player[1], "Calculator(Hard)");

				PvAIHard(0);
				break;
			}
		}
		fflush(stdin);
	} while (op != 0);

	return 1;
}



int* CheckWinOrLose()
{
	int aux = 0, *auxliber = NULL;

	for (int x = 1; x >= 0; --x)
	{
		//Verificam pe linie
		for (int i = 0; i < 3; ++i)
		{
			aux = 0;
			auxliber = NULL;
			for (int j = 0; j < 3; ++j)
				if (v[i][j] == x + 1)
					aux++;
				else if (v[i][j] == 0)
					auxliber = &v[i][j];

			if (aux == 2 && auxliber != NULL)
				return auxliber;
		}

		//Verificam pe coloana
		for (int i = 0; i < 3; ++i)
		{
			aux = 0;
			auxliber = NULL;
			for (int j = 0; j < 3; ++j)
				if (v[j][i] == x + 1)
					aux++;
				else if (v[j][i] == 0)
					auxliber = &v[j][i];

			if (aux == 2 && auxliber != NULL)
				return auxliber;
		}

		//Verificam DP
		aux = 0;
		auxliber = NULL;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
				if (i == j)
				{
					if (v[i][j] == x + 1)
						aux++;
					else if (v[i][j] == 0)
						auxliber = &v[i][j];
				}
		}

		if (aux == 2 && auxliber != NULL)
			return auxliber;

		//Verificam DS
		aux = 0;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
				if (i + j == 2)
				{
					if (v[i][j] == x + 1)
						aux++;
					else if (v[i][j] == 0)
						auxliber = &v[i][j];
				}
		}

		if (aux == 2 && auxliber != NULL)
			return auxliber;

	}

	return NULL;
}