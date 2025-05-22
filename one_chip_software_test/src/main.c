// #include <stdio.h>
#define int32_t unsigned int
int32_t test() {
  int32_t a = 7;
  int32_t b = a + 5;
  int32_t c = a - 1;
  int32_t d = b + c;
  int32_t e = b * c;
  int32_t f = b / c;
  int32_t g = b % c;
  int32_t h = b & 1;
  int32_t i = b | 1;
  int32_t j = b ^ 3;
  int32_t k = b ^ c;
	k = k << 3;
	k = k >> 3;
	k = k << a;
	k = k >> a;
  int32_t l = b & c & d;
  int32_t m = b | c | 1;
  return a+b+c+d+e+f+g+h+i+j+k+l+m;
}

int32_t main(void) {
    int32_t a ;
    a = test();
    // printf("a is %d\n", a);
  return 0;
}
