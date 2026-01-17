#include<bits/stdc++.h>
#include "worki.h"
using namespace std;


int id_counter = 0;
unique_ptr<worek> biurko = std::make_unique<worek>(-1);
forward_list<przedmiot> przedmioty;
forward_list<worek> worki;


przedmiot *nowy_przedmiot()
{
    // tworzymy przedmiot na początku listy i przekazujemy mu uchwyt na biurko
    przedmioty.emplace_front();
    przedmiot* p = &przedmioty.front();
    p->handle_of_parent = biurko->self_handle;
    // update ilości elementów na biurku
    biurko->ile++;

    return p;
}

worek *nowy_worek()
{
    // tworzymy worek na poczatku listy z własnym id i uchwytem na biurko
    worki.emplace_front(id_counter++, biurko->self_handle);

    return &worki.front();
}

void wloz(przedmiot *co, worek *gdzie)
{
    // zmieniamy uchwyt na który wskazuje przedmiot co
    co->handle_of_parent = gdzie->self_handle;
    // zwiększamy licznik przedmiotów w worku gdzie
    gdzie->ile++;
}

void wloz(worek *co, worek *gdzie)
{
    // zmieniamy uchwyt na który wskazuje worek co na uchwyt worka gdzie
    co->handle_of_parent = gdzie->self_handle;
    // zwiększamy licznik przedmiotów w worku gdzie
    gdzie->ile += co->ile;
}

void wyjmij(przedmiot *p)
{
    // zmniejszamy licznik przedmiotów w worku w którym jest przedmiot p
    (*p->handle_of_parent)->ile--;
    // zmieniamy uchwyt na który wskazuje przedmiot p na uchwyt na biurko
    p->handle_of_parent = biurko->self_handle;
}

void wyjmij(worek *w)
{
    // zmniejszamy licznik przedmiotów w worku w którym jest worek w
    (*w->handle_of_parent)->ile -= w->ile;
    // zmieniamy uchwyt na który wskazuje worek w na uchwyt na biurko
    w->handle_of_parent = biurko->self_handle;
}

int w_ktorym_worku(przedmiot *p)
{
    return (*p->handle_of_parent)->id;
}

int w_ktorym_worku(worek *w)
{
    return (*w->handle_of_parent)->id;
}

int ile_przedmiotow(worek *w)
{
    return w->ile;
}

void na_odwrot(worek *w)
{
    // zmieniamy liczbę przedmiotów w worku w
    w->ile = biurko->ile - w->ile;
    // dla biurka i worka w zamieniamy wartości wskaźników uchwytów, wartości uchwytów na siebie
    swap(*w->self_handle, *biurko->self_handle);
    swap(w->self_handle, biurko->self_handle);
    // w->handle_of_parent wskazuje teraz na uchwyt worka w więc poprawiamy to 
    w->handle_of_parent = biurko->self_handle;
}

void gotowe()
{
    biurko.reset();
    worki.clear();
    przedmioty.clear();
}