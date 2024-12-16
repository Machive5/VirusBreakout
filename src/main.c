#include "include/main.h"

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
    drawStartPage(win);
    init();


    while (TRUE)
    {
        if (player_level_up) {
            drawUpgradePage(win);
            usleep(100000);
            continue;
        }

        if (g_player.health <= 0) {
            drawGameOverUI(win);
            usleep(100000);
            continue;
        }

        switch (g_scene)
        {
        case 1:
            drawStartPage(win);
            break;

        case 2:

            if (g_player.health <= 0)
            {
                break;
            }

            drawArena(win);
            drawStatusUI(win);

            playerMovement(win);

            spawnEnemy(win);
            enemyMovement(win);

            spawnBullets(win);
            bulletsMovement(win);

            spawnBos(win);
            bossMovement(win);

            bossBulletsSpawn(win);
            bossBulletsMovement(win);

            playerAndBosCollisionCheck(win);
            bossBulletAndPlayerCollisionCheck(win);
            bossAndPlayerBulletCollisionCheck(win);
            enemyAndPlayerBulletCollisionCheck(win);
            playerAndEnemyCollisionCheck(win);

            g_player.timeCooldown += g_player.cooldown;
            boss.timeCooldown += boss.cooldown;
            player_invisible -= 0.1;

            break;
        }

        if (g_scene == -1) {
            endwin();
            break;
        }

        usleep(100000);
    }
    endwin();

    return 0;
}

void drawStartPage(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    int ypos = ((g_win_size[1] - 6)) * 40 / 100;
    int xpos = (g_win_size[0] - 108) / 2;

    //mvwprintw(win, 4, 2, "Hello, world!");

    for (int i = 0; i < 6; i++) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, ypos + i, xpos, title[i]);
        wattroff(win, COLOR_PAIR(1));
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
        mvwprintw(win, ypos + 7, (g_win_size[0] - 14) / 2, "< start game >");
        mvwprintw(win, ypos + 8, (g_win_size[0] - 9) / 2, "exit game");
    }
    else {
        mvwprintw(win, ypos + 7, (g_win_size[0] - 10) / 2, "start game");
        mvwprintw(win, ypos + 8, (g_win_size[0] - 13) / 2, "< exit game >");
    }

    wrefresh(win);
}

void drawUpgradePage(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    int ypos = ((g_win_size[1] - 6)) * 40 / 100;
    int xpos = (g_win_size[0] - 65) / 2;

    //mvwprintw(win, 4, 2, "Hello, world!");

    for (int i = 0; i < 6; i++) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, ypos + i, xpos, level_up[i]);
        wattroff(win, COLOR_PAIR(1));
    }

    int input = getch();

    switch (input)
    {
    case 'w':
        g_select_menu -= 1;
        if (g_select_menu < 0)
        {
            g_select_menu = 0;
        }

        break;

    case 's':
        g_select_menu += 1;
        if (g_select_menu > 2)
        {
            g_select_menu = 2;
        }

        break;
    case '\n':
        if (g_select_menu == 0) {
            player_hp_max += 1;
            g_player.health = player_hp_max;
            player_exp_max += 3;
            player_level_up = false;
        }
        else if (g_select_menu == 1) {
            g_player.speed += 0.1;
            g_bullet_speed += 0.1;
            player_exp_max += 3;
            player_level_up = false;
        }
        else if (g_select_menu == 2) {
            g_bullet_count += 1;
            player_exp_max += 3;
            player_level_up = false;
        }
        g_player.health = player_hp_max;

        if (g_player.level % 2 != 0) {
            enemy_health += 2;
            enemy_max_amount += 1;
        }

        if (g_player.level % 3 != 0) {
            enemy.dmg += 1;
        }
        break;
    }

    if (g_select_menu == 0) {
        mvwprintw(win, ypos + 7, (g_win_size[0] - 9) / 2, "< hp +1 >");
        mvwprintw(win, ypos + 8, (g_win_size[0] - 8) / 2, "speed +1");
        mvwprintw(win, ypos + 9, (g_win_size[0] - 9) / 2, "bullet +1");

    }
    else if (g_select_menu == 1) {
        mvwprintw(win, ypos + 7, (g_win_size[0] - 5) / 2, "hp +1");
        mvwprintw(win, ypos + 8, (g_win_size[0] - 12) / 2, "< speed +1 >");
        mvwprintw(win, ypos + 9, (g_win_size[0] - 9) / 2, "bullet +1");

    }
    else {
        mvwprintw(win, ypos + 7, (g_win_size[0] - 5) / 2, "hp +1");
        mvwprintw(win, ypos + 8, (g_win_size[0] - 8) / 2, "speed +1");
        mvwprintw(win, ypos + 9, (g_win_size[0] - 13) / 2, "< bullet +1 >");
    }

    wrefresh(win);
}

