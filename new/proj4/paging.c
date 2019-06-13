typedef struct{
    unsigned char presence;   /* could be single bit */
    unsigned short pad;
    unsigned char pfn;        /* 8 bits */
  } PTE;

  typedef struct{
    unsigned char valid;      /* could be single bit */
    unsigned short vpn;       /* 16 bits */
    unsigned char pfn;        /* 8 bits */
  } TLBE;

  typedef struct{
    unsigned char valid;      /* could be single bit */
    unsigned char use_vector; /* 8 bits for pseudo-LRU replacement */
    unsigned short vpn;       /* 16 bits */
  } CME;


int main(){
  

    return 0;



}
