#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

// ==========================================

class Locuinta {
protected:
    char* numeClient;
    int suprafataUtila;
    float discount;

public:
    Locuinta() : numeClient(nullptr), suprafataUtila(0), discount(0) {}

    Locuinta(const char* nume, int suprafata, float disc)
        : suprafataUtila(suprafata), discount(disc) {
        if (nume) {numeClient = new char[strlen(nume) + 1];strcpy(numeClient, nume);} else{ numeClient = nullptr;}
    }

    Locuinta(const Locuinta& l)
        : suprafataUtila(l.suprafataUtila), discount(l.discount) {
        if (l.numeClient) {
            numeClient = new char[strlen(l.numeClient) + 1];
            strcpy(numeClient, l.numeClient);
        } else numeClient = nullptr;
    }

    Locuinta& operator=(const Locuinta& l) {
        if (this != &l) {
            delete[] numeClient;
            suprafataUtila = l.suprafataUtila;
            discount = l.discount;
            if (l.numeClient) {
                numeClient = new char[strlen(l.numeClient) + 1];
                strcpy(numeClient, l.numeClient);
            } else numeClient = nullptr;
        }
        return *this;
    }

    virtual ~Locuinta() {delete[] numeClient;}
    operator int() const {return suprafataUtila;}
    Locuinta& operator++() {
        discount += 1;
        if (discount > 10) discount = 10;
        return *this;
    }
    Locuinta operator++(int) {
        Locuinta temp = *this;
        discount += 1;
        if (discount > 10) discount = 10;
        return temp;
    }
    Locuinta& operator--() {
        discount -= 1;
        if (discount < 0) discount = 0;
        return *this;
    }
    Locuinta operator--(int) {
        Locuinta temp = *this;
        discount -= 1;
        if (discount < 0) discount = 0;
        return temp;
    }
    bool esteInchiriata() const {return numeClient != nullptr;}
    const char* getClient() const {return numeClient;}
    virtual float CalculChirie(int X, int Y) = 0;
};

// =========================================

class Apartament : public Locuinta {
    int etaj;

public:
    Apartament() : Locuinta(), etaj(0) {}

    Apartament(const char* nume, int suprafata, float disc, int etaj)
        : Locuinta(nume, suprafata, disc), etaj(etaj) {}

    Apartament(const Apartament& a) : Locuinta(a), etaj(a.etaj) {}

    float CalculChirie(int X, int Y) override {
        return X * suprafataUtila * (1 - Y * discount / 100.0f);
    }
};

// ========================================

class Casa : public Locuinta {
    int suprafataCurte;
    int nrEtaje;
    float* suprafataEtaje;

public:
    Casa() : Locuinta(), suprafataCurte(0), nrEtaje(0), suprafataEtaje(nullptr) {}

    Casa(const char* nume, int suprafata, float disc,
         int supCurte, int nrEtaje, float* suprafete)
        : Locuinta(nume, suprafata, disc),
          suprafataCurte(supCurte), nrEtaje(nrEtaje) {

        suprafataEtaje = new float[nrEtaje];
        for (int i = 0; i < nrEtaje; i++)
            suprafataEtaje[i] = suprafete[i];
    }

    Casa(const Casa& c) : Locuinta(c),
        suprafataCurte(c.suprafataCurte), nrEtaje(c.nrEtaje) {

        suprafataEtaje = new float[nrEtaje];
        for (int i = 0; i < nrEtaje; i++)
            suprafataEtaje[i] = c.suprafataEtaje[i];
    }

    Casa& operator=(const Casa& c) {
        if (this != &c) {
            Locuinta::operator=(c);
            delete[] suprafataEtaje;
            suprafataCurte = c.suprafataCurte;
            nrEtaje = c.nrEtaje;
            suprafataEtaje = new float[nrEtaje];
            for (int i = 0; i < nrEtaje; i++)
                suprafataEtaje[i] = c.suprafataEtaje[i];
        }
        return *this;
    }

    ~Casa() {
        delete[] suprafataEtaje;
    }

    float CalculChirie(int X, int Y) override {
        return X * (suprafataUtila + 0.2f * suprafataCurte) *
               (1 - Y * discount / 100.0f);
    }
};

// ===================== CLASA AGENTIE IMOBILIARA =====================

class AgentieImobiliara {
    vector<Locuinta*> locuinte;

public:
    AgentieImobiliara() {}

    AgentieImobiliara& operator+=(Locuinta* l) {
        locuinte.push_back(l);
        return *this;
    }

    AgentieImobiliara& operator-=(Locuinta* l) {
        for (auto it = locuinte.begin(); it != locuinte.end(); ++it) {
            if (*it == l) {
                locuinte.erase(it);
                break;
            }
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const AgentieImobiliara& a) {
        out << "Clienti pentru locuintele inchiriate:\n";
        for (const auto l : a.locuinte) {
            if (l->esteInchiriata())
                out << l->getClient() << "\n";
        }
        return out;
    }

    vector<Locuinta*>& getLista() { return locuinte; }
};

// =========================================

template <class T>
class Gestiune {
    vector<pair<T, float>> lista;

public:
    void adauga(T obj, float chirie) {
        lista.push_back({obj, chirie});
    }
};

template <>
class Gestiune<char*> {
    vector<string> clienti;

public:
    void adauga(char* nume) {
        clienti.push_back(nume);
    }
};


int main() {
    AgentieImobiliara agentie;

    Apartament* a1 = new Apartament("Michael", 50, 5, 3);
    Apartament* a2 = new Apartament(nullptr, 40, 2, 1);

    float suprafete[] = {30, 40};
    Casa* c1 = new Casa("Johnsonn", 100, 10, 200, 2, suprafete);
    Casa* c2 = new Casa(nullptr, 80, 3, 150, 2, suprafete);

    agentie += a1;
    agentie += a2;
    agentie += c1;
    agentie += c2;

    cout << agentie;

    float sumaInchiriate = 0, sumaNeinchiriate = 0;

    for (const auto it = agentie.getLista().begin(); it != agentie.getLista().end(); ++it) {
        Locuinta* l = *it;
        float chirie = l->CalculChirie(10, 1);

        if (l->esteInchiriata())
            sumaInchiriate += chirie;
        else
            sumaNeinchiriate += chirie;
    }

    cout << "\nSuma chirii locuinte inchiriate: " << sumaInchiriate;
    cout << "\nSuma chirii locuinte neinchiriate: " << sumaNeinchiriate << "\n";

    Gestiune<Apartament*> gestApart;
    gestApart.adauga(a1, a1->CalculChirie(10, 1));

    Gestiune<char*> gestClienti;
    char numeTest[] = "Test";
    gestClienti.adauga(numeTest);

    delete a1;delete a2;delete c1;delete c2;
    return 0;
}
