#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <climits>
#include <cstring>

#define N 32
// Number of bits to represent int
// #define INT_BITS 32
 

using namespace std;

// This is optimal and O(2^n)
void compute_xors(vector<long>& tape){
  vector<unsigned> bb;
  for (auto& x: tape){
    if (x > 0)
      bb.push_back(x);
    else
      bb.erase(find(bb.begin(), bb.end(), abs(x)));

    if (bb.empty())
      cout << 0 << endl;
    else{
      // Now compute the maximum xor.
      unsigned xv = 0, temp = 0; 		// The max xored value.
      unsigned bb_size = bb.size()-1;
      for (unsigned i = 1; i < (unsigned(2 << bb_size)); ++i) {
	bitset<N> indices(i);
	cout << indices << "\n";
	temp = 0;
	for (unsigned count = 0; count <= bb_size; ++count)
	  temp ^= (indices[count]) ? bb[count] : 0;
	cout << temp << "\n";
	// 101101
	xv = (temp > xv) ? temp : xv;
      }

      cout << xv << "\n";
      
    }
  }
}

vector<unsigned> get_indices(bitset<N> lbits, unsigned req_bits){
  bitset<N> flbits = bitset<N>(lbits).flip();
  vector<unsigned> set_bits;
  for (unsigned i=0; i < req_bits; ++i)
    if (flbits[i])
      set_bits.push_back(i);

  return set_bits;
}

unsigned compute_greedy_max(bitset<N> lbits, vector<unsigned> procs, vector<unsigned>& bb,
			    unsigned req_bits){
  vector<unsigned> diff;
  set_difference(bb.begin(), bb.end(), procs.begin(), procs.end(),
		 inserter(diff, diff.begin()));

  for(auto &x: bb)
    cout << "bbs: " << x << "\n";

  for (auto &x: procs)
    cout << "procs: " << x << "\n";

  for (auto &x: diff)
    cout << "diffs:" << x << "\n";

  sort(diff.begin(), diff.end(), greater<unsigned long>());
  // Now just iterate through the diff.
  bitset<N> obits(lbits);
  bitset<N> temp(obits);
  vector<unsigned> to_add;
  bool added = false;
  for (auto &x: bb){
    if((find(procs.begin(), procs.end(), x) != procs.end()))
      continue;
    cout << x << "\n";
    temp = obits;
    cout << "before: " << temp.to_ulong() << endl;
    temp ^= x;
    cout << "after: " << temp.to_ulong() << endl;
    if (temp.to_ulong() > lbits.to_ulong()) {
      lbits = temp;
      cout << "updated lbits to:" << lbits.to_ulong() << "\n";
      if (added){
	// Remove the last one from procs
	cout << "replacing last" << "\n";
	procs[procs.size()-1] = x;
      }
      else {
	cout << "pushing to last:" << x << "\n";
	procs.push_back(x);
	added = true;
      }
      cout << "last procs: " << *(procs.end()-1) << "\n";
    }
  }
  if (lbits == obits) {
    return lbits.to_ulong();
  }
  else {
    cout << "calling again:" << lbits.to_ulong() << "\n";
    return compute_greedy_max(lbits, procs, bb, req_bits);
  }
}

// This is O(n^3), but not optimal
void compute_max_xor(vector<long>& tape){

  vector<unsigned> bb;
  unsigned oc = 0;
  for (auto& x: tape){
    cout << ++oc << ":" << "\n";
    if (x > 0)
      bb.push_back(x);
    else
      bb.erase(find(bb.begin(), bb.end(), abs(x)));

    if (bb.empty())
      cout << 0 << endl;

    // The main computation
    else{
      sort(bb.begin(), bb.end(), greater<unsigned long>()); // first value is maximum
      vector<unsigned int> processed;
      // How many bits we need to represent the largest number.
      bitset<N>temp(bb[0]);
      unsigned i = N-1;
      while (i > 0){
	if (temp[i])
	  break;
	--i;
      }
      unsigned req_bits = i+1;

      cout << "bb: " << " ";
      for (auto &x: bb)
      	cout << x << " ";
      cout << "\n";
      // Computation
      bitset<N> flbits(*bb.begin());
      for (auto it = bb.begin(); it != bb.end(); ++it) {
	bitset<N> lbits (*it);
	cout << "initial lbits: " << lbits.to_ulong() << "\n";
	// Add it to processed.
	// processed.clear();
	processed.push_back(lbits.to_ulong());
	unsigned max_greedy = compute_greedy_max(lbits, processed, bb, req_bits); 
	cout << "mg: " << max_greedy << " " << lbits.to_ulong() << "\n";
	flbits = (max_greedy > flbits.to_ulong()) ? max_greedy : flbits;
	cout << "updated lbits: " << flbits.to_ulong() << "\n";
      }

      // cout << "Ans: " << " ";
      cout << flbits.to_ulong() << "\n";
	
    }
  }
}

