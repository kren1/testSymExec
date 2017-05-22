// RUN: %p/../../build/tool -r %s -- > %t1
// RUN: %clang -xc -c -o %t2 %t1 && grep -V 'main'
#include <stdio.h>
#include <stdbool.h>
int main();
int blarp() {
    printf("hello\n");
    return 0;
}
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
