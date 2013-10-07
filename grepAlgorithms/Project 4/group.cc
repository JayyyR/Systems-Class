//*****************************************************
// CS 303 Algorithms
// Project 4
// Fall 2012
// Joseph Acosta
//*****************************************************

#include "group.h"
#include <cstdlib>  // include for the function exit()
#include <iostream> // include for the input/output members
#include <iomanip>  // include for input/output formatting
#include <fstream>  // include for the file i/o members
#include <vector>

using namespace std;
using std::ifstream;

extern double Random(void);
extern double user_time();
extern double system_time();

//GroupOfNumbers groupToCopy; //create global variable for group to copy

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
  
  for (long i=0; i<total_requested;i++){
    indata>>num;
    if(!indata){
      cerr<< "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
      cerr<< "GroupOfNumbers: Number requested from file: "<< total_requested << endl;
      cerr<< "GroupOfNumbers: Number reputedly available: " << fileSpecify << endl;
      cerr<< "GroupOfNumbers: Number actually available: " << i << endl;
      cerr<< "GroupOfNumbers: line: " << (__LINE__ - 5) << endl;
      cerr<< "GroupOfNumbers: file: " << __FILE__ << endl;
      cerr<< "GroupOfNumbers: exit: " << 5 << endl;
      indata.close();
      exit(5); 
    }
    _group[i]=num;
    
  }
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

bool GroupOfNumbers::sort_select(long k, long & kth_smallest) const{
  //check if the kth number requested is out of bounds or not
  if( k<= 0 || k >= (_total+1))
    return false;
  else{
  
    //create copy to do sort on
  GroupOfNumbers copy = *this;
 
  //sort the list in order
  copy.quicksort();
  kth_smallest = copy._group[k-1];
  //cout<< "The kth smallest number is: " << kth_smallest << endl;
  return true;
  
  }
}

bool GroupOfNumbers::partial_sort_select(long k, long & kth_smallest) const{
  if( k<= 0 || k >= (_total+1))
    return false;
  else{
    
    //create small list of k elements
    GroupOfNumbers kGroup(k);
    for(long i = 0; i<k; i++){
      kGroup.add(_group[i]);
    }   
    //sort those k elements
    kGroup.quicksort();
    
    //do comparisons required for partial sort
    for (long i =k; i<_total; i++){
      
      //if the entry in the outer group is less than the
      //last entry in the sorted group of k, discard that
      //last entry and place the entry from the outer group in the
      //correctly sorted spot
      if (_group[i] < kGroup._group[k-1]){
	int j;
	
	kGroup._group[k-1] = _group[i];
	//cout << "group k-1 is  "<< kGroup._group[k-1] << endl;
	long newEntryPosition = (k-1); 
	long tmp = kGroup._group[k-1];
	  for (j=newEntryPosition;j>0 && tmp<kGroup._group[j-1]; j--){
	    kGroup._group[j]= kGroup._group[j-1];
	  }
	  kGroup._group[j]=tmp;
	}

      } 
    
    kth_smallest = kGroup._group[k-1];
    //cout<< "The kth smallest number is: " << kth_smallest << endl;
    
    return true;
  }
}

bool GroupOfNumbers::min_heap_select(long k, long & kth_smallest) const{
  if( k<= 0 || k >= (_total+1))
    return false;
  else{
    //heapify elements in minimum heap
    GroupOfNumbers groupToCopy = *this;
    for (int i = groupToCopy._total/2; i>=0; i--)
      groupToCopy.percDownMin(i, groupToCopy._total);
    
    //delete min k times to get the kth smallest element
    for (int j = groupToCopy._total-1, l = k-1; l>0; j--,l--){
      swap(groupToCopy._group[0], groupToCopy._group[j]);
      groupToCopy.percDownMin(0, j);
      
    }
    
    kth_smallest = groupToCopy._group[0];
    return true;
  }
}

