// RUN: %clang -o %t1 %s
// RUN: %p/../../build/tool -swapBranches %s -- | %clang -xc -o %t2 -
// RUN: %t1 > %t3
// RUN: %t2 > %t4
// RUN: diff %t3 %t4
#include <stdio.h>
#include <stdbool.h>
int main() {
   for( int i = -5; i < 5; i++) {
       if( i < 0) {
          printf("if branch i:%d",i);
        } else {
          printf("else branc i:%d",i);
        }
   } 
   return 0;
}
