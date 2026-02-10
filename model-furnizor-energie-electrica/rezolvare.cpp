/*
    ====================== CERINȚĂ SUBIECT OOP ======================

    Se consideră o aplicație pentru gestionarea activității unui furnizor
    de energie electrică alternativă. Se vor urmări atribute specifice precum:
        - nume/denumire client
        - sursă energie (enum)
        - consum lunar efectiv (vector alocat dinamic)
        - consum lunar estimat (vector alocat dinamic)
        - număr contract
        - durată contract (număr luni)
        - preț kWh
        - etc.

    ------------------------------------------------------------------
    (3p) CERINȚE PENTRU CLASA PRINCIPALĂ
    ------------------------------------------------------------------
    - Toate datele membre trebuie să fie private.
    - Clasa trebuie să conțină cel puțin 4 câmpuri, dintre care unul
      obligatoriu alocat dinamic (ex.: consumurile lunare).
    - Trebuie definite:
        * constructor(i)
        * destructor
        * constructor de copiere
        * operator=
        * metode de acces (get/set)
        * operatorul de afișare <<
    - Pentru sursa de energie se folosește un tip enumerativ (enum).
    - Se folosește un membru static sau const (ex.: TVA).

    ------------------------------------------------------------------
    (1p) OPERATORUL FUNCTIE ()
    ------------------------------------------------------------------
    - Se definește operatorul () astfel încât să permită modificarea
      consumului lunar efectiv dintr-o anumită lună.
      Exemplu:
            contract(5, 120.5);  // modifică luna 5 cu valoarea 120.5

    ------------------------------------------------------------------
    (1p) OPERATOR DE CONVERSIE LA double
    ------------------------------------------------------------------
    - Operatorul de conversie la double trebuie să returneze:
        → suma totală a diferențelor lunare dintre consumul efectiv
          și consumul estimat.

    ------------------------------------------------------------------
    (2p) METODE SUPLIMENTARE
    ------------------------------------------------------------------
    1) O metodă care determină luna cu cea mai mică diferență de consum.
       - Se calculează |efectiv[i] - estimat[i]|
       - Se returnează indexul/luna cu diferența minimă.

    2) O metodă care calculează valoarea totală a consumului efectiv.
       - Se însumează toate valorile din vectorul consumEfectiv.

    ------------------------------------------------------------------
    (2p) EXEMPLIFICAREA CONCEPTULUI DE VIRTUALIZARE
    ------------------------------------------------------------------
    - Se definește o clasă abstractă (cu cel puțin o metodă virtuală pură).
    - Clasa principală moștenește această clasă și implementează metoda.

    ------------------------------------------------------------------
    (1p) CONTAINER STL PENTRU CĂUTARE RAPIDĂ DUPĂ NR. CONTRACT
    ------------------------------------------------------------------
    - Se recomandă folosirea unui container cu acces O(1):
            unordered_map<int, ContractEnergetic>
      → permite regăsirea rapidă după numărul contractului.
*/

#include<iostream>
#include<cmath>
using namespace std;
enum SursaEnergie{SOLAR,EOLIAN,HIDRO,FOSIL};
class Contract {
public:
    virtual double totalConsumEfectiv()=0;
    virtual ~Contract() {};
};

class ContractEnergie:public Contract {
private:
    string numeClient;
    SursaEnergie sursa;
    int nrContract;
    int durata;
    double pretKWh;
    static double TVA;
    double * consumEfectiv;
    double * consumEstimat;
public:
    ContractEnergie():numeClient(nullptr), {}
    ContractEnergie(string nume, SursaEnergie s, int nr, int d, double pret) :
    numeClient(nume), sursa(s), nrContract(nr), durata(d), pretKWh(pret)
    { consumEfectiv = new double[durata];
        consumEstimat = new double[durata];
        for (int i = 0; i < durata; i++){ consumEfectiv[i] = 0; consumEstimat[i] = 0; }
    }
    ContractEnergie(const ContractEnergie& c) :
    numeClient(c.numeClient), sursa(c.sursa), nrContract(c.nrContract), durata(c.durata), pretKWh(c.pretKWh)
    { consumEfectiv = new double[durata];
        consumEstimat = new double[durata];
        for (int i = 0; i < durata; i++) { consumEfectiv[i] = c.consumEfectiv[i]; consumEstimat[i] = c.consumEstimat[i]; }
    }
    ContractEnergie& operator=(const ContractEnergie& c) {
        if (this != &c)
            { numeClient = c.numeClient; sursa = c.sursa; nrContract = c.nrContract; durata = c.durata; pretKWh = c.pretKWh;
            delete[] consumEfectiv; delete[] consumEstimat;
            consumEfectiv = new double[durata];
            consumEstimat = new double[durata];
            for (int i = 0; i < durata; i++) { consumEfectiv[i] = c.consumEfectiv[i]; consumEstimat[i] = c.consumEstimat[i]; } }
        return *this;
    }
    ~ContractEnergie() {
        delete[] consumEfectiv;
        delete[] consumEstimat;
    }
    void operator()(int luna,double valoare) {
        if (luna>=0 && luna<durata){consumEfectiv[luna]=valoare;}
    }
    operator double() const {
        double total = 0;
        for (int i = 0; i < durata; i++)
        {       total += abs(consumEfectiv[i] - consumEstimat[i]);}
        return total; }
    int lunaMinimaDiferenta() const {
        double minDif = abs(consumEfectiv[0] - consumEstimat[0]);
        int index = 0; for (int i = 1; i < durata; i++)
            { double dif = abs(consumEfectiv[i] - consumEstimat[i]);
            if (dif < minDif) { minDif = dif; index = i; }
            }
        return index;
    }

    friend ostream& operator<<(ostream& out,const ContractEnergie&c ) {
        out << "Client: " << c.numeClient << "\n";
        out << "Nr contract: " << c.nrContract << "\n";
        out << "Durata: " << c.durata << " luni\n";
        out << "Pret kWh: " << c.pretKWh << "\n";
        out << "Consum efectiv: ";
        for (int i = 0; i < c.durata; i++)
            out << c.consumEfectiv[i] << " ";
        out << "\nConsum estimat: ";
        for (int i = 0; i < c.durata; i++)
            out << c.consumEstimat[i] << " ";
        out << "\n";
        return out;
    }
    int getNrContract()const{return nrContract;}
};

 double ContractEnergie::TVA = 0.19;


