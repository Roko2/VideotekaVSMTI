#include "VsmtiVideoteka.h"

VsmtiVideoteka::VsmtiVideoteka(int broj,int odabirKorisnika)
{	
	m_sBrojUlazaka = broj;
	m_nodabirKorisnika = odabirKorisnika;
	if (broj == 1 && odabirKorisnika == 1) {
		UcitajXMLZanrove();
		UcitajXMLFilmove();
		UcitajXMLGledatelje();
		UcitajXMLGledateljFilmova();
		Izbornik();
	}
	if (broj == 2 && odabirKorisnika == 1) {
		vZanrovi.clear();
		vGledatelji.clear();
		vFilmovi.clear();
		vGledateljiNaFilmu.clear();
		UcitajXMLZanrove();
		UcitajXMLFilmove();
		UcitajXMLGledatelje();
		UcitajXMLGledateljFilmova();
		Izbornik();
	}
}
bool VsmtiVideoteka::UcitajXMLFilmove()
{
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("filmovi.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("data");
	if (oRoot == nullptr) return false;

	XMLElement* oFilm = oRoot->FirstChildElement("filmovi");
	if (oFilm == nullptr) return false;
	for (XMLElement* oElem = oFilm->FirstChildElement("film"); oElem != NULL; oElem = oElem->NextSiblingElement())
	{
		string sNaziv = oElem->Attribute("naziv");
		string sOpis = oElem->Attribute("opis");
		int nZanrId= atoi(oElem->Attribute("zanrid"));
		string nSifra = oElem->Attribute("sifra");

		Zanr* oZanr = PronadiZanrPoId(nZanrId);

		vFilmovi.push_back(new Film(sNaziv, sOpis, oZanr, nSifra));
	}
	return true;
}

bool VsmtiVideoteka::UcitajXMLGledatelje()
{
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("gledatelji.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;

	XMLElement* oGledatelj = oRoot->FirstChildElement("Osoba");
	if (oGledatelj == nullptr) return false;
	for (XMLElement* oElem = oGledatelj->FirstChildElement("Gledatelj"); oElem != NULL; oElem = oElem->NextSiblingElement())
	{
		string sIme = oElem->Attribute("ime");
		string sPrezime = oElem->Attribute("prezime");
		int nGodinaRodjenja = atoi(oElem->Attribute("godina"));
		string	nOib = oElem->Attribute("oib");
		vGledatelji.push_back(new Gledatelj(sIme, sPrezime, nGodinaRodjenja,nOib));
	}
	return true;
}

bool VsmtiVideoteka::UcitajXMLZanrove()
{
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("zanrovi.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;

	XMLElement* oZanrovi = oRoot->FirstChildElement("zanrovi");
	if (oZanrovi == nullptr) return false;
	for (XMLElement* oZanr = oZanrovi->FirstChildElement("zanr");  oZanr != NULL;  oZanr = oZanr->NextSiblingElement())
	{
		int nIdZanra = atoi(oZanr->Attribute("id"));
		string sNazivZanra = oZanr->Attribute("naziv");
		vZanrovi.push_back(new Zanr(nIdZanra, sNazivZanra));	
	}
	return true;
}

bool VsmtiVideoteka::UcitajXMLGledateljFilmova() {
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("Sifrarnik.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("data");
	if (oRoot == nullptr) return false;

	XMLElement* oSifrarnik = oRoot->FirstChildElement("sifrarnici");
	if (oSifrarnik == nullptr) return false;
	for (XMLElement* oFilm = oSifrarnik->FirstChildElement("film");  oFilm != NULL;  oFilm = oFilm->NextSiblingElement())
	{
		string nSifraFilma = oFilm->Attribute("sifra");
		Film* ooFilm = PronadiFilmPoSifri(nSifraFilma);
		for (XMLElement* oGledatelj = oFilm->FirstChildElement("gledatelj"); oGledatelj != NULL; oGledatelj = oGledatelj->NextSiblingElement()) {

			string nOibGledatelja = oGledatelj->Attribute("oib");

			Gledatelj* ooGledatelj = PronadiGledateljaPoOib(nOibGledatelja);

			vGledateljiNaFilmu.push_back(new GledateljiFilmova(ooFilm, ooGledatelj));
		}
	}
	return true;
}

Gledatelj* VsmtiVideoteka::PronadiGledateljaPoOib(string nOib)
{
	Gledatelj *oGledatelj = nullptr;
	for (size_t i = 0; i < vGledatelji.size(); i++)
	{
		if (vGledatelji[i]->m_nOibGledatelja == nOib)
		{
			oGledatelj = vGledatelji[i];
		}
	}
	return oGledatelj;
}
Film* VsmtiVideoteka::PronadiFilmPoSifri(string sifra)
{
	Film *oFilm = nullptr;
	for (size_t i = 0; i < vFilmovi.size(); i++)
	{
		if (vFilmovi[i]->m_nSifra== sifra)
		{
			oFilm = vFilmovi[i];
		}
	}
	return oFilm;
}

Zanr* VsmtiVideoteka::PronadiZanrPoId(int id) {
	Zanr* oZanr = nullptr;
	for (size_t i = 0; i < vZanrovi.size(); i++) {
		if (vZanrovi[i]->m_nIdZanra == id) {
			oZanr = vZanrovi[i];
		}
	}
	return oZanr;
}
GledateljiFilmova* VsmtiVideoteka::PronadiGledateljaFilmaPoOib(string oib) {
	GledateljiFilmova* oGledateljFilmova = nullptr;
	for (size_t i = 0; i < vGledateljiNaFilmu.size(); i++) {
		if (vGledateljiNaFilmu[i]->m_oGledatelj->m_nOibGledatelja == oib) {
			oGledateljFilmova = vGledateljiNaFilmu[i];
		}
	}
	return oGledateljFilmova;
}

void VsmtiVideoteka::IspisiSveGledatelje()
{
	cout << "\n";
	int rbr = 1;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "Rbr", "Ime","Prezime","Godina rodjenja","OIB" });
	for (size_t i = 0; i < vGledatelji.size(); i++) {
		string godinaRodjenja = to_string(vGledatelji[i]->m_nGodinaRodjenja);
		table.addRow({ to_string(rbr)+".",vGledatelji[i]->m_sIme , vGledatelji[i]->m_sPrezime , godinaRodjenja, vGledatelji[i]->m_nOibGledatelja });
		rbr++;
		godinaRodjenja = "";
	}
	cout << table;
}

void VsmtiVideoteka::IspisiSveFilmove() {
	int rbr = 1;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "Rbr", "Zanr","Film","Opis","Sifra" });
	for (size_t i = 0; i < vFilmovi.size(); i++) {
		
		table.addRow({ to_string(rbr)+"." ,vFilmovi[i]->m_oZanr->m_sNazivZanra , vFilmovi[i]->m_sNazivFilma , vFilmovi[i]->m_sOpisFilma, vFilmovi[i]->m_nSifra });
		rbr++;
	}
	cout << table;
}

