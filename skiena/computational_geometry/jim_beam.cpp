#include <iostream>
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdlib>

#define N 6

using namespace std;

void input(){
  string tests;
  getline(cin, tests);
  unsigned tt = unsigned(stoi(tests));
  if (tt < 1 || tt > 100) {
    throw ("Too many or too few input test cases");
    exit(1);
  }
  string line;
  const char* ll;
  for (unsigned i = 0; i < tt; ++i) {
    getline(cin, line);		// What does this do?
    ll = line.c_str();
    char* end;
    // There can be only 6 of these whatever happens.
    long x1, x2, y1, y2, x4, y4;
    long x3 = 0, y3 = 0;
    unsigned count = 0;
    for(long j = strtol(ll, &end, 10);
	(ll != end) && (count < 6);
	j = strtol(ll, &end, 10)) {
      ll = end;
      switch (count) {
      case 0:
	x1 = j;
	break;
      case 1:
	y1 = j;
	break;
      case 2:
	x2 = j;
	break;
      case 3:
	y2 = j;
	break;
      case 4:
	x4 = j;
	break;
      case 5:
	y4 = j;
	break;
      }
      ++count;
    }
    // First find the denominator of the line:
    float den = ((x1-x2) * (y3-y4)) - ((y1-y2)*(x3-x4));

    // cout << den << "\n";
    if (den == 0) {
      // Get slope of the gun line
      float m = (y4/x4);
      // Now if the x1*m = y1 and x2*m = y2 then NO! else YES!
      // // Is the wall contained inside the field of view?
      if (min(x1, x2) >= min(x3, x4) && max(x2, x1) <= max(x3, x4)
	  && min(y1, y2) >= min(y3, y4) && max(y1, y2) <= max(y3, y4)) {
	if (m*x1 == y1 && m*x2 == y2) {
	  cout << "NO" << "\n";
	}
	else
	  cout << "YES" << "\n";
      }
      else
      	cout << "YES" << "\n";
    }
    else{
      float num1 = (x1*y2-y1*x2)*(x3-x4) -((x1-x2)*(x3*y4-y3*x4));
      float num2 = (x1*y2-y1*x2)*(y3-y4) - ((y1-y2)*(x3*y4-y3*x4));
      float xi = num1/den;
      float yi = num2/den;
      if (xi >= min(x3, x4) && xi <= max(x3, x4) && yi >= min(y3, y4) && yi <= max(y3, y4)
	  && xi >= min(x1, x2) && xi <= max(x1, x2)
	  && yi >= min(y1, y2) && yi <= max(y1, y2)) {
	cout << "NO" << "\n";
      }
      else {
	cout << "YES" << "\n";
      }
    }
  }
}


int main(void)
{
  input();
  return 0;
}

