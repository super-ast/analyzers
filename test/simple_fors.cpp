#include <iostream>
using namespace std;

int main() {
  int x;
  int n = 1; 
  int k = 1;

  /* Accepted fors */
  // Normal for
  for (int i = 0; i < n; ++i);
  for (int i = 0; i < n; i--);
  for (x = 0; x < n; ++x);
  // Incremented by constant
  for (int i = 0; i < n; i += 3);
  for (int i = 0; i < n; i = i + 3);
  for (int i = 0; i < n; i = 3 + i);
  for (int i = 0; i < n; i += k);
  for (int i = 0; i < n; i += k) {
    x += n;
  }
  for (int i = 0; i < n; i += k) {
    x += -k;
  }
  for (int i = 0; i < n; i = i + k);
  for (int i = 0; i < n; i = k + i);

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; j += i);


  /* Incorrect fors */
  // Using complex conditions
  for (int i = 0; i < n && i < n; ++i);
  for (int i = 0; i < n && i >= 0; --i);

  // Post iteration is not init var
  for (int i = 0; i < n; ++x);

  // Init var modified inside for 
  for (int i = 0; i < n; ++i) {
    ++i;
  }
  for (int i = 0; i < n; ++i) {
    i = x;
  }
  for (int i = 0; i < n; ++i) {
    i += 1;
  }
  for (int i = 0;i < n; ++i) {
    cin >> i;
  }

  // Modified by a non constant value
  for (int i = 0; i < n; i += i); 
  for (int i = 0; i < n; i += k) {
    k++;
  }
  for (int i = 0; i < n; i += k) {
    x = k++;
  }

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++i);
}