void VsmtiVideoteka::GledateljiNaFilmovima() {
	string sifraFilma;
	double f;
	int rbr = 1;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "Rbr","Ime","Prezime","Godina rodjenja","OIB" });
	bool stanje = false;
	vector<GledateljiFilmova>gledatelji;
	IspisiSveFilmove();
	cout << "Upisi sifru filma kojeg zelis vidjeti!" << endl;
unosFilma:
	cout << "Sifra filma:";
	while (getline(cin, sifraFilma))
	{
		stringstream ss(sifraFilma);
		if (ss >> f)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Sifra filma: ";
	}
	if (PronadiFilmPoSifri(sifraFilma) == nullptr) {
		cout << "Unijeli ste sifru nepostojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosFilma;
	}
	
	for (size_t i = 0; i < vGledateljiNaFilmu.size(); i++) {
		if (sifraFilma == vGledateljiNaFilmu[i]->m_oFilm->m_nSifra) {
			if (vGledateljiNaFilmu[i]->m_oGledatelj != nullptr) {
				stanje = true;
				GledateljiFilmova oGledatelji(vGledateljiNaFilmu[i]->m_oFilm,vGledateljiNaFilmu[i]->m_oGledatelj);
				gledatelji.push_back(oGledatelji);
			}
			if (vGledateljiNaFilmu[i]->m_oGledatelj == nullptr && stanje==false) {
				cout <<"Nema niti jednog gledatelja na filmu!" << endl;
				goto Izlaz;
			}
		}
	}
	rbr = 1;
	if (stanje==true) {
		for (size_t j = 0; j < gledatelji.size(); j++) {
			string godinaRodjenja = to_string(gledatelji[j].m_oGledatelj->m_nGodinaRodjenja);
			table.addRow({ to_string(rbr) + ".",gledatelji[j].m_oGledatelj->m_sIme ,gledatelji[j].m_oGledatelj->m_sPrezime,godinaRodjenja,gledatelji[j].m_oGledatelj->m_nOibGledatelja });
			rbr++;
			godinaRodjenja = "";
		}
	}
	if (stanje == true) {
		cout << table;
	}
	Izlaz:
	Izlaz();
}

void VsmtiVideoteka::PrikaziSveIzdaneFilmove() {
	vector<string>gledatelji;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "Rbr","Naziv filma","Gledatelji na filmu" });
	int rbr = 1;
	cout << "------------------------------------IZDANI FILMOVI----------------------------------------" << endl;
	for (size_t k = 0; k < vFilmovi.size(); k++) {
		for (size_t i = 0; i < vGledateljiNaFilmu.size(); i++) {
				if (vGledateljiNaFilmu[i]->m_oFilm->m_nSifra == vFilmovi[k]->m_nSifra && vGledateljiNaFilmu[i]->m_oGledatelj != nullptr) {
					gledatelji.push_back(vGledateljiNaFilmu[i]->m_oGledatelj->m_sIme + " " + vGledateljiNaFilmu[i]->m_oGledatelj->m_sPrezime);
				}
			}

		double br = 0;
		string temp = "";
		for (size_t l = 0; l < gledatelji.size(); l++)
		{
			br++;
			if (gledatelji.size() == br)
			{
				temp += gledatelji[l];
			}
			else
			{
				temp += gledatelji[l] + ", ";
			}
		}
		if (br != 0) {
			table.addRow({ to_string(rbr)+".",vFilmovi[k]->m_sNazivFilma,temp });
			rbr++;	
		}
		gledatelji.clear();
		temp.clear();
	}
	cout << table;
	Izlaz();
}

