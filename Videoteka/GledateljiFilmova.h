#pragma once
#include <string>
#include <iostream>
#include "Film.h"
#include "Gledatelj.h"

using namespace std;
class GledateljiFilmova
{
public:
	GledateljiFilmova(Film* oFilm,Gledatelj* oGledatelj);
	~GledateljiFilmova();
	
	Film* m_oFilm;
	Gledatelj* m_oGledatelj;
};

