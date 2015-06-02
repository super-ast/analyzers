void do_something(int a) {

}

int main() {
  int a; // error
  for (int i = 0; i < 10; i = i++) {
    do_something(a);
  }

  int b;
  for (int i = 0; i < 10; i = i++) {

  }
  do_something(b);

  int i; // error
  for (i = 0; i < 10; i = i+1) {

  }
  for (i = i+1; i < 20; i = i+1) {
  
  }

  int x; // error
  if (a == 0) {
    x = 10;
  }
  else {
    x = 20;
  }

  int y; 
  if (a == 0) {
    y = 10;
  }
  else {
    y = 20;
  }
  do_something(y);
}
