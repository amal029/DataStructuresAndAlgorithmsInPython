#include <iostream>
#include <vector>
#include <algorithm>

#define SUM 14
#define SIZE 7

using namespace std;

template<class T>
vector<T> mqsort (vector<T> v){
  vector<T> lt, gt, eq;
  eq.push_back(*(v.end()-1));	// This is my pivot point
  for (auto it = v.begin() ; it < v.end()-1; it++) {
    if (*it < *eq.begin())
      lt.push_back(*it);
    else if (*it == *eq.begin())
      eq.push_back(*it);
    else gt.push_back(*it);
  }
  if (lt.size() > 1) lt = mqsort(lt);
  if (gt.size() > 1) gt = mqsort(gt);
  // Concat the whole vector together.
  for (auto it = eq.begin(); it != eq.end(); it++)
    lt.push_back(*it);
  for(auto it = gt.begin(); it != gt.end(); ++it)
    lt.push_back(*it);

  return lt;
}


int main(void)
{
  int sum = SUM;
  int _set[] = {2, 3, 1, 6, 7, 8, 9};
  vector<int> set (_set, _set+SIZE);
  vector<int> ret;


  // First sort out the set.
  set = mqsort(set);
  cout << "sorted, size: " << set.size() << endl;
  // for (vector<int>::iterator it = set.begin(); it != set.end(); it++) {
  //   cout << *it << (it < ret.end() ? "" : ",");
  // }
  // cout << endl;
  // sort(set.begin(), set.end());

  // Now start subtracting the sum from the largest value if sum is not
  // already in the set.
  while (sum > 0 && set.size() > 0) {
    vector<int>::iterator to_add;
    if (*set.end() == sum) to_add = set.end();
    else if ((to_add = find(set.begin(), set.end(), sum)) != set.end()) ;
    else {
      // Find the largest int that when subtracted from sum does not
      // make it less than zero.
      vector<int>::iterator _rit = set.end();
      do
	{
	  if (sum - *_rit > 0) {
	    to_add = _rit;
	    break;
	  }
	  else _rit--;
	  
	} while (_rit != set.begin());
    }
    sum -= *to_add;
    ret.push_back(*to_add);
    set.erase(to_add);
  }


  // This is the output set.
  cout << "{";
  for (auto it=ret.begin(); it != ret.end(); it++)
    cout << *it << (it < ret.end()-1 ? "," : "");
  cout << "}" << endl;

  return 0;
}
