//
//  Sorting_DSA.cpp
//
//
//  Created by Zheng Liu on 4/25/18.
//

// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2018

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
    public:
    unsigned int val1;
    unsigned int val2;
    char val3;
    string val4;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename.c_str());
    if (!input) {
        cerr << "Error: could not open " << filename << endl;
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->val1 >> pData->val2 >> pData->val3 >> pData->val4;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename.c_str());
    if (!output) {
        cerr << "Error: could not open " << filename << endl;
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << endl;

    // Write the data
    for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
        output << (*ipD)->val1 << " "
        << (*ipD)->val2 << " "
        << (*ipD)->val3 << " "
        << (*ipD)->val4 << endl;
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &, int field);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.  Executing sort..." << endl;

    int field = 0;
    cout << "Enter an integer from 1 - 4, representing field to sort: ";
    try {
        cin >> field;
        if (field < 1 || field > 4) {
            cerr << "Error: invalid field" << endl;
            exit(1);
        }
    }
    catch (...) {
        cerr << "Error: invalid field" << endl;
        exit(1);
    }

    clock_t t1 = clock();
    sortDataList(theList, field);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

//For baseline tests and comp is used in field1Sort
bool comp(const Data* d1, const Data* d2){
    return (d1->val1 < d2->val1);
}

bool comp2(const Data* d1, const Data* d2) {
	return (d1->val2 < d2->val2);
}

bool comp3(const Data* d1, const Data* d2) {
	return (d1->val3 < d2->val3);
}

bool comp4(const Data* d1, const Data* d2) {
	return (d1->val4 < d2->val4);
}


//Simple insertion sort because the data is K-sorted, K=20
void field1Sort(list<Data*> &l){
    list<Data*>::iterator temp1;
    Data *d = new Data();
    d->val1 = 0;
    l.push_front(d);
    for(list<Data*>::iterator ipD2 = next(l.begin(),2);ipD2!=l.end();ipD2++){
        list<Data*>::iterator temp1 = ipD2;
        d = *ipD2;
        for(list<Data*>::iterator ipD1=ipD2;comp(d,*(--ipD1));){
            *temp1 = *ipD1;
            temp1--;
        }
        *temp1 = d;
    }
    l.pop_front();

}

//A simple radix sort with base being 2048(32 to 3 digits). The base is found by experiments.
void field2Sort(list<Data*> &l){
    unsigned int power =1;
    for(int digit =0;digit<3;digit++){

    vector<Data*> buckets[2048];
	for (list<Data*>::const_iterator it = l.begin(); it != l.end(); it++) {
		buckets[((*it)->val2/power)%2048].push_back(*it);
	}
    power *= 2048;
	list<Data*>::iterator it = l.begin();
	for (size_t i = 0; i < 2048; i++){
		size_t size = buckets[i].size();
		for (size_t j = 0; j < size; j++){
			*it = buckets[i].at(j);
			it++;
		}
	}
    }
}

//A simple bucket sort
void field3Sort(list<Data*> &l){
	vector<Data*> buckets[94];
	for (list<Data*>::const_iterator it = l.begin(); it != l.end(); it++) {
		buckets[(int)(*it)->val3-'!'].push_back(*it);
	}
	list<Data*>::iterator it = l.begin();
	for (size_t i = 0; i < 94; i++){
		size_t size = buckets[i].size();
		for (size_t j = 0; j < size; j++){
			*it = buckets[i].at(j);
			it++;
		}
	}
}

/*Field 4 is a string of 25 printables. The algorithm uses recursive MSD radix sort. The subroutine for the first two digits is bucket sort
and for the rest of digits is 3-way quicksort(fast for smaller datasets). The combination is selected by experiments.
*/
void field4Sort(list<Data*> &l, list<Data*>::iterator lo, list<Data*>::iterator hi, int digit) {
    if ( digit < 2) {//by tests, use radix sort for the first two digits is most efficient
        std::vector<Data*> bucket[94];
        for (list<Data *>::iterator it = lo; it != hi; it++){
        bucket[((((*it) -> val4).at(digit) - '!'))].push_back(*it);
        }
        list<Data *>::iterator it = lo;
        for (size_t i = 0; i < 94; i++){
            size_t size = bucket[i].size();
            list<Data *>::iterator left = it;
            for (size_t j = 0; j < size; j++){
                   *it = bucket[i].at(j);
                   it++;
           }
            list<Data *>::iterator right = prev(it);
            if (left !=  next(right) && left != right) {
                field4Sort(l, left, right, digit + 1);
            }
        }//above is the radix sort

    //below is the 3-way quicksort
    }else if (digit < 25){

      char pivot = (*hi) -> val4.at(digit);
      list<Data *>::iterator first = lo;
      list<Data *>::iterator second = lo;
      list<Data *>::iterator third = prev(hi);

      //partition
      while (second != next(third)) {
          Data* cur = *second;
          if ((*second)->val4.at(digit) <  pivot) {
              iter_swap(first++, second++);
          }else if ((*second)->val4.at(digit) > pivot) {
              iter_swap(second, third--);
          }else {
              second++;
          }
      }

      //sort
      iter_swap(hi, second);
      if (first != lo && prev(first) != lo) {
          field4Sort(l, lo, prev(first), digit);
      }
      if (first != second){
          field4Sort(l, first, second, digit + 1);
      }
      if (second != hi && next(second) != hi ) {
          field4Sort(l, next(second), hi, digit);
     }
   }
}


void sortDataList(list<Data *> &l, int field) {
    if (field == 1) {
        field1Sort(l);
    }else if (field == 2){
        field2Sort(l);
    }else if (field == 3) {
        field3Sort(l);
    }else {
        field4Sort(l, l.begin(), l.end(), 0);
    }
}
