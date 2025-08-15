// #include "stdio.h"

// #define MAX_SIZE 20

// int a_set[MAX_SIZE];

// int get_element(int index) {
//     return (MAX_SIZE - index);
// }

int add(int a, int b){
    int c;
    c = a + b;
    return c;
}

int main(){
    int a = 2;
    int b,c;

    b = 1;
    a = a + b;

    c = add(a,b);

    return c;
}
