#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include  "gamedata.h"
#include "string.h"
#include "parser.h"

WeaponType weapons[MAX_WEAPON_TYPES] = {0};
int weapon_count = 0;
//Various helper functions used in the weapon loading function
static int LookupEnum(const EnumEntry *table, int count, const char *s, int fallback) {
    if (s == NULL) return fallback;
    for (int i = 0; i < count; i++) {
        if (strcmp(s, table[i].name) == 0) return table[i].value;
    }
    fprintf(stderr, "[%s]len=%zu\n", s, strlen(s));
    fprintf(stderr, "Unknown enum string: %s\n", s);
    return fallback;
}

ProjType StringToProjType(const char *s) {
    static const EnumEntry table[] = {
        {"BULLET", BULLET}, {"LASER", LASER}, {"BOMB", BOMB},
        {"MISSILE", MISSILE}, {"PLASMA", PLASMA}, {"EMP", EMP},
        {"CHAINLIGHTNING", CHAINLIGHTNING}
    };
    return (ProjType)LookupEnum(table, sizeof table / sizeof table[0], s, BULLET);
}

FireModes StringToFireModes(const char *s) {
    static const EnumEntry table[] = {
        {"REGULAR",REGULAR}, {"BURST_FIRE", BURST_FIRE},
        {"RAPID_FIRE", RAPID_FIRE}, {"SUPERCHARGE", SUPERCHARGE},
        {"CHARGING", CHARGING}
    };
    return (FireModes)LookupEnum(table, sizeof table / sizeof table[0], s, REGULAR);
}

WeaponID StringToWeaponID(const char *s) {
    static const EnumEntry table[] = {
        {"EMPTY_ID", EMPTY_ID}, {"AUTOPISTOL_ID", AUTOPISTOL_ID},
        {"MACHINEGUN_ID", MACHINEGUN_ID}, {"LASRIFLE_PLAYER_ID", LASRIFLE_PLAYER_ID},
        {"BOMB_PLAYER_ID", BOMB_PLAYER_ID}, {"PLASMARIFLE_PLAYER_ID", PLASMARIFLE_PLAYER_ID},
        {"MISSILE_PLAYER_ID", MISSILE_PLAYER_ID}, {"EMP_ID", EMP_ID},
        {"LIGHTNING_ID", LIGHTNING_ID}, {"SHOTGUN_ID", SHOTGUN_ID},
        {"GRUNT_WEAPON_ID", GRUNT_WEAPON_ID}, {"LASERCANNON_ID", LASERCANNON_ID},
        {"PLASMACANNON_ID", PLASMACANNON_ID}, {"RAPIDFIRE_RIFLE_ID", RAPIDFIRE_RIFLE_ID},
        {"LASER_RIFLE_ENEMY_ID", LASER_RIFLE_ENEMY_ID}, {"BOMB_ENEMY_ID", BOMB_ENEMY_ID},
        {"HUNTER_RIFLE_ID", HUNTER_RIFLE_ID}, {"JET_CANNON_ID", JET_CANNON_ID},
        {"FLYFORT_CANNON_ID", FLYFORT_CANNON_ID}, {"SPIRAL_CANNON_ID", SPIRAL_CANNON_ID},
        {"CARRIER_CANNON_ID", CARRIER_CANNON_ID}, {"CARRIER_FLAK_ID", CARRIER_FLAK_ID},
        {"FRIGATE_FLAK_ID", FRIGATE_FLAK_ID}, {"FRIGATE_LASER_ID", FRIGATE_LASER_ID},
        {"MINIGUN_ID", MINIGUN_ID}, {"GRAND_CANNON_ID", GRAND_CANNON_ID},
        {"PLASMA_STORM_ID", PLASMA_STORM_ID}, {"MISSILE_STORM_ID", MISSILE_STORM_ID}
    };
    return (WeaponID)LookupEnum(table, sizeof table / sizeof table[0], s, EMPTY_ID);
}
void ParseWeaponArray(cJSON *array, WeaponType *weapons, int *i, int max) {
    cJSON *item;
    cJSON_ArrayForEach(item, array) {
        if (*i >= max) break;
        WeaponType *w = &weapons[*i];

        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "display_name");
        cJSON *cd   = cJSON_GetObjectItemCaseSensitive(item, "cooldown_frames");
        cJSON *num  = cJSON_GetObjectItemCaseSensitive(item, "number");
        cJSON *ang  = cJSON_GetObjectItemCaseSensitive(item, "angle");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(item, "type");
        cJSON *mode = cJSON_GetObjectItemCaseSensitive(item, "modes");
        cJSON *id   = cJSON_GetObjectItemCaseSensitive(item, "weapon_id");

        if (cJSON_IsString(name)) w->display_name = strdup(name->valuestring);
        if (cJSON_IsNumber(cd))   w->cooldown_frames = cd->valueint;
        if (cJSON_IsNumber(num))  w->number = num->valueint;
        if (cJSON_IsNumber(ang))  w->angle = (float)ang->valuedouble;

        if (cJSON_IsString(type)) w->type = StringToProjType(type->valuestring);
        if (cJSON_IsString(mode)) w->modes = StringToFireModes(mode->valuestring);
        if (cJSON_IsString(id))   w->weapon_id = StringToWeaponID(id->valuestring);

        (*i)++;
    }
}
//Function to load weapons
void LoadWeapons(){
FILE* weapon_file = fopen("Data/weapons.json", "rb");
if(weapon_file == NULL) {
        fprintf(stderr, "ERROR:Weapon file not opened!");
        return;   
    }
fseek(weapon_file, 0, SEEK_END); 
long int size = ftell(weapon_file);
 rewind(weapon_file);
char *weapons_raw = malloc(size + 1);
 if (weapons_raw == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
         free(weapons_raw);
        return ;
    }
size_t n = fread(weapons_raw, 1, size, weapon_file);
weapons_raw[n] = '\0';
fclose(weapon_file);
cJSON *root = cJSON_Parse(weapons_raw);
if (root == NULL) {
    fprintf(stderr, "JSON parse error\n");
    fclose(weapon_file);
    free(weapons_raw);
    return;
}

cJSON *player = cJSON_GetObjectItemCaseSensitive(root, "player_weapons");
cJSON *enemy  = cJSON_GetObjectItemCaseSensitive(root, "enemy_weapons");

int i = 0;
if (cJSON_IsArray(player)) ParseWeaponArray(player, weapons, &i, 100);
if (cJSON_IsArray(enemy))  ParseWeaponArray(enemy,  weapons, &i, 100);
weapon_count = i;  

cJSON_Delete(root);  
 fprintf(stderr, "\n Succesfully loaded weapons.\n");
free(weapons_raw);
}
