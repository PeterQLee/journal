#include <termios.h>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>

int encrypt(char *file,char *pass, int passlen) {
  FILE *f=fopen(file,"rb");
  if (f== NULL) {
    //file DNE, exit
    return -1;
  }
  size_t sz;
  fseek(f,0L,SEEK_END);
  sz=ftell(f);
  fseek(f,0L,SEEK_SET);
  char *dat=malloc(sizeof(char)*sz);
  char c;
  int last=0;
  int i=0;
  int j;
  while (i<sz) {
    c=getc(f);
    dat[i]=(last+c)&0xff;
    for (j=0;j<passlen;j++) {
      dat[i]=dat[i]^((int)pass[j]);
    }
    
    last=(int)dat[i];
    i++;
  }
  fclose(f);
  f=fopen(file,"w");
  for (i=0;i<sz;i++) {
    putc(dat[i],f);
  }
  free(dat);
  fclose(f);
  return 0;
}
int decrypt(char *file,char *pass, int passlen) {
  FILE *f=fopen(file,"rb");
  if (f== NULL) {
    //file DNE, exit
    return -1;
  }
  size_t sz;
  fseek(f,0L,SEEK_END);
  sz=ftell(f);
  fseek(f,0L,SEEK_SET);
  char *dat=malloc(sizeof(char)*sz);
  char c;
  int last=0;
  int i=0;
  int j;
  while (i<sz) {
    c=getc(f);
    int prelast=last;
    last=(int)c;
    for (j=0;j<passlen;j++) {
      c=c^((int)pass[j]);
    }

    dat[i]=(c-prelast)&0xff;

    
    
    i++;
  }
  fclose(f);
  f=fopen(file,"w");
  for (i=0;i<sz;i++) {
    putc(dat[i],f);
  }
  free(dat);
  fclose(f);
  return 0;
}
int main(int argc,char **argv){
  char *d;
  int n;

  printf ("Enter the password: ");
  filter();
  initscr();
  cbreak();
  noecho();
  
  char *s=malloc(sizeof(char)*30);
  scanf("%s",s);
  if (argc==2) {

    decrypt(argv[1],s,strlen(s));
    char  com[40];
    strcpy(com,"emacs -l config ");
    strcat(com,argv[1]);
    system(com);
    encrypt(argv[1],s,strlen(s));
    
  }
 
  endwin();
  echo();
  printf("\n%s\n",s);
  free(s);
}
