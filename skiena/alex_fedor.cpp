#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;
struct nwwn
{
  unsigned dest;
  unsigned weight;
  unsigned nedges;
};

struct edgeT
{
  unsigned source;
  unsigned dest;
  unsigned weight;
};

typedef vector<nwwn*> aT;
typedef aT* alistT;

unsigned long gcd(unsigned long a, unsigned long b) {
  if (a == 0)
    return b;
  else if (b == 0)
    return a;

  // cout << b << " " << a << " "<< b%a << "\n";
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

#define _new(a) a = mat_new(n)
double** mat_new(unsigned n) {
  double** x = (double**) malloc(sizeof(double*) * n);
  x[0] = (double*) malloc(sizeof(double) * n * n);
 
  for(unsigned i = 0; i< n; ++i)
    x[i] = x[0] + n * i;

  for(unsigned i = 0; i < n; ++i)
    for(unsigned j = 0; j < n; ++j)
      x[i][j] = 0;
  return x;
}
double LUPDeterminant(double **A, int *P, int N) {

  double det = A[0][0];

  for (int i = 1; i < N; i++)
    det *= A[i][i];

  if ((P[N] - N) % 2 == 0)
    return det; 
  else
    return -det;
}

int LUPDecompose(double **A, int N, double Tol, int *P) {

  int i, j, k, imax; 
  double maxA, *ptr, absA;

  for (i = 0; i <= N; i++)
    P[i] = i; //Unit permutation matrix, P[N] initialized with N

  for (i = 0; i < N; i++) {
    maxA = 0.0;
    imax = i;

    for (k = i; k < N; k++)
      if ((absA = fabs(A[k][i])) > maxA) { 
	maxA = absA;
	imax = k;
      }

    if (maxA < Tol) return 0; //failure, matrix is degenerate

    if (imax != i) {
      //pivoting P
      j = P[i];
      P[i] = P[imax];
      P[imax] = j;

      //pivoting rows of A
      ptr = A[i];
      A[i] = A[imax];
      A[imax] = ptr;

      //counting pivots starting from N (for determinant)
      P[N]++;
    }

    for (j = i + 1; j < N; j++) {
      A[j][i] /= A[i][i];

      for (k = i + 1; k < N; k++)
	A[j][k] -= A[j][i] * A[i][k];
    }
  }

  return 1;  //decomposition done 
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

inline bool in_all_edges (unsigned s, unsigned d, vector<edgeT*>& e){
  bool ret = false;
  for(unsigned i = 0; i < e.size(); ++i){
    if (e[i]->source == s && e[i]->dest == d) {
      ret = true;
      break;
    }
  }

  return ret;
}

void debug_alist(alistT *alist, unsigned nodes){
  for(unsigned s = 0; s < nodes; ++s){
    cout << s+1 << ": [" ;
    for (auto it = alist[s]->begin(); it != alist[s]->end(); ++it) {
      cout << "[";
      cout << ((*it)->dest+1) << "," << (*it)->weight << ","
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

unsigned long compute_tmst(alistT* alist, unsigned nodes, vector<edgeT*>& oe){
  unsigned long tmst = 1; 
  typedef vector<unsigned> parentT;
  typedef parentT* pT;
  // This is the cost for each node
  unsigned* costs = (unsigned*) calloc(nodes, sizeof(unsigned));

  // Initialize the costs of the nodes
  // costs[0] = 0;
  for(unsigned i = 1; i < nodes; ++i)
    costs[i] = numeric_limits<unsigned>::max();

  // cout << "start1" << "\n";
  // This is the list of parents for each node
  pT *parents = (pT*) calloc(nodes, sizeof(pT));
  // Initialize the vector.
  for (unsigned i = 0; i < nodes; ++i)
    parents[i] = new parentT();

  // The edge-set
  vector<edgeT*> edges, oedges; // source, dest, weight

  // cout << "start2" << "\n";
  // This is the queue to process.
  vector<unsigned> Q;

  // First push all the nodes into the Q
  for(unsigned i = 0; i < nodes; ++i)
    Q.push_back(i);

  // The processed nodes
  vector<unsigned> procs;
  // Now sort it according to the costs attached to each node.
  auto f = [&costs](unsigned a, unsigned b){return (costs[a] < costs[b]);};
  // cout << "sort size: " << Q.size() <<"\n";
  // unsigned count = 0;
  // for (auto it = Q.begin(); it != Q.end(); ++it) {
  //   cout << "count: " << count << " value: " << *it << ",";
  //   ++count;
  // }
  // cout << "\n";
  std::sort(Q.begin(), Q.end(), f);
  // cout << "B: " << Q[0] << "\n";
  // cout << Q.size() << "\n";
  bool tt = false;
  while (!Q.empty()) {
    unsigned node = Q[0]; 	// Always get the top of the Q
    // The edges in the tree.
    // We will use a canonical representation later on for comparison.
    if(node != 0) {
      unsigned count = 0;
      for (auto it = parents[node]->begin(); it != parents[node]->end(); ++it) {
	edgeT *e = new edgeT();
	// unsigned s = min(node, (*it));
	// unsigned d = max(node, (*it));
	e->source = *it;
	e->dest = node;
	e->weight = costs[node];
	if (count == 0) {
	  edges.push_back(e);
	}
	else if (count > 0) {
	  // These are the other edges that should also lead to a MST.
	  oedges.push_back(e);
	}
	++count;
      }
    }
    // cout << "next node" << "\n";
    procs.push_back(node);
    // Delete the top of the queue
    Q.erase(Q.begin());
    if (!tt)
      // cout << Q.size() << "\n";
    tt = true;
    // cout << "processing: " << node << "\n";
    // cout << "cost: " << costs[node] << "\n";
    // Iterate through all of node's neighbors.
    for (auto it = alist[node]->begin(); it != alist[node]->end(); ++it){
      // If neighbor is in the Q
      unsigned n = (*it)->dest;
      unsigned nw = (*it)->weight;
      // cout << "neigbor: " << n << "weight: " << nw << "\n";
      if ((find(Q.begin(), Q.end(), n)) != Q.end()) {
	// This means the neighbor still needs to be processed.
	if (nw < costs[n]) {
	  // cout << "ncost:" << costs[n] << "\n";
	  // Now is the important point
	  costs[n] = nw;	// This is the new cost.
	  // cout << "new ncost:" << costs[n] << "\n";
	  // Now set the parent
	  // All you need to do is reset the parent vector
	  parents[n]->clear();
	  parents[n]->push_back(node);
	}
	else if (nw == costs[n]){
	  // This means we add to the vector of parents.
	  // cout << "Equal costs!" << costs[n] << "==" << nw << "\n";
	  parents[n]->push_back(node);
	}
      }
    }
    // Re-sort the queue
    sort(Q.begin(), Q.end(), f);
    // cout << "QSIZE: " << Q.size() << "\n";
  }
  // cout << "meme" << "\n";
  // cout << procs.size() << "\n";
  // for (auto it = procs.begin(); it != procs.end(); ++it) {
  //   cout << "procs: " << *it << " ";
  //   cout << "cost: " << costs[*it] << "\n";
  // }

  // TODO: Do the replacement of edges.

  // Debug
  cout << "MST edges" << "\n";
  unsigned mstw = 0;
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    cout << (*it)->source+1 << "--" << (*it)->dest+1 << "w: " << (*it)->weight << "\n";
    mstw += (*it)->weight;
  }
  cout << "Other edges that can lead to a MST size: " << oedges.size() << "\n";
  for (auto it = oedges.begin(); it != oedges.end(); ++it) {
    cout << (*it)->source+1 << "--" << (*it)->dest+1 << "w: " << (*it)->weight << "\n";
  }
  cout << "MST weight: " << mstw << "\n";

  // TODO: Now calculate the edge set difference

  // Now we can calculate the number of MSTs in the graph
  for(unsigned i = 1; i < nodes; ++i) {
    cout << "node:" << i+1 << "parents: " << parents[i]->size() << "\n";
    // tmst *= parents[i]->size();
  }

  // cout << "tmst 1:" << tmst << "\n";

  // Calculate the number of MSTs with parallel edges
  // TODO: Check that this works
  for(unsigned i = 0 ; i < nodes; ++i){
    for (auto it = alist[i]->begin(); it != alist[i]->end(); ++it) {
      tmst *= (*it)->nedges;
    }
  }


  free(costs);
  // Freeing the parents
  for(unsigned i = 0; i < nodes; ++i)
    free(parents[i]);
  free(parents);

  // Freeing the edges
  for (auto it = edges.begin(); it != edges.end(); ++it)
    free(*it);

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
  // cout << "Calculating" << "\n";
  // Construct laplacian matrix
  int* lmatrix = (int*) calloc(nodes*nodes, sizeof(int));
  laplacian(nmatrix, nodes, nodes, lmatrix);
  // debug_matrix(nmatrix, nodes, nodes, "Adj-matrix: \n");
  // debug_matrix(lmatrix, nodes, nodes, "Laplacian-matrix: \n");

  // int *lp = (int*) calloc((nodes-1), sizeof(int*));
  double **lp = mat_new(nodes-1);
  int *P = (int*) calloc(nodes-1, sizeof(int));
  for (unsigned i = 1; i < nodes; ++i) {
    for (unsigned j = 1; j < nodes; ++j) {
      lp[(i-1)][(j-1)] = lmatrix[i*nodes+j];
    }
  }
  // debug_matrix(lp[0], nodes-1, nodes-1, "Laplacian-matrix*: \n");

  LUPDecompose(lp, nodes-1, 0.000000000000001, P);
  // debug_matrix(lp[0], nodes-1, nodes-1, "Decomposed matrix: \n");
  // cout << error << " decomposed" << "\n";
  uint64_t tst = static_cast<uint64_t>(ceil(LUPDeterminant(lp, P, nodes-1)));
  // cout << "tutu" << "\n";
  // First calculate the lp decomposition
  // cout << "TST: " << tst << "\n";
  // cout << sizeof(unsigned long) << " " << sizeof(double) << "\n";	

  // Finally free things
  free(nmatrix);
  free(lmatrix);
  free(lp[0]);
  free(lp);
  free(P);

  // Debug
  // debug_alist(alist, nodes);

  // Make the overall edge set.
  vector<edgeT*> all_edges;
  for(unsigned i = 0; i < nodes; ++ i){
    for (auto it = alist[i]->begin(); it != alist[i]->end(); ++it) {
      unsigned s = min(i, (*it)->dest);
      unsigned d = max(i, (*it)->dest);
      if (!in_all_edges(s, d, all_edges)) {
	edgeT *e = new edgeT();
	e->source = s;
	e->dest = d;
	e->weight = (*it)->weight;
	all_edges.push_back(e);
      }
    }
  }
  // debug edges
  for (auto it = all_edges.begin(); it != all_edges.end(); ++it) {
    cout << (*it)->source+1 << "--" << (*it)->dest+1 << "w: " << (*it)->weight << "\n";
  }

  // Now we can actually compute the prim's path for number of MSTs with
  // keeping tag on ties.
  // cout << "computing" << "\n";
  unsigned long tmst = compute_tmst(alist, nodes, all_edges);
  // cout << "TMST:" << tmst << "\n";

  // This is freeing the adj-list
  for (unsigned s = 0; s < nodes; s++) {
    for (auto it = alist[s]->begin(); it != alist[s]->end(); ++it) free(*it);
    free(alist[s]);
  }
  free(alist);

  // The result.
  unsigned long g = gcd(tmst, (unsigned long)tst);
  cout << tmst/g <<"/"<<tst/g << "\n";
}

int main(void)
{
  make_adg_list();
  return 0;
}

