#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <tuple>

using namespace std;
struct nwwn
{
  unsigned dest;
  unsigned weight;
  unsigned nedges;
};

typedef vector<nwwn*> aT;
typedef aT* alistT;

unsigned long gcd(unsigned long a, unsigned long b) {
  if (a == 0)
    return b;
  else if (b == 0)
    return a;

  if (a < b)
    return gcd(a, b % a);
  else
    return gcd(b, a % b);
}

template <typename T>
void debug_matrix(T* matrix, unsigned rows, unsigned columns, const char* s) {
  // Debug
  cout << s << "[" << "\n";
  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < columns; j++) {
      cout << matrix[i*columns+j] << " ";
    }
    cout << "\n";
  }
  cout << "]" << "\n";
}


double Determinant(int *a,int n) {
  int i,j,j1,j2;
  double det = 0;
  int *m = NULL;

  // debug_matrix(a, n, n, "A: \n");
  if (n < 1) { /* Error */

  } else if (n == 1) { 
    det = a[0];
  } else if (n == 2) {
    det = a[0] * a[3] - a[2] * a[1];
  } else {
    det = 0;
    for (j1=0;j1<n;j1++) { // Column by column
      // Allocated the required smaller array space
      m = (int*) calloc((n-1)*(n-1), sizeof(int));

      // Now fill in this new array with the correct values.
      for (i=1;i<n;i++) {
	j2 = 0;
	for (j=0;j<n;j++) {
	  if (j == j1)
	    continue;
	  m[(i-1)*(n-1)+j2] = a[i*n+j];
	  // m[i-1][j2] = a[i][j];
	  j2++;
	}
      }
      // debug m
      // debug_matrix(m, n-1, n-1, "M: \n");
      det += pow(-1.0,1.0+j1+1.0) * a[0+j1] * Determinant(m,n-1);
      free(m);
    }
  }
  return(det);
}

unsigned get_deg(unsigned i, unsigned* matrix, unsigned columns){
  unsigned ret = 0;
  for (unsigned j = 0; j < columns; ++j) {
    if(matrix[i*columns+j] != 0)
      ret += matrix[i*columns+j];
  }
  return ret;
}

void laplacian(unsigned* matrix, unsigned rows, unsigned columns, int* res){
  if (rows != columns) {
    exit(1);			// It should be square matrix.
  }

  // Now construct the damn thing.
  for (unsigned i = 0; i < rows; ++i) {
    for (unsigned j = 0; j < columns; ++j) {
      if (i == j) {
	// Here we need to know the degree of i
	res[i*columns+j] = get_deg(i, matrix, columns);
      }
      else if ((i != j) && matrix[i*columns+j] != 0)
	res[i*columns+j] = -matrix[i*columns+j];
    }
  }
}

void debug_alist(alistT *alist, unsigned nodes){
  for(unsigned s = 0; s < nodes; ++s){
    cout << s << ": [" ;
    for (auto it = alist[s]->begin(); it != alist[s]->end(); ++it) {
      cout << "[";
      cout << ((*it)->dest) << "," << (*it)->weight << ","
	   << (*it)->nedges; 
      cout << "]";
    }
    cout << "]" << "\n";
  }
}

void update_alist(alistT *alist, unsigned s, unsigned d, unsigned w){
  bool found = false;
  for (auto it = alist[s]->begin(); it != alist[s]->end(); ++it) {
    if ((*it)->dest == d) {
      found = true;
      // Now we can look at the weights.
      if (w == (*it)->weight)
	(*it)->nedges += 1;
      else if(w < (*it)->weight){
	(*it)->weight = w;
	(*it)->nedges = 1;
      }
    }
  }
  if (!found){
    //we need to do stuff! 
    nwwn *n_w = (nwwn*) malloc(sizeof(nwwn));
    n_w->dest = d;
    n_w->weight = w;
    n_w->nedges = 1;
    alist[s]->push_back(n_w);
  }
}

unsigned compute_tmst(alistT* alist, unsigned nodes){
  unsigned tmst = 1; 
  return tmst;
}

