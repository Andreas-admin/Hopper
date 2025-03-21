// -----------------------------------------------------
// datum.cpp
// -----------------------------------------------------

#include "datum.hpp"

Datum::Datum() : tag(1), monat(1), jahr(2000){}

Datum::Datum( int tag, int monat, int jahr)			// überladener Konstruktor
{
	if( !setDatum( tag, monat, jahr) )
		tag = 1; monat = 1; jahr = 2000; 		// Falls Datum ungültig.
}

int Datum::getTag() const { return tag; }
int Datum::getMonat() const { return monat; }
int Datum::getJahr() const { return jahr; }

std::string Datum::asString() const // Datum als String zurückgeben
{ 
	std::string datumString;
	std::stringstream iostream; 				// Zur Konvertierung Zahl -> String und Formatierung.
	iostream << std::setfill('0') << std::setw(2) << tag << '.' << std::setw(2) << monat << '.' << jahr;
	iostream >> datumString;
	return datumString;
}

void Datum::setDatum() 		// Aktuelles Datum holen und
{							// den Datenelementen zuweisen.
	struct tm *zeit; 		// Zeiger auf Struktur tm.
	time_t sec; 			// Für die Sekunden.
	time(&sec); 			// Aktuelle Zeit holen.
	zeit = localtime(&sec); // Eine Struktur vom Typ tm
							// initialisieren und Zeiger
							// darauf zurückgeben.
	tag = (short) zeit->tm_mday;
	monat = (short) zeit->tm_mon + 1;
	jahr = (short) zeit->tm_year + 1900;
}

bool isSchaltjahr(int jahr)
{
	return (jahr % 4 == 0 && jahr % 100 != 0) || (jahr % 400 == 0);
}

bool Datum::getSchaltjahr() const
{
    return isSchaltjahr(jahr);
}

bool Datum::setDatum( int tg, int mn, int jr)
{
	if( jr < 1900  ||  tg < 1 || tg > 31 || mn < 1 || mn > 12 )
	{
		return false;
	} 
	switch(mn) // Monate mit weniger als 31 Tagen
	{
		case 2:
		if( isSchaltjahr(jr))
		{	
			if( tg > 29)
			{
				return false;			
			}
		}
		else if( tg > 28)
		{
			return false;
		}
		break;

		case 4:
		case 6:
		case 9:
		case 11:
		if( tg > 30) 
		{
			return false;
		}
	}
	tag = (short) tg;
	monat = (short) mn;
	jahr = (short) jr;
	return true;
}

// -----------------------------------------------------
void Datum::print() 		// Datum anzeigen
{
	std::cout << asString() << std::endl;
}

bool Datum::operator<(const Datum& anderes) const
{
	if (jahr < anderes.jahr)
	{
		return true;
	}

	if (monat < anderes.monat)
	{
		return true;	
	}

	if (tag < anderes.tag)
	{
		return true;
	}
		
	return false;
}

bool Datum::operator>(const Datum& anderes) const
{
	if (jahr > anderes.jahr)
	{
		return true;
	}

	if (monat > anderes.monat)
	{
		return true;	
	}

	if (tag > anderes.tag)
	{
		return true;
	}
		
	return false;
}

int monatsTage(int mo, bool sj)
{
	if (mo == 1 || mo == 3 || mo == 5 || mo ==7 || mo == 8 || mo == 10 || mo == 12)
	{
		return 31;
	}
	else if (mo == 4 || mo == 6 || mo == 9 || mo == 11)
	{
		return 30;
	}
	else if (sj)
	{
		return 29;
	}
	else
	{
		return 28;
	}
}

int datDiff(Datum& von, Datum& bis)
{
    int d = 0;
    int m = bis.getMonat();
    int j = bis.getJahr();

    if ((von.getJahr() > j) 
        || (von.getJahr() == j && von.getMonat() > m)
        || (von.getJahr() == j && von.getMonat() == m && von.getTag() > bis.getTag()))
    { return 0; }

    d = bis.getTag() - von.getTag();
    while (m > von.getMonat() || j > von.getJahr())
    {
        m--;
        if ( m == 0 )
        {
            j--;
            m = 12;
        }
        d += monatsTage(m, isSchaltjahr(j));
    }
    return d;
}