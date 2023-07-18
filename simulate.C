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

#define EMPTY INT_MAX
#define MARKED (INT_MAX-1)

inline int randomInt(int n) 
{
  double d;
  do { d = genrand(); } while (d == 1.0);
  return int(d*n);
}

int *hashValueTable;
int hash(int x, int m) {return hashValueTable[x] % m; }

/* @brief print hashtable index, hash, and key, and path count in debug mode
 * 
 * @returns void
 */
void printEntry(int i, int x, int m, size_t p) {
  if (x == EMPTY) { cout << "E "; return;}
  if (x == MARKED) { cout << "D," << p << " "; return;}
  cout << i << ","<< ::hash(x, m) <<  "," << x << "," << p << " ";
}


// Notes:
//  A deleted value is marked EMPTY = INT_MAX
class HashTable {
  public:
    int *t;                                         // hash slots
    size_t *path;                                   // non-negative path count over slot i
    int m;                                          // table size
    int full;                                       // number of valid entries
    int empty;                                      // number of empty cells
    
    /* @brief initialises the hashtable
     *
     */
    HashTable(int vm) {
      m = vm;
      full = 0;
      empty = 2*m;
      t = new int[2*m];                             // originally hashtable was 2m w/ no wrap-around
      for (int i = 0;  i < 2*m;  i++) t[i] = EMPTY;
      path = new size_t[2*m];
      for (int i = 0;  i < 2*m;  i++) path[i] = 0;
    }

    ~HashTable() { 
      delete [] t;
      delete [] path;
    }

    /* @brief insert an element with key x
     *
     * @return insertion cost
     */
    int insert(int x) {
      int hx = ::hash(x, m);
      // once search_max is used as an indicator to signal we have searched the whole table
      //  caps search when implementing wrap-around 
      int search_max = m;
      // an element is always inserted in our simulation so it's okay to increment full
      full++;

      int i;
      for (i = hx; search_max > 0 ;i++) {
        search_max--;
        path[i]++;
        if (t[i] == EMPTY) {
          t[i] = x;
          empty--;
          break;
        }
        if (t[i] == MARKED) {
          t[i] = x;
          break;
        }
        // set i to wrap around once we hit the end of the hash table
        if (i == m - 1) {
          i = -1;
        }
      }
      return m - search_max;
    }
    
    /* @brief remove key x from the hashtable
     * 
     * We decrement the number of paths over each hash slot at the same time
     * If path count hit zero over any tombstone, we can remove the tombstones
     *
     * @returns void
     */
    void remove(int x) {
      int i, j;
      int search_max = m;
      int hx = ::hash(x, m);
      int key;
      // we're definitely removing the element in the simulation so it's okay to decrement full
      full--;
  
      for (i = hx; search_max > 0;  i++) {
        path[i]--;
        Assert1(t[i] != EMPTY);
        key = t[i];
        if (key != x && path[i] == 0) {
          Assert1(t[i] == MARKED);
          t[i] = EMPTY;
          empty++;
        }
        if (key == x) {
          if (path[i] != 0) {
            t[i] = MARKED;
          } else {
            t[i] = EMPTY;
            empty++;
          }
          break;
        }
        search_max--;
        // set i to wrap around once we hit the end of the hash table
        if (i == m - 1) i = -1;
      }
    }

     /* @brief finds the key x in the has table
     *
     * @returns 0 if not present or
     *          the number of slots looked through if found
     */
    int find(int x) {
      int hx = ::hash(x, m);
      int searched = m - hx; // only used to calculate cost if wraps around
      int i;
      for (i=hx;  t[i] != EMPTY && i < m;  i++) {
        if (t[i] == x) return i-hx+1; 
      }
      if (i == m) {
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
        printEntry(i, t[i], m, path[i]);
      }  
      cout << endl;
    }
    
