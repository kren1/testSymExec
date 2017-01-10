// RUN: %clang -o %t1 %s
// RUN: %p/../../build/tool -deadCond %s -- | %clang -xc -o %t2 -
// RUN: %t1 > %t3
// RUN: %t2 > %t4
// RUN: diff %t3 %t4
#include <stdio.h>
#include <stdbool.h>
int main() {
   for( int i = -50; i < 50; i++) {
       if( i < 0) {
          i = i -1; 
          i = i + 2;
        } 
        printf("i:%d",i);
   } 
    return 0;
}