// This is O(n)
// Function to return maximum XOR subset in set[]
void maxSubarrayXOR(unsigned *set, unsigned n, unsigned IB) {
  // Initialize index of chosen elements
  int index = 0;
 
  // Traverse through all bits of integer starting from
  // the most significant bit (MSB)
  // for (int i=INT_BITS-1; i>=0; i--)
  for (int i=IB-1; i>=0; i--)
    {
      // Initialize index of maximum element and the maximum element
      unsigned maxInd = index, maxEle = 0;
      for (unsigned j=index; j<n; j++)
	{
	  // If i'th bit of set[j] is set and set[j] is greater
	  // than max so far.
	  if ( (set[j]&(1<<i))!= 0 && set[j]>maxEle )
	    maxEle = set[j], maxInd = j;
	}
 
      // If there was no element with i'th bit set, move to smaller i
      if (maxEle == 0)
	continue;
 
      // Put maximum element with i'th bit set at index 'index'
      swap(set[index], set[maxInd]);
 
      // Update maxInd and increment index
      maxInd = index;
 
      // Do XOR of set[maxIndex] with all numbers having i'th
      // bit as set.
      for (unsigned j=0; j<n; j++)
	{
	  // XOR set[maxInd] those numbers which have the i'th
	  // bit set
	  if ((j!=maxInd) && ((set[j] & (1<<i)) !=0))
	    set[j] = set[j]^set[maxInd];
	}
 
      // Increment index of chosen elements
      index++;
    }
 
  // Final result is XOR of all elements
  int res = 0;
  for (unsigned i=0; i<n; i++)
    res ^= set[i];
  cout << res << "\n";
}

void gaussian(vector<int> &tape, unsigned* bcopy){
  vector<unsigned> bb;
  for (auto& x: tape){
    if (x > 0)
      bb.push_back(x);
    else
      bb.erase(find(bb.begin(), bb.end(), abs(x)));

    if (bb.empty())
      cout << 0 << endl;

    // The main computation
    else{
      sort(bb.begin(), bb.end(), greater<unsigned long>()); // first value is maximum
      bitset<N>temp(bb[0]);
      unsigned i = N-1;
      while (i > 0){
	if (temp[i])
	  break;
	--i;
      }
      unsigned req_bits = i+1;
      // maxSubarrayXOR(bb, bb.size(), req_bits);
      // unsigned* bcopy = (unsigned*)malloc(sizeof(unsigned)*bb.size());
      memcpy(bcopy, &bb[0], sizeof(unsigned)*bb.size());
      maxSubarrayXOR(bcopy, bb.size(), req_bits);
      // free(bcopy);
    }
  }
}
 
void input (void){
  string tests;
  getline(cin, tests);
  unsigned tt = unsigned(stoi(tests));
  if (tt < 1 || tt > 5*unsigned(pow(10.0f, 5.0f))) {
    throw ("Too many or too few input test cases");
    exit(1);
  }
  // string line;
  // const char* ll;
  // getline(cin, line);		// What does this do?
  // ll = line.c_str();
  // char* end;
  // There can be only 6 of these whatever happens.
  unsigned count = 0;
  vector<int> tape;
  int value;
  while (count < tt) {
    cin >> value;
    tape.push_back(value);
    ++count;
  }
  // for(long j = strtol(ll, &end, 10);
  //     (ll != end) && (count < 6);
  //     j = strtol(ll, &end, 10)) {
  //   ll = end;
  //   tape.push_back(j);
  // }

  // Now we can compute the outputs
  // compute_xors(tape);

  // Compute xors greedy, but not optimal.
  // compute_max_xor(tape);

  // O(n) and optimal based on gaussian elimination.
  unsigned* bcopy = (unsigned*) malloc(sizeof(unsigned)*tt);
  gaussian(tape, bcopy);
  free(bcopy);
  // cout << "tutu1" << "\n";
}


int main(void)
{
  input();
  return 0;
}
