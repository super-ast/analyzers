void do_something(int a) {

}

int main() {
  int a = 1; // error
  a = 2; // error
  int b = 0;
  a = b * 2;
  do_something(a);
  for (int i = 0; i < 10; i++) {
    a = i; // error
  }
  for (int i = 0; i < 10; i++) {
    a = i;
    do_something(a);
  }
}
