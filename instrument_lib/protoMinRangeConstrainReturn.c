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
  while(prev - ub > 1) {
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

  printf("#1 lb: %ld, ub: %ld\n", lb, ub);
  //searching the lowest lb
  long ubForlb = ub;
  lb = min_int;
  prev = lb - 10;
  while(lb - prev > 2) {
    if(h > lb) {
        prev = lb;
        lb = lb + (ubForlb - lb) / 2;
    } else {
       ubForlb = lb;
       lb = prev; 
    }
  }
  printf("#2 lb: %ld, ub: %ld\n", lb, ub);

  printf("g_346: h %d\n", h);
  printf("g_886: g %d\n", g);

//  print_symbolic("g_346", &h, 32);
//  print_symbolic("g_886", &g, 16);
}
