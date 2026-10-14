#ifndef PARSER_H
#define PARSER_H
#include "gamedata.h"
#include "cJSON.h"
#define MAX_WEAPON_TYPES 200
#define MAX_ENEMY_DEFS 200
#define MAX_PLAYER_TYPES 50
typedef struct { const char *name; int value; } EnumEntry;
void ParseWeaponArray(cJSON *array, WeaponType *weapons, int *i, int max);
static int LookupEnum(const EnumEntry *table, int count, const char *s, int fallback);
ProjType StringToProjType(const char *s);
FireModes StringToFireModes(const char *s);
void LoadWeapons(); 
const WeaponType *find_weapon(const char *name);
extern WeaponType weapons[MAX_WEAPON_TYPES];
extern int weapon_count;
typedef struct EnemyDefs{
    EnemyType type;
    float dx, dy;
    int hp;
    char symbol;            // was char *
    char *sprite_path;      // new
    int width, height;
    int cooldown_frames;
    EnemyBehavior behavior;
    char *weapon;
} EnemyDefs;
extern EnemyDefs enemy_defs[MAX_ENEMY_DEFS];
extern int enemy_def_count;
void LoadEnemyDefs(void);
#endif