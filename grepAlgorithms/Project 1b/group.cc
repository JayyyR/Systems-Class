//*****************************************************
// CS 303 Algorithms
// Project 1, Part A
// Fall 2012
// Joseph Acosta
//*****************************************************

#include "group.h"
#include <cstdlib>  // include for the function exit()
#include <iostream> // include for the input/output members
#include <iomanip>  // include for input/output formatting
#include <fstream>  // include for the file i/o members

using namespace std;
using std::ifstream;

extern double Random(void);
extern double user_time();
extern double system_time();


//constructor
GroupOfNumbers::GroupOfNumbers(long defaultLimit):_LIMIT(defaultLimit), _total(0){
  if (defaultLimit < default_LIMIT) {
    _LIMIT = default_LIMIT;
  }
  try{
    _group = new long[_LIMIT];
  }
  catch(bad_alloc& ba){
    cerr<< "GroupOfNumbers: bad_alloc" << endl;
    cerr<< "GroupOfNumbers: allocation of size "<< _LIMIT << " failed" << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 1 << endl;
    exit(1); 
  }
}

//copy constructor
GroupOfNumbers::GroupOfNumbers(const GroupOfNumbers & G):_LIMIT(default_LIMIT), _total(0), _group(NULL){
  *this = G;
}

//Protected equalikely method
long GroupOfNumbers::Equilikely(long lower, long upper){ // use a < b
  return (lower + (long) (( upper - lower + 1) * Random()) );
}

//File read constructor
GroupOfNumbers::GroupOfNumbers(const char* fname, long total_requested):_LIMIT(total_requested), _total(total_requested){

  //make sure total_requested is not less than the defualt limit of 10, if it is
  //adjust appropriately
  if (total_requested < default_LIMIT){ 
    _LIMIT = default_LIMIT;
  }	
  
  //try to allocate an array of correct size
  try{
    _group = new long[_LIMIT];
  }
  catch(bad_alloc& ba){
    cerr<< "GroupOfNumbers: bad_alloc" << endl;
    cerr<< "GroupOfNumbers: allocation of size "<< _LIMIT << " failed" << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 1 << endl;
    exit(1); 
  }
  
  ifstream indata; // indata is like cin
  long num; // variable for input value
  
  indata.open(fname); // opens the file
  
  // if file couldn't be opened
  if(!indata) { 
    cerr<< "GroupOfNumbers: "<< fname << ": No such file" << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 2) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 3 << endl;
    exit(3); 
  }
  
  //check if the total requested is greater than number specified in the file
  indata >> num;
  long fileSpecify = num;
  if (num < total_requested){
    cerr<< "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
    cerr<< "GroupOfNumbers: Number requested from file: "<< total_requested << endl;
    cerr<< "GroupOfNumbers: Number reputedly available: " << num << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 4) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 4 << endl;
    exit(4); 
  }
  indata >> num;
  long i;
  for (i=0;!indata.eof(); i++){
    
    if (i<total_requested){
      _group[i] = num;
    }
    indata >> num;
    
  
    //add last number to array if it's still needed
    if (indata.eof() && ((i+1)<total_requested)){
      _group[i+1] = num;
    }
    
  }
  long actualNum = (i+1);
  if (actualNum < total_requested){
    cerr<< "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
    cerr<< "GroupOfNumbers: Number requested from file: "<< total_requested << endl;
    cerr<< "GroupOfNumbers: Number reputedly available: " << fileSpecify << endl;
    cerr<< "GroupOfNumbers: Number actually available: " << actualNum << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 5 << endl;
    exit(5); 
  }
  indata.close();
}

//random generator constructor
GroupOfNumbers::GroupOfNumbers(long lower, long upper, long total_requested):_LIMIT(total_requested), _total(total_requested){
  if (total_requested < default_LIMIT){ //if the total requested is less than the default limit
    _LIMIT = default_LIMIT;		// make sure there are still default limit number of spaces in the array
  }					// but total requested will be the total number of keys no matter what
  
  //Check if lower<=upper
  if (lower > upper){
    cerr<< "GroupOfNumbers: incorrect values for the arguments lower and upper:"<< "(" <<lower<<","<<upper<<")"<< endl;
    cerr<< "GroupOfNumbers: lower is requrired to be less than or equal to upper" << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 9) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 2 << endl;
    exit(2);
  }
  
  try{
    _group = new long[_LIMIT];
  }
  catch(bad_alloc& ba){
    cerr<< "GroupOfNumbers: bad_alloc" << endl;
    cerr<< "GroupOfNumbers: allocation of size "<< _LIMIT << " failed" << endl;
    cerr<< "GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
    cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
    cerr<< "GroupOfNumbers: exit: " << 1 << endl;
    exit(1); 
  }
  long i;
  for (i=0; i< total_requested; i++){
    _group[i] = Equilikely(lower,upper);
  }
}

//destructor
GroupOfNumbers::~GroupOfNumbers(){
  delete[]_group;
}

//accessors
bool GroupOfNumbers::isEmpty() const{
  return (_total == 0);
}

long GroupOfNumbers::total() const{
  return _total;
}

void GroupOfNumbers::displayGroupOfNumbers() const{
  long i;
  cout.setf(ios::right);
  for (i=0; i<_total; i++){
    if ((i%10) == 0){
      cout <<'\n'<<setw(5)<<_group[i];
    }
    else{
      cout<<setw(5)<<_group[i];
    }
  }
  cout<<endl;
}

void GroupOfNumbers::writeGroupOfNumbersTo(const char* fname) const{
  ofstream outdata; // outdata is like cin
  long i; // loop index
  outdata.open(fname); // opens the file
  outdata<< _total;
   for (i=0; i<_total; i++){
    if ((i%10) == 0){
      outdata <<'\n'<<" "<<_group[i];
    }
    else{
      outdata<<" "<<_group[i];
    }
  }
  outdata.close();
  
}

//mutators 
bool GroupOfNumbers::add(const long& newNumber){
  bool test = (_total < _LIMIT);
  if (test){
    _group[_total] = newNumber;
    _total++;
  }
  return test;
}

bool GroupOfNumbers::remove(const long& number){
  bool test;
  long i;
  for (i=0;(i<_total) && (_group[i] != number); i++){
  }
  test = (i<_total);
  if (test){
    _group[i] = _group[--_total];
  }
  return test;
}

GroupOfNumbers & GroupOfNumbers::operator=(const GroupOfNumbers & G){
  if (this != &G){
    delete[]_group;
    try{
      _group = new long[G._LIMIT];
    }
    catch(bad_alloc){
      cerr<<"GroupOfNumbers: bad alloc" << endl;
      cerr<<"GroupOfNumbers: allocation of size "<< G._LIMIT << "failed" << endl;
      cerr<<"GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
      cerr<<"GroupOfNumbers: file: " << __FILE__ << endl;
      cerr<<"GroupOfNumbers: exit: " << 1 << endl;
      exit(1);
    }
    _LIMIT = G._LIMIT;
    long i;
    for (i = 0; i< G._total; i++){
      _group[i] = G._group[i];
    }
    _total = G._total;
  }
  return *this;
}
