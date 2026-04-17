#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum Tile
{
    TILE_EMPTY = 0,
    TILE_WALL,
    TILE_PLAYER,
    TILE_NPC,
    TILE_ENEMY,
    TILE_BOSS,
    TILE_CHEST,
    TILE_EXIT
};

enum QuestState
{
    QUEST_NOT_STARTED = 0,
    QUEST_ACTIVE,
    QUEST_BOSS_DOWN,
    QUEST_COMPLETE
};

typedef struct
{
    int x;
    int y;
    int hp;
    int maxHp;
    int atk;
    int xp;
    int gold;
    int potions;
    int level;
} Player;

typedef struct
{
    int x;
    int y;
    int hp;
    int atk;
    bool alive;
    bool boss;
    const char *name;
} Enemy;

#define MAP_W 32
#define MAP_H 20
#define MAX_ENEMIES 5

static PrintConsole topScreen;
static PrintConsole bottomScreen;

static Player player;
static Enemy enemies[MAX_ENEMIES];
static enum QuestState questState = QUEST_NOT_STARTED;
static bool chestOpened = false;
static bool showQuestLog = false;
static bool gameWon = false;
static bool talkedToMarshal = false;

static const int marshalX = 3;
static const int marshalY = 3;
static const int chestX = 16;
static const int chestY = 10;
static const int exitX = 28;
static const int exitY = 17;

static char mapData[MAP_H][MAP_W + 1] = {
    "################################",
    "#..............#...............#",
    "#..P...........#...............#",
    "#..N...........#...............#",
    "#..............#.......###.....#",
    "#..............#...............#",
    "#......####....................#",
    "#.................E............#",
    "#......................####....#",
    "#...........####...............#",
    "#...............C..............#",
    "#.......E......................#",
    "#....................#####.....#",
    "#..............................#",
    "#.................E............#",
    "#..............######..........#",
    "#.........................B....#",
    "#...........................X..#",
    "#..............................#",
    "################################"
};

static void resetEnemies(void)
{
    enemies[0] = (Enemy){17, 7, 6, 2, true, false, "Raider"};
    enemies[1] = (Enemy){8, 11, 7, 2, true, false, "Raider"};
    enemies[2] = (Enemy){18, 14, 8, 3, true, false, "Raider Veteran"};
    enemies[3] = (Enemy){26, 16, 16, 4, true, true, "Dust Warden"};
    enemies[4] = (Enemy){-1, -1, 0, 0, false, false, ""};
}

static void resetGame(void)
{
    player.x = 3;
    player.y = 2;
    player.hp = 20;
    player.maxHp = 20;
    player.atk = 4;
    player.xp = 0;
    player.gold = 5;
    player.potions = 1;
    player.level = 1;

    questState = QUEST_NOT_STARTED;
    chestOpened = false;
    showQuestLog = false;
    gameWon = false;
    talkedToMarshal = false;
    resetEnemies();
}

static int findEnemyAt(int x, int y)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
            return i;
    }
    return -1;
}

static bool isWalkable(int x, int y)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
        return false;

    if (mapData[y][x] == '#')
        return false;

    if (findEnemyAt(x, y) >= 0)
        return false;

    return true;
}

static void tryLevelUp(void)
{
    const int threshold = player.level * 10;
    if (player.xp >= threshold)
    {
        player.xp -= threshold;
        player.level++;
        player.maxHp += 4;
        player.hp = player.maxHp;
        player.atk += 1;
    }
}

static void damagePlayer(int amount)
{
    player.hp -= amount;
    if (player.hp <= 0)
    {
        resetGame();
    }
}

static void rewardEnemyKill(const Enemy *enemy)
{
    player.gold += enemy->boss ? 15 : 4;
    player.xp += enemy->boss ? 12 : 5;
    tryLevelUp();
}

static void attackAdjacentEnemy(void)
{
    static const int dirs[4][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    for (int i = 0; i < 4; i++)
    {
        int tx = player.x + dirs[i][0];
        int ty = player.y + dirs[i][1];
        int idx = findEnemyAt(tx, ty);

        if (idx >= 0)
        {
            enemies[idx].hp -= player.atk;
            if (enemies[idx].hp <= 0)
            {
                enemies[idx].alive = false;
                rewardEnemyKill(&enemies[idx]);
                if (enemies[idx].boss)
                {
                    questState = QUEST_BOSS_DOWN;
                }
            }
            else
            {
                damagePlayer(enemies[idx].atk);
            }
            return;
        }
    }
}

static void interact(void)
{
    if (abs(player.x - marshalX) + abs(player.y - marshalY) == 1)
    {
        talkedToMarshal = true;
        if (questState == QUEST_NOT_STARTED)
        {
            questState = QUEST_ACTIVE;
        }
        return;
    }

    if (!chestOpened && abs(player.x - chestX) + abs(player.y - chestY) == 1)
    {
        chestOpened = true;
        player.gold += 10;
        player.potions += 1;
        return;
    }

    if (questState == QUEST_BOSS_DOWN && abs(player.x - exitX) + abs(player.y - exitY) == 1)
    {
        questState = QUEST_COMPLETE;
        gameWon = true;
        return;
    }
}

static void usePotion(void)
{
    if (player.potions > 0 && player.hp < player.maxHp)
    {
        player.potions--;
        player.hp += 8;
        if (player.hp > player.maxHp)
            player.hp = player.maxHp;
    }
}

static void movePlayer(int dx, int dy)
{
    int nx = player.x + dx;
    int ny = player.y + dy;
    if (isWalkable(nx, ny))
    {
        player.x = nx;
        player.y = ny;
    }
}

static void updateEnemies(void)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].alive)
            continue;

        int dist = abs(player.x - enemies[i].x) + abs(player.y - enemies[i].y);
        if (dist == 1)
        {
            damagePlayer(enemies[i].atk);
            continue;
        }

        if (dist <= 6)
        {
            int dx = 0;
            int dy = 0;
            if (player.x < enemies[i].x) dx = -1;
            if (player.x > enemies[i].x) dx = 1;
            if (player.y < enemies[i].y) dy = -1;
            if (player.y > enemies[i].y) dy = 1;

            int tryX = enemies[i].x + dx;
            int tryY = enemies[i].y;
            if (isWalkable(tryX, tryY) && !(tryX == player.x && tryY == player.y))
            {
                enemies[i].x = tryX;
            }
            else
            {
                tryX = enemies[i].x;
                tryY = enemies[i].y + dy;
                if (isWalkable(tryX, tryY) && !(tryX == player.x && tryY == player.y))
                {
                    enemies[i].y = tryY;
                }
            }
        }
    }
}