void make_adg_list(void){
  unsigned nodes, edges;
  cin >> nodes;
  cin >> edges;
  if (nodes < 1 || nodes > 100)
    exit(1);
  if (edges < 1 || edges > 100)
    exit(1);

  // This is the adjacency matrix.
  unsigned* nmatrix = (unsigned*) calloc(nodes*nodes, sizeof(unsigned));

  alistT *alist = (alistT*) calloc(nodes, sizeof(alistT));
  // Initialize alist
  for (unsigned i = 0; i < nodes; ++i)
    alist[i] = nullptr;

  // Debug
  // debug_matrix(nmatrix, nodes, nodes, "Adj_matrix: ");

  // Now read the input and fill in the matrix.
  unsigned count = 0;
  while (count < edges) {
    unsigned i, j, w;
    cin >> i;
    cin >> j;
    cin >> w;
    // Fill in the matrix
    nmatrix[(i-1)*nodes+(j-1)] += 1;
    nmatrix[(j-1)*nodes+(i-1)] += 1;

    // i -- j connection alist
    --i;
    --j;
    if (alist[i] == nullptr) {
      // we need to initialize the list for this node
      // Now make the tuple
      // cout << "pushing: " << i << " " << j << "\n";
      nwwn *n_w = (nwwn*) malloc(sizeof(nwwn));
      n_w->dest = j;
      n_w->weight = w;
      n_w->nedges = 1;
      // Now make the vector of nodes
      aT *nl = new aT();
      nl->push_back(n_w);
      alist[i] = nl;
    }
    else {
      // This means it already exists and hence, we just need to update
      // it.
      update_alist(alist, i, j, w);
    }
    // The opposite way j -- i
    if (alist[j] == nullptr) {
      // we need to initialize the list for this node
      // First make vector of parallel edges
      // tuple<unsigned, unsigned, unsigned> n_w = make_tuple(i, w, 1);
      // cout << "pushing: " << j << " " << i << "\n";
      nwwn *n_w = (nwwn*) malloc(sizeof(nwwn));
      n_w->dest = i;
      n_w->weight = w;
      n_w->nedges = 1;
      // Now make the vector of nodes
      aT *nl = new aT();
      nl->push_back(n_w);
      alist[j] = nl;
      // cout << "SIZE J-I:" << alist[j]->size() << "\n";
      // cout << (*alist[j])[0]->dest << "\n";
    }
    else {
      // This means it already exists and hence, we just need to update
      // it.
      update_alist(alist, j, i, w);
    }
    // Now the alist
    ++count;
  }

  // Construct laplacian matrix
  int* lmatrix = (int*) calloc(nodes*nodes, sizeof(int));
  laplacian(nmatrix, nodes, nodes, lmatrix);
  // debug_matrix(nmatrix, nodes, nodes, "Adj-matrix: \n");
  // debug_matrix(lmatrix, nodes, nodes, "Laplacian-matrix: \n");
  int* lp = (int*) calloc((nodes-1)*(nodes-1), sizeof(int));
  for (unsigned i = 1; i < nodes; ++i) {
    for (unsigned j = 1; j < nodes; ++j) {
      lp[(i-1)*(nodes-1)+(j-1)] = lmatrix[i*nodes+j];
    }
  }
  // debug_matrix(lp, nodes-1, nodes-1, "Laplacian-matrix*: \n");
  unsigned tst = Determinant(lp, nodes-1);

  // Finally free things
  free(nmatrix);
  free(lmatrix);
  free(lp);

  // Debug
  // debug_alist(alist, nodes);
  // Now we can actually compute the prim's path for number of MSTs with
  // keeping tag on ties.
  unsigned tmst = compute_tmst(alist, nodes);


  // This is freeing the adj-list
  for (unsigned s = 0; s < nodes; s++) {
    for (auto it = alist[s]->begin(); it != alist[s]->end(); ++it) free(*it);
    free(alist[s]);
  }
  free(alist);

  // The result.
  cout << tmst/gcd(tmst, tst) <<"/"<<tst/gcd(tmst, tst) << "\n";
}

int main(void)
{
  make_adg_list();
  return 0;
}

