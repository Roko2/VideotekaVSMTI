#include "Filmovi.h"

Filmovi::Filmovi(string naziv, string zanr, string sifra) {
	m_sNazivLowerCaseFilma = naziv;
	m_sNazivLowerCaseZanra = zanr;
	m_sIstaSifraFilma = sifra;
}

Filmovi::~Filmovi() {

}
