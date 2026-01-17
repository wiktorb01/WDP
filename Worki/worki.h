#ifndef WORKI_H
#define WORKI_H


// Należy wypełnić
struct worek {
    // Twój kod zaczyna się tu
    int id;
    int ile;
    worek** self_handle; // własny uchwyt
    worek** handle_of_parent; // uchwyt ojca

    // konstruktor allokujący dynamicznie pamięć na stosie dla wskaźnika do przedmiotu
    // dzięki temu możemy stworzyć uchwyt (worek**) na siebie
    worek(int _id = -1, worek** _handle_of_parent = nullptr) : id(_id), ile(0), handle_of_parent(_handle_of_parent) {
        this->self_handle = new worek*; // dynamicznie allokujemy wskaźnik na dany worek
        *this->self_handle = this; // przypisujemy naszemu wskażnikowi na worek adres danego worka
    };

    // destruktor (potrzebny żeby usunąć dynamicznie allokowane wskaźniki na worki)
    ~worek() {
        if (self_handle) {
            delete self_handle;
            self_handle = nullptr;
        }
    }
    // A kończy tu
};
// Należy wypełnić
struct przedmiot {
    // Twój kod zaczyna się tu
    worek** handle_of_parent; // uchwyt ojca
    // A kończy tu
};


// Nowy przedmiot na biurku
przedmiot *nowy_przedmiot();

// Nowy worek na biurku; otrzymuje kolejny numer, począwszy od 0.
worek *nowy_worek();

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(przedmiot *co, worek *gdzie);

// Wkłada worek co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(worek *co, worek *gdzie);

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
void wyjmij(przedmiot *p);

// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
void wyjmij(worek *w);

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot *p);

// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek *w);

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek *w);

// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
void na_odwrot(worek *w);

// Kończy i zwalnia pamięć
void gotowe();

#endif
