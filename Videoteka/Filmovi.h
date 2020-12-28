#pragma once
#include <iostream>
#include <string>
using namespace std;
class Filmovi
{
public:
	Filmovi(string naziv, string zanr, string sifra);
	~Filmovi();

	string m_sNazivLowerCaseFilma;
	string m_sNazivLowerCaseZanra;
	string m_sIstaSifraFilma;
};

