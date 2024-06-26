#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <time.h>

using namespace std;

int IloscDanych;
int WysokoscWykresu;
char*** tabela;
float skala;
int okres;

string kosz;
char znak;
char przycisk;

int NumerOstatni;
int NumerLinii;
int Day;
int Month;
int Year;

string NazwaPlikuWejsciowego;
string PlikWyjsciowy;

ifstream plik;
ofstream plikWykresu;
ofstream pliklog("AkcjeUzytkownika.log", ios::app);

time_t ltime;

struct Dane
{
    int Rok, Miesiac, Dzien;
   float Otwarcie, Najwyzszy, Najnizszy, Zamkniecie;
};


struct Dane_Okres
{
    float OtwarcieMax, NajwyzszyMax, NajnizszyMin, ZamkniecieMax;
};


void obslugaPrzyciskow(char);
void WczytajDane();
void KolorSwiecy(Dane_Okres, int);
void RysujSwiece();
void CienSwiecyGora(Dane_Okres, int);
void CienSwiecyDol(Dane_Okres, int);
void Margines();
void UsunTabele();
void PusteMiejsca();
void StworzTabele();
void Ostatnie200Linijek();
void WczytajIloscLinijek();
void KawalekWykresu();
void WyborDaty(Dane&);


int main()
{
    cout << "Witaj w generatorze wykresu swiecowego!!" << endl;
    cout << "Dane Autora:" << endl;
    cout << "Jakub Litwin 198355" << endl;
    cout << "MENU:\n" << endl;
    cout << "Nacisnij q, aby wyjsc z programu." << endl;
    cout << "Nacisnij g, aby narysowac podstawowy wykres w pliku chart.txt" << endl;
    cout << "Nacisnij h, aby dostosowac parametry do pliku txt" << endl;
    cout << "Nacisnij z, aby wybrac plik wejsciowy .csv oraz wyjsciowy .txt" << endl;

    cin >> przycisk;

    time(&ltime);

    pliklog << ("%s", ctime(&ltime)) << "  Nacisniecie przycisku: " << przycisk << endl;

    obslugaPrzyciskow(przycisk);
}


void obslugaPrzyciskow(char przycisk)
{
    switch (przycisk)
    {
    case 'q':
        exit(0);
        break;

    case 'g':

        NazwaPlikuWejsciowego = "intc_us_data.csv";
        PlikWyjsciowy = "chart.txt";

        plik.open(NazwaPlikuWejsciowego.c_str());
        plikWykresu.open(PlikWyjsciowy.c_str());

        system("cls");

        okres = 1;
        WysokoscWykresu = 51;
        IloscDanych = 201;
        skala = 1;

        WczytajIloscLinijek();
        Ostatnie200Linijek();
        StworzTabele();
        PusteMiejsca();
        WczytajDane();

        cout << "Plik zostal wygenerowany :)" << endl;

        break;

    case 'h':
        system("cls");

        cout << "Podaj wysokosc wykresu: (min: 100) " << endl;

        cin >> WysokoscWykresu;

        skala = (WysokoscWykresu / 100);

        cout << "podaj date poczatkowa YYYY-MM-DD:  ";

        cin >> Year >> znak >> Month >> znak >> Day;

        cout << endl;

        cout << "Podaj szerokosc wykresu: (Max: 505 w notepad !!!) " << endl;

        cin >> IloscDanych;

        cout << "Podaj okres jednej swiecy:\n dzien: 1\n tydzien: 5\n miesiac: 20\n";

        cin >> okres;

        cout << endl;

        WysokoscWykresu += 1;
        IloscDanych += 1;

        WczytajIloscLinijek();
        StworzTabele();
        PusteMiejsca();
        WczytajDane();

        exit(0);
        break;

    case 'z':
        system("cls");

        cout << "Podaj nazwe pliku wejsciowego:" << endl;

        cin >> NazwaPlikuWejsciowego;

        cout << endl;

        cout << "Podaj nazwe pliku wyjsciowego: " << endl;

        cin >> PlikWyjsciowy;

        cout << endl;

        plik.open(NazwaPlikuWejsciowego.c_str());
        plikWykresu.open(PlikWyjsciowy.c_str());

        system("cls");

        main();

    default:
        system("cls");

        main();
    }

    plik.close();
    plikWykresu.close();
}


void WczytajDane()
{
    Dane wykres;

    Dane_Okres wykres_Okres;

    if (!plik)
    {
        cout << "Plik .csv nie zostal otwarty :( " << endl;
        return;
    }

    if (!plikWykresu)
    {
        cout << "Plik .txt nie zostal otwarty :( " << endl;
        return;
    }

    int i = 1;
    int x = 0;

   
    if(przycisk != 'g')
    {
	 WyborDaty(wykres);
    }
   

    while (i < IloscDanych)
    {
        x = 0;

        wykres_Okres.OtwarcieMax =0;
        wykres_Okres.ZamkniecieMax =0;
        wykres_Okres.NajnizszyMin =0;
        wykres_Okres.NajwyzszyMax =0;

        while (x < okres)
        {

            plik >> wykres.Rok >> znak >> wykres.Miesiac >> znak >> wykres.Dzien >> znak >> wykres.Otwarcie >> znak >> wykres.Najwyzszy >> znak >> wykres.Najnizszy >> znak >> wykres.Zamkniecie >> kosz;

            if (x == 0)
            {
                wykres_Okres.NajnizszyMin = wykres.Najnizszy * skala;
            }

            if (wykres.Otwarcie > wykres_Okres.OtwarcieMax)
            {
                wykres_Okres.OtwarcieMax = wykres.Otwarcie * skala;
            }

            if (wykres.Zamkniecie > wykres_Okres.ZamkniecieMax)
            {
                wykres_Okres.ZamkniecieMax = wykres.Zamkniecie * skala;
            }

            wykres_Okres.NajwyzszyMax = wykres.Najwyzszy * skala;

            KolorSwiecy(wykres_Okres, i);
            CienSwiecyGora(wykres_Okres, i);
            CienSwiecyDol(wykres_Okres, i);

            ++x;
        }
        ++i;
    }
    Margines();
    
     time(&ltime);

            pliklog << ("%s", ctime(&ltime)) << "  Wygenerowanie wykresu " << endl;
            
    RysujSwiece();
    KawalekWykresu();
    UsunTabele();

    time(&ltime);

    pliklog << ("%s", ctime(&ltime)) << "  Narysowanie wykresu " << endl;
    pliklog.close();
}