void drawArena(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    for (int i = 1; i < g_win_size[0] - 1; i++) {
        mvwprintw(win, 5, i, "-");
    }

    wrefresh(win);
}

void drawStatusUI(WINDOW* win) {
    int HP = (int)((float)g_player.health / player_hp_max * 10);
    int XP = (int)((float)player_exp / player_exp_max * 10);
    int bossHP = (int)((float)boss.health / boss_health * 10);

    wattron(win, COLOR_PAIR(4));
    switch (XP)
    {
    case 0:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP □ □ □ □ □ □ □ □ □ □");
        break;
    case 1:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ □ □ □ □ □ □ □ □ □");
        break;
    case 2:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ □ □ □ □ □ □ □ □");
        break;
    case 3:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ □ □ □ □ □ □ □");
        break;
    case 4:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ □ □ □ □ □ □");
        break;
    case 5:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ □ □ □ □ □");
        break;
    case 6:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ ■ □ □ □ □");
        break;
    case 7:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ ■ ■ □ □ □");
        break;
    case 8:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ ■ ■ ■ □ □");
        break;
    case 9:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ ■ ■ ■ ■ □");
        break;
    case 10:
        mvwprintw(win, 2, round((g_win_size[0] - 91) / 2), "XP ■ ■ ■ ■ ■ ■ ■ ■ ■ ■");
        break;
    }
    wattroff(win, COLOR_PAIR(4));

    wattron(win, COLOR_PAIR(3));
    switch (HP)
    {
    case 0:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP □ □ □ □ □ □ □ □ □ □");
        break;
    case 1:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ □ □ □ □ □ □ □ □ □");
        break;
    case 2:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ □ □ □ □ □ □ □ □");
        break;
    case 3:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ □ □ □ □ □ □ □");
        break;
    case 4:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ □ □ □ □ □ □");
        break;
    case 5:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ □ □ □ □ □");
        break;
    case 6:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ ■ □ □ □ □");
        break;
    case 7:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ ■ ■ □ □ □");
        break;
    case 8:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ ■ ■ ■ □ □");
        break;
    case 9:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ ■ ■ ■ ■ □");
        break;
    case 10:
        mvwprintw(win, 2, 32 + round((g_win_size[0] - 90) / 2), "HP ■ ■ ■ ■ ■ ■ ■ ■ ■ ■");
        break;
    }
    wattroff(win, COLOR_PAIR(3));

    if (boss_spawned) {
        wattron(win, COLOR_PAIR(3));
        switch (bossHP)
        {
        case 0:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP □ □ □ □ □ □ □ □ □ □");
            break;
        case 1:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ □ □ □ □ □ □ □ □ □");
            break;
        case 2:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ □ □ □ □ □ □ □ □");
            break;
        case 3:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ □ □ □ □ □ □ □");
            break;
        case 4:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ □ □ □ □ □ □");
            break;
        case 5:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ □ □ □ □ □");
            break;
        case 6:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ ■ □ □ □ □");
            break;
        case 7:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ ■ ■ □ □ □");
            break;
        case 8:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ ■ ■ ■ □ □");
            break;
        case 9:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ ■ ■ ■ ■ □");
            break;
        case 10:
            mvwprintw(win, 4, round((g_win_size[0] - 28) / 2), "Boss HP ■ ■ ■ ■ ■ ■ ■ ■ ■ ■");
            break;
        }
        wattroff(win, COLOR_PAIR(3));
    }

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 2, 64 + round((g_win_size[0] - 90) / 2), "LVL: %d", g_player.level);
    wattroff(win, COLOR_PAIR(1));

    mvwprintw(win, 2, 79 + round((g_win_size[0] - 90) / 2), "Score: %d", player_score);

    wrefresh(win);
}

