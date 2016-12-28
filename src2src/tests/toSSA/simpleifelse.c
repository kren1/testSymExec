// RUN: %clang -o %t1 %s
// RUN: %p/../../build/tool -toSSA %s -- | %clang -xc -o %t2 -
// RUN: %t1 > %t3
// RUN: %t2 > %t4
// RUN: diff %t3 %t4
#include <stdio.h>
int main() {
    for(int n = -50; n < 50; n++) {
        int i = n;
        if( i < 0) {
          i = i -1; 
          i = i + 2;
        } else {
          i = 1;
        }
        printf("%d \n", i);
    }
    return 0;
}
