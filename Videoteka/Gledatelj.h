#pragma once
#include "Osoba.h"
#include <string>
#include <iostream>

using namespace std;
class Gledatelj :
	public Osoba
{
public:
	Gledatelj(string ime, string prezime, int godinaRodjenja,string oib);
	~Gledatelj();


	string m_nOibGledatelja;

};

