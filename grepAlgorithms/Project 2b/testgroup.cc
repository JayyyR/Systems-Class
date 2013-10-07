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
  //double start, finish;
  /*
  //reverse order
  GroupOfNumbers G(250000);
  fstream data;
  data.open("sortedArrayFile");
  for (long i=250001;i>1;){
    data<<--i << "  ";
  }
  data.close();
  
  ifstream indata;
  indata.open("sortedArrayFile");
  long num;
  for (long i = 0; i<250001; i++){
    indata>>num;
    G.add(num);
  }
  *//*
  GroupOfNumbers G(0,250,250);
  G.displayGroupOfNumbers();
  start = user_time() + system_time();
  G.quick_choose_middle();
  finish = user_time() + system_time();
  cout << "\nIt took approximately " << finish - start  << endl;
  //G.displayGroupOfNumbers();*/
  
 
   GroupOfNumbers F(0, 250,250);
   GroupOfNumbers Q=F;
   GroupOfNumbers R=F;
   GroupOfNumbers U = F;
   cout << "after quicksort" << endl;
   F.quicksort();
   F.displayGroupOfNumbers();
   cout<<"after quicksort pivot first" << endl;
   Q.quick_choose_first();
  Q.displayGroupOfNumbers();
  cout << "after quicksort pivot middle" << endl;
   R.quick_choose_middle();
   R.displayGroupOfNumbers();
   cout<<"after quicksort pivot end" <<endl;
   U.quick_choose_last();
   U.displayGroupOfNumbers();
  
}