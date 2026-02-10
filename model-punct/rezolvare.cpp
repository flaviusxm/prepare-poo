
    // CERINTA COMPLETA:
    //
    // Clasa Punct (3p):
    // - contine: nume (string), abscisa si ordonata (double)
    // - constructori (0,5p)
    // - functii de acces (0,5p)
    // - operator<< : afiseaza sub forma {nume_locatie, (abscisa, ordonata)} (1p)
    // - operator() : returneaza distanta dintre doua puncte (1p)
    //
    // Clasa Traseu (6p):
    // - gestioneaza o colectie de obiecte de tip Punct
    // - constructori si functii de acces (1p)
    // - operator<< : adauga un punct in traseu (1p)
    // - operator[] : primeste numele unei locatii si returneaza adresa punctului corespunzator
    //   sau NULL daca acesta nu exista (1p)
    // - operator double : returneaza lungimea totala a traseului (suma distantelor intre punctele succesive) (1p)
    // - operator<< : afiseaza pe ecran numele traseului, toate punctele sale (folosind tab
    //   pentru indentare) si lungimea sa totala (1p)
    // - sa se scrie traseul intr-un fisier, ordonand punctele dupa axa Ox (crescator) pentru a evita
    //   "intoarcerile" (mentinand insa primul punct fix ca punct de plecare) (1p)
    //
    // ATENTIE: In urma sortarii pe Ox lungimea traseului se poate modifica! (Impact?)
    // ====================================================================================================

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
using namespace std;

class Punct {
    string nume;
    double x, y;

public:
    Punct() : nume(""), x(0.0), y(0.0) {}
    Punct(string nume, double x, double y) : nume(nume), x(x), y(y) {}


    string getNume() const { return nume; }
    double getX() const { return x; }
    double getY() const { return y; }

    friend ostream& operator<<(ostream& out, const Punct& p) {
        out << "{" << p.nume << ", (" << p.x << ", " << p.y << ")}";
        return out;
    }


    double operator()(const Punct& p2) const {
        return sqrt(pow(x - p2.x, 2) + pow(y - p2.y, 2));
    }
};

class Traseu {
    string nume;
    vector<Punct> puncte;

public:
    Traseu():nume(""),puncte(nullptr_t){}
    Traseu(string nume, const vector<Punct> &p):nume(nume),puncte(p) {}
    Traseu& operator<<(const Punct& p) {
        puncte.push_back(p);
        return *this;
    }


    Punct* operator[](string numeLoc) {
        for (int i = 0; i < puncte.size(); i++) {
            if (puncte[i].getNume() == numeLoc) return &puncte[i];
        }
        return NULL;
    }


    operator double() const {
        double lungime = 0;
        for (size_t i = 1; i < puncte.size(); i++) {
            lungime += puncte[i-1](puncte[i]);
        }
        return lungime;
    }


    friend ostream& operator<<(ostream& out, const Traseu& t) {
        out << "Traseu: " << t.nume << endl;
        for (const auto& p : t.puncte) {
            out << "\t" << p << endl;
        }
        out << "Lungime traseu: " << (double)t << endl;
        return out;
    }


    void scrieInFisier(string numeFisier) {
        if (puncte.size() < 2) return;

        vector<Punct> copie = puncte;

        sort(copie.begin() + 1, copie.end(), [](const Punct& a, const Punct& b) {return a.getX() < b.getX();});

        ofstream f(numeFisier);
        f << "Traseu ordonat" << nume << endl;
        for (const auto& p : copie) {
            f << "\t" << p << endl;
        }

        double lungimeNoua = 0;
        for(size_t i=1; i < copie.size(); i++) lungimeNoua += copie[i-1](copie[i]);
        f << "Lungime noua: " << lungimeNoua << endl;
        f.close();
    }
};
int main() {

    Punct p1("A", 0, 0);
    Punct p2("B", 10, 5);

    Traseu t("traseu test");
    t << p1 << p2 << p3;

    cout << "Afisare" << endl;
    cout << t << endl;

    Punct* cautat = t["A"];
    if (cautat) cout << "Punct gasit: " << *cautat << endl;
    t.scrieInFisier("fisier.txt");
    cout << "\nTraseul a fost salvatd " << endl;

    return 0;
}