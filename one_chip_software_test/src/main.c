// #include <stdio.h>
// #define int32_t unsigned int
int test() {
  // int a = 7;
  // int b = a + 5;
  // int c = a - 1;
  // int d = b + c;
  // int e = b * c;
  // int f = b / c;
  // int g = b % c;
  // int h = b & 1;
  // int i = b | 1;
  // int j = b ^ 3;
  // int k = b ^ c;
	// k = k << 3;
	// k = k >> 3;
	// k = k << a;
	// k = k >> a;
  // int l = b & c & d;
  // int m = b | c | 1;
  // return a+b+c+d+e+f+g+h+i+j+k+l+m;
  int a = 0b10101010;
  int b = 0b00001100;
  int c;

  __asm__ volatile(
    
    "bsw %0, %1, %2"  // 让编译器决定 %0, %1, %2 对应哪个物理寄存器
    : "=r"(c)         // 输出：n
    : "r"(a),         // 输入：n (数据)
      "r"(b)          // 输入：o (掩码)
    : // **不再需要 clobber list**，因为编译器知道它分配的寄存器的使用情况。
  );

  return c;
}

int main(void) {
    int a ;
    a = test();
    // printf("a is %d\n", a);
  return a;
}
