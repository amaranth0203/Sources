#include     <stdlib.h>
#include     <curses.h>
#include     <sys/types.h> 
#include     <sys/stat.h>  
#include     <fcntl.h>
#include     <errno.h>
 
 
#define StartX  1
#define StartY  1
 
void initial(void)
{
initscr();
  cbreak();
  nonl();
  noecho();
  intrflush(stdscr,FALSE);
  keypad(stdscr,TRUE);
  refresh();
}
 
int main(int argc, char **argv)
{
int x=StartX;
  int y=StartY;
  int ch;
 
 
initial();
  box(stdscr,'|','-');
  attron(A_REVERSE);
  mvaddstr(0,40,"My Curses Program");
  attroff(A_REVERSE);
  move(x,y);
  do {
        ch=getch();
    switch(ch) {
            case KEY_UP:
                --y;
                break;
            case KEY_DOWN:
                ++y;
                break;
            case KEY_RIGHT:
                ++x;
                break;
            case KEY_LEFT:
                --x;
                break;
            case '\r':
                ++y;
                x=1;
                break;
            case '\t':
                x+=7;
                break;
            case 8://判断是否 BACKSPACE
                mvaddch(y,--x,' ');
        break;
      case 27://[ESC]键
          endwin();
          exit(1);
          break;
      default:
                addch(ch);
                x++;
                if(x == 99) {
                    x=1;
                    y++;
                }
                break;
        }
    move(y,x);
    } while (1);
  exit(1);
}
