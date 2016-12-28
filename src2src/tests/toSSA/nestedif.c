// RUN: %clang -o %t1 %s
// RUN: %p/../../build/tool -toSSA %s -- | %clang -xc -o %t2 -
// RUN: %t1 > %t3
// RUN: %t2 > %t4
// RUN: diff %t3 %t4
#include <stdio.h>
int main() {
    for(int n = -50; n < 50; n++) {
        int i = n;
        int x = n + 12;
        if( i < 0) {
          i = i - 1; 
          i = i + 2;
          x = i - 2;

          if(x < 10) {
            i = 1;
          }


        } 
        printf("%d \n", i);
    }
    return 0;
}
