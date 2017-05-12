#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
char __klee__instr_filename;
unsigned b;
int safe_add_func_int32_t_s_s_si2;
void(a)(p1) { p1 ?: p1 + safe_add_func_int32_t_s_s_si2; }
int c(p1, p2) { return p1 % p2; }
short f;
void k(p1) { (5535 >> p1) ?: f; }
int h = 1;
short g;
unsigned *m = &h;
int main___trans_tmp_6;
int i;
unsigned short j;
int main() {
  short d;
  klee_make_symbolic(&h, 4,  "h");
  klee_make_symbolic(&g, 2,  "g");
  main___trans_tmp_6 = g;
  {
    int e = main___trans_tmp_6;
    d = e;
  }
  j = d;
  a(j > *m);
  i = (47 % (*m));
  k(i);
  printf("checksum = %X\n", b);
  int min_int = -2147483648;
  long ub = 2147483647;
  long lb = min_int; 
  long prev = ub + 10;

  printf("#0 lb: %ld, ub: %ld\n", lb, ub);
  
  char test_case_id[20];
  snprintf(&test_case_id, 20, "%d-", rand());
  printf("test: %s", test_case_id);

  while(prev - ub > 0) {
//  printf("\t#a lb: %ld, ub: %ld, prev: %ld\n", lb, ub, prev);
    if(h < ub){
       prev = ub;
       ub = ub - (ub - lb) / 2;
    } 
    //at this point ub is smaller than h can be, and prev is ub that can still shrink
    else {
      lb = ub;
      ub = prev;
      prev = ub + 2;
    }
//  printf("\t#b lb: %ld, ub: %ld, prev: %ld\n", lb, ub, prev);

  }

//  printf("#1 lb: %ld, ub: %ld\n", lb, ub);
  //searching the lowest lb
  long ubForlb = ub;
  lb = min_int;
  prev = lb - 10;
  while(lb - prev > 0) {
    if(h >= lb) {
        prev = lb;
        lb = lb + (ubForlb - lb) / 2;
    } else {
       ubForlb = lb;
       lb = prev; 
       prev = lb - 2;
    }
  }
//  printf("#2 lb: %ld, ub: %ld\n", lb, ub);
  FILE *fp = fopen("locks.file", "rb");

//  printf("fp: %p\n", fp);
  if(fp != NULL){
  
  /*char line[20];
  while(getline(&line, 20, fp) != -1) {
    printf("read line %s", line);
    if(strncmp(line,test_case_id,20) == 0) {
        fclose(fp);
        klee_silent_exit(0);
    }
  }*/
 // char fil[201];
//  for(char*p = fil; p < fil+200;p++) p = 0;
  char *fil= calloc(201,1);
  ssize_t rd = fread(fil, 1, 200, fp);
  int berr = errno;
//  printf("fread %d eerrno %d\n", rd, berr);
  if(rd > 0) {
 // printf("data %s \n", fil);
  if(strstr(fil,test_case_id) != NULL) {
    fclose(fp);
    klee_silent_exit(0);
  }
}
  fclose(fp);
}

  printf("#3 lb: %ld, ub: %ld\n", lb, ub);
  printf("GOOD BRANCH\n");
  fp = fopen("locks.file","a");

  fprintf(fp, "%s", test_case_id);
  fclose(fp);
  printf("g_346: h %d\n", h);
  printf("g_886: g %d\n", g);

//  print_symbolic("g_346", &h, 32);
//  print_symbolic("g_886", &g, 16);
}
