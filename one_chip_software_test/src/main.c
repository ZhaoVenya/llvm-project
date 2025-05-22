// #include <stdio.h>
// #define int32_t unsigned int
int test() {
  int a = 7;
  int b = a + 5;
  int c = a - 1;
  int d = b + c;
  int e = b * c;
  int f = b / c;
  int g = b % c;
  int h = b & 1;
  int i = b | 1;
  int j = b ^ 3;
  int k = b ^ c;
	k = k << 3;
	k = k >> 3;
	k = k << a;
	k = k >> a;
  int l = b & c & d;
  int m = b | c | 1;
  return a+b+c+d+e+f+g+h+i+j+k+l+m;
}

int main(void) {
    int a ;
    a = test();
    // printf("a is %d\n", a);
  return a;
}
