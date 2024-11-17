#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>

int g_win_size[2] = {0,0};
int g_select_menu = 0;
int g_scene = 1;

const char title[][300] = {
    "████████████████████████████████████████████████████████████████████████████████",
    "█▄─█─▄█▄─▄█▄─▄▄▀█▄─██─▄█─▄▄▄▄███▄─▄─▀█▄─▄▄▀█▄─▄▄─██▀▄─██▄─█─▄█─▄▄─█▄─██─▄█─▄─▄─█",
    "██▄▀▄███─███─▄─▄██─██─██▄▄▄▄─████─▄─▀██─▄─▄██─▄█▀██─▀─███─▄▀██─██─██─██─████─███",
    "▀▀▀▄▀▀▀▄▄▄▀▄▄▀▄▄▀▀▄▄▄▄▀▀▄▄▄▄▄▀▀▀▄▄▄▄▀▀▄▄▀▄▄▀▄▄▄▄▄▀▄▄▀▄▄▀▄▄▀▄▄▀▄▄▄▄▀▀▄▄▄▄▀▀▀▄▄▄▀▀"
};

void draw_menu(WINDOW *win){
    int nSize[2] = {0,0};
    getmaxyx(stdscr, nSize[1], nSize[0]); 

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win,0,0);

    int ypos = ((g_win_size[1]-4))*40/100;
    int xpos = (g_win_size[0]-80)/2;

    //mvwprintw(win, 4, 2, "Hello, world!");

    for (int i=0; i<4; i++){
        mvwprintw(win, ypos+i, xpos, title[i]);
    }

    int input = getch();

    switch (input)
    {
        case 'w':
            g_select_menu = 0;
            break;
        
        case 's':
            g_select_menu = 1;
            break;
        case '\n':
            g_scene = 2;
            break;
    }

    if (g_select_menu == 0){
        mvwprintw(win, ypos+5, (g_win_size[0]-14)/2, "< start game >");
        mvwprintw(win, ypos+6, (g_win_size[0]-9)/2, "exit game");
    }
    else{
        mvwprintw(win, ypos+5, (g_win_size[0]-10)/2, "start game");
        mvwprintw(win, ypos+6, (g_win_size[0]-13)/2, "< exit game >");
    }

    wrefresh(win);
}

int main(int argc, char *argv[]){

    setlocale(LC_ALL, "");
    raw();
    initscr();
    cbreak();
	noecho();             
	keypad(stdscr, TRUE); 
	curs_set(0); 
    nodelay(stdscr, 1);

    WINDOW *win = newwin(g_win_size[0],g_win_size[1],0,0);

    while (TRUE)
    {
        draw_menu(win);
        
        usleep(100000);
    }

    return 0;
}