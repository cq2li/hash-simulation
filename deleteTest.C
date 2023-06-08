// experiment with hashing with linear probing where
// deletion just marks the fields but these
// marker are kept at a minimum
// (c) Peter Sanders 2018
// free for academic use but cite
// the paper
// "Hashing with Linear Probing and Referential Integrity"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <functional>
using namespace std;
#define DEBUGLEVEL 0
#include "util.h"
#include "mt-real.c"

inline int randomInt(int n) 
{
  double d;
  do { d = genrand(); } while (d == 1.0);
  return int(d*n);
}

int *hashValueTable;
int hash(int x, int m) {return hashValueTable[x] % m; }

#define EMPTY INT_MAX
#define MARKED (INT_MAX-1)

void printEntry(int x) {
  if (x == EMPTY) { cout << "E "; return;}
  if (x == MARKED) { cout << "D "; return;}
  cout << x << " ";
}


// linear probing, no wrap around
class HashTable {
  // -1 = empty
  // -2 =
public:
  // a deleted entry has value INT_MAX
  int *t;
  int m; // table size
  int full; // number of valid entries
  int empty; // number of empty cells
  HashTable(int vm) {
    m = vm;
    full = 0;
    empty = 2*m;
    t = new int[2*m];
    for (int i = 0;  i < 2*m;  i++) t[i] = EMPTY;
  }
  ~HashTable() { delete [] t; }

  // insert an element with key x
  void insert(int x) {
    int hx = hash(x, m);
    full++;
    for (int i = hx; ;i++) {
      if (t[i] == EMPTY) {
	t[i] = x;
        empty--;
	return;
      }
      if (t[i] == MARKED) {
	t[i] = x;
	return;
      }
    } 
  }

  void remove(int x) {
    int i,j;
    int hx = hash(x, m);
    full--;
    // find the element
    for (i = hx; ;  i++) {
      if (t[i] == x) break;
      Assert1(t[i] != EMPTY);
    }
    
    t[i] = MARKED; // provisorically mark as deleted

    // look to the right
    int hy = 2*m; // smallest hash function value encountered
    for (j = i+1; t[j] != EMPTY;  j++) {
      if (t[j] != MARKED) {
	int hj = hash(t[j], m);
	if (hj < hy) hy = hj;
      }
    }
    
    // look to the left
    for (j = i;  j >= hx; j--) {
      if (t[j]==MARKED) {
	if (hy>j) {
	  t[j] = EMPTY; // remove unnecessary marks
	  empty++;
	}
      } else {
	int hj = hash(t[j], m);
	if (hj < hy) hy = hj;
      }
    }
  }

  // 0 if not present
  // otherwise # of visited cells
  int find(int x) {
    int hx = hash(x, m);
    for (int i=hx;  t[i] != EMPTY;  i++) {
      if (t[i] == x) return i-hx+1; 
    }  
    return 0;
  }

  void print() {
    int i;
    for (i=0;  i<m;  i++) {
      printEntry(t[i]);
    }  
    while (t[i] != EMPTY) {
      printEntry(t[i]);
      i++;
    }
    cout << endl;
  }
};


void validate(HashTable& ht, int from, int to) {
  int marked, full, empty, i;
  int m=ht.m;
  Assert(ht.full == to-from+1);
  for (i = from;  i <= to; i++) Assert(ht.find(i));

  for(marked=0, i=0;  i<2*m; i++) marked += (ht.t[i]==MARKED);
  Assert(marked+ht.full+ht.empty == 2*m);

  for(full=0, i=0;  i<2*m; i++) full += (ht.t[i]<MARKED);
  Assert(full == ht.full);

  for(empty=0, i=0;  i<2*m; i++) empty += (ht.t[i]==EMPTY);
  Assert(empty == ht.empty);
}

int main(int argc, char **argv) {
  int m = atoi(argv[1]); // table size
  int n = atoi(argv[2]); // number of elements
  int Kmax = atoi(argv[3]); // length of the experiment
  int seed = atoi(argv[4]); // seed
  sgenrand(seed); 

  cout << "#m=" << m << ", n=" << n << ", Kmax=" << Kmax << endl;

  hashValueTable = new int[n+Kmax];
  for (int i = 0;  i < n+Kmax;  i++) hashValueTable[i]=randomInt(m);

  cout << "# m n K freeFraction successfull unsuccessfull" << endl;

  HashTable ht(m);
  Debug2(validate(ht, 1, 0));
  Debug3(cout << "initialized" << endl);
  int i,j;
  for (i = 0; i < n;  i++) {
    ht.insert(i);
    Debug3(ht.print());
    Debug2(validate(ht, 0, i));
  }

  int K = 1;
  long long int count;
  for (;  i < Kmax+n;  i++){
    //ht.print();
    ht.remove(i-n); 
    ht.insert(i); 
    if (i-n == K || i-n == Kmax-1) {
      cout << m << " " << n << " " << i-n << " ";
      cout << (double)(ht.empty-ht.m)/ht.m << " "; // fraction of free cells
      count = 0;
      for (j=i-n;  j<i; j++) count += ht.find(j);
      cout << (double)count/n << " "; // avg. successful search time
      count=0;
      for (int k=0; k<m;  k++) {
	count++;
	for (j=k;  ht.t[j] != EMPTY;  j++) count++;  
      }
      cout <<  (double)count/m << endl; //  avg. failed search time
      K = K*2;
    }
  }
}
