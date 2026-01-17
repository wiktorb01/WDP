#include"zbior_ary.h"

#include<math.h>
#include<stdlib.h>
#include<stdio.h>


static int Q = 0;

int reszta(int a) {
  return (int)(((long long)a % Q + Q) % Q);
}

int max(int a, int b) {
  return a < b ? b : a;
}

int min(int a, int b) {
  return a < b ? a : b;
}

void appendAry(zbior_ary * A, int a, int b) {
  int k = (int) A -> ary++;
  A -> a[k] = a;
  A -> b[k] = b;
  A -> moc += (unsigned)(((long long) b - (long long) a) / (long long)Q) + 1;
  return;
}

/* 
  Zwraca pusty zbior_ary
  @param n wielkosci tablic a[], b[] 
  */
zbior_ary pustyZbiorAry(unsigned n) {
  zbior_ary C;
  C.a = (int * ) malloc( n * sizeof(int));
  C.b = (int * ) malloc( n * sizeof(int));
  C.ary = 0;
  C.moc = 0;
  return C;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b) {
  zbior_ary A;
  A.ary = 1;
  A.a = (int * ) malloc( A.ary * sizeof(int));
  A.a[0] = a;
  Q = q;
  A.b = (int * ) malloc( A.ary * sizeof(int));
  A.b[0] = b;
  A.moc = (unsigned)(((long long) b - (long long) a) / (long long)Q) + 1;
  return A;
}

zbior_ary singleton(int a) {
  return ciag_arytmetyczny(a, Q, a);
}

zbior_ary suma(zbior_ary A, zbior_ary B) {
  zbior_ary C = pustyZbiorAry(A.ary + B.ary);
  unsigned i = 0, j = 0; // i - indeks A, j - indeks B
  int A_a = A.a[0], A_b = A.b[0]; // aktualnie rozwazany podciag A
  while (i < A.ary && j < B.ary) {
    if (reszta(A_a) < reszta(B.a[j]) || (reszta(A_a) == reszta(B.a[j]) && (long long)A_b + Q < B.a[j])) {
      appendAry( & C, A_a, A_b);
      i++;
      if (i < A.ary) {
        A_a = A.a[i];
        A_b = A.b[i];
      }
    } else if (reszta(A_a) > reszta(B.a[j]) || (reszta(A_a) == reszta(B.a[j]) && A_a > (long long)B.b[j] + Q )) {
      appendAry( & C, B.a[j], B.b[j]);
      j++;
    } else /* kiedy dane podzbiory arytmetyczne przecinają się */ {
      // rozszerzam przedział A
      A_a = min(A_a, B.a[j]);
      A_b = max(A_b, B.b[j]);
      j++;
      while (i + 1 < A.ary && reszta(A.a[i + 1]) == reszta(A_a) && A.a[i + 1] <= (long long)A_b + Q ) {
        A_b = max(A.b[i + 1], A_b);
        i++;
      }
    }
  }
  // dołączamy pozostałe podzbiory 
  while (i < A.ary) {
    appendAry( & C, A_a, A_b);
    i++;
    if (i < A.ary) {
      A_a = A.a[i];
      A_b = A.b[i];
    }
  }
  while (j < B.ary) {
    appendAry( & C, B.a[j], B.b[j]);
    j++;
  }
  return C;
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
  zbior_ary C = pustyZbiorAry(A.ary + B.ary);
  unsigned i = 0, j = 0; // i - indeks A, j - indeks B
  while (i < A.ary && j < B.ary) {
    if (reszta(A.a[i]) < reszta(B.a[j]) || (reszta(A.a[i]) == reszta(B.a[j]) && A.b[i] < B.a[j])) {
      i++;
    } else if (reszta(A.a[i]) > reszta(B.a[j]) || (reszta(A.a[i]) == reszta(B.a[j]) && A.a[i] > B.b[j])) {
      j++;
    } else /* jeśli dane podzbiory A i B się przecinaja*/ {
      if (A.b[i] > B.b[j]) {
        appendAry( & C, max(A.a[i], B.a[j]), min(A.b[i], B.b[j]));
        j++;
      } else if (A.b[i] <= B.b[j]) {
        appendAry( & C, max(A.a[i], B.a[j]), min(A.b[i], B.b[j]));
        i++;
      }
    }
  }
  return C;
}

zbior_ary roznica(zbior_ary A, zbior_ary B) {
  zbior_ary C = pustyZbiorAry(A.ary + B.ary);
  unsigned i = 0, j = 0; // i - indeks A, j - indeks B
  if (A.ary == 0) return C;
  int A_a = A.a[0], A_b = A.b[0]; // aktualnie rozwazany podciag A
  while (i < A.ary && j < B.ary) {
    if (reszta(A_a) < reszta(B.a[j]) || (reszta(A_a) == reszta(B.a[j]) && A_b < B.a[j])) {
      appendAry( & C, A_a, A_b);
      i++;
      if (i < A.ary) {
        A_a = A.a[i];
        A_b = A.b[i];
      }
    } else if (reszta(A_a) > reszta(B.a[j]) || (reszta(A_a) == reszta(B.a[j]) && A_a > B.b[j])) {
      j++;
    } else /* jeśli dane podzbiory A i B się przecinaja*/ {
      if (A_a >= B.a[j] && B.b[j] >= A_b) {// B pokrywa A
        i++;
        if (i < A.ary) {
          A_a = A.a[i];
          A_b = A.b[i];
        }
      } else if (B.a[j] > A_a) {
        appendAry( & C, A_a, B.a[j] - Q);
        A_a = B.a[j]; //obcinam poczatek podciagu A
      } else if (A_b > B.b[j]) {
        A_a = B.b[j] + Q;  // wycinam z A podciag B
        j++;
      }
    }
  }
  while (i < A.ary) {
    appendAry( & C, A_a, A_b);
    i++;
    if (i < A.ary) {
      A_a = A.a[i];
      A_b = A.b[i];
    }
  }
  return C;
}

bool nalezy(zbior_ary A, int b) {
  if (A.ary == 0) return false;
  unsigned l = 0, r = A.ary-1;
  while (l < r) {
    unsigned mid = (l + r) / 2;
    if (reszta(A.b[mid]) < reszta(b)) {
      l = mid + 1;
    } else if (reszta(A.b[mid]) > reszta(b)) {
      r = mid;
    } else if (b > A.b[mid]) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }
  if (A.a[l] <= b && A.b[l] >= b && reszta(A.a[l]) == reszta(b)) {
    return true;
  }
  return false;
}

unsigned moc(zbior_ary A) {
  return A.moc;
}

unsigned ary(zbior_ary A) {
  return A.ary;
}
