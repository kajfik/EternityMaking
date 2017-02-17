// EternityCpp.cpp : main project file.


#include "iostream"
#include "stdio.h"
#include "math.h"
#include <time.h>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <locale.h>
#include <random>
using namespace std;

static int const szyrzkaPola = 11;
static int const wyszkaPola = 11;
static int const iloscKostek = szyrzkaPola*wyszkaPola;
static int const iloscKolorowBocznych = 5;
static int const iloscKolorowSrodkowych = 17;
int xypole[szyrzkaPola][wyszkaPola];
int kostki[iloscKostek][4];
int kostki2[iloscKostek][4];
int skosneKostkiHoryzontalne[szyrzkaPola - 1][wyszkaPola];
int skosneKostkiWertykalne[szyrzkaPola][wyszkaPola - 1];
int iloscSkosnychKostekBocznych = 2 * szyrzkaPola + 2 * wyszkaPola - 4;
int iloscSkosnychKostekSrodkowych = 2 * szyrzkaPola * wyszkaPola - 3 * szyrzkaPola - 3 * wyszkaPola + 4;
int iloscSkosnychKostek = 2 * szyrzkaPola*wyszkaPola - szyrzkaPola - wyszkaPola;
int iloscKostekBocznychPlusRogi = 2 * szyrzkaPola + 2 * wyszkaPola - 4;

int timer = time(NULL);
mt19937 rng(timer);
uniform_int_distribution<int> uni(1, iloscKolorowBocznych);
uniform_int_distribution<int> uni2(iloscKolorowBocznych + 1, iloscKolorowBocznych + iloscKolorowSrodkowych);
uniform_int_distribution<int> uni3(0, 3);
uniform_int_distribution<int> uni4(4, iloscKostekBocznychPlusRogi - 1);
uniform_int_distribution<int> uni5(iloscKostekBocznychPlusRogi, iloscKostek - 1);



void reset()
{
	for (int a = 0; a < szyrzkaPola; a++)
	{
		for (int b = 0; b < szyrzkaPola; b++)
		{
			xypole[a][b] = 256;
		}
	}
	for (int a = 0; a < iloscKostek; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			kostki[a][b] = -1;
		}
	}
}

void skosneBoczneKostki()
{
	int kolory[iloscKolorowBocznych];
	for (int a = 0; a < iloscKolorowBocznych; a++)
	{
		kolory[a] = 0;
	}
	int kolor = 0;
	for (int a = 0; a < iloscSkosnychKostekBocznych; a++)
	{
		kolory[kolor]++;
		kolor++;
		if (kolor == iloscKolorowBocznych)
		{
			kolor = 0;
		}
	}

	for (int a = 0; a < szyrzkaPola - 1; a++)
	{
		while (true)
		{
			kolor = uni(rng);
			if (kolory[kolor - 1] > 0)
			{
				break;
			}
		}
		skosneKostkiHoryzontalne[a][0] = kolor;
		kolory[kolor - 1]--;
		while (true)
		{
			kolor = uni(rng);
			if (kolory[kolor - 1] > 0)
			{
				break;
			}
		}
		skosneKostkiHoryzontalne[a][wyszkaPola - 1] = kolor;
		kolory[kolor - 1]--;
	}

	for (int b = 0; b < wyszkaPola - 1; b++)
	{
		int kolor;
		while (true)
		{
			kolor = uni(rng);
			if (kolory[kolor - 1] > 0)
			{
				break;
			}
		}
		skosneKostkiWertykalne[0][b] = kolor;
		kolory[kolor - 1]--;
		while (true)
		{
			kolor = uni(rng);
			if (kolory[kolor - 1] > 0)
			{
				break;
			}
		}
		skosneKostkiWertykalne[szyrzkaPola - 1][b] = kolor;
		kolory[kolor - 1]--;
	}
}

