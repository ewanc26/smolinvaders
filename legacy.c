#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

int main(void){
  struct termios old,new; int x=20,p=-1,ex=5,ey=2,dx=1,score=0; char c;
  tcgetattr(0,&old); new=old; new.c_lflag&=~(ICANON|ECHO); tcsetattr(0,TCSANOW,&new);
  printf("\033[2J\033[?25l");
  for(;;){
    printf("\033[H\033[JScore:%d  a/d move, space fire, q quit\n",score);
    for(int y=0;y<20;y++){ for(int z=0;z<48;z++){
      char s=' '; if(y==19&&(z==x||z==x+1||z==x+2))s='A';
      if(y==ey&&(z==ex||z==ex+1||z==ex+2))s='W';
      if(y==p)s='|'; putchar(s);
    } putchar('\n'); }
    if(p>=0&&p==ey){ if(x+1>=ex&&x<=ex+2)score++,ex=rand()%45,ey=2,p=-1; else p--; }
    else if(p>=0)p--;
    if((ex+=dx)<0||ex>44)dx=-dx,ey++;
    if(ey>18){ puts("GAME OVER"); break; }
    fd_set f; struct timeval t={0,70000}; FD_ZERO(&f); FD_SET(0,&f);
    if(select(1,&f,0,0,&t)>0){ read(0,&c,1); if(c=='q')break; if(c=='a'&&x)x--; if(c=='d'&&x<45)x++; if(c==' '&&p<0)p=18; }
  }
  tcsetattr(0,TCSANOW,&old); printf("\033[?25h"); return 0;
}
