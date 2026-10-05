#ifndef PARSER_H
#define PARSER_H
#include "gamedata.h"
#include "cJSON.h"
#define MAX_WEAPON_TYPES 100
#define MAX_ENEMY_TYPE 100
#define MAX_PLAYER_TYPES 50
//Only call functions starting with Load outside this header as all others are helper functions
typedef struct { const char *name; int value; } EnumEntry;
void ParseWeaponArray(cJSON *array, WeaponType *weapons, int *i, int max);
static int LookupEnum(const EnumEntry *table, int count, const char *s, int fallback);
ProjType StringToProjType(const char *s);
FireModes StringToFireModes(const char *s);
void LoadWeapons(); 
extern WeaponType weapons[MAX_WEAPON_TYPES];
extern int weapon_count;
#endif