void skosneSrodkoweKostki()
{
	int kolory[iloscKolorowSrodkowych];
	for (int a = 0; a < iloscKolorowSrodkowych; a++)
	{
		kolory[a] = 0;
	}
	int kolor = 0;

	for (int a = 0; a < iloscSkosnychKostekSrodkowych; a++)
	{
		kolory[kolor]++;
		kolor++;
		
		if (kolor == iloscKolorowSrodkowych)
		{
			kolor = 0;
		}
	}
	
	for (int a = 0; a < szyrzkaPola - 1; a++)
	{
		for (int b = 1; b < wyszkaPola - 1; b++)
		{
			while (true)
			{
				kolor = uni2(rng);
				if (kolory[kolor - iloscKolorowBocznych - 1] > 0)
				{
					break;
				}
			}
			skosneKostkiHoryzontalne[a][b] = kolor;
			kolory[kolor - iloscKolorowBocznych - 1]--;
		}
	}
	for (int b = 0; b < wyszkaPola - 1; b++)
	{
		for (int a = 1; a < szyrzkaPola - 1; a++)
		{
			while (true)
			{
				kolor = uni2(rng);
				if (kolory[kolor - iloscKolorowBocznych - 1] > 0)
				{
					break;
				}
			}
			skosneKostkiWertykalne[a][b] = kolor;
			kolory[kolor - iloscKolorowBocznych - 1]--;
		}
	}
}

bool jeDobre()
{
	for (int a = 0; a < iloscKostek; a++)
	{
		for (int b = a + 1; b < iloscKostek; b++)
		{
			for (int c = 0; c < 4; c++)
			{
				if (kostki2[a][(0 + c) % 4] == kostki2[b][0] && kostki2[a][(1 + c) % 4] == kostki2[b][1] && kostki2[a][(2 + c) % 4] == kostki2[b][2] && kostki2[a][(3 + c) % 4] == kostki2[b][3])
				{
					return false;
				}
			}
		}
	}
	for (int a = 0; a < iloscKostek; a++)
	{
		for (int c = 0; c < 2; c++)
		{
			if (kostki2[a][c] == kostki2[a][(c + 2) % 4] && kostki2[a][(c + 1) % 4] == kostki2[a][(c + 3) % 4])
			{
				return false;
			}
		}
	}
	return true;
}