void drawGameOverUI(WINDOW* win) {
    int nSize[2] = { 0,0 };
    getmaxyx(stdscr, nSize[1], nSize[0]);

    wclear(win);
    g_win_size[0] = nSize[0];
    g_win_size[1] = nSize[1];
    wresize(win, g_win_size[1], g_win_size[0]);
    box(win, 0, 0);

    int ypos = ((g_win_size[1] - 6)) * 40 / 100;
    int xpos = (g_win_size[0] - 74) / 2;

    //mvwprintw(win, 4, 2, "Hello, world!");

    for (int i = 0; i < 6; i++) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, ypos + i, xpos, game_over[i]);
        wattroff(win, COLOR_PAIR(1));
    }

    mvwprintw(win, ypos + 7, (g_win_size[0] - 15) / 2, "your score: %d", player_score);

    switch (getch())
    {
    case 'w':
        play_again = true;
        break;

    case 's':
        play_again = false;
        break;
    case '\n':
        if (play_again) {
            endwin();
            char* args[] = { "./virus-breakout", NULL };
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else {
            g_scene = -1;
            g_player.health = 1;
        }
        break;
    }

    if (play_again) {
        mvwprintw(win, ypos + 9, (g_win_size[0] - 14) / 2, "< play again >");
        mvwprintw(win, ypos + 10, (g_win_size[0] - 9) / 2, "exit game");
    }
    else {
        mvwprintw(win, ypos + 9, (g_win_size[0] - 10) / 2, "play again");
        mvwprintw(win, ypos + 10, (g_win_size[0] - 13) / 2, "< exit game >");
    }

    wrefresh(win);
}

void playerMovement(WINDOW* win) {
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

    if ((g_player.pos[1] + g_player.direction[1]) >= g_win_size[1] || (g_player.pos[1] + g_player.direction[1]) <= 5) {
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

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, (g_player.pos[1]), (g_player.pos[0]), g_player.avatar);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);

    return;
}

void spawnBullets(WINDOW* win) {
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
                g_bullets[j].dmg = g_bullet_dmg;
                g_bullets[j].speed = g_bullet_speed;
                g_bullets[j].pos[0] = g_player.pos[0] + (2 * g_bullets[j].direction[0]);
                g_bullets[j].pos[1] = g_player.pos[1] + (2 * g_bullets[j].direction[1]);
                break;
            }
        }
    }
    g_bullet_rotation = (g_bullet_rotation + 1) % 8;
    g_player.timeCooldown = 0;
    wrefresh(win);
}

void bulletsMovement(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_bullets) / sizeof(g_bullets[0])); i++) {
        if (((g_bullets[i].pos[0] + g_bullets[i].direction[0]) >= g_win_size[0] || (g_bullets[i].pos[0] + g_bullets[i].direction[0]) <= 0) || ((g_bullets[i].pos[1] + g_bullets[i].direction[1]) >= g_win_size[1] || (g_bullets[i].pos[1] + g_bullets[i].direction[1]) <= 5)) {
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

        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, g_bullets[i].pos[1], g_bullets[i].pos[0], g_bullets[i].avatar);
        wattroff(win, COLOR_PAIR(2));
    }
    wrefresh(win);
}

