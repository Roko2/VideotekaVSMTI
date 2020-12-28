#pragma once
#include <string>
#include <iostream>

using namespace std;

class Osoba
{
	public:
		Osoba(string ime,string prezime,int godinaRodjenja);
		~Osoba();

		string m_sIme;
		string m_sPrezime;
		int m_nGodinaRodjenja;

};

