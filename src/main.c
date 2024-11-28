#include "include/main.h"


void draw_start_page(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    int ypos = ((g_win_size[1] - 4)) * 40 / 100;
    int xpos = (g_win_size[0] - 80) / 2;

    //mvwprintw(win, 4, 2, "Hello, world!");

    for (int i = 0; i < 4; i++) {
        mvwprintw(win, ypos + i, xpos, title[i]);
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
        if (g_select_menu == 0) {
            g_scene = 2;
        }
        else {
            g_scene = -1;
        }
        break;
    }

    if (g_select_menu == 0) {
        mvwprintw(win, ypos + 5, (g_win_size[0] - 14) / 2, "< start game >");
        mvwprintw(win, ypos + 6, (g_win_size[0] - 9) / 2, "exit game");
    }
    else {
        mvwprintw(win, ypos + 5, (g_win_size[0] - 10) / 2, "start game");
        mvwprintw(win, ypos + 6, (g_win_size[0] - 13) / 2, "< exit game >");
    }

    wrefresh(win);
}

void draw_arena(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    wrefresh(win);
}

void draw_player(WINDOW* win) {
    int key = getch();

    switch (key)
    {
    case 'w':
        g_player.direction[0] = 0;
        g_player.direction[1] = -1;
        break;
    case 's':
        g_player.direction[0] = 0;
        g_player.direction[1] = 1;
        break;
    case 'a':
        g_player.direction[0] = -1;
        g_player.direction[1] = 0;
        break;
    case 'd':
        g_player.direction[0] = 1;
        g_player.direction[1] = 0;
        break;
    }

    if ((g_player.pos[0] + g_player.direction[0]) >= g_win_size[0] || (g_player.pos[0] + g_player.direction[0]) <= 0) {
        g_player.direction[0] = 0;
    }

    if ((g_player.pos[1] + g_player.direction[1]) >= g_win_size[1] || (g_player.pos[1] + g_player.direction[1]) <= 0) {
        g_player.direction[1] = 0;
    }

    if (g_player.walk >= 1) {
        g_player.pos[0] += g_player.direction[0];
        g_player.pos[1] += g_player.direction[1];
        g_player.walk = 0;
    }

    else {
        g_player.walk += g_player.speed;
    }

    wclear(win);
    mvwprintw(win, (g_player.pos[1]), (g_player.pos[0]), g_player.avatar);
    wrefresh(win);

    return;
}

void spawn_bullets(WINDOW* win) {
    if (g_player.timeCooldown < 1) {
        return;
    }
    for (int i = 0; i < g_bullet_count; i++) {
        for (int j = i; j < (sizeof(g_bullets) / sizeof(g_bullets[0])); j++) {
            if (g_bullets[j].lifetime <= 0) {
                int dir = (i + g_bullet_rotation) % 8;
                switch (dir)
                {
                case 0:
                    g_bullets[j].direction[0] = 1;
                    g_bullets[j].direction[1] = 0;
                    break;
                case 1:
                    g_bullets[j].direction[0] = 1;
                    g_bullets[j].direction[1] = 1;
                    break;
                case 2:
                    g_bullets[j].direction[0] = 0;
                    g_bullets[j].direction[1] = 1;
                    break;
                case 3:
                    g_bullets[j].direction[0] = -1;
                    g_bullets[j].direction[1] = 1;
                    break;
                case 4:
                    g_bullets[j].direction[0] = -1;
                    g_bullets[j].direction[1] = 0;
                    break;
                case 5:
                    g_bullets[j].direction[0] = -1;
                    g_bullets[j].direction[1] = -1;
                    break;
                case 6:
                    g_bullets[j].direction[0] = 0;
                    g_bullets[j].direction[1] = -1;
                    break;
                case 7:
                    g_bullets[j].direction[0] = 1;
                    g_bullets[j].direction[1] = -1;
                    break;
                }
                g_bullets[j].lifetime = 10;
                g_bullets[j].pos[0] = g_player.pos[0] + (2 * g_bullets[j].direction[0]);
                g_bullets[j].pos[1] = g_player.pos[1] + (1 * g_bullets[j].direction[1]);
                break;
            }
        }
    }
    g_bullet_rotation = (g_bullet_rotation + 1) % 8;
    g_player.timeCooldown = 0;
    wrefresh(win);
}

void draw_bullets(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_bullets) / sizeof(g_bullets[0])); i++) {
        if (((g_bullets[i].pos[0] + g_bullets[i].direction[0]) >= g_win_size[0] || (g_bullets[i].pos[0] + g_bullets[i].direction[0]) <= 0) || ((g_bullets[i].pos[1] + g_bullets[i].direction[1]) >= g_win_size[1] || (g_bullets[i].pos[1] + g_bullets[i].direction[1]) <= 0)) {
            g_bullets[i].lifetime = 0;
        }

        if (g_bullets[i].lifetime <= 0) {
            continue;
        }

        if (g_bullets[i].walk >= 1) {
            g_bullets[i].pos[0] += g_bullets[i].direction[0];
            g_bullets[i].pos[1] += g_bullets[i].direction[1];
            g_bullets[i].lifetime--;
        }
        else {
            g_bullets[i].walk += g_bullets[i].speed;
        }

        mvwprintw(win, g_bullets[i].pos[1], g_bullets[i].pos[0], g_bullets[i].avatar);
    }
    wrefresh(win);
}

void spawn_enemy(WINDOW* win) {
    int x = (rand() % g_win_size[0]);
    int y = (rand() % g_win_size[1]);

    if (enemy_spawn_cooldown >= enemy_spawn_rate && enemy_spawn_counter < enemy_max_amount) {
        for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
            if (g_enemy[i].health == 0) {
                g_enemy[i].pos[0] = x;
                g_enemy[i].pos[1] = y;
                g_enemy[i].health = enemy.health;
                g_enemy[i].speed = enemy.speed;
                g_enemy[i].dmg = enemy.dmg;
                break;
            }
        }
        enemy_spawn_cooldown = 0;
        enemy_spawn_counter += 1;
    }
    else if (enemy_spawn_counter < enemy_max_amount) {
        enemy_spawn_cooldown += 0.1;
    }
}

void draw_enemy(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        if (g_enemy[i].health > 0) {
            mvwprintw(win, g_enemy[i].pos[1], g_enemy[i].pos[0], g_enemy[i].avatar);
        }
    }
    wrefresh(win);
};

// ---------------------------------main control----------------------------------

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");
    raw();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, 1);
    srand(time(NULL));

    WINDOW* win = newwin(g_win_size[0], g_win_size[1], 0, 0);
    draw_start_page(win);
    // player set
    g_player.pos[0] = (g_win_size[0] - 5) / 2;
    g_player.pos[1] = (g_win_size[1] - 1) / 2;

    // bullet set
    struct Bullet bullet = {
        "o",
        {0,0},
        {0,0},
        5,
        0,
        0.3,
        0,
    };

    for (int i = 0; i < (sizeof(g_bullets) / sizeof(g_bullets[0])); i++) {
        g_bullets[i] = bullet;
    }

    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        g_enemy[i] = enemy;
        g_enemy[i].health = 0;
    }

    while (TRUE)
    {
        switch (g_scene)
        {
        case 1:
            draw_start_page(win);
            break;

        case 2:
            draw_arena(win);
            draw_player(win);
            spawn_enemy(win);
            draw_enemy(win);
            g_player.timeCooldown += g_player.cooldown;
            spawn_bullets(win);
            draw_bullets(win);
            break;
        }

        if (g_scene == -1) {
            break;
        }

        usleep(100000);
    }
    endwin();

    return 0;
}