void spawnEnemy(WINDOW* win) {
    int x = (rand() % g_win_size[0]);
    int y = (rand() % (g_win_size[1] - 5)) + 5;

    if (enemy_spawn_cooldown >= enemy_spawn_rate && enemy_spawn_counter < enemy_max_amount) {
        for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
            if (g_enemy[i].health <= 0) {
                g_enemy[i].pos[0] = x;
                g_enemy[i].pos[1] = y;
                g_enemy[i].health = enemy_health;
                g_enemy[i].speed = enemy.speed;
                g_enemy[i].dmg = enemy.dmg;
                g_enemy[i].point = enemy_point;

                enemy_spawn_cooldown = 0;
                enemy_spawn_counter += 1;
                break;
            }
        }
    }
    else if (enemy_spawn_counter < enemy_max_amount) {
        enemy_spawn_cooldown += 0.1;
    }
    wrefresh(win);
}

void enemyMovement(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        if (g_enemy[i].health > 0 && g_enemy[i].walk >= 1) {

            g_enemy[i].direction[0] = 0;
            g_enemy[i].direction[1] = 0;

            if (g_enemy[i].pos[0] > g_player.pos[0]) {
                g_enemy[i].direction[0] = -1;
            }
            else if (g_enemy[i].pos[0] < g_player.pos[0]) {
                g_enemy[i].direction[0] = 1;
            }

            if (g_enemy[i].pos[1] > g_player.pos[1]) {
                g_enemy[i].direction[1] = -1;
            }
            else if (g_enemy[i].pos[1] < g_player.pos[1]) {
                g_enemy[i].direction[1] = 1;
            }

            if ((g_enemy[1].pos[0] + g_enemy[1].direction[0]) >= g_win_size[0] || (g_enemy[1].pos[0] + g_enemy[1].direction[0]) <= 0) {
                g_enemy[1].direction[0] = 0;
            }

            if ((g_enemy[1].pos[1] + g_enemy[1].direction[1]) >= g_win_size[1] || (g_enemy[1].pos[1] + g_enemy[1].direction[1]) <= 5) {
                g_enemy[1].direction[1] = 0;
            }

            g_enemy[i].pos[0] += g_enemy[i].direction[0];
            g_enemy[i].pos[1] += g_enemy[i].direction[1];
            g_enemy[i].walk = 0;
        }
        else {
            g_enemy[i].walk += g_enemy[i].speed;
        }

        if (g_enemy[i].health > 0) {
            wattron(win, COLOR_PAIR(3));
            mvwprintw(win, g_enemy[i].pos[1], g_enemy[i].pos[0], g_enemy[i].avatar);
            wattroff(win, COLOR_PAIR(3));
        }
    }
    wrefresh(win);
};

void spawnBos(WINDOW* win) {

    if (boss_spawned) {
        return;
    }

    if (g_player.level % 5 == 0) {
        int x = (rand() % g_win_size[0] - 3) + 3;
        int y = (rand() % (g_win_size[1] - 8)) + 5;

        boss.pos[0] = x;
        boss.pos[1] = y;
        boss.health = boss_health;
        boss.dmg = boss_dmg;
        boss.speed = boss_speed;
        boss_spawned = true;
    }
}

void bossMovement(WINDOW* win) {
    if (boss.health <= 0 || boss_spawned == false) {
        return;
    }

    if (boss.walk >= 1) {
        boss.direction[0] = 0;
        boss.direction[1] = 0;

        if (boss.pos[0] > g_player.pos[0]) {
            boss.direction[0] = -1;
        }
        else if (boss.pos[0] < g_player.pos[0]) {
            boss.direction[0] = 1;
        }

        if (boss.pos[1] > g_player.pos[1]) {
            boss.direction[1] = -1;
        }
        else if (boss.pos[1] < g_player.pos[1]) {
            boss.direction[1] = 1;
        }

        if ((boss.pos[0] + 3 + boss.direction[0]) >= g_win_size[0] || (boss.pos[0] - 3 + boss.direction[0]) <= 0) {
            boss.direction[0] = 0;
        }

        if ((boss.pos[1] + 3 + boss.direction[1]) >= g_win_size[1] || (boss.pos[1] - 3 + boss.direction[1]) <= 5) {
            boss.direction[1] = 0;
        }

        boss.pos[0] += boss.direction[0];
        boss.pos[1] += boss.direction[1];
        boss.walk = 0;
    }
    else {
        boss.walk += boss.speed;
    }

    wattron(win, COLOR_PAIR(4));
    for (int i = 0; i < 5; i++) {
        mvwprintw(win, boss.pos[1] - 2 + i, boss.pos[0] - 2, boss.avatar[i]);
    }
    wattroff(win, COLOR_PAIR(4));
}

