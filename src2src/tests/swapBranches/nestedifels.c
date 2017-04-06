// RUN: %clang -o %t1 %s
// RUN: %p/../../build/tool -swapBranches %s -- > %t5
// RUN: %clang -xc -o %t2 %t5
// RUN: %t1 > %t3
// RUN: %t2 > %t4
// RUN: diff %t3 %t4 && bash -c ' diff %s %t5 | grep "i < 0"'
#include <stdio.h>
#include <stdbool.h>
int main() {
   for( int i = -5; i < 5; i++) {
       if( i < 0) {
          printf("if branch #1 i:%d",i);
        } else {

          printf("else branc #1 i:%d",i);

          if( i > 2) {
            printf("if branch #2 i:%d",i);
          } else {
            printf("else branc #2 i:%d",i);
          }

        }
   } 
   return 0;
}
