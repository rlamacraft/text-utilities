#include<stdio.h>

void usage() {
  printf("You need to provide a file name\n");
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    usage();
    return(1);
  }
  FILE *fp;
  char con[100];
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Error opening file");
    return(-1);
  }
  while (fgets(con, 100, fp) != NULL)
    printf("%s", con);
  fclose(fp);
  return 0;
}