void VsmtiVideoteka::TrazilicaFilmova() {	
	string pojam;
	bool stanje = false;
	int rbr = 1;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "Rbr","Naziv","Opis","Zanr","Sifra" });
	cout << "Unesite naziv(dio naziva) filma kojeg zelite pronaci ili naziv zanra prema kojem trazite!" << endl;
	cout << "Pojam: ";
	//cin.ignore(256, '\n');
	getline(cin, pojam);
	transform(pojam.begin(), pojam.end(), pojam.begin(), ::tolower);
	vector<Filmovi>vFilmoviOpet;
	for (size_t k = 0; k < vFilmovi.size(); k++) {
				Filmovi film(vFilmovi[k]->m_sNazivFilma, vFilmovi[k]->m_oZanr->m_sNazivZanra, vFilmovi[k]->m_nSifra);
				vFilmoviOpet.push_back(film);
		}
	for (size_t i = 0; i < vFilmovi.size(); i++) {
		for (size_t j = 0; j < vFilmoviOpet.size(); j++) {
			transform(vFilmoviOpet[j].m_sNazivLowerCaseFilma.begin(), vFilmoviOpet[j].m_sNazivLowerCaseFilma.end(), vFilmoviOpet[j].m_sNazivLowerCaseFilma.begin(), ::tolower);
			transform(vFilmoviOpet[j].m_sNazivLowerCaseZanra.begin(), vFilmoviOpet[j].m_sNazivLowerCaseZanra.end(), vFilmoviOpet[j].m_sNazivLowerCaseZanra.begin(), ::tolower);
			if (vFilmoviOpet[j].m_sNazivLowerCaseFilma.find(pojam) != string::npos || vFilmoviOpet[j].m_sNazivLowerCaseZanra.find(pojam) != string::npos) {
				stanje = true;
				if (vFilmovi[i]->m_nSifra == vFilmoviOpet[j].m_sIstaSifraFilma) {
					table.addRow({to_string(rbr)+".",vFilmovi[i]->m_sNazivFilma,vFilmovi[i]->m_sOpisFilma,vFilmovi[i]->m_oZanr->m_sNazivZanra,vFilmovi[i]->m_nSifra});
					rbr++;
					}
				}	
			}	
		}

	if (!stanje) {
		cout << "Vasem pojmu ne odgovara niti jedan film ili zanr po kojem trazite!" << endl;
	}
	else {
		cout << table;
	}
	Izlaz();
}

