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
#include <vector>
#include <numeric>
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
      int hx = ::hash(x, m);
      int search_max = m; // GL: cap search in wrap-around
      full++; // GL: True under stable load
      for (int i = hx; search_max > 0 ;i++) {
        if (t[i] == EMPTY) {
          t[i] = x;
          empty--;
          return;
        }
        if (t[i] == MARKED) {
          t[i] = x;
          return;
        }
        // GL
        search_max--;
        if (i == m - 1) i = -1;
        // GL

      } 
    }

    void remove(int x) {
      int i,j;
      int search_max = m;
      int hx = ::hash(x, m);
      full--;
      // find the element
      for (i = hx; search_max > 0;  i++) {
        if (t[i] == x) break;
        Assert1(t[i] != EMPTY);
        // GL
        search_max--;
        if (i == m - 1) i = -1;
        // GL
      }

      t[i] = MARKED; // provisorically mark as deleted
      int MARK_idx = i; // GL: track slot for x
      
      // look to the right
      /*
      int hy = 2*m; // smallest hash function value encountered GL: this should be fine unchanged
      for (j = i+1; t[j] != EMPTY && max_search > 0;  j++) {
        if (t[j] != MARKED) {
          int hj = ::hash(t[j], m);
          if (hj < hy) hy = hj;
        }
        max_search--;
        if (j == m - 1) j = -1;
      }
      */   
      // GL
      int hy = 2*m; // smallest hash function value encountered GL: this should be fine unchanged
      for (j = i+1; t[j] != EMPTY && j < m; j++) {
        if (t[j] != MARKED) {
          int hj = ::hash(t[j], m);
          if (hj > j) return; //GL: if a hash of a key is after the store, that means we looped all the way around and so no tombstone between hx and mark_idx can be deleted
          if (hj < hy) hy = hj;
        }
      }
      if (j == m) j == 0; //GL: looping around since the termination wasn't due to empty
      for (j = i+1; t[j] != EMPTY && j < MARK_idx; j++) {
        if (t[j] != MARKED) {
          int hj = ::hash(t[j], m);
          if (hj > j && hj <= hx) return; //GL: if a hash of a key is before or on the hash, it wraps so we cannot convert tombstones to empty from [hx,MARK_idx]
          if (hj < hy && hj > j) hy = hj; //GL: we don't care if h <= j since the search path for t[j] doesn't go through [hx, MARK_idx]
        }
      }
      // GL
      // look to the left
      for (j = i;  j >= hx; j--) {
        if (t[j]==MARKED) {
          if (hy>j) {
            t[j] = EMPTY; // remove unnecessary marks
            empty++;
          }
        } else {
          int hj = ::hash(t[j], m);
          if (hj < hy) hy = hj;
        }
      }
    }

    // 0 if not present
    // otherwise # of visited cells
    int find(int x) {
      int hx = ::hash(x, m);
      int searched = 0;
      int i;
      for (i=hx;  t[i] != EMPTY && i < m;  i++) {
        if (t[i] == x) return i-hx+1; 
      }
      if (i == m) {
        searched = i - hx + 1;
        i = 0;
      }
      for (i = 0; t[i] != EMPTY && i < hx; i++) {
        if (t[i] == x) return i+1+searched;
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
    
    // 1 if insert
    // 0 if delete
     
    int fillDecisionTable(vector<int>* table, int m, int num_dels, int w) {
      double fill = 0;
      double load;
      double rand_gen;
      double total_weight;
      int count = 0;
      while (num_dels > 0) {
        load = fill/m;
        total_weight = (load + w*(1 - load));
        rand_gen = genrand();
        Debug2(cout << "iter: " << table.size() << " del_weight: " << load/total_weight << ", rng: " << rand_gen << " ");
        if (rand_gen > load/total_weight) {
          table->push_back(1);
          fill++;
          count++;
          Debug2(cout << "insert" << endl);
        } else {
          table->push_back(0);
          fill--;
          num_dels--;
          Debug2(cout << "delete" << endl);
        }
      }
      return count;
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
  int w = atoi(argv[2]); // relative weight for decision
  int Kmax = atoi(argv[3]); // number of deletions / length of the exp
  int seed = atoi(argv[4]); // seed
  size_t to_delete = 0;
  size_t to_insert = 0;
  sgenrand(seed); 


  HashTable ht(m);
  vector<int> decisionTable;
  size_t total_inserts = ht.fillDecisionTable(&decisionTable, m, Kmax, w);
  size_t total_iter = decisionTable.size();
  cout << "# m=" << m << ", E[load]=(w/(1+w))=" << (double)w/(1+w) << ", Kmax(sim. dels.)=" << Kmax << endl;
  // cout << "# total ops: " << total_iter << endl;
  cout << "#m n K freeFraction successfull unsuccessfull E[Load]" << endl;
  hashValueTable = new int[total_inserts];
  for (int i = 0;  i < total_inserts;  i++) hashValueTable[i]=randomInt(m);
  
  Debug2(validate(ht, 1, 0));
  Debug3(cout << "initialized" << endl);
  int i,j;
  
  int K = 1;
  long long int count;
  for (i = 0;  i < total_iter;  i++){
    //ht.print();
    if (i == K || i == total_iter - 1) {
      cout << m << " " << (double)(to_insert - to_delete)/m << " " << i << " ";
      cout << (double)(ht.empty-ht.m)/ht.m << " "; // fraction of free cells
      count = 0;
      for (j=to_delete; j < to_insert; j++) count += ht.find(j); 
      cout << (double)count/(to_insert - to_delete) << " "; // avg. successful search time
      count=0;
      for (int k=0; k<m;  k++) {
        // count++;
        for (j=k;  ht.t[j] != EMPTY;  j++) count++;
        if (j == m) j = 0;
        for (; j < k && ht.t[j] != EMPTY; j++) count++;
      }
      cout <<  (double)count/m << " "; //  avg. failed search time
      cout <<  (double)w/(1+w) << endl; //  avg. failed search time
      K = K*2;
    }
    if (decisionTable[i] == 1) {
      ht.insert(to_insert);
      to_insert++;
    } else {
      ht.remove(to_delete); //GL: perform operations after summary statistics have been calculated
      to_delete++;
    }
  }

}