    /* @brief Fills a vector to decide insert/delete for the simulation
     * 
     * If index i is 1, then key i is inserted into the hash table
     * If insert and 0, then key i is deleted from the hash table
     *
     * @returns number of keys inserted in the simulation
     */
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

/* @brief check the number of crossings on any path is less than or equal to m (zero indexed)
 *        and that zero crossings exists iff corresponding hash table element is empty
 *
 * @returns void
 */
void validatePath(HashTable& ht, size_t m) {
  for (size_t i = 0;  i < m; i++) {
    Assert(ht.path[i] < m)
    Assert((ht.path[i] > 0 && ht.t[i] != EMPTY) || (ht.path[i] == 0 && ht.t[i] == EMPTY))
  }
}

/* @brief runs the simulation
 *
 */
int main(int argc, char **argv) {
  double start = cpuTime();
  int m = atoi(argv[1]);    // table size
  int w = atoi(argv[2]);    // relative weight for decision aka k in the output
  int Kmax = atoi(argv[3]); // number of deletions, simulation ends at Kmax deletions
  int seed = atoi(argv[4]); // seed
  size_t to_delete = 0;     // the next key to be deleted
  size_t to_insert = 0;     // the next key to be inserted
  sgenrand(seed); 


  HashTable ht(m);
  vector<int> decisionTable;
  size_t total_inserts = ht.fillDecisionTable(&decisionTable, m, Kmax, w);
  size_t total_iter = decisionTable.size();
  cout << "# m = " << m << ", Approx. E[load]=(k/(1+k))=" << (double)w/(1+w) << ", Kmax(Number of Deletions)=" << Kmax << endl;
  // cout << "# total ops: " << total_iter << endl;
  cout << "#m n K freeFraction successful-search unsuccessful-search insertion timeavg-insertion timeavg-load k elapsed-sec" << endl;
  hashValueTable = new int[total_inserts];
  for (int i = 0;  i < total_inserts;  i++) hashValueTable[i]=randomInt(m);
  
  int i,j;
  int K = 1;
  long long int count;
  long long int insert_cost = 0;
  long long int num_inserts = 0;
  long long int timeavg_load = 0;
  
  // calculate how many iterations is 10% of the total iterations
  int iter_decile = (int)(0.1 * total_iter);
  int last_i = 0;
  for (i = 0;  i < total_iter;  i++){
    Debug1(validatePath(ht, m));

    // output statistics when deletions reach a threshold or on the last iteration
    if (to_delete == K || i == total_iter - 1) {      
      Debug2(ht.print());
      cout << m << " " << (double)(to_insert - to_delete)/m << " " << to_delete << " ";

      // fraction of free cells (empty only, does not include tombstones)
      cout << (double)(ht.empty-ht.m)/ht.m << " ";
      
      // avg. successful search time over all keys in the table
      // E[Successful Search Cost]
      count = 0;
      for (j = to_delete;  j < to_insert;  j++) count += ht.find(j); 
      cout << (double)count/(to_insert - to_delete) << " ";
      
      // avg. unsuccessful search time over all slots in the table 
      // E[Unsuccessful Search Cost]
      count = 0;
      for (int k = 0;  k < m;  k++) {
        count++;
        if (ht.t[k] == EMPTY) continue;
        for (j = k + 1;  ht.t[j] != EMPTY;  j++) count++;
        if (j == m) j = 0;
        for (; j < k && ht.t[j] != EMPTY; j++) count++;
      }
      cout << (double)count/m << " ";

      // E[Insert Cost]
      count = 0;
      for (int k = 0;  k < m;  k++) {
        count++;
        if (ht.t[k] == EMPTY || ht.t[k] == MARKED) continue;
        for (j = k + 1;  ht.t[j] != EMPTY && ht.t[j] != MARKED;  j++) count++;
        if (j == m) j = 0;
        for (; j < k && ht.t[j] != EMPTY && ht.t[j] != MARKED; j++) count++;
      }
      cout << (double)count/m << " ";

      // insertion cost amortized over the number of inserts since the last statistic
      // E[Amortized insertion Cost]
      cout << (double)insert_cost/num_inserts << " ";

      // average load over the last 10% of the iterations
      cout << (i == total_iter - 1 ? (double)(timeavg_load)/m/iter_decile : 0) << " ";

      // k and elapsed time
      cout << w << " " << (int)(cpuTime() - start) << endl;// k
      
      // reset variables for the next iteration
      K += 0.1 * Kmax;
      insert_cost = 0;
      num_inserts = 0;
      last_i = i;
    }
    
    // Insert or delete according to the decision array
    if (decisionTable[i] == 1) {
      insert_cost += ht.insert(to_insert);
      num_inserts++;
      to_insert++;
    } else {
      ht.remove(to_delete);
      to_delete++;
    }

    // track time averaged load over the last 1/10 iterations
    if (i > (int)(0.9 * total_iter)) {
      timeavg_load += (to_insert - to_delete);
    }
  }

}
