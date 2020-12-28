#pragma once
#include <string>
#include <iostream>
using namespace std;
class GledateljLowerCase
{
public:
	GledateljLowerCase(string ime,string prezime,string oib);
	~GledateljLowerCase();

	string m_sImeLowerCase;
	string m_sPrezimeLowerCase;
	string m_sOib;
};

