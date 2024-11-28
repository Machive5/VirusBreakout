#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// -------------------------------------object template------------------------------------
struct Player
{
    char avatar[300];
    int pos[2];
    int level;
    int direction[2];
    float health;
    float walk;
    float speed;
    float timeCooldown;
    float cooldown;
};

struct Bullet
{
    char avatar[3];
    int pos[2];
    int direction[2];
    float dmg;
    float walk;
    float speed;
    int lifetime;
};

struct Enemy
{
    char avatar[300];
    int pos[2];
    int direction[2];
    float walk;
    float speed;
    float dmg;
    float health;
};

// -------------------------------global variable--------------------------------------

int g_win_size[2] = { 0,0 };
int g_select_menu = 0;
int g_scene = 1;

const char title[][300] = {
    "████████████████████████████████████████████████████████████████████████████████",
    "█▄─█─▄█▄─▄█▄─▄▄▀█▄─██─▄█─▄▄▄▄███▄─▄─▀█▄─▄▄▀█▄─▄▄─██▀▄─██▄─█─▄█─▄▄─█▄─██─▄█─▄─▄─█",
    "██▄▀▄███─███─▄─▄██─██─██▄▄▄▄─████─▄─▀██─▄─▄██─▄█▀██─▀─███─▄▀██─██─██─██─████─███",
    "▀▀▀▄▀▀▀▄▄▄▀▄▄▀▄▄▀▀▄▄▄▄▀▀▄▄▄▄▄▀▀▀▄▄▄▄▀▀▄▄▀▄▄▀▄▄▄▄▄▀▄▄▀▄▄▀▄▄▀▄▄▀▄▄▄▄▀▀▄▄▄▄▀▀▀▄▄▄▀▀"
};

//---------------------------------init bulets---------------------------------------
struct Bullet g_bullets[100];
int g_bullet_count = 1;
int g_bullet_rotation = 0;

//--------------------------------set enemy------------------------------------------
int enemy_max_amount = 5;
int enemy_spawn_counter = 0;
float enemy_spawn_rate = 1.5;
float enemy_spawn_cooldown = 0;
float enemy_health = 5;
struct Enemy g_enemy[100];


//----------------------------------init player---------------------------------
struct Player g_player = {
    "⚉",
    {0,0},  //pos
    1,      //level
    {0,0},  //dir
    100,    //health
    0,      //walk
    0.4f,   //speed
    0,      //time cooldown
    0.1f,   //cooldown
};

//---------------------------------enemy---------------------------------------------
struct Enemy enemy = {
    "⚉",
    {0,0},  //pos
    {0,0},  //dir
    0,      //walk
    0.2f,   //speed
    10,     //dmg
    5,      //health
};

// -------------------------------function-------------------------------------------
void draw_start_page(WINDOW* win);
void draw_arena(WINDOW* win);
void draw_player(WINDOW* win);
void spawn_bullets(WINDOW* win);
void draw_bullets(WINDOW* win);
void spawn_enemy(WINDOW* win);
void draw_enemy(WINDOW* win);