void dejKostki()
{
	while (true)
	{
		skosneBoczneKostki();
		skosneSrodkoweKostki();
		int kostka = 0;
		kostki2[kostka][0] = skosneKostkiHoryzontalne[0][0];
		kostki2[kostka][1] = skosneKostkiWertykalne[0][0];
		kostki2[kostka][2] = 0;
		kostki2[kostka][3] = 0;
		kostka++;
		kostki2[kostka][0] = skosneKostkiWertykalne[szyrzkaPola - 1][0];
		kostki2[kostka][1] = skosneKostkiHoryzontalne[szyrzkaPola - 2][0];
		kostki2[kostka][2] = 0;
		kostki2[kostka][3] = 0;
		kostka++;
		kostki2[kostka][0] = skosneKostkiWertykalne[0][wyszkaPola - 2];
		kostki2[kostka][1] = skosneKostkiHoryzontalne[0][wyszkaPola - 1];
		kostki2[kostka][2] = 0;
		kostki2[kostka][3] = 0;
		kostka++;
		kostki2[kostka][0] = skosneKostkiHoryzontalne[szyrzkaPola - 2][wyszkaPola - 1];
		kostki2[kostka][1] = skosneKostkiWertykalne[szyrzkaPola - 1][wyszkaPola - 2];
		kostki2[kostka][2] = 0;
		kostki2[kostka][3] = 0;
		kostka++;
		for (int a = 0; a < szyrzkaPola - 2; a++)
		{
			kostki2[kostka][0] = skosneKostkiWertykalne[a + 1][0];
			kostki2[kostka][1] = skosneKostkiHoryzontalne[a][0];
			kostki2[kostka][2] = 0;
			kostki2[kostka][3] = skosneKostkiHoryzontalne[a + 1][0];
			kostka++;
			kostki2[kostka][0] = skosneKostkiWertykalne[a + 1][wyszkaPola - 2];
			kostki2[kostka][1] = skosneKostkiHoryzontalne[a + 1][wyszkaPola - 1];
			kostki2[kostka][2] = 0;
			kostki2[kostka][3] = skosneKostkiHoryzontalne[a][wyszkaPola - 1];
			kostka++;
		}
		for (int b = 0; b < wyszkaPola - 2; b++)
		{
			kostki2[kostka][0] = skosneKostkiHoryzontalne[0][b + 1];
			kostki2[kostka][1] = skosneKostkiWertykalne[0][b + 1];
			kostki2[kostka][2] = 0;
			kostki2[kostka][3] = skosneKostkiWertykalne[0][b];
			kostka++;
			kostki2[kostka][0] = skosneKostkiHoryzontalne[szyrzkaPola - 2][b + 1];
			kostki2[kostka][1] = skosneKostkiWertykalne[szyrzkaPola - 1][b];
			kostki2[kostka][2] = 0;
			kostki2[kostka][3] = skosneKostkiWertykalne[szyrzkaPola - 1][b + 1];
			kostka++;
		}
		for (int a = 0; a < szyrzkaPola - 2; a++)
		{
			for (int b = 0; b < wyszkaPola - 2; b++)
			{
				kostki2[kostka][0] = skosneKostkiWertykalne[a + 1][b];
				kostki2[kostka][1] = skosneKostkiHoryzontalne[a + 1][b + 1];
				kostki2[kostka][2] = skosneKostkiWertykalne[a + 1][b + 1];
				kostki2[kostka][3] = skosneKostkiHoryzontalne[a][b + 1];
				kostka++;
			}
		}

		if (jeDobre())
		{
			break;
		}
	}
	for (int a = 0; a < 4; a++)
	{
		while (true)
		{
			int kostka = uni3(rng);
			if (kostki[kostka][0] == -1)
			{
				kostki[kostka][0] = kostki2[a][0];
				kostki[kostka][1] = kostki2[a][1];
				kostki[kostka][2] = kostki2[a][2];
				kostki[kostka][3] = kostki2[a][3];
				break;
			}
		}
	}
	for (int a = 4; a < iloscKostekBocznychPlusRogi; a++)
	{
		while (true)
		{
			int kostka = uni4(rng);
			if (kostki[kostka][0] == -1)
			{
				kostki[kostka][0] = kostki2[a][0];
				kostki[kostka][1] = kostki2[a][1];
				kostki[kostka][2] = kostki2[a][2];
				kostki[kostka][3] = kostki2[a][3];
				break;
			}
		}
	}
	for (int a = iloscKostekBocznychPlusRogi; a < iloscKostek; a++)
	{
		while (true)
		{
			int kostka = uni5(rng);
			if (kostki[kostka][0] == -1)
			{
				int posunyci = uni3(rng);
				kostki[kostka][(0 + posunyci) % 4] = kostki2[a][0];
				kostki[kostka][(1 + posunyci) % 4] = kostki2[a][1];
				kostki[kostka][(2 + posunyci) % 4] = kostki2[a][2];
				kostki[kostka][(3 + posunyci) % 4] = kostki2[a][3];
				break;
			}
		}
	}
	
}

void ZapiszKostkiDoKostkiTxt()
{
	ofstream file2;
	file2.open("C:\\Eternity\\kostki.txt", std::ofstream::out | std::ofstream::trunc);
	file2.close();
	file2.open("C:\\Eternity\\kostki.txt", std::ios::app);
    {
		for (int b = 0; b < wyszkaPola; b++)
		{
			int poczontek = szyrzkaPola * b + 1;
			int koniec = szyrzkaPola * (b + 1);
			file2 << "/* " << (poczontek >= 100 ? "" : poczontek >= 10 ? " " : "  ") << ("%d", poczontek) << " - " << (koniec >= 100 ? "" : koniec >= 10 ? " " : "  ") << ("%d", koniec) << " */ ";
			for (int a = 0; a < szyrzkaPola; a++)
			{
				file2 << "{ ";
				for (int c = 0; c < 4; c++)
				{
					file2 << (kostki[b*szyrzkaPola + a][c] >= 10 ? "" : " ") << ("%d", kostki[b*szyrzkaPola + a][c]);
					if (c == 3 && (a + 1)*(b + 1) != iloscKostek){ file2 << " }, "; }
					else if (c == 3 && (a + 1)*(b + 1) == iloscKostek) { file2 << " }"; }
					else { file2 << ", "; }
				}
			}
			if (b != wyszkaPola - 1)
			{
				file2 << "\n";
			}
		}
		file2.close();
	};
}

int main(/*array<System::String ^> ^args*/)
{
	reset();
	dejKostki();
	ZapiszKostkiDoKostkiTxt();
	system("pause");
	return 0;
}
