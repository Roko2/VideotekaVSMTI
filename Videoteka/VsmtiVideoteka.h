#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Film.h"
#include "Gledatelj.h"
#include "Zanr.h"
#include "tinyxml2.h"
#include "GledateljiFilmova.h"
#include <algorithm>
#include "Filmovi.h"
#include <time.h>
#include "GledateljLowerCase.h"
#include "Oibi.h"
#include "CppConsoleTable.hpp"

using namespace std;
using namespace tinyxml2;
using ConsoleTable = samilton::ConsoleTable;
class VsmtiVideoteka
{
public:
	VsmtiVideoteka(int broj,int odabirKorisnika);
	~VsmtiVideoteka();

	vector<Film*> vFilmovi;
	vector<Gledatelj*> vGledatelji;
	vector<Zanr*> vZanrovi;
	vector<GledateljiFilmova*>vGledateljiNaFilmu;

	bool UcitajXMLFilmove();
	bool UcitajXMLGledatelje();
	bool UcitajXMLZanrove();
	bool UcitajXMLGledateljFilmova();
	void IspisiSveGledatelje();
	void IspisiSveFilmove();
	void PovratFilma();
	void TrazilicaGledatelja();
	void DodavanjeBrisanjeAzuriranje();
	void TrazilicaFilmova();
	void DodavanjeFilma();
	void BrisanjeFilma();
	void AzuriranjeFilma();
	void IspisiSveZanrove();
	void Izlaz();
	void Izbornik();
	void IzdavanjeFilma();
	void GledateljiNaFilmovima();
	void PrikaziSveIzdaneFilmove();
	void DodavanjeGledatelja(string Oib);

	int m_sBrojUlazaka;
	int m_nodabirKorisnika;

	Gledatelj* PronadiGledateljaPoOib(string nOib);
	Film* PronadiFilmPoSifri(string sifra);
	Zanr* PronadiZanrPoId(int id);
	GledateljiFilmova* PronadiGledateljaFilmaPoOib(string oib);

};

