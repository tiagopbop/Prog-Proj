#include <map>
#include <list>
#include <string>
#include <iostream>
#include <climits>

using namespace std;
 
string cat_keys(list<map<string, unsigned>> lst) {
  unsigned smallest = UINT_MAX;
  string smallest_concat;
  // traverse the list of maps
  for (auto m : lst) {
    string this_concat;
    unsigned this_smallest = UINT_MAX;
    // iterate over the map element of the list
    for (auto kv : m) {
      this_concat += kv.first;
      if (kv.second < this_smallest) 
        this_smallest = kv.second;
    }
    if (this_smallest < smallest) {
      smallest = this_smallest;
      smallest_concat = this_concat;
    }
  }
  return smallest_concat;
}