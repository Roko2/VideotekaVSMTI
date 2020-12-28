#include "Gledatelj.h"


Gledatelj::Gledatelj(string ime, string prezime, int godinaRodjenja, string oib):Osoba(ime,prezime,godinaRodjenja)
{
	m_nOibGledatelja = oib;
}

Gledatelj::~Gledatelj()
{
}

