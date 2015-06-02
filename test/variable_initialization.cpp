int sum(int a, int b) {
  int res; // error
  res = a + b;
  return res;
}

int main() {
  int a; // error
  a = 10;
  int b = 20;
  int result; // error
  result = sum(a, b);
  for (int i = 0; i < result; i++) {
    int c = sum(a, i);
  }
}
