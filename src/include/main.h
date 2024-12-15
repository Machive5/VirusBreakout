#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// -====================================object template=======================================
struct Player
{
    char avatar[300];
    int pos[2];
    int level;
    int direction[2];
    int health;
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
    int dmg;
    int health;
    int point;
};

struct Boss
{
    char avatar[5][300];
    int pos[2];
    int direction[2];
    float walk;
    float speed;
    int dmg;
    int health;
    float timeCooldown;
    float cooldown;
};

// ===========================================global variable============================================

int g_win_size[2] = { 0,0 };
int g_select_menu = 0;
int g_scene = 1;
bool play_again = true;

const char title[][300] = {
    "██╗   ██╗██╗██████╗ ██╗   ██╗███████╗    ██████╗ ██████╗ ███████╗ █████╗ ██╗  ██╗ ██████╗ ██╗   ██╗████████╗",
    "██║   ██║██║██╔══██╗██║   ██║██╔════╝    ██╔══██╗██╔══██╗██╔════╝██╔══██╗██║ ██╔╝██╔═══██╗██║   ██║╚══██╔══╝",
    "██║   ██║██║██████╔╝██║   ██║███████╗    ██████╔╝██████╔╝█████╗  ███████║█████╔╝ ██║   ██║██║   ██║   ██║   ",
    "╚██╗ ██╔╝██║██╔══██╗██║   ██║╚════██║    ██╔══██╗██╔══██╗██╔══╝  ██╔══██║██╔═██╗ ██║   ██║██║   ██║   ██║   ",
    " ╚████╔╝ ██║██║  ██║╚██████╔╝███████║    ██████╔╝██║  ██║███████╗██║  ██║██║  ██╗╚██████╔╝╚██████╔╝   ██║   ",
    "  ╚═══╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝    ╚═╝   "
};

const char level_up[][300] = {
    "██╗     ███████╗██╗   ██╗███████╗██╗         ██╗   ██╗██████╗ ██╗",
    "██║     ██╔════╝██║   ██║██╔════╝██║         ██║   ██║██╔══██╗██║",
    "██║     █████╗  ██║   ██║█████╗  ██║         ██║   ██║██████╔╝██║",
    "██║     ██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║         ██║   ██║██╔═══╝ ╚═╝",
    "███████╗███████╗ ╚████╔╝ ███████╗███████╗    ╚██████╔╝██║     ██╗",
    "╚══════╝╚══════╝  ╚═══╝  ╚══════╝╚══════╝     ╚═════╝ ╚═╝     ╚═╝"
};

const char game_over[][300] = {
    " ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ ",
    "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗",
    "██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝",
    "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗",
    "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║",
    " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝"
};


//---------------------------------set bulets---------------------------------------
struct Bullet g_bullets[100];
struct Bullet enemy_bullets[100];
int g_bullet_count = 1;
int g_bullet_rotation = 0;
float g_bullet_speed = 0.3;
int g_bullet_dmg = 5;

//--------------------------------set enemy------------------------------------------
int enemy_max_amount = 5;
int enemy_spawn_counter = 0;
float enemy_spawn_rate = 1.5;
float enemy_spawn_cooldown = 0;
int enemy_health = 5;
int enemy_point = 10;
struct Enemy g_enemy[100];

//---------------------------------set player--------------------------------------------
float player_invisible = 0;
int player_exp = 0;
int player_exp_max = 3;
int player_hp_max = 5;
bool player_level_up = false;
float player_score = 0;
//-------------------------------------------set bos----------------------------------
bool boss_spawned = false;
int boss_health = 100;
int boss_dmg = 10;
float boss_speed = 0.1f;
float boss_cooldown = 0.02f;

//----------------------------------inisiasi player---------------------------------
struct Player g_player = {
    "⚉",
    {0,0},  //pos
    1,      //level
    {0,0},  //dir
    5,      //health
    0,      //walk
    0.4f,   //speed
    0,      //time cooldown
    0.1f,   //cooldown
};

//---------------------------------inisiasi enemy---------------------------------------------
struct Enemy enemy = {
    "⚉",
    {0,0},  //pos
    {0,0},  //dir
    0,      //walk
    0.2f,   //speed
    1,      //dmg
    0,      //health
    0,      //point
};

//---------------------------------inisiasi Bos---------------------------------------------
struct Boss boss = {
    {" =#= ",
     "|X X|",
     "#o o#",
     "|X-X|",
     " =#= "},
    {0,0},  //pos
    {0,0},  //dir
    0,      //walk
    0.1f,   //speed
    10,      //dmg
    100,    //health
    0,      //time cooldown
    0.02f,      //cooldown
};

// ==================================function=========================================
void drawStartPage(WINDOW* win);
void drawArena(WINDOW* win);
void drawUpgradePage(WINDOW* win);
void drawStatusUI(WINDOW* win);
void drawGameOverUI(WINDOW* win);

void spawnBullets(WINDOW* win);
void spawnEnemy(WINDOW* win);
void spawnBos(WINDOW* window);
void bossBulletsSpawn(WINDOW* win);

void playerMovement(WINDOW* win);
void bulletsMovement(WINDOW* win);
void enemyMovement(WINDOW* win);
void bossMovement(WINDOW* window);
void bossBulletsMovement(WINDOW* win);

void playerAndBosCollisionCheck(WINDOW* win);
void bossBulletAndPlayerCollisionCheck(WINDOW* win);
void bossAndPlayerBulletCollisionCheck(WINDOW* win);
void enemyAndPlayerBulletCollisionCheck(WINDOW* win);
void playerAndEnemyCollisionCheck(WINDOW* win);

void init();
