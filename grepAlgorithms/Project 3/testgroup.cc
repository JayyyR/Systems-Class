#include "group.h"
#include <cstdlib>  // include for the function exit()
#include <iostream> // include for the input/output members
#include <iomanip>  // include for input/output formatting
#include <fstream>  // include for the file i/o members

using namespace std;
using std::ifstream;
extern double user_time();
extern double system_time();

int main(){
  double start, finish;
  

  GroupOfNumbers G(-10,100,50);
  GroupOfNumbers A= G;
  G.displayGroupOfNumbers();
  A.quicksort();
  A.displayGroupOfNumbers();
  //G.displayGroupOfNumbers();
  long small;
  long x= 16;
  start = user_time() + system_time();
  G.sort_select(x, small);
  finish = user_time() + system_time();
  cout<< "The kth smallest number is (sort select): " << small << endl;
  cout << "\nIt took approximately " << finish - start  << endl;
  
  long small2;
  start = user_time() + system_time();
  G.partial_sort_select(x, small2);
  finish = user_time() + system_time();
  cout<< "The kth smallest number is (partial sort select): " << small2 << endl;
  cout << "\nIt took approximately " << finish - start  << endl;
  
  long small3;
  start = user_time() + system_time();
  G.min_heap_select(x, small3);
  finish = user_time() + system_time();
  cout<< "The kth smallest number is (min heap select): " << small3 << endl;
  cout << "\nIt took approximately " << finish - start  << endl;
  
  long small4;
  start = user_time() + system_time();
  G.partial_max_heap_select(x, small4);
  finish = user_time() + system_time();
  cout<< "The kth smallest number is (partial max heap select): " << small4 << endl;
  cout << "\nIt took approximately " << finish - start  << endl;
  
  long small5;
  start = user_time() + system_time();
  G.quick_select(x, small5);
  finish = user_time() + system_time();
  cout<< "The kth smallest number is (quick select): " << small5 << endl;
  cout << "\nIt took approximately " << finish - start  << endl;
 
 
 
 
 /*
  GroupOfNumbers F(0,40,40);
  F.displayGroupOfNumbers();
  F.heapsort();
  F.displayGroupOfNumbers();*/
}