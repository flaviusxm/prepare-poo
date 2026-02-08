/*

    SUBIECT – GESTIONAREA UNUI MAGAZIN DE POWERBANK-URI


(3p) Se consideră o aplicație pentru gestionarea activității unui magazin
care vinde baterii externe de tip PowerBank. Se vor urmări atribute
precum: capacitate, greutate, număr intrări/ieșiri, culoare etc.

Clasa trebuie să conțină:
    • cel puțin 4 câmpuri private
    • unul dintre câmpuri alocat dinamic
    • constructori, destructor, operator=, constructor de copiere
    • operator << pentru afișare
    • un membru static sau const

(1p) Se vor defini operatorii += și -= care permit:
    • încărcarea powerbank-ului (creșterea capacității)
    • descărcarea powerbank-ului (scăderea capacității)

(1p) Se definește operatorul == care compară două obiecte PowerBank
și returnează true dacă TOATE atributele sunt egale.

(2p) Se exemplifică relația „is-a” prin derivarea unei clase noi
din PowerBank (ex: PowerBankWireless). Se testează în main().

(1p) Se explică conceptele:
    • early binding  – legare statică (metode non-virtuale)
    • late binding   – legare dinamică (metode virtuale)

(1p) Se exemplifică o funcție template în C++.


*/

#include <iostream>
#include <cstring>
using namespace std;
class PowerBank {
private:
    char* culoare;
    int capacitate;
    float greutate;
    int nrPorturi;
    static int nrPowerBank;

public:
    PowerBank() : capacitate(0), greutate(0), nrPorturi(0) {
        culoare = nullptr;
        nrPowerBank++;
    }

    PowerBank(const char* c, int cap, float gr, int porturi)
        : capacitate(cap), greutate(gr), nrPorturi(porturi) {

        culoare = new char[strlen(c) + 1];
        strcpy(culoare, c);
        nrPowerBank++;
    }

    PowerBank(const PowerBank& p)
        : capacitate(p.capacitate), greutate(p.greutate), nrPorturi(p.nrPorturi) {

        culoare = new char[strlen(p.culoare) + 1];
        strcpy(culoare, p.culoare);
        nrPowerBank++;
    }

    PowerBank& operator=(const PowerBank& p) {
        if (this != &p) {
            delete[] culoare;
            capacitate = p.capacitate;
            greutate = p.greutate;
            nrPorturi = p.nrPorturi;

            culoare = new char[strlen(p.culoare) + 1];
            strcpy(culoare, p.culoare);
        }
        return *this;
    }

    ~PowerBank() {
        delete[] culoare;
    }


    PowerBank& operator+=(int mAh) {
        capacitate += mAh;
        return *this;
    }

    PowerBank& operator-=(int mAh) {
        capacitate -= mAh;
        if (capacitate < 0) capacitate = 0;
        return *this;
    }


    bool operator==(const PowerBank& p) const {return capacitate == p.capacitate &&greutate == p.greutate &&nrPorturi == p.nrPorturi &&strcmp(culoare, p.culoare) == 0;}
    friend ostream& operator<<(ostream& out, const PowerBank& p) {out << "PowerBank: " << p.culoare<< ", " << p.capacitate << " mAh"<< ", " << p.greutate << " g"<< ", porturi: " << p.nrPorturi;return out;}

    static int getNrPowerBank() { return nrPowerBank; }
};

int PowerBank::nrPowerBank = 0;


//is-a
class PowerBankWireless : public PowerBank {
private:
    bool incarcareWireless;

public:
    PowerBankWireless(const char* c, int cap, float gr, int porturi, bool wireless)
        : PowerBank(c, cap, gr, porturi), incarcareWireless(wireless) {}

    friend ostream& operator<<(ostream& out, const PowerBankWireless& p) {
        out << (PowerBank&)p << ", wireless: " << (p.incarcareWireless ? "DA" : "NU");
        return out;
    }
};


// template exemplificare
template <class T>
T maxim(T a, T b) {
    return (a > b ? a : b);
}



/*
    early binding:

        - Alegerea funcției care va fi apelată se face LA COMPILARE.
        - Compilatorul știe exact ce funcție să apeleze înainte ca
          programul să pornească.
        - funcții obișnuite (non-virtuale)
        - metode non-virtuale din clase
        - supraincărcarea funcțiilor (function overloading)
        - operatori supraincărcați
        - este MAI RAPID (nu există cost de runtime)
        - NU permite polimorfism real
        - tipul obiectului este determinat la compilare


   late binding:

        - Alegerea funcției care va fi apelată se face LA EXECUȚIE.
        - Programul decide în timp real ce funcție să apeleze în
          funcție de tipul REAL al obiectului, nu de tipul pointerului.
        - metode VIRTUALE
        - clase derivate care suprascriu metode virtuale
        - permite POLIMORFISMUL (comportament diferit pentru obiecte diferite)
        - este puțin mai lent (decizia se face la runtime)
        - tipul obiectului este determinat în timpul execuției


*/



int main() {
    PowerBank p1("Samsung", 10000, 350, 2);
    PowerBank p2("Apple", 14000, 230, 4);
    cout << p1 << "\n";
    cout << p2 << "\n\n";
    p1 += 2000;
    p2 -= 3000;
    cout << "Dupa incarcare/descarcare:\n";
    cout << p1 << "\n";
    cout << p2 << "\n\n";

    cout << "p1 == p2 ? " << (p1 == p2 ? "DA" : "NU") << "\n\n";

    PowerBankWireless pw("Huawei", 12000, 260, 4, true);
    cout << pw << "\n\n";
    cout << "Maxim(5, 9) = " << maxim(5, 9) << "\n";

    return 0;
}
