// autor: Wiktor Biały
// code review: Filip Rogowski
#include<stdlib.h>

#include<stdio.h>

#include <stdbool.h>

const long double margin = 1E-8; // precision error margin

typedef struct {
   long double x, y;
}
point;

typedef struct {
   int k; // k -> pointer to a folded sheet
   point P1, P2;
}
folded_sheet;

typedef struct {
   point center;
   long double radius;
}
circle;

typedef struct {
   point P1, P2;
}
rectangle;

typedef struct {
   char type;
   rectangle rec;
   circle cir;
   folded_sheet fold;
}
Sheet;

void add_rectangle(Sheet * sheet, point P1, point P2) {
   sheet -> type = 'P';
   sheet -> rec.P1 = P1;
   sheet -> rec.P2 = P2;
   sheet -> cir = (circle) {0};
   sheet -> fold = (folded_sheet) {0};
   return;
}

void add_circle(Sheet * sheet, point A, long double radius) {
   sheet -> type = 'K';
   sheet -> cir.center = A;
   sheet -> cir.radius = radius;
   sheet -> rec = (rectangle) {0};
   sheet -> fold = (folded_sheet) {0};
   return;
}

void add_folded_sheet(Sheet * sheet, int id_of_sheet_folded, point P1, point P2) {
   sheet -> type = 'Z';
   sheet -> fold.k = id_of_sheet_folded;
   sheet -> fold.P1 = P1;
   sheet -> fold.P2 = P2;
   sheet -> rec = (rectangle) {0};
   sheet -> cir = (circle) {0};
   return;
}

// point reflection over a fold
point flipped(folded_sheet * fold, point A) {
   long double dx, dy, a, b, px, py;
   dx = fold -> P2.x - fold -> P1.x;
   dy = fold -> P2.y - fold -> P1.y;
   a = (dx * dx - dy * dy) / (dx * dx + dy * dy);
   b = 2 * dx * dy / (dx * dx + dy * dy);
   px = a * (A.x - fold -> P1.x) + b * (A.y - fold -> P1.y) + fold -> P1.x;
   py = b * (A.x - fold -> P1.x) - a * (A.y - fold -> P1.y) + fold -> P1.y;
   return (point) { px, py };
}

// distance squared
long double distance(point A, point B) {
   return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}

long double cross_product(point P1, point P2, point A) {
   return (P2.x - P1.x) * (A.y - P1.y) - (A.x - P1.x) * (P2.y - P1.y);
}

bool in_circle(circle cir, point A) {
   return distance(cir.center, A) <= (cir.radius * cir.radius + margin);
}

bool in_rectangle(rectangle rec, point A) {
   return (A.x <= rec.P2.x + margin && A.x + margin >= rec.P1.x) && (A.y <= rec.P2.y + margin && A.y + margin >= rec.P1.y);
}

int count_layers(Sheet * sheet, int k, point A) {
   if (sheet[k].type == 'P') {
      return in_rectangle(sheet[k].rec, A);
   }
   if (sheet[k].type == 'K') {
      return in_circle(sheet[k].cir, A);
   }
   long double cros_prod = cross_product(sheet[k].fold.P1, sheet[k].fold.P2, A);
   if (cros_prod < -margin) { // when A is on the right side of the fold
      return 0;
   }
   if (cros_prod <= margin && cros_prod >= -margin) { // A is an edge of the fold
      return count_layers(sheet, sheet[k].fold.k, A);
   }
   return count_layers(sheet, sheet[k].fold.k, A) + count_layers(sheet, sheet[k].fold.k, flipped( & sheet[k].fold, A));
}

signed main() {
   int n, q;
   if (scanf("%d%d", & n, & q) != 2) return 1;
   point A, B;
   long double radius = 0;
   int k = 0;
   char c = 0;
   Sheet * sheet = (Sheet * ) malloc((unsigned) n * sizeof(Sheet));
   for (int i = 0; i < n; i++) {
      if (scanf(" %c", & c) != 1) return 1;
      switch (c) {
      case 'P':
         if (scanf("%Lf%Lf%Lf%Lf", & A.x, & A.y, & B.x, & B.y) != 4) return 1;
         add_rectangle( & sheet[i], A, B);
         break;
      case 'K':
         if (scanf("%Lf%Lf%Lf", & A.x, & A.y, & radius) != 3) return 1;
         add_circle( & sheet[i], A, radius);
         break;
      case 'Z':
         if (scanf("%d%Lf%Lf%Lf%Lf", & k, & A.x, & A.y, & B.x, & B.y) != 5) return 1;
         add_folded_sheet( & sheet[i], k - 1, A, B);
         break;
      }
   }
   for (int i = 0; i < q; i++) {
      if (scanf("%d%Lf%Lf", & k, & A.x, & A.y) != 3) return 1;
      int wynik = count_layers(sheet, k - 1, A);
      printf("%d\n", wynik);
   }
   free(sheet);
}