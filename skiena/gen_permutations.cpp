#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <array>
using namespace std;

#define SIZE 5


vector<int*> permute (int* set, unsigned size) {
  vector<int*> perms;
  for (unsigned i = 0; i < size; ++i) {
    int* temp = (int*) malloc (size * sizeof(int));

    // This is the starting array.
    for (unsigned j = 0; j < size; j++) {
      if (i == j) temp[0] = set[i];
      else if (j > i) temp[j] = set[j];
      else temp[j+1] = set[j];
    }

    if (size-1 > 3) {
      vector<int*> pperm = permute (temp+1, size-1);
      // append the temp[0] to start of the returned values.
      for (auto it = pperm.begin(); it != pperm.end(); it++) {
	int* ttemp = (int*) malloc (size * sizeof (int));
	ttemp[0] = temp[0];
	for (unsigned y=0; y<size; y++)
	  ttemp[y+1] = (*it)[y];
	free(*it);
	perms.push_back(ttemp);
      }
    }
    else{
      int* rtemp = (int*) malloc (size * sizeof(int));
      for (unsigned k = 0; k < size; k++)
	rtemp[k] = temp[k];

      perms.push_back(rtemp);	// push the first one in

      while (true) {
	for (unsigned j = 1; j < size-1; j++) {
	  // Swap the elements
	  int tt = temp[j];
	  temp[j] = temp[j+1];
	  temp[j+1] = tt;
	  unsigned counter = 0;
	  for (unsigned k = 0; k < size; k++)
	    counter += (rtemp[k] == temp[k]) ? 1 : 0;

	  // Break from the while loop
	  if (counter == size)
	    goto L;

	  int* ttemp = (int*) malloc (size * sizeof (int));
	  for (unsigned k = 0; k < size; k++)
	    ttemp[k] = temp[k];

	  // Now push the copy of the array into the vector.
	  perms.push_back(ttemp);
	}
      }
    L : free(temp);
    }
  }
  return perms;
}

int main(void){
  int set[SIZE];

  // Initialized the array, which will be permuted.
  for (int i = 0; i < SIZE; i++)
    set[i] = i+1;

  // permute
  vector<int*> perms = permute (set, SIZE);

  // Print out the permutations.
  cout << "# of permutations: " << perms.size() << endl;
  for (auto it = perms.begin(); it != perms.end(); it++) {
    cout << "{";
    for (int j=0; j<SIZE; j++) {
      cout << (*it)[j] << (j < SIZE-1 ? "," : "");
    }
    free(*it);
    cout << "}" << endl;
  }

  return 0;
}