void bossBulletsSpawn(WINDOW* win) {
    if (boss.timeCooldown < 1 || boss.health <= 0 || boss_spawned == false) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = i; j < (sizeof(enemy_bullets) / sizeof(enemy_bullets[0])); j++) {
            if (enemy_bullets[j].lifetime <= 0) {
                switch (i)
                {
                case 0:
                    enemy_bullets[j].direction[0] = 1;
                    enemy_bullets[j].direction[1] = 0;
                    break;
                case 1:
                    enemy_bullets[j].direction[0] = 1;
                    enemy_bullets[j].direction[1] = 1;
                    break;
                case 2:
                    enemy_bullets[j].direction[0] = 0;
                    enemy_bullets[j].direction[1] = 1;
                    break;
                case 3:
                    enemy_bullets[j].direction[0] = -1;
                    enemy_bullets[j].direction[1] = 1;
                    break;
                case 4:
                    enemy_bullets[j].direction[0] = -1;
                    enemy_bullets[j].direction[1] = 0;
                    break;
                case 5:
                    enemy_bullets[j].direction[0] = -1;
                    enemy_bullets[j].direction[1] = -1;
                    break;
                case 6:
                    enemy_bullets[j].direction[0] = 0;
                    enemy_bullets[j].direction[1] = -1;
                    break;
                case 7:
                    enemy_bullets[j].direction[0] = 1;
                    enemy_bullets[j].direction[1] = -1;
                    break;
                }
                enemy_bullets[j].lifetime = 25;
                enemy_bullets[j].dmg = boss.dmg / 2;
                enemy_bullets[j].speed = 3 * boss_speed;
                enemy_bullets[j].pos[0] = boss.pos[0] + (4 * enemy_bullets[j].direction[0]);
                enemy_bullets[j].pos[1] = boss.pos[1] + (4 * enemy_bullets[j].direction[1]);
                break;
            }
        }
    }
    boss.timeCooldown = 0;
    wrefresh(win);
}

void bossBulletsMovement(WINDOW* win) {
    for (int i = 0; i < (sizeof(enemy_bullets) / sizeof(enemy_bullets[0])); i++) {
        if (((enemy_bullets[i].pos[0] + enemy_bullets[i].direction[0]) >= g_win_size[0] || (enemy_bullets[i].pos[0] + enemy_bullets[i].direction[0]) <= 0) || ((enemy_bullets[i].pos[1] + enemy_bullets[i].direction[1]) >= g_win_size[1] || (enemy_bullets[i].pos[1] + enemy_bullets[i].direction[1]) <= 5)) {
            enemy_bullets[i].lifetime = 0;
        }

        if (enemy_bullets[i].lifetime <= 0) {
            continue;
        }

        if (enemy_bullets[i].walk >= 1) {
            enemy_bullets[i].pos[0] += enemy_bullets[i].direction[0];
            enemy_bullets[i].pos[1] += enemy_bullets[i].direction[1];
            enemy_bullets[i].lifetime--;
        }
        else {
            enemy_bullets[i].walk += enemy_bullets[i].speed;
        }

        wattron(win, COLOR_PAIR(4));
        mvwprintw(win, enemy_bullets[i].pos[1], enemy_bullets[i].pos[0], enemy_bullets[i].avatar);
        wattroff(win, COLOR_PAIR(4));
    }
    wrefresh(win);
}

