#include "Film.h"

Film::Film(string naziv, string opis, Zanr* oZanr,string sifra)
{
	m_sNazivFilma = naziv;
	m_sOpisFilma = opis;
	m_oZanr = oZanr;
	m_nSifra = sifra;

}

Film::~Film() 
{
}



