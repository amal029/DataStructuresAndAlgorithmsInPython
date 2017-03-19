#include <iostream>
#include <vector>
using namespace std;

#define SIZE 10

int main(void){
  int set[SIZE];

  // Initialized the array, which will be permuted.
  for (int i = 0; i < SIZE; i++)
    set[i] = i+1;


  vector<int*> perms;

  for (unsigned i = 0; i < SIZE; ++i) {
    int* temp = (int*) malloc (SIZE * sizeof(int));
    // This is the starting array.
    for (unsigned j = 0; j < SIZE; j++) {
      if (i == j) temp[0] = set[i];
      else if (j > i) temp[j] = set[j];
      else temp[j+1] = set[j];
    }

    int* rtemp = (int*) malloc (SIZE * sizeof(int));
    for (unsigned k = 0; k < SIZE; k++)
      rtemp[k] = temp[k];

    perms.push_back(rtemp);	// push the first one in

    while (true) {
      for (int j = 1; j < SIZE-1; j++) {
	// Swap the elements
	int tt = temp[j];
	temp[j] = temp[j+1];
	temp[j+1] = tt;
	unsigned counter = 0;
	for (unsigned k = 0; k < SIZE; k++)
	  counter += (rtemp[k] == temp[k]) ? 1 : 0;

	// Break from the while loop
	if (counter == SIZE)
	  goto L;

	int* ttemp = (int*) malloc (SIZE * sizeof (int));
	for (unsigned k = 0; k < SIZE; k++)
	  ttemp[k] = temp[k];

	// Now push the copy of the array into the vector.
	perms.push_back(ttemp);
      }
    }

  L : free(temp);
  }

  // Print out the permutations.
  cout << "# of permuations: " << perms.size() << endl;
  for (auto it = perms.begin(); it != perms.end(); it++) {
    cout << "{";
    for (int j=0; j<SIZE; j++) {
      cout << (*it)[j] << (j < SIZE-1 ? "," : "");
    }
    cout << "}" << endl;
  }

  return 0;
}