void VsmtiVideoteka::IzdavanjeFilma() {
	double d,f;
	bool gledatelj = false;
	bool gledateljNaFilmu = true;
	bool GledateljPostoji = false;
	string oibGledatelja,sifraFilma,odabir;
	int switchVarijabla;
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("Sifrarnik.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("data");
	XMLElement* oSifrarnik = oRoot->FirstChildElement("sifrarnici");
	cout << "Kako bi iznajmili film unesite Vas Oib te sifru filma!" << endl;
	cout << "UNOSENJE OIB-a-->" << endl;
	IspisiSveGledatelje();
	unosOiba:
	cout << "OIB gledatelja:";
	while (getline(cin, oibGledatelja))
	{
		stringstream ss(oibGledatelja);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "OIB gledatelja: ";
	}
	if (oibGledatelja.length() < 6 || oibGledatelja.length() > 6) {
		cout << "Vas OIB ima nevazeci format.\nUnesite 6 znamenaka kako bi Vas oib bio valjan." << endl;
		goto unosOiba;
	}
	if (PronadiGledateljaPoOib(oibGledatelja) == nullptr) {
		cout << "Unijeli ste OIB nepostojeceg gledatelja!\nZelite li postati novi gledatelj filmova?." << endl;
		cout << "->Da\n->Ne" << endl;
	ponovniUnos:
		cout << "Odabir: ";
		cin >> odabir;
		transform(odabir.begin(), odabir.end(), odabir.begin(), ::tolower);
		if (odabir == "da") {
			switchVarijabla = 1;
		}
		if(odabir != "da"){
			switchVarijabla = 3;
		}
		if (odabir == "ne") {
			switchVarijabla = 2;
		}
		if (PronadiGledateljaPoOib(oibGledatelja) != nullptr) {
			for (size_t i = 0; i < vGledatelji.size(); i++) {
				if (vGledatelji[i]->m_nOibGledatelja == oibGledatelja) {
					cout << "Vas Oib vec postoji." << endl;
					GledateljPostoji = true;
				}
			}
		}
		if (GledateljPostoji == false) {
			switch (switchVarijabla) {
			case 1: {
				DodavanjeGledatelja(oibGledatelja);
				break;
			}
			case 2: {
				goto Izlaz;
			}
			default: {
				cout << "Unijeli ste ne postojeci izbor!" << endl;
				goto ponovniUnos;
			}
			}
		}
	}
		cout << "\nUNOSENJE SIFRE FILMA-->" << endl;
		IspisiSveFilmove();
	unosFilma:
		cout << "Sifra filma:";
		while (getline(cin, sifraFilma))
		{
			stringstream ss(sifraFilma);
			if (ss >> f)
			{
				if (ss.eof())
				{
					break;
				}
			}
			cout << "Ovaj unos prima samo brojeve!" << endl;
			cout << "Sifra filma: ";
		}
		if (PronadiFilmPoSifri(sifraFilma) == nullptr) {
			cout << "Unijeli ste sifru nepostojeceg filma.\nUnesite sifru filma ponovno." << endl;
			goto unosFilma;
		}
		for (size_t i = 0; i < vGledateljiNaFilmu.size(); i++) {
			if (vGledateljiNaFilmu[i]->m_oGledatelj == nullptr) {
				gledateljNaFilmu = false;
			}
			if (vGledateljiNaFilmu[i]->m_oGledatelj != nullptr && gledateljNaFilmu == true) {
				if (vGledateljiNaFilmu[i]->m_oGledatelj->m_nOibGledatelja == oibGledatelja && vGledateljiNaFilmu[i]->m_oFilm->m_nSifra == sifraFilma) {
					cout << "Ovaj film je vec iznajmljen ovom gledatelju!" << endl;
					goto Izlaz;
				}
			}
			gledateljNaFilmu = true;
		}
		for (size_t j = 0; j < vGledateljiNaFilmu.size(); j++) {
			for (XMLElement* oFilm = oSifrarnik->FirstChildElement("film"); oFilm != NULL; oFilm = oFilm->NextSiblingElement())
			{
				if (sifraFilma == oFilm->Attribute("sifra")) {
					for (XMLElement* oGledatelj = oFilm->FirstChildElement("gledatelj"); oGledatelj != NULL; oGledatelj = oGledatelj->NextSiblingElement()) {
						string prazanOib = oGledatelj->Attribute("oib");
						if (prazanOib == "") {
							oGledatelj->SetAttribute("oib", oibGledatelja.c_str());
							cout << "Vas gledatelj uspjesno je dodan!" << endl;
							oDoc.SaveFile("Sifrarnik.xml");
							goto Izlaz;
						}
					}
				}
			}
			if (vGledateljiNaFilmu[j]->m_oGledatelj != nullptr) {
				gledatelj = true;
			}
		}
		if (gledatelj) {
			cout << "Vas film je vec iznajmljen do kraja, ne mozete iznajmiti ovaj film!" << endl;
		}
	Izlaz:
	Izlaz();
}

void VsmtiVideoteka::PovratFilma() {
	double d, f;
	string oibGledatelja, sifraFilma;
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("Sifrarnik.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
	}
	XMLNode* oRoot = oDoc.FirstChildElement("data");
	XMLElement* oSifrarnik = oRoot->FirstChildElement("sifrarnici");
	cout << "Odaberite sifru filma kojeg zelite vratiti i upisite vas OIB!" << endl;
	IspisiSveFilmove();
	unosFilma:
	cout << "Sifra filma: ";
	while (getline(cin, sifraFilma))
	{
		stringstream ss(sifraFilma);
		if (ss >> f)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Sifra filma: ";
	}
	if (PronadiFilmPoSifri(sifraFilma) == nullptr) {
		cout << "Unijeli ste sifru nepostojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosFilma;
	}
	unosGledatelja:
	cout << "OIB: ";
	while (getline(cin, oibGledatelja))
	{
		stringstream ss(oibGledatelja);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "OIB: ";
	}
	if (PronadiGledateljaPoOib(oibGledatelja) == nullptr) {
		cout << "Unijeli ste OIB nepostojeceg gledatelja!\nUnesite OIB gledatelja ponovno." << endl;
		goto unosGledatelja;
	}

	bool povratGledatelja = false;
	for (size_t i = 0; i < vGledateljiNaFilmu.size(); i++) {
		if (vGledateljiNaFilmu[i]->m_oGledatelj != nullptr) {
			if (vGledateljiNaFilmu[i]->m_oGledatelj->m_nOibGledatelja == oibGledatelja && vGledateljiNaFilmu[i]->m_oFilm->m_nSifra == sifraFilma) {
				povratGledatelja = true;
			}
		}
	}
	if (povratGledatelja == false) {
			cout << "Ovaj film niste prethodno iznajmili, ne mozete vratiti ovaj film!" << endl;
			goto Izlaz;
	}

	if (povratGledatelja == true) {
		for (XMLElement* oFilm = oSifrarnik->FirstChildElement("film"); oFilm != NULL; oFilm = oFilm->NextSiblingElement())
		{
			if (sifraFilma == oFilm->Attribute("sifra")) {
				for (XMLElement* oGledatelj = oFilm->FirstChildElement("gledatelj"); oGledatelj != NULL; oGledatelj = oGledatelj->NextSiblingElement()) {
					string prazanOib = oGledatelj->Attribute("oib");
					if (prazanOib == oibGledatelja) {
						oGledatelj->SetAttribute("oib", "");
						cout << "Vas gledatelj uspjesno je vracen!" << endl;
						oDoc.SaveFile("Sifrarnik.xml");
						goto Izlaz;
					}
				}
			}
		}
	}
	Izlaz:
	Izlaz();
}
void VsmtiVideoteka::DodavanjeFilma() {
	double d,f;
	string sifraFilma,nazivFilma,opisFilma,idZanra;
	cout << "Za dodavanje filma upisite sifru filma,naziv filma, ID zanra i opis filma." << endl;
unosSifreFilma:
	cout << "Sifra(mora biti znamenkasti broj)->";
	while (getline(cin, sifraFilma))
	{
		stringstream ss(sifraFilma);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Sifra filma: ";
	}
	if (sifraFilma.length() < 4 || sifraFilma.length() > 4) {
		cout << "Sifra filma mora biti cetveroznamenkasti broj!\nUnesite sifru filma ponovno!" << endl;
		goto unosSifreFilma;
	}
	if (PronadiFilmPoSifri(sifraFilma) != nullptr) {
		cout << "Unijeli ste sifru vec postojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosSifreFilma;
	}
	cout << "Naziv filma->";
	getline(cin, nazivFilma);
	cout << "Opis filma->";
	getline(cin, opisFilma);
	cout << "Za unos zanra odaberite jedan broj ID zanra kojeg zelite dodijeliti vasem filmu!" << endl;
	IspisiSveZanrove();
	unosIDZanra:
	cout << "ID zanra(mora biti znamenkasti broj)->";
	while (getline(cin, idZanra))
	{
		stringstream ss(idZanra);
		if (ss >> f)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "ID zanra: ";
	}
	if (PronadiZanrPoId(stoi(idZanra)) == nullptr) {
		cout << "Unijeli ste ID nepostojeceg zanra.\nUnesite ID zanra ponovno!" << endl;
		goto unosIDZanra;
	}
	
	XMLDocument oXmlDoc;
	oXmlDoc.LoadFile("filmovi.xml");
	XMLElement* oRoot = oXmlDoc.FirstChildElement("data");
	XMLElement* pChild = oRoot->FirstChildElement("filmovi");
	XMLElement* pFilm = oXmlDoc.NewElement("film");
	oXmlDoc.InsertFirstChild(pFilm);
	pFilm->SetAttribute("zanrid", idZanra.data());
	pFilm->SetAttribute("naziv", nazivFilma.data());
	pFilm->SetAttribute("opis", opisFilma.data());
	pFilm->SetAttribute("sifra", sifraFilma.data());
	pChild->InsertEndChild(pFilm);
	cout << "Film je uspjesno dodan!" << endl;
	oXmlDoc.SaveFile("filmovi.xml");

	string prazan = "";
	XMLDocument oDoc;
	oDoc.LoadFile("Sifrarnik.xml");
	XMLElement* oRooto = oDoc.FirstChildElement("data");
	XMLElement* pSifrarnik = oRooto->FirstChildElement("sifrarnici");
	XMLElement* pNoviFilm = oDoc.NewElement("film");
	oDoc.InsertFirstChild(pNoviFilm);
	pNoviFilm->SetAttribute("sifra", sifraFilma.data());
	pSifrarnik->InsertEndChild(pNoviFilm);
	oDoc.SaveFile("Sifrarnik.xml");

	XMLDocument oDocs;
	oDocs.LoadFile("Sifrarnik.xml");
	XMLElement* oRoots = oDocs.FirstChildElement("data");
	XMLElement* pChilds = oRoots->FirstChildElement("sifrarnici");
	XMLElement* pFilmovi = pChilds->FirstChildElement("film");
	for (pChilds = pFilmovi; pChilds != NULL; pChilds = pChilds->NextSiblingElement()) {
		if (pChilds->Attribute("sifra") == sifraFilma) {
			for (int i = 0; i < 10; i++) {
				XMLElement* pGledatelj = oDocs.NewElement("gledatelj");
				oDocs.InsertFirstChild(pGledatelj);
				pGledatelj->SetAttribute("oib", prazan.data());
				pChilds->InsertEndChild(pGledatelj);
			}
		}
	}
	oDocs.SaveFile("Sifrarnik.xml");
	Izlaz();	
}
void VsmtiVideoteka::DodavanjeGledatelja(string oib) {
	double f;
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int Year = 1900 + newtime.tm_year;
	int ProbablyYear = 1800 + newtime.tm_year;
	XMLDocument oXmlDoc;
	oXmlDoc.LoadFile("gledatelji.xml");
	XMLElement* oRoot = oXmlDoc.FirstChildElement("dataset");
	XMLElement* pChild = oRoot->FirstChildElement("Osoba");
	XMLElement* pGledatelj = oXmlDoc.NewElement("Gledatelj");
	string Ime,Prezime,godinaRodjenja;
	cout << "Za dodavanje gledatelja upisite Vase ime, prezime, godinuRodjenja i Oib." << endl;
	cout << "Ime->";
	cin.ignore(256, '\n');
	getline(cin, Ime);
	cout << "Prezime->";
	getline(cin, Prezime);
	unosGodine:
	cout << "Godina rodjenja->";
	while (getline(cin, godinaRodjenja))
	{
		stringstream ss(godinaRodjenja);
		if (ss >> f)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Godina rodjenja: ";
	}
	if (stoi(godinaRodjenja)< ProbablyYear || stoi(godinaRodjenja) > Year) {
		cout << "Vasa godina rodenja nije realna.\nUnesite 4 znamenke u razmaku od 100 gledajuci "<<Year<<" godinu, ovisno o tome kada ste rodeni!" << endl;
		goto unosGodine;
	}

		oXmlDoc.InsertFirstChild(pGledatelj);
		pGledatelj->SetAttribute("ime", Ime.data());
		pGledatelj->SetAttribute("prezime", Prezime.data());
		pGledatelj->SetAttribute("godina", godinaRodjenja.data());
		pGledatelj->SetAttribute("oib", oib.data());
		pChild->InsertEndChild(pGledatelj);
		oXmlDoc.SaveFile("gledatelji.xml");
		cout << "Gledatelj je uspjesno dodan!" << endl;

}

void VsmtiVideoteka::IspisiSveZanrove() {
	cout << "--------------------ZANROVI----------------------" << endl;
	ConsoleTable table(samilton::Alignment::centre);
	table.addRow({ "ID", "Naziv"});
	for (size_t i = 0; i < vZanrovi.size(); i++) {
		string rbr = to_string(vZanrovi[i]->m_nIdZanra);
		table.addRow({ rbr ,vZanrovi[i]->m_sNazivZanra });
		rbr = "";
	}
	cout << table;
}
void VsmtiVideoteka::BrisanjeFilma() {
	string sifraFilma;
	double d;
	cout << "Unesite sifru filma kojeg zelite obrisati." << endl;
	IspisiSveFilmove();
unosSifreFilma:
	cout << "Sifra(mora biti znamenkasti broj)->";
	while (getline(cin, sifraFilma))
	{
		stringstream ss(sifraFilma);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Sifra filma: ";
	}
	if (PronadiFilmPoSifri(sifraFilma) == nullptr) {
		cout << "Unijeli ste sifru nepostojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosSifreFilma;
	}
	XMLDocument oXmlDoc;
	oXmlDoc.LoadFile("filmovi.xml");
	XMLElement* oRoot = oXmlDoc.FirstChildElement("data");
	XMLElement* pChild = oRoot->FirstChildElement("filmovi");
	for (XMLElement* oFilm = pChild->FirstChildElement("film"); oFilm != NULL; oFilm = oFilm->NextSiblingElement())
	{
		if (oFilm->Attribute("sifra") == sifraFilma) {
			pChild->DeleteChild(oFilm);
			cout << "Film je uspjesno obrisan!" << endl;
			goto Izlaz;
		}
	}
Izlaz:
	XMLDocument oDoc;
	oDoc.LoadFile("Sifrarnik.xml");
	XMLElement* oRooto = oDoc.FirstChildElement("data");
	XMLElement* pSifrarnik = oRooto->FirstChildElement("sifrarnici");
	for (XMLElement* oFilmSifrarnik = pSifrarnik->FirstChildElement("film"); oFilmSifrarnik != NULL; oFilmSifrarnik = oFilmSifrarnik->NextSiblingElement())
	{
		if (oFilmSifrarnik->Attribute("sifra") == sifraFilma) {	
			oFilmSifrarnik->DeleteChildren();
			oDoc.DeleteNode(oFilmSifrarnik);
			break;
		}
	}
	oXmlDoc.SaveFile("filmovi.xml");
	oDoc.SaveFile("Sifrarnik.xml");
	Izlaz();
}
void VsmtiVideoteka::AzuriranjeFilma() {
	string sifraFilma,novaSifraFilma;
	double d,f;
	cout << "Za azuriranje filma odaberite sifru filma kojeg zelite azurirati." << endl;
	IspisiSveFilmove();
unosSifreFilma:
	cout << "Sifra(mora biti znamenkasti broj)->";
	while (getline(cin, sifraFilma))
	{
		stringstream ss(sifraFilma);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Sifra filma: ";
	}
	if (PronadiFilmPoSifri(sifraFilma) == nullptr) {
		cout << "Unijeli ste sifru nepostojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosSifreFilma;
	}
	cout << "Napisite novu sifru filma." << endl;
unosNoveSifreFilma:
	cout << "Nova sifra(mora biti znamenkasti broj)->";
	while (getline(cin, novaSifraFilma))
	{
		stringstream ss(novaSifraFilma);
		if (ss >> f)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Nova sifra filma: ";
	}
	if (novaSifraFilma.length() < 4 || novaSifraFilma.length() > 5) {
		cout << "Sifra filma mora biti cetveroznamenkasti broj!\nUnesite sifru filma ponovno!" << endl;
		goto unosNoveSifreFilma;
	}
	if (PronadiFilmPoSifri(novaSifraFilma) != nullptr) {
		cout << "Unijeli ste sifru vec postojeceg filma.\nUnesite sifru filma ponovno." << endl;
		goto unosNoveSifreFilma;
	}
	XMLDocument oXmlDoc;
	oXmlDoc.LoadFile("filmovi.xml");
	XMLElement* oRoot = oXmlDoc.FirstChildElement("data");
	XMLElement* pChild = oRoot->FirstChildElement("filmovi");
	for (XMLElement* oFilm = pChild->FirstChildElement("film"); oFilm != NULL; oFilm = oFilm->NextSiblingElement())
	{
		if (oFilm->Attribute("sifra") == sifraFilma) {
			oFilm->SetAttribute("sifra", novaSifraFilma.data());
			cout << "Film je uspjesno azuriran!" << endl;
		}
	}

	XMLDocument oDoc;
	oDoc.LoadFile("Sifrarnik.xml");
	XMLElement* oRooto = oDoc.FirstChildElement("data");
	XMLElement* pSifrarnik = oRooto->FirstChildElement("sifrarnici");
	for (XMLElement* oFilmSifrarnik = pSifrarnik->FirstChildElement("film"); oFilmSifrarnik != NULL; oFilmSifrarnik = oFilmSifrarnik->NextSiblingElement())
	{
		if (oFilmSifrarnik->Attribute("sifra") == sifraFilma) {
			oFilmSifrarnik->SetAttribute("sifra", novaSifraFilma.data());
		}
	}
	oXmlDoc.SaveFile("filmovi.xml");
	oDoc.SaveFile("Sifrarnik.xml");
	Izlaz();
}

void VsmtiVideoteka::DodavanjeBrisanjeAzuriranje() {
	string a;
	double d;
	cout << "Odaberi radnju! " << endl;
	cout << "1 - Dodavanje filma \n2 - Brisanje filma \n3 - Azuriranje filma" << endl;
	while (getline(cin, a))
	{
		stringstream ss(a);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Vas odabir: ";
	}

	switch (stoi(a)) {
	case 1: {
		DodavanjeFilma();
		break;
	}
	case 2: {
		BrisanjeFilma();
		break;
	}
	case 3: {
		AzuriranjeFilma();
		break;
	}
	default: {
		cout << "Unijeli ste nepostojeci izbor!" << endl;
		Izlaz();
		break;
	}
	}
}

void VsmtiVideoteka::TrazilicaGledatelja() {
	double d;
	ConsoleTable tablicaGledatelja(samilton::Alignment::centre);
	ConsoleTable tablicaFilmova(samilton::Alignment::centre);
	tablicaGledatelja.addRow({ "Rbr","Ime","Prezime","Godina rodjenja","OIB" });
	tablicaFilmova.addRow({ "Rbr","Naziv","Opis","Zanr","Sifra" });
	bool gledateljFilma = false;
	size_t l = 0;
	size_t m = 0;
	vector<string>gledatelj;
	string pojam,odabir,ImePrezime,a;
	vector<Oibi>vOibi;
	vector<int>vRedniBrojevi;
	bool stanje = false;
	bool oibValja = false;
	int rbr=1,rbr2=1,prijelaz=0;
	cout << "Unesite ime ili prezime gledatelja kojeg zelite pronaci (ne mora biti puni naziv)!" << endl;
	cout << "Pojam: ";
	//cin.ignore(256, '\n');
	getline(cin, pojam);
	transform(pojam.begin(), pojam.end(), pojam.begin(), ::tolower);
	vector<GledateljLowerCase>vLowerCaseGledatelji;
	for (size_t k = 0; k < vGledatelji.size(); k++) {
		GledateljLowerCase gledatelj(vGledatelji[k]->m_sIme, vGledatelji[k]->m_sPrezime,vGledatelji[k]->m_nOibGledatelja);
		vLowerCaseGledatelji.push_back(gledatelj);
	}
	for (size_t i = 0; i < vGledatelji.size(); i++) {
		for (size_t j = 0; j < vLowerCaseGledatelji.size(); j++) {
			transform(vLowerCaseGledatelji[j].m_sImeLowerCase.begin(), vLowerCaseGledatelji[j].m_sImeLowerCase.end(), vLowerCaseGledatelji[j].m_sImeLowerCase.begin(), ::tolower);
			transform(vLowerCaseGledatelji[j].m_sPrezimeLowerCase.begin(), vLowerCaseGledatelji[j].m_sPrezimeLowerCase.end(), vLowerCaseGledatelji[j].m_sPrezimeLowerCase.begin(), ::tolower);
			if (vLowerCaseGledatelji[j].m_sImeLowerCase.find(pojam) != string::npos || vLowerCaseGledatelji[j].m_sPrezimeLowerCase.find(pojam) != string::npos) {
				stanje = true;
				if (vGledatelji[i]->m_nOibGledatelja == vLowerCaseGledatelji[j].m_sOib) {
					string godinaRodjenja = to_string(vGledatelji[i]->m_nGodinaRodjenja);
					tablicaGledatelja.addRow({ to_string(rbr)+".",vGledatelji[i]->m_sIme,vGledatelji[i]->m_sPrezime,godinaRodjenja,vGledatelji[i]->m_nOibGledatelja });
					Oibi osoba(rbr, vGledatelji[i]->m_nOibGledatelja);
					vOibi.push_back(osoba);
					vRedniBrojevi.push_back(rbr);
					rbr++;
					godinaRodjenja = "";
				}
			}
		}
	}
	if (stanje == true) {
		cout << tablicaGledatelja;
		cout << "Zelite li saznati vise informacija o nekom od gledatelja?\n->Da\n->Ne" << endl;
	unosOdabira:
		cout << "Odabir: ";
		cin >> odabir;
		transform(odabir.begin(), odabir.end(), odabir.begin(), ::tolower);
		if (odabir == "da") {
			cout << "Upisite redni broj koji se nalazi ispred gledatelja kako bi saznali vise informacija o njemu." << endl;
			cin.ignore(256, '\n');
		unosRbr:
			cout << "Redni broj: ";
			
			while (getline(cin, a))
			{
				stringstream ss(a);
				if (ss >> d)
				{
					if (ss.eof())
					{
						break;
					}
				}
				cout << "Ovaj unos prima samo brojeve!" << endl;
				cout << "Redni broj: ";
			}
			for (l = 0; l < vOibi.size(); l++) {
				for (m = 0; m < vRedniBrojevi.size(); m++) {
					if (vOibi[l].m_nRbr == vRedniBrojevi[m] && vRedniBrojevi[m] == stoi(a)) {
						oibValja = true;
					}
				}
			}

			if (oibValja == false) {
				cout << "Unijeli ste nepostojeci redni broj odabranih gledatelja.\nUnesite redni broj ponovno." << endl;
				goto unosRbr;
			}
			if (oibValja == true) {
				for (size_t ii = 0; ii < vGledateljiNaFilmu.size(); ii++) {
					if (vGledateljiNaFilmu[ii]->m_oGledatelj != nullptr) {
						for (l = 0; l < vOibi.size(); l++) {
							if (vGledateljiNaFilmu[ii]->m_oGledatelj->m_nOibGledatelja == vOibi[l].m_sOib) {
								if (vOibi[l].m_nRbr == stoi(a)) {
									if (prijelaz == 0) {
										cout << "Iznajmljeni filmovi vaseg gledatelja " << vGledateljiNaFilmu[ii]->m_oGledatelj->m_sIme + " " + vGledateljiNaFilmu[ii]->m_oGledatelj->m_sPrezime << " su->\n " << endl;
									}
									tablicaFilmova.addRow({ to_string(rbr2)+".",vGledateljiNaFilmu[ii]->m_oFilm->m_sNazivFilma,vGledateljiNaFilmu[ii]->m_oFilm->m_sOpisFilma,vGledateljiNaFilmu[ii]->m_oFilm->m_oZanr->m_sNazivZanra,vGledateljiNaFilmu[ii]->m_oFilm->m_nSifra });
									rbr2++;
									gledateljFilma = true;
									prijelaz=1;
								}
							}
						}
					}
				}
				if (gledateljFilma == true) {
					cout << tablicaFilmova;
				}
				for (size_t kk = 0; kk < vGledateljiNaFilmu.size(); kk++) {
					if (vGledateljiNaFilmu[kk]->m_oGledatelj != nullptr && gledateljFilma == false) {
						cout << "Vas gledatelj nije iznajmio/la niti jedan film!" << endl;
						goto Izlaz;
					}
				}
			}
		}
		if (odabir == "ne") {
			goto Izlaz;
		}
		if (odabir != "da") {
			cout << "Unijeli ste ne postojecu opciju. Unesite odabir ponovno." << endl;
			goto unosOdabira;
		}
	}
	if (!stanje) {
		cout << "Vasem pojmu ne odgovara niti jedan gledatelj!" << endl;
		goto Izlaz;
	}
	Izlaz:
		Izlaz();
	}
	
void VsmtiVideoteka::Izbornik()
{
	string a;
	double d;
	cout << "Odaberi radnju! " << endl;
	cout << "1 - Izdavanje filma \n2 - Povrat filma \n3 - Dodavanje/Brisanje/Azuriranje filma \n4 - Trazilica filmova prema kljucnoj rijeci ili zanru"
		"\n5 - Trazilica gledatelja \n6 - Prikazi sve izdane filmove\n7 - Prikazi sve gledatelje na filmovima\n8 - Izlaz" << endl;
	while (getline(cin, a))
	{
		stringstream ss(a);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Vas odabir: ";
	}

	switch (stoi(a)) {
	case 1: {
		IzdavanjeFilma();
		break;
	}
	case 2: {
		PovratFilma();
		break;
	}
	case 3: {
		DodavanjeBrisanjeAzuriranje();
		break;
	}
	case 4: {
		TrazilicaFilmova();
		break;
	}
	case 5: {
		TrazilicaGledatelja();
		break;
	}
	case 6: {
		PrikaziSveIzdaneFilmove();
		break;
	}
	case 7: {
		GledateljiNaFilmovima();
		break;
	}
	case 8: {
		break;
	}
	default: {
		cout << "Unijeli ste nepostojeci izbor!" << endl;
		Izlaz();
		break;
	}	
   }
}

void VsmtiVideoteka::Izlaz() {

	string y;
	bool drugiPut = false;
	cout << "\n\nOdaberite opciju!" << endl;
	cout << "1 - Pocetni izbornik" << endl;
	cout << "2 - Izlaz iz programa" << endl;
	double d;
	while (getline(cin, y))
	{
		stringstream ss(y);
		if (ss >> d)
		{
			if (ss.eof())
			{
				break;
			}
		}
		cout << "Ovaj unos prima samo brojeve!" << endl;
		cout << "Vas odabir: ";
	}
	if (stoi(y) == 1) {
		system("cls");
		drugiPut = true;
		VsmtiVideoteka opetUlaz = VsmtiVideoteka(2,stoi(y));
	}
	if (drugiPut == false) {
		switch (stoi(y))
		{
		case 1:
			system("cls");
			Izbornik();
			break;
		case 2:
			break;
		default:
			cout << "Pogresan odabir!\n Unesite ponovno!" << endl;
			Izlaz();
			break;
		}
	}
}

VsmtiVideoteka::~VsmtiVideoteka() {
}