void playerAndBosCollisionCheck(WINDOW* win) {
    if (boss.health <= 0 || !boss_spawned) {
        return;
    }

    if (((boss.pos[0] + 2 >= g_player.pos[0] && boss.pos[1] + 2 >= g_player.pos[1]) && (boss.pos[0] - 2 <= g_player.pos[0] && boss.pos[1] - 2 <= g_player.pos[1])) && player_invisible <= 0) {
        g_player.health -= boss.dmg;
        player_invisible = 1;
    }
}

void bossBulletAndPlayerCollisionCheck(WINDOW* win) {
    for (int i = 0; i < (sizeof(enemy_bullets) / sizeof(enemy_bullets[0])); i++) {
        if (enemy_bullets[i].pos[0] == g_player.pos[0] && enemy_bullets[i].pos[1] == g_player.pos[1] && g_player.health > 0 && enemy_bullets[i].lifetime > 0) {
            g_player.health -= enemy_bullets[i].dmg;
            enemy_bullets[i].lifetime = 0;
        }
    }
}

void bossAndPlayerBulletCollisionCheck(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_bullets) / sizeof(g_bullets[0])); i++) {
        if (((boss.pos[0] + 2 >= g_bullets[i].pos[0] && boss.pos[1] + 2 >= g_bullets[i].pos[1]) && (boss.pos[0] - 2 <= g_bullets[i].pos[0] && boss.pos[1] - 2 <= g_bullets[i].pos[1])) && boss.health > 0 && g_bullets[i].lifetime > 0) {
            boss.health -= g_bullets[i].dmg;
            g_bullets[i].lifetime = 0;

            if (boss.health <= 0) {
                boss_spawned = false;
                player_score += 1000;
                g_bullet_dmg *= 2;
                player_exp = player_exp_max;
                boss_cooldown *= 2;
                boss_health *= 2;
                boss_speed *= 2;
                boss_dmg *= 2;

                if (player_exp >= player_exp_max) {
                    g_player.level += 1;
                    player_exp = 0;
                    player_level_up = true;
                }
            }
        }
    }
}

void enemyAndPlayerBulletCollisionCheck(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        for (int j = 0; j < (sizeof(g_bullets) / sizeof(g_bullets[0])); j++) {
            if (g_enemy[i].pos[0] == g_bullets[j].pos[0] && g_enemy[i].pos[1] == g_bullets[j].pos[1] && g_enemy[i].health > 0 && g_bullets[j].lifetime > 0) {
                g_enemy[i].health -= g_bullets[j].dmg;

                if (g_enemy[i].health <= 0) {
                    enemy_spawn_counter -= 1;

                    player_exp += 1;
                    player_score += g_enemy[i].point;
                    if (player_exp >= player_exp_max) {
                        g_player.level += 1;
                        player_exp = 0;
                        player_level_up = true;
                    }
                }

                g_bullets[j].lifetime = 0;
            }
        }
    }
}

void playerAndEnemyCollisionCheck(WINDOW* win) {
    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        if (g_enemy[i].pos[0] == g_player.pos[0] && g_enemy[i].pos[1] == g_player.pos[1] && player_invisible <= 0) {
            g_player.health -= g_enemy[i].dmg;
            player_invisible = 1;
        }
    }
}

void init()
{
    // player set
    g_player.pos[0] = (g_win_size[0] - 1) / 2;
    g_player.pos[1] = (g_win_size[1] - 1) / 2;

    // bullet set
    struct Bullet bullet = {
        "o",
        {0,0},//pos
        {0,0},//dir
        g_bullet_dmg,//dmg
        0,//walk
        g_bullet_speed,//speed
        0,//lifetime
    };

    //init bullet
    for (int i = 0; i < (sizeof(g_bullets) / sizeof(g_bullets[0])); i++) {
        g_bullets[i] = bullet;
    }
    //init enemy bullet
    for (int i = 0; i < (sizeof(enemy_bullets) / sizeof(enemy_bullets[0])); i++) {
        enemy_bullets[i] = bullet;
    }
    //init enemy
    for (int i = 0; i < (sizeof(g_enemy) / sizeof(g_enemy[0])); i++) {
        g_enemy[i] = enemy;
    }

    //init color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
}