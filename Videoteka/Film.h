#pragma once
#include <string>
#include <iostream>
#include "Zanr.h"
#include "Gledatelj.h"
#include <vector>

using namespace std;
class Film
{
public:
	Film(string naziv, string opis, Zanr* oZanr,string sifra);
	~Film();

	string m_sNazivFilma;
	string m_sOpisFilma;
	string m_nSifra;
	Zanr* m_oZanr;
};

