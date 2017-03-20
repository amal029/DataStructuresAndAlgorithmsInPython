#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

#define N 32 			// Change this for a larger set

int main(int argc, char* argv[]) {
  if (argc > 1) {
    unsigned char n = stoi (argv[1]);
    if (n > 31 || n <= 0)
      cerr << "Cannot handle sets with size > 31 || <= 0" << endl;
    else {
      vector<bitset<N>> aset;
      // Do the compuation here
      unsigned int d = (2 << (n-1));
      for (unsigned int i = 1; i < d; i++) {
	bitset<N> set(i);
	aset.push_back(set);
      }

      // Print
      cout << "{{},";
      for (auto it = aset.begin(); it != aset.end(); it++) {
	cout << "{";
	for (int j = n-1; j >= 0; j--)
	  cout << (*it)[j] << (j > 0 ? "," : "");
	cout << "}" << (it < aset.end()-1 ? "," : "");
      }
      cout << "}" << endl;
      
    }
  }
  return 0;
}