void KolorSwiecy(Dane_Okres wykres_Okres, int i)
{
    if (wykres_Okres.ZamkniecieMax < wykres_Okres.OtwarcieMax)
    {
        for (int j = round(wykres_Okres.ZamkniecieMax); j < round(wykres_Okres.OtwarcieMax); j++)
        {
            tabela[j][i][0] = int(35); 
        }
    }
    if (wykres_Okres.OtwarcieMax < wykres_Okres.ZamkniecieMax)
    {
        for (int j = round(wykres_Okres.OtwarcieMax); j < round(wykres_Okres.ZamkniecieMax); j++)
        {
            tabela[j][i][0] = int(79); 
        }
    }
}


void CienSwiecyGora(Dane_Okres wykres_Okres, int i)
{
    if (wykres_Okres.OtwarcieMax < wykres_Okres.ZamkniecieMax)
    {
        for (int j = round(wykres_Okres.NajnizszyMin); j < round(wykres_Okres.OtwarcieMax); j++)
        {
            tabela[j][i][0] = int(124);
        }
    }

    else if (wykres_Okres.ZamkniecieMax < wykres_Okres.OtwarcieMax)
    {
        for (int j = round(wykres_Okres.NajnizszyMin); j < round(wykres_Okres.ZamkniecieMax); j++)
        {
            tabela[j][i][0] = int(124);
        }
    }
}


void CienSwiecyDol(Dane_Okres wykres_Okres, int i)
{
    if (wykres_Okres.OtwarcieMax > wykres_Okres.ZamkniecieMax)
    {
        for (int j = round(wykres_Okres.OtwarcieMax); j < round(wykres_Okres.NajwyzszyMax); j++)
        {
            tabela[j][i][0] = int(124);
        }
    }

    if (wykres_Okres.ZamkniecieMax > wykres_Okres.OtwarcieMax)
    {
        for (int j = round(wykres_Okres.ZamkniecieMax); j < round(wykres_Okres.NajwyzszyMax); j++)
        {
            tabela[j][i][0] = int(124);
        }
    }
}


void RysujSwiece()
{
    for (int i = WysokoscWykresu - 1; i >= 0; i--)
    {
        plikWykresu << setw(10) << (i / skala) << setw(3);

        for (int j = 0; j < IloscDanych; j++)
        {
            plikWykresu << tabela[i][j][0] << ' ';
        }
        plikWykresu << endl;
    }
    plikWykresu << setw(IloscDanych * 2) << (IloscDanych - 1) << " dni" << endl;
    plikWykresu << "Data poczatku wykresu: " << Year << "-" << Month << "-" << Day;
}


void Margines()
{
    for (int i = 0; i < WysokoscWykresu; i++)
    {
        tabela[i][0][0] = '|';
    }

    for (int i = 1; i < IloscDanych; i++)
    {
        tabela[0][i][0] = '-';
    }
    tabela[0][0][0] = '+';
    tabela[0][IloscDanych - 1][0] = '>';
    tabela[WysokoscWykresu - 1][0][0] = '^';

}


void UsunTabele()
{
    for (int i = 0; i < WysokoscWykresu; i++)
    {
        for (int j = 0; j < IloscDanych; j++)
        {
            delete[] tabela[i][j];
            tabela[i][j] = NULL;
        }
        delete[] tabela[i];
        tabela[i] = NULL;
    }
    delete[] tabela;
    tabela = NULL;
}


void PusteMiejsca()
{
    for (int i = WysokoscWykresu - 1; i >= 0; i--)
    {
        for (int j = 0; j < IloscDanych; j++)
        {
            tabela[i][j][0] = ' ';
        }
    }
}


void StworzTabele()
{
    tabela = new char** [WysokoscWykresu];
    for (int i = 0; i < WysokoscWykresu; ++i)
    {
        tabela[i] = new char* [IloscDanych];
        for (int j = 0; j < IloscDanych; ++j)
        {
            tabela[i][j] = new char;
        }
    }
}


void Ostatnie200Linijek()
{
    while (NumerLinii != (NumerOstatni - IloscDanych))
    {
        NumerLinii++;
        plik >> kosz;
    }
}


void WczytajIloscLinijek()
{
    while (plik >> kosz)
    {
        NumerOstatni++;
    }

    plik.close();

    plik.open(NazwaPlikuWejsciowego.c_str());
}


void KawalekWykresu()
{
    for (int i = 50; i >= 0; i--)
    {
        for (int j = 0; j < 50; j++)
        {
            cout << tabela[i][j][0] << ' ';
        }
        cout << endl;
    }
}


void WyborDaty(Dane &wykres)
{
    int i=0;
    while ((wykres.Dzien != Day) || (wykres.Miesiac != Month) || (wykres.Rok != Year))
    {
        if (i == 0)
        {
            plik >> kosz;
        }

        plik >> wykres.Rok >> znak >> wykres.Miesiac >> znak >> wykres.Dzien >> kosz;
        i++;
    }
}