static void buildRenderBuffer(char out[MAP_H][MAP_W + 1])
{
    for (int y = 0; y < MAP_H; y++)
    {
        memcpy(out[y], mapData[y], MAP_W + 1);
    }

    if (!chestOpened)
        out[chestY][chestX] = 'C';

    if (questState == QUEST_BOSS_DOWN)
        out[exitY][exitX] = 'X';

    out[marshalY][marshalX] = 'M';

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].alive)
            out[enemies[i].y][enemies[i].x] = enemies[i].boss ? 'B' : 'E';
    }

    out[player.y][player.x] = '@';
}

static const char *questText(void)
{
    switch (questState)
    {
        case QUEST_NOT_STARTED:
            return "Talk to Marshal Rowan";
        case QUEST_ACTIVE:
            return "Defeat the Dust Warden";
        case QUEST_BOSS_DOWN:
            return "Reach the beacon exit";
        case QUEST_COMPLETE:
            return "Level complete";
        default:
            return "Unknown";
    }
}

static void renderTop(void)
{
    consoleSelect(&topScreen);
    printf("\x1b[2J");
    printf("ASHEN FRONTIER DS\n");
    printf("LV %d  HP %d/%d  ATK %d\n", player.level, player.hp, player.maxHp, player.atk);
    printf("XP %d  GOLD %d  POT %d\n", player.xp, player.gold, player.potions);
    printf("QUEST: %s\n\n", questText());

    if (gameWon)
    {
        printf("Beacon restored. Zone clear.\n");
        printf("START = restart the run\n");
    }
    else if (showQuestLog)
    {
        printf("Quest Log\n");
        printf("- Marshal Rowan posted a bounty\n");
        printf("- Loot the road if you can\n");
        printf("- Drop the Dust Warden\n");
        printf("- Light the beacon and leave\n");
    }
    else if (!talkedToMarshal)
    {
        printf("Find the Marshal at the outpost.\n");
        printf("A = interact\nB = attack\nX = quest log\n");
    }
    else if (questState == QUEST_ACTIVE)
    {
        printf("Road is hostile. Raiders close in.\n");
        printf("Chest at mid-route. Boss near shrine.\n");
    }
    else if (questState == QUEST_BOSS_DOWN)
    {
        printf("Boss down. The beacon is live.\n");
        printf("Head southeast and interact.\n");
    }
}

static void renderBottom(void)
{
    char frame[MAP_H][MAP_W + 1];

    buildRenderBuffer(frame);

    consoleSelect(&bottomScreen);
    printf("\x1b[2J");

    for (int y = 0; y < MAP_H; y++)
    {
        printf("%s\n", frame[y]);
    }

    printf("\nLegend: @ You  M Marshal  E Raider  B Boss  C Chest  X Exit\n");
}

static void initVideo(void)
{
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    consoleInit(&topScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
    consoleInit(&bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 30, 0, false, true);
}

int main(void)
{
    irqEnable(IRQ_VBLANK);
    initVideo();
    resetGame();

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();

        int down = keysDown();
        touchPosition touch;
        touchRead(&touch);

        if (down & KEY_START)
            resetGame();

        if (!gameWon)
        {
            if (down & KEY_UP) movePlayer(0, -1);
            if (down & KEY_DOWN) movePlayer(0, 1);
            if (down & KEY_LEFT) movePlayer(-1, 0);
            if (down & KEY_RIGHT) movePlayer(1, 0);
            if (down & KEY_A) interact();
            if (down & KEY_B) attackAdjacentEnemy();
            if (down & KEY_L) usePotion();
            if (down & KEY_X) showQuestLog = !showQuestLog;

            // Simple touch hook: tap lower-right corner to use potion.
            if (touch.py > 150 && touch.px > 190)
                usePotion();

            updateEnemies();
        }

        renderTop();
        renderBottom();
    }

    return 0;
}