bool GroupOfNumbers::partial_max_heap_select(long k, long & kth_smallest) const{
  if( k<= 0 || k >= (_total+1))
    return false;
  else{
    
    //heapify first k elements
    GroupOfNumbers copy = *this;
    for (int i = k/2; i>=0; i--)
      copy.percolate_down(i, k);
    
    //compare remaining elements past k in the heapify
    for (long i=k; i<copy._total; i++){
      
      if (copy._group[i] < copy._group[0]){
	
	//replace top of heap with new number
	copy._group[0] = copy._group[i];
	//percolate that new number into correct spot
	copy.percolate_down(0,k);
      } 
    }
    //place top of heap in kth smallest
   kth_smallest = copy._group[0];
   return true;
  }
}

bool GroupOfNumbers::quick_select(long k, long & kth_smallest) const{
  if( k<= 0 || k >= (_total+1))
    return false;
  else{
    //create copy and use quick select algorithm
    GroupOfNumbers copy = *this;
    copy.quickSelectHelp(0,copy._total-1,k);
    
    kth_smallest = copy._group[k-1];
    return true;
  }
}

bool GroupOfNumbers::linear_select(long k, long & kth_smallest) const{
  //for now...
  GroupOfNumbers copy = *this;
  copy.linearSelectHelp(0, _total-1, k);
  kth_smallest = copy._group[k-1];
  return true;
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

//insertion sort
void GroupOfNumbers::insertion_sort(){
  insertion_sort(0, (_total-1));
}

//heapsort
void GroupOfNumbers::heapsort(){
  for (int i = _total/2; i >= 0; i --)
    percolate_down(i, _total);
  for (int j = _total-1; j>0; j--){
    swap(_group[0], _group[j]);
    percolate_down(0,j);
  }
}

//mergesort
void GroupOfNumbers::mergesort(){
  if (_total > 0){
    long tmpArray[_total];
    mergesort(tmpArray, 0,(_total-1));
  }
}

//quicksort
void GroupOfNumbers::quicksort(){
  quicksort(0, (_total-1));
}

//new quicksort methods for testing
void GroupOfNumbers::quick_choose_first(){
  quick_choose_first(0, (_total-1));
}
void GroupOfNumbers::quick_choose_middle(){
  quick_choose_middle(0, (_total-1));
}
void GroupOfNumbers::quick_choose_last(){
  quick_choose_last(0, (_total-1));
}


//protected methods for quicksort with new pivot rules
//first is pivot
void GroupOfNumbers::quick_choose_first(long left, long right){
  if( left + CutOff <= right){
    long pivot = _group[left];
    swap(_group[left], _group[right]);
    
    //Begin partitioning
    long i = left, j = right -1;
    for( ; ; ){
      while( _group[i] < pivot && i < right ) {//traverse the array, but make sure i and j dont run off boundaries
	i++;
      }
      while( pivot < _group[j] && j > left){
	j--;
      }
      if (i<j){
	//cout<< "group[i] in swap is" << _group[i]<< endl;
	// cout<< "group[j] in swap is" << _group[j]<< endl;
	swap( _group[i], _group[j]);
	i++;  //must advance i and j here just incase we are caught in an infinite for loop
	j--;  // advancing them here will ensure our while checks keep traversing thru the array
      }
      else
	break;
    }
    swap( _group[i], _group[right]); //restore pivot
    
    if (i>0){
      quick_choose_first( left, i-1); //sort small elements
    }
    if (i<right){
      quick_choose_first( i+1, right); //sort large elements
    }
  }
  else{ //insertion sort on the subarray
    insertion_sort(left, right);
  }
}

//middle is pivot
void GroupOfNumbers::quick_choose_middle(long left, long right){
  if( left + CutOff <= right){
    long middle = (left+right)/2;
    long pivot = _group[middle];
    swap(_group[middle], _group[right]);
    
    //Begin partitioning
    long i = left, j = right -1;
    for( ; ; ){
      while( _group[i] < pivot && i < right ) {//traverse the array, but make sure i and j dont run off boundaries
	i++;
      }
      while( pivot < _group[j] && j > left){
	j--;
      }
      if (i<j){
	swap( _group[i], _group[j]);
	i++;  //must advance i and j here just incase we are caught in an infinite for loop
	j--;  // advancing them here will ensure our while checks keep traversing thru the array
      }
      else
	break;
    }
    swap( _group[i], _group[right]); //restore pivot
    
    if (i>0){
      quick_choose_middle( left, i-1); //sort small elements
    }
    if (i<right){
      quick_choose_middle( i+1, right); //sort large elements
    }
  }
  else{ //insertion sort on the subarray
    insertion_sort(left, right);
  }
}

//last is pivot
void GroupOfNumbers::quick_choose_last(long left, long right){
  if( left + CutOff <= right){
    long pivot = _group[right];
    
    //Begin partitioning
    long i = left, j = right -1;
    for( ; ; ){
      while( _group[i] < pivot && i < right ) {//traverse the array, but make sure i and j dont run off boundaries
	i++;
      }
      while( pivot < _group[j] && j > left){
	j--;
      }
      if (i<j){
	swap( _group[i], _group[j]);
	i++;  //must advance i and j here just incase we are caught in an infinite for loop
	j--;  // advancing them here will ensure our while checks keep traversing thru the array
      }
      else
	break;
    }
    
    swap( _group[i], _group[right]); //restore pivot
    
    if (i>0){
      quick_choose_last( left, i-1); //sort small elements
    }
    if (i<right){
      quick_choose_last( i+1, right); //sort large elements
    }
  }
  else{ //insertion sort on the subarray
    insertion_sort(left, right);
  }
}

//protected methods for quicksort
long GroupOfNumbers::median_of_3(long left, long right){
  
  long center = (left+right)/2;
  
  if( _group[center] < _group[left])
    swap( _group[left], _group[center]);
  
  if( _group[right] < _group[left])
    swap( _group[left], _group[right]);
  
  if( _group[right] < _group[center])
    swap( _group[center], _group[right]);
  
  //place pivot at position right-1
    swap(_group[center], _group[right-1]);
    return _group[right-1];
}

void GroupOfNumbers::quicksort(long left, long right){
  if( left + CutOff <= right){
    long pivot = median_of_3(left, right);
    
    //Begin partitioning
    long i = left, j = right -1;
    for( ; ; ){
      while( _group[++i] < pivot ) {}
      while( pivot < _group[--j]){}
      if (i<j)
	swap( _group[i], _group[j]);
      else
	break;
    }
    
    swap( _group[i], _group[right-1]); //restore pivot
    
    
    quicksort( left, i-1); //sort small elements
    quicksort( i+1, right); //sort large elements
  }
  else{ //insertion sort on the subarray
    insertion_sort(left, right);
  }
}
//protected methods for mergesort
void GroupOfNumbers::merge(long *temp_group, long leftPos, long rightPos, long rightEnd){
  long leftEnd = rightPos -1;
  long tmpPos = leftPos;
  long numElements = rightEnd - leftPos + 1;
  
  while(leftPos <= leftEnd && rightPos <= rightEnd){
    if (_group[leftPos] <= _group[rightPos])
      temp_group[tmpPos++] = _group[leftPos++];
    else
      temp_group[tmpPos++] = _group[rightPos++];
  }
  while(leftPos <= leftEnd)
    temp_group[tmpPos++] = _group[leftPos++];
  
  while(rightPos<= rightEnd)
    temp_group[tmpPos++] = _group[rightPos++];
  
  for(int i =0; i <numElements; i++, rightEnd--)
    _group[rightEnd] = temp_group[rightEnd];
}

void GroupOfNumbers::mergesort(long *temp_group, long left, long right){
  if(left<right){
    int center = (left + right) / 2;
    mergesort(temp_group, left, center);
    mergesort(temp_group, center + 1, right);
    merge(temp_group, left, center + 1, right);
  }
}

//protected perc down min method for heapsort select
void GroupOfNumbers::percDownMin(long i, long n){
  int child;
  long tmp;
  //GroupOfNumbers groupToCopy = *this;
  for( tmp = _group[i]; left_child(i)<n; i = child){
    
    child = left_child(i);
    if( child != n-1 && _group[child] > _group[child + 1])
      child++;
    if (tmp > _group[child])
      _group[i] = _group[child];
    else
      break;
  }
  _group[i] = tmp;
}

void GroupOfNumbers::quickSelectHelp(long left, long right, long k){
  if (left+CutOff <= right)
  {
    long pivot = median_of_3(left, right);
    
    //begin partitioning
    int i = left, j = right-1;
    for(;;)
    {
      while(_group[++i] < pivot){}
      while(pivot< _group[--j]){}
      if (i < j)
	swap(_group[i], _group[j]);
      else
	break;
    }
    
    swap(_group[i],_group[right-1]); //restore pivot
    
    //Recurse
    if(k<=i)
      quickSelectHelp(left,i-1,k);
    else if(k>i+1)
      quickSelectHelp(i+1,right,k);
  }
  else
    insertion_sort(left, right);
}

void GroupOfNumbers::linearSelectHelp(long left, long right, long k){
  if (left+CutOff <= right)
  {
    //this loop sorts each group of 5
    long start = left;
    long end = left + 4;
    for(long i =1; i<= (right-left+1)/5; i++){
      quicksort(start,end);
      start = start +5;
      end = end + 5;
    }
   
    //put medians of each group of 5 in back of group
    long extras = ((right-left)+1)%5;
    long medIndex = (right+1)-extras-3;
    long whereToSwap = ((right+1)-1);
    long medCount = 0;
    for(long i =1; i<= (right-left+1)/5; i++){
      
      swap(_group[medIndex], _group[whereToSwap]);
      
      medCount++;
      whereToSwap--;
      medIndex = medIndex-5;
    }
 
    linearSelectHelp((whereToSwap+1), (right+1)-1, (medCount+1)/2);

    //set pivot to median of medians which are at the end of the list
    long pivot = _group[(right+1)-((medCount+1)/2)];
    
    //place pivot in the right position
    swap(_group[(right+1)-((medCount+1)/2)], _group[right]);
 
    long i = left, j = right -1;
    for( ; ; ){
      while( _group[i] < pivot && i < right ) {//traverse the array, but make sure i and j dont run off boundaries
	i++;
      }
      while( pivot < _group[j] && j > left){
	j--;
      }
      if (i<j){
	swap( _group[i], _group[j]);
	i++;  //must advance i and j here just incase we are caught in an infinite for loop
	j--;  // advancing them here will ensure our while checks keep traversing thru the array
      }
      else
	break;
    }
    
    swap( _group[i], _group[right]); //restore pivot
    
    //Recurse
    if(k<=i){
      linearSelectHelp(left,i-1,k);}
    else if(k>i+1){
      linearSelectHelp(i+1,right,k);}
    
  }
  else{
    insertion_sort(left, right);
  }
  
  
}

//protected perc down method for heapsort
void GroupOfNumbers::percolate_down(long i, long n){
  int child;
  long tmp;
  
  for( tmp = _group[i]; left_child(i)<n; i = child){
    child = left_child(i);
    if( child != n-1 && _group[child] < _group[child + 1])
      child++;
    if (tmp < _group[child])
      _group[i] = _group[child];
    else
      break;
  }
  _group[i] = tmp;
}
//protected insertion sort
void GroupOfNumbers::insertion_sort(long left, long right){
  int j;
  
  //need to set up algorithm so it can handle input in the middle of an array
  //for quicksort
  for (int p = left+1; p < (right+1); p++){
    long tmp = _group[p];
    for (j=p;j>left && tmp<_group[j-1]; j--){
      _group[j]=_group[j-1];
    }
    _group[j]=tmp;
  }
}
