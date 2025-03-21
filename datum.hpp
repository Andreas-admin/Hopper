// -----------------------------------------------------
// datum.h
// -----------------------------------------------------
#ifndef DATUM_H // Mehrfaches Inkludieren verhindern.
#define DATUM_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <vector>

class Datum
{
private:
	short tag, monat, jahr;
public:
	Datum();

	Datum( int tag, int monat, int jahr);			// überladener Konstruktor
	void setDatum(); 								// Setzt das aktuelle Datum.
	bool setDatum( int tag, int monat, int jahr);	// setzt ein Datum und prüft es.

	int getTag() const;
	int getMonat() const;
	int getJahr() const;

	bool getSchaltjahr() const;
	std::string asString() const;
	void print();

	bool operator<(const Datum& anderes) const;
	bool operator>(const Datum& anderes) const;
};

// und noch die Prototypen

bool isSchaltjahr(int);
int monatsTage(int, bool);
int datDiff(Datum&, Datum&);


#endif // DATUM_H
