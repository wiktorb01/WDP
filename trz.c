// Wiktor Biały
#include<stdlib.h>

#include<stdio.h>

#include <stdbool.h>

#include <limits.h>

typedef struct {
    int id; //indeks motelu
    int siec; // siec do ktorej nalezy motel
    int odl; // odleglosc od poczatku autostrady
}
motel;

// przechowuje 3 motele o różnych sieciach
typedef struct {
    motel t[3];
}
candidates;

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void out(motel first) {
    printf("%d %d %d \n", first.id, first.siec, first.odl);
    return;
}

void add(candidates * motels, motel mot) {
    if (motels -> t[0].siec == 0) {
        motels -> t[0] = mot;
        return;
    } else if (motels -> t[1].siec == 0 && motels -> t[0].siec != mot.siec) {
        motels -> t[1] = mot;
        return;
    } else if (motels -> t[1].siec != mot.siec && motels -> t[0].siec != mot.siec) {
        motels -> t[2] = mot;
    }
    return;
}

void update(candidates * motels, motel mot, bool is_highest_index_oldest) {
    bool exists = 1;
    for (int i = 0; i < 3; i++) {
        if (mot.siec == motels -> t[i].siec) {
            motels -> t[i] = mot;
            exists = 0;
        }
    }
    if (exists) {
        int oldest = 0;
        if (!is_highest_index_oldest) {
            oldest = INT_MAX;
            for (int i = 0; i < 3; i++) oldest = min(oldest, motels -> t[i].id);
        } else {
            oldest = 0;
            for (int i = 0; i < 3; i++) oldest = max(oldest, motels -> t[i].id);
        }
        int i = 0;
        while (motels -> t[i].id != oldest) {
            i++;
        }
        motels -> t[i] = mot;
    }
    return;
}

// sprawdza czy dana trojka jest kolejnymi 3 motelami o różnych sieciach
bool check_3(motel first, motel second, motel third) {
    bool czy = 1;
    if (!(first.id < second.id)) czy = 0;
    if (!(second.id < third.id)) czy = 0;
    if (!(first.id < third.id)) czy = 0;
    if (first.siec == second.siec || second.siec == third.siec || first.siec == third.siec) czy = false;
    return czy;
}

int min_odl(motel first, motel second, motel third) {
    return min(second.odl - first.odl, third.odl - second.odl);
}

int max_odl(motel first, motel second, motel third) {
    return max(second.odl - first.odl, third.odl - second.odl);
}

signed main() {
    int n, najbliższa = INT_MAX, najdalsza = 0;
    if (!scanf("%d", & n)) return 1;
    motel * mot = (motel * ) malloc((unsigned) n * sizeof(motel));
    if (!mot) return 1;
    int a = 0, b = 0, c = 0;
    for (int i = 0; i < n; i++) {
        mot[i].id = i;
        if (!scanf("%d", & mot[i].siec)) return 1;
        if (!scanf("%d", & mot[i].odl)) return 1;
        // Sprawdzamy czy są 3 rożne sieci moteli
        if (!a || !b || !c) {
            if (!a) a = mot[i].siec;
            else if (!b && mot[i].siec != a) b = mot[i].siec;
            else if (!c && mot[i].siec != a && mot[i].siec != b) c = mot[i].siec;
        }
    }
    // Jeśli nie ma 3 rożnych sieci 
    if (!a || !b || !c) {
        printf("0 0\n");
        return 0;
    }

    // Najpierw liczę najdalszą trójkę
    // pamiętam 3 pierwsze i 3 ostatnie motele o różnych sieciach
    candidates last_3 = {
        0
    }, first_3 = {
        0
    };
    for (int i = 0; i < n && first_3.t[2].siec == 0; i++) add( & first_3, mot[i]);
    for (int i = n - 1; i >= 0 && last_3.t[2].siec == 0; i--) add( & last_3, mot[i]);

    // szukamy najdalszej trójki
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {

                if (check_3(first_3.t[j], mot[i], last_3.t[k])) {
                    najdalsza = max(najdalsza, min_odl(first_3.t[j], mot[i], last_3.t[k]));
                }
            }
        }
    }

    // Teraz szukamy najbliższej trójki
    // Najpierw dla każdego ineksu znajdziemy 3 poprzednie i 3 następne motele o różnych sieciach
    candidates * prev_3 = malloc((unsigned) n * sizeof(candidates));
    prev_3[0].t[0] = mot[0];
    prev_3[0].t[1] = mot[0];
    prev_3[0].t[2] = mot[0];
    for (int i = 1; i < n; i++) {
        prev_3[i] = prev_3[i - 1];
        update( & prev_3[i], mot[i], 0);
    }

    candidates * next_3 = malloc((unsigned) n * sizeof(candidates));
    next_3[n - 1].t[0] = mot[n - 1];
    next_3[n - 1].t[1] = mot[n - 1];
    next_3[n - 1].t[2] = mot[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        next_3[i] = next_3[i + 1];
        update( & next_3[i], mot[i], 1);
    }

    // Rozważamy najbliższe trójki względem danego indeksu i oraz prev_3[i-1] oraz next_3[i+1]
    for (int i = 1; i < n - 1; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (check_3(prev_3[i - 1].t[j], mot[i], next_3[i + 1].t[k])) {
                    najbliższa = min(najbliższa, max_odl(prev_3[i - 1].t[j], mot[i], next_3[i + 1].t[k]));
                }
            }
        }
    }

    printf("%d %d\n", najbliższa, najdalsza);
    free(mot);
    free(prev_3);
    free(next_3);
}