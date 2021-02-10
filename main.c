#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#define COLOR_RED "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

//printf the errors with the color red :D
//ask the user if they want to exit the game in every function
typedef struct Ship
{
    int length, width;
} Ship;
typedef struct User
{
    char name[50], str_coins[10];
    int coins, tmp_coins, missile;
    bool winner;
} User;
typedef struct Coord
{
    int x, y;
} Coord;
typedef struct node
{
    Ship info;
    Coord head, tail;
    int hit;
    struct node *next;
} node;
typedef struct Playback
{
    char **map;
    struct Playback *next;
} Playback;
//global vars
int maprow = 10, mapcol = 10;
int numOfTotalShips = 10, largest_ship_area = 5;
Ship ships[30];
User users[2];

//prototypes
void settings(); //bonus
void init_ship_info();
char **init_map();
bool change_ships();
char *username();
int search(char name[50]);
void battleWithFriend(char **map1, char **map2, char **map_enemy1, char **map_enemy2, node *head1, node *head2);
void battleWithBot(char **map1, char **map2, char **map_enemy1, char **map_enemy2, node *head1, node *head2);
char **complete_explosion(char **map, char **map_enemy, node *curr_ship);
void missile_attack(node **head, char ***map, char ***map_enemy, int *coins);
node *createNode(int len, int width);
node *putships_manual(char ***map);
node *putships_auto(char ***map);
char **update_map(int x_head, int y_head, int x_tail, int y_tail, char **map);
bool check_map(int x_head, int y_head, int x_tail, int y_tail, char **map);
void displayMap(char **map);
void scoreboard(); //print the sorted list of each user's coins
//void savegame(int *coin1, int *coin2);   //i have no idea about this, maybe a .bin file or sth
void savescores();
void printlist(node *head);

int main()
{
    char **map1 = (char **)malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++)
        map1[i] = (char *)malloc(mapcol * sizeof(char));
    char **map2 = (char **)malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++)
        map2[i] = (char *)malloc(mapcol * sizeof(char));
    char **map_enemy1 = (char **)malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++)
        map_enemy1[i] = (char *)malloc(mapcol * sizeof(char));
    char **map_enemy2 = (char **)malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++)
        map_enemy2[i] = (char *)malloc(mapcol * sizeof(char));

    node *head1, *head2;

    init_ship_info();
    int choice, n, i;
    do
    {
        printf("\t1.Play with a friend\n\t2.Play with bot\n\t3.Load last game\n\t4.Playback last game\n\t5.Settings\n\t6.Scoreboard\n\t7.Exit\n");
        scanf("%d", &choice);
        getchar();
        system("cls");
        switch (choice)
        {
        case 1:
            printf("\tPlayer 1:\n");
            strcpy(users[0].name, username());
            users[0].coins = search(users[0].name);
            system("cls");
            printf("\tPlayer 2:\n");
            strcpy(users[1].name, username());
            users[1].coins = search(users[1].name);
            system("cls");
            map1 = init_map();
            map2 = init_map();
            map_enemy1 = init_map();
            map_enemy2 = init_map();
            //put the ships on the board
            printf("\t%s\n", users[0].name);
            printf("\tThis is your map:\n");
            displayMap(map1);
            printf("\tHow would you like to locate your ships in the map?\n");
            printf("\t1) Manual\n\t2) Auto\n");
            scanf("%d", &n);
            if (n == 1)
                head1 = putships_manual(&map1);
            else if (n == 2)
            {
                i = 1;
                while (i)
                {
                    map1 = init_map(); //reset the map
                    head1 = putships_auto(&map1);
                    printf("\tPress 0 to continue\n\tPress 1 to get another map\n");
                    scanf("%d", &i);
                    getchar();
                }
            }
            Sleep(1000);
            system("cls");

            printf("\t%s\n", users[1].name);
            printf("\tThis is your map:\n");
            displayMap(map2);
            printf("\tHow would you like to locate your ships in the map?\n");
            printf("\t1) Manual\n\t2) Auto\n");
            scanf("%d", &n);
            if (n == 1)
                head2 = putships_manual(&map2);
            else if (n == 2)
            {
                i = 1;
                while (i)
                {
                    map2 = init_map(); //reset the map
                    head2 = putships_auto(&map2);
                    printf("\tPress 0 to continue\n\tPress 1 to get another map\n");
                    scanf("%d", &i);
                    getchar();
                }
            }
            Sleep(1000);
            system("cls");
            battleWithFriend(map1, map2, map_enemy1, map_enemy2, head1, head2);
            break;
        case 2:
            strcpy(users[0].name, username());
            users[0].coins = search(users[0].name);
            system("cls");
            map1 = init_map();
            map2 = init_map();
            map_enemy1 = init_map();
            map_enemy2 = init_map();
            //put the ships on the board
            printf("\t%s\n", users[0].name);
            printf("\tThis is your map:\n");
            displayMap(map1);
            printf("\tHow would you like to locate your ships in the map?\n");
            printf("\t1) Manual\n\t2) Auto\n");
            scanf("%d", &n);
            if (n == 1)
                head1 = putships_manual(&map1);
            else if (n == 2)
            {
                i = 1;
                while (i)
                {
                    map1 = init_map(); //reset the map
                    head1 = putships_auto(&map1);
                    printf("\tPress 0 to continue\n\tPress 1 to get another map\n");
                    scanf("%d", &i);
                    getchar();
                }
            }
            Sleep(1000);
            system("cls");
            head2 = putships_auto(&map2);
            battleWithBot(map1, map2, map_enemy1, map_enemy2, head1, head2);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            settings();
            system("PAUSE");
            system("cls");
            break;
        case 6:
            scoreboard();
            system("pause");
            break;
        case 7:
            break;
        default:
            printf("\tWrong input!\n");
            break;
        }
    } while (choice != 7);

    printf(COLOR_GREEN "Bye :)");
    return 0;
}

void scoreboard()
{
    //get data of all the users
    //User *all = (User*)malloc(sizeof(User));
    User all[500];
    FILE *fp = fopen("users.txt", "r");
    int i, j, num;
    for (i = 0; !feof(fp); i++)
    {
        //all = (User*)realloc(all, (i + 1) * sizeof(User));
        fscanf(fp, "%s %s", &all[i].name, &all[i].str_coins);
        all[i].coins = atoi(all[i].str_coins);
    }
    //sort the users with respect to their score
    num = i;
    for (i = 0; i < num - 1; i++)
    {
        for (j = 0; j < num - i - 1; j++)
        {
            if (all[j].coins < all[j + 1].coins)
            {
                int tmpscore;
                char *tmp;
                strcpy(tmp, all[j].name);
                tmpscore = all[j].coins;
                strcpy(all[j].name, all[j + 1].name);
                all[j].coins = all[j + 1].coins;
                strcpy(all[j + 1].name, tmp);
                all[j + 1].coins = tmpscore;
            }
        }
    }
    //print the sorted list
    for (i = 0; i < num; i++)
    {
        printf("\t%s %d\n", all[i].name, all[i].coins);
    }
}

void settings()
{
    int choice, i, area = 0;
    printf("\t1.Ships\n\t2.Map size\n\t3.Theme\n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        printf("\tEnter the number of ships you would like to have:");
        scanf("%d", &numOfTotalShips);
        getchar();
        while (!change_ships())
        {
            printf("\tTry again\n");
            printf("\tEnter the number of ships you would like to have:");
            scanf("%d", &numOfTotalShips);
            getchar();
        }
        break;
    case 2:
        i = 0;
        while (area <= 2 * (maprow + mapcol) - 4)
        {
            area += (ships[i].length + 1) * (ships[i].width + 1);
            i++;
        }
        for (; i < numOfTotalShips; i++)
            area += (ships[i].length + 2) * (ships[i].width + 2);
        while (1)
        {
            printf("\tCurrent map size is %dx%d\n", maprow, mapcol);
            printf("\tEnter the number of rows of the new map:");
            scanf("%d", &maprow);
            printf("\tEnter the number of columns of the new map:");
            scanf("%d", &mapcol);
            getchar();
            if (mapcol * maprow < area)
                printf("\tMap size is too small! Choose another size\n");
            else
                break;
        }
        break;
    case 3:
    default:
        printf("\tWrong input!\n");
        break;
    }
}

void init_ship_info()
{
    int i;
    //we have 10 ships and the map is 10x10
    ships[0].length = 5;
    ships[0].width = 1;
    for (i = 1; i <= 2; i++)
    {
        ships[i].length = 3;
        ships[i].width = 1;
    }
    for (i = 3; i <= 5; i++)
    {
        ships[i].length = 2;
        ships[i].width = 1;
    }
    for (i = 6; i <= 9; i++)
    {
        ships[i].length = 1;
        ships[i].width = 1;
    }
}

char **init_map()
{
    char **map = (char **)malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++)
        map[i] = (char *)malloc(mapcol * sizeof(char));
    for (int i = 0; i < maprow; i++)
    {
        for (int j = 0; j < mapcol; j++)
            map[i][j] = '.';
    }

    return map;
}

bool change_ships()
{
    int i, count;
    int len, width, num, area = 0;

    for (i = 0; i < numOfTotalShips;)
    {
        printf("\tSHIP %d\n", i + 1);
        printf("\tLength:");
        scanf("%d", &len);
        printf("\tWidth:");
        scanf("%d", &width);
        printf("\tHow many?");
        scanf("%d", &num);
        getchar();
        int j = 1;
        while ((area <= 2 * (maprow + mapcol) - 4) && j <= num)
        {
            area += (len + 1) * (width + 1);
            j++;
        }
        while (j <= num)
        {
            area += (len + 2) * (width + 2);
            j++;
        }
        if (area > maprow * mapcol)
        {
            printf("\tCannot locate the ships in the map!\n");
            return false;
        }
        for (count = 0; count < num; count++, i++)
        {
            ships[i].length = len;
            ships[i].width = width;
        }
    }
    //find the largest ship
    area = 0;
    int max = ships[i].length * ships[i].width;
    for (i = 0; i < numOfTotalShips; i++)
    {
        int tmp = ships[i].length * ships[i].width;
        if (tmp > max)
            max = tmp;
    }
    largest_ship_area = max;
    printf("\tChanges are successfuly applied :)\n");
    return true;
}

char *username()
{
    char *name = (char *)malloc(50 * sizeof(char));
    int choice;
    printf("\t1.Create new user\n\t2.Choose from available users\n");
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
        printf("\tEnter a username:");
        gets(name);
        while (search(name) != 0)
        {
            printf("\t%s already exists, try another one:", name);
            gets(name);
        }
        FILE *fp = fopen("users.txt", "a");
        fprintf(fp, "%s 0\n", name);
        fclose(fp);
    }
    else if (choice == 2)
    {
        FILE *fp = fopen("users.txt", "r");
        char tmpname[50], coins[10];
        printf("\tThis is the list of the usernames:\n");
        while (!feof(fp))
        {
            fscanf(fp, "%s %s", tmpname, coins);
            printf("\t%s %s\n", tmpname, coins);
        }
        printf("\tEnter the username you want to choose\n");
        gets(name);
        while (search(name) == 0)
        {
            printf("\t%s does not exist! Try again:", name);
            gets(name);
        }
        fclose(fp);
    }
    return name;
}
int search(char name[50])
{
    char tmpname[50], coins[10];
    FILE *fpin = fopen("users.txt", "r");
    while (!feof(fpin))
    {
        fscanf(fpin, "%s %s", tmpname, coins);
        if (strcmp(name, tmpname) == 0)
        {
            fclose(fpin);
            int score = atoi(coins);
            return score;
        }
    }
    fclose(fpin);
    return 0;
}

node *createNode(int len, int width)
{
    node *res = (node *)malloc(sizeof(node));
    res->info.length = len;
    res->info.width = width;
    res->next = NULL;
    return res;
}

node *putships_manual(char ***map)
{
    printf("\tNOTE: For entering the coordinates of the start and the end of a ship\n");
    printf("\tthat has a width larger than 1, follow the instruction below:\n");
    printf("\tSuppose the ship is like this:\n");
    printf("\t....\n");
    printf("\t....\n");
    printf("\t1) For entering the x,y of the start of the ship, enter the coordinates of the first dot\n");
    printf("\t2) For entering the x,y of the end   of the ship, enter the coordinates of the last  dot\n");
    printf("\t");
    system("PAUSE");

    int i, x, y, deltaX, deltaY;
    node *head = (node *)malloc(sizeof(node));
    while (1)
    {
        printf("SHIP 1\n");
        head->info.length = ships[0].length;
        head->info.width = ships[0].width;
        printf("Width: %d Length: %d\n", ships[0].width, ships[0].length);
        printf("Enter the coordinates (x,y) of the start of this ship\n");
        scanf("%d %d", &x, &y);
        getchar();
        while (x < 0 || x >= mapcol || y < 0 || y >= maprow)
        {
            printf("Invalid input! Try again:\n");
            scanf("%d %d", &x, &y);
            getchar();
        }
        head->head.x = x;
        head->head.y = y;
        printf("Enter the coordinates (x,y) of the end   of this ship\n");
        scanf("%d %d", &x, &y);
        getchar();
        while (x < 0 || x >= mapcol || y < 0 || y >= maprow)
        {
            printf("Invalid input! Try again:\n");
            scanf("%d %d", &x, &y);
            getchar();
        }
        head->tail.x = x;
        head->tail.y = y;
        head->next = NULL;
        deltaX = head->tail.x - head->head.x + 1;
        deltaY = head->tail.y - head->tail.y + 1;
        if ((deltaX == head->info.length || deltaY == head->info.length) && (deltaX == head->info.width || deltaY == head->info.width))
            break;
        else
            printf("\tCoordinates does not match the size of the ship!\n");
    }
    head->hit = 0;
    //update the map
    *map = update_map(head->head.x, head->head.y, head->tail.x, head->tail.y, *map);
    displayMap(*map);

    node *tmp = head, *new;
    for (i = 1; i < numOfTotalShips; i++)
    {
        while (1)
        {
            printf("SHIP %d\n", i + 1);
            new = createNode(ships[i].length, ships[i].width);
            printf("Width: %d Length: %d\n", ships[i].width, ships[i].length);
            printf("Enter the coordinates (x,y) of the start of this ship\n");
            scanf("%d %d", &x, &y);
            getchar();
            while (x < 0 || x >= mapcol || y < 0 || y >= maprow)
            {
                printf("Invalid input! Try again:\n");
                scanf("%d %d", &x, &y);
                getchar();
            }
            new->head.x = x;
            new->head.y = y;
            printf("Enter the coordinates (x,y) of the end   of this ship\n");
            scanf("%d %d", &x, &y);
            getchar();
            while (x < 0 || x >= mapcol || y < 0 || y >= maprow)
            {
                printf("Invalid input! Try again:\n");
                scanf("%d %d", &x, &y);
                getchar();
            }
            new->tail.x = x;
            new->tail.y = y;
            new->next = NULL;
            deltaX = new->tail.x - new->head.x + 1;
            deltaY = new->tail.y - new->head.y + 1;
            if ((deltaX == new->info.length || deltaY == new->info.length) && (deltaX == new->info.width || deltaY == new->info.width))
            {
                if (check_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map))
                    break;
                else
                    printf("\tCannot locate this ship! Try again\n");
            }
            else
            {
                printf("\tCoordinates does not match the size of the ship!\n");
            }
        }

        *map = update_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map);
        system("cls");
        displayMap(*map);
        new->hit = 0;
        tmp->next = new;
        tmp = tmp->next;
    }
    return head;
}

node *putships_auto(char ***map)
{
    srand(time(NULL));
    int botShipDirection = rand() % 2;
    int i, x, y, deltaX, deltaY;
    node *tmp, *new;
    node *head = (node *)malloc(sizeof(node));
    head = createNode(ships[0].length, ships[0].width);
    if (botShipDirection == 0)
    { //vertical
        head->head.x = rand() % (mapcol - ships[0].width);
        head->head.y = rand() % (maprow - ships[0].length);
        head->tail.x = head->head.x + ships[0].width - 1;
        head->tail.y = head->head.y + ships[0].length - 1;
    }
    else
    { //horizontal
        head->head.x = rand() % (mapcol - ships[0].length);
        head->head.y = rand() % (maprow - ships[0].width);
        head->tail.x = head->head.x + ships[0].length - 1;
        head->tail.y = head->head.y + ships[0].width - 1;
    }
    *map = update_map(head->head.x, head->head.y, head->tail.x, head->tail.y, *map);
    head->hit = 0;
    tmp = head;
    for (int i = 1; i < numOfTotalShips; i++)
    {
        new = createNode(ships[i].length, ships[i].width);
        botShipDirection = rand() % 2;
        while (1)
        {
            if (botShipDirection == 0)
            { //vertical
                new->head.x = rand() % (mapcol - ships[i].width);
                new->head.y = rand() % (maprow - ships[i].length);
                new->tail.x = new->head.x + ships[i].width - 1;
                new->tail.y = new->head.y + ships[i].length - 1;
            }
            else
            { //horizontal
                new->head.x = rand() % (mapcol - ships[i].length);
                new->head.y = rand() % (maprow - ships[i].width);
                new->tail.x = new->head.x + ships[i].length - 1;
                new->tail.y = new->head.y + ships[i].width - 1;
            }
            if (check_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map))
                break;
        }
        *map = update_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map);
        new->hit = 0;
        tmp->next = new;
        tmp = tmp->next;
    }
    system("cls");
    displayMap(*map);
    return head;
}

char **update_map(int x_head, int y_head, int x_tail, int y_tail, char **map)
{
    int x, y;
    for (y = y_head; y <= y_tail; y++)
    {
        for (x = x_head; x <= x_tail; x++)
        {
            map[y][x] = 's';
        }
    }
    for (y = (y_head - 1 >= 0 ? y_head - 1 : y_head); y <= (y_tail + 1 < maprow ? y_tail + 1 : y_tail); y++)
    {
        for (x = (x_head - 1 >= 0 ? x_head - 1 : x_head); x <= (x_tail + 1 < mapcol ? x_tail + 1 : x_tail); x++)
        {
            if (map[y][x] != 's')
                map[y][x] = 'w';
        }
    }
    return map;
}

void printlist(node *head)
{
    node *tmp = head;
    for (int i = 1; tmp; i++)
    {
        printf("ship %d\n", i);
        printf("%dx%d\n", tmp->info.length, tmp->info.width);
        printf("(%d,%d) (%d,%d)\n", tmp->head.x, tmp->head.y, tmp->tail.x, tmp->tail.y);
        printf("hits: %d\n", tmp->hit);
        tmp = tmp->next;
    }
    system("PAUSE");
}

bool check_map(int x_head, int y_head, int x_tail, int y_tail, char **map)
{
    int x, y;
    for (y = y_head; y <= y_tail; y++)
    {
        for (x = x_head; x <= x_tail; x++)
        {
            if (map[y][x] == 's')
            {
                //printf("\tCannot locate this ship! Try again\n");
                return false;
            }
            if (map[y][x] == 'w')
            {
                //printf("\tCannot locate this ship! Try again\n");
                return false;
            }
        }
    }
    return true;
}

void displayMap(char **map)
{
    printf("\t  ");
    for (int i = 0; i < mapcol; i++)
        printf("%d ", i);
    printf("\n");
    for (int i = 0; i < maprow; i++)
    {
        printf("\t%d ", i);
        for (int j = 0; j < mapcol; j++)
        {
            if (map[i][j] == 'w')
            {
                printf(COLOR_CYAN "%c" COLOR_RESET " ", map[i][j]);
            }
            else if (map[i][j] == 'e')
            {
                printf(COLOR_YELLOW "%c" COLOR_RESET " ", map[i][j]);
            }
            else if (map[i][j] == 'c')
            {
                printf(COLOR_RED "%c" COLOR_RESET " ", map[i][j]);
            }
            else if (map[i][j] == 's')
            {
                printf(COLOR_GREEN "%c" COLOR_RESET " ", map[i][j]);
            }
            else if (map[i][j] == 'x')
                printf(COLOR_RED "%c" COLOR_RESET " ", map[i][j]);
            else
            {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }
}

char **complete_explosion(char **map, char **map_enemy, node *curr_ship)
{
    int x_head = curr_ship->head.x, y_head = curr_ship->head.y;
    int x_tail = curr_ship->tail.x, y_tail = curr_ship->tail.y;
    int x, y;
    for (y = (y_head - 1 >= 0 ? y_head - 1 : y_head); y <= (y_tail + 1 <= maprow ? y_tail + 1 : y_tail); y++)
    {
        for (x = (x_head - 1 >= 0 ? x_head - 1 : x_head); x <= (x_tail + 1 <= mapcol ? x_tail + 1 : x_tail); x++)
        {
            if (map[y][x] == 's' || map[y][x] == 'e')
                map_enemy[y][x] = 'c';
            if (map[y][x] == 'w')
                map_enemy[y][x] = 'w';
        }
    }
    return map_enemy;
}

void battleWithFriend(char **map1, char **map2, char **map_enemy1, char **map_enemy2, node *head1, node *head2)
{   
    users[0].missile = 1;
    users[1].missile = 1;
    int x, y, choice;
    node *curr, *prev;
    bool bonus;
    int coins1 = 0, coins2 = 0;

    while (head1 && head2)
    {
        //printf("\t" COLOR_YELLOW "%s      %s", users[0].name, users[1].name);
        //printf(COLOR_RESET "\t  %d             %d\n", coins1, coins2);
        bonus = true;
        Sleep(1000); system("cls");
        printf("\t%s\n", users[0].name);
        printf("\t1.Attack\n\t2.Missile attack\n\t3.Save and quit\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            //displayMap(map1);
            while (bonus)
            {
                if (head1 == NULL)
                    break;
                system("cls");
                printf(COLOR_YELLOW "\t%s: ", users[0].name);
                printf(COLOR_RESET "%d\n", coins1);
                printf(COLOR_YELLOW "\t%s: ", users[1].name);
                printf(COLOR_RESET "%d\n", coins2);
                displayMap(map1);
                printf("\n");
                displayMap(map_enemy1); //enemy's map before shooting
                printf("\tEnter the x,y coordinates of the target\n");
                scanf("%d %d", &x, &y);
                system("CLS");
                if (map_enemy1[y][x] == '.' && (map2[y][x] == '.' || map2[y][x] == 'w'))
                {
                    map2[y][x] = 'x';
                    map_enemy1[y][x] = 'w';
                    bonus = false;
                }
                else if (map2[y][x] == 's')
                {
                    //find the target ship in the linked list
                    curr = head2;
                    while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    if (curr->hit != curr->info.length * curr->info.width - 1)
                    { //the ship is not completely exploded
                        map2[y][x] = 'e';
                        map_enemy1[y][x] = 'e';
                        coins1++;
                        curr->hit++;
                    }
                    else
                    {
                        map_enemy1 = complete_explosion(map2, map_enemy1, curr);
                        map2[y][x] = 'e';
                        printf("Complete explosion!\n");
                        //remove the ship from the linked list
                        if (curr == head2)
                            head2 = head2->next;
                        else
                            prev->next = curr->next;
                        int destroyed_ship_area = curr->info.length * curr->info.width;
                        int score = 5 * largest_ship_area / destroyed_ship_area;
                        coins1 += score + 1;
                        if (head2 == NULL)
                            break;
                    }
                }
                else if (map2[y][x] == 'e' || map2[y][x] == 'x')
                    printf("\tYou have already hit this target! Try again\n");
                displayMap(map1);
                displayMap(map_enemy1); //show the result
                Sleep(1000);
            }
        }
        else if (choice == 2)
        {
            if (coins1 >= 100 && users[0].missile != 0)
            {
                users[0].missile = 0; //use the 1 chance to launch a missile attack
                coins1 -= 100;        //pay 100 coins
                displayMap(map1);
                displayMap(map_enemy1);
                missile_attack(&head2, &map2, &map_enemy1, &coins1);
            }
            else if (coins1 < 100)
                printf("\tYou don't have enough coins!\n");
            else if (coins1 >= 100 && users[0].missile == 0)
                printf("\tYou can't perform missile attack anymore :(\n");
        }
        else if (choice == 3)
        {
            //savegame(&coins1, &coins2);
        }

        bonus = true;
        Sleep(1000); system("cls");
        printf("\t%s\n", users[1].name);
        printf("\t1.Attack\n\t2.Missile attack\n\t3.Save and quit\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            //displayMap(map2);
            while (bonus)
            {
                if (head2 == NULL)
                    break;
                system("cls");
                printf(COLOR_YELLOW "\t%s: ", users[0].name);
                printf(COLOR_RESET "%d\n", coins1);
                printf(COLOR_YELLOW "\t%s: ", users[1].name);
                printf(COLOR_RESET "%d\n", coins2);
                displayMap(map2); //player2's map
                printf("\n");
                displayMap(map_enemy2); //enemy's map before shooting
                printf("\tEnter the x,y coordinates of the target\n");
                scanf("%d %d", &x, &y);
                system("cls");
                if (map1[y][x] == '.' || map1[y][x] == 'w')
                {
                    map1[y][x] = 'x';
                    map_enemy2[y][x] = 'w';
                    bonus = false;
                }
                else if (map1[y][x] == 's')
                {
                    //find the target ship in the linked list
                    curr = head1;
                    while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    if (curr->hit != curr->info.length * curr->info.width - 1)
                    {
                        map1[y][x] = 'e';
                        map_enemy2[y][x] = 'e';
                        coins2++;
                        curr->hit++;
                    }
                    else
                    {
                        printf("Complete explosion!\n");
                        map_enemy2 = complete_explosion(map1, map_enemy2, curr);
                        map1[y][x] = 'e';
                        //unlink the current ship from the linked list
                        if (curr == head1)
                            head1 = head1->next;
                        else
                            prev->next = curr->next;
                        int destroyed_ship_area = curr->info.length * curr->info.width;
                        int score = 5 * largest_ship_area / destroyed_ship_area;
                        coins2 += score;
                        if (head1 == NULL)
                            break;
                    }
                }
                else if (map1[y][x] == 'e' || map1[y][x] == 'x')
                    printf("\tYou have already hit this target! Try again\n");
                displayMap(map2);
                displayMap(map_enemy2); //show the result
                Sleep(1000);
            }
        }
        else if (choice == 2)
        {
            if (coins2 >= 100 && users[1].missile != 0)
            {
                users[1].missile = 0; //use the 1 chance to launch a missile attack
                coins2 -= 100;        //pay 100 coins
                displayMap(map2);
                displayMap(map_enemy2);
                missile_attack(&head1, &map1, &map_enemy2, &coins2);
            }
            else if (coins2 < 100)
                printf("\tYou don't have enough coins!\n");
            else if (coins2 >= 100 && users[1].missile == 0)
                printf("\tYou can't perform missile attack anymore :(\n");
        }
        else if (choice == 3)
        {
            users[0].tmp_coins = coins1;
            users[1].tmp_coins = coins2;
        }
        else
            printf("\tInvalid input!\n");
    }
    //end of the game
    if (head1 == NULL && head2 != NULL)
    {
        printf("\t%s won!\n", users[1].name);
        users[1].coins += coins2;
        users[0].coins += coins1 / 2;
    }
    else if (head2 == NULL && head1 != NULL)
    {
        printf("\t%s won!\n", users[0].name);
        users[0].coins += coins1;
        users[1].coins += coins2 / 2;
    }
    else
    {
        printf("\tIt's a tie\n");
        users[1].coins += coins2;
        users[0].coins += coins1;
    }
    savescores();
}

void battleWithBot(char **map1, char **map2, char **map_enemy1, char **map_enemy2, node *head1, node *head2)
{
    srand(time(0));
    int choice, difficulty;
    printf("\tChoose the game's difficulty:\n\t(1)easy\n\t(2)hard\n");
    scanf("%d", &difficulty);
    getchar();
    system("pause");
    //missile = 0;
    users[0].missile = 0; // --> human
    users[1].missile = 0; // --> bot
    int x, y;
    node *curr, *prev;
    bool bonus;
    int coins1 = 0, coins2 = 0;
    while (head1 && head2)
    {
        bonus = true;
        printf("\t%s\n", users[0].name);
        printf("\t1.Attack\n\t2.Missile attack\n\t3.Save and quit\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            while (bonus)
            {
                if (head1 == NULL)
                    break;
                system("cls");
                printf(COLOR_YELLOW "\t%s: ", users[0].name);
                printf(COLOR_RESET "%d\n", coins1);
                printf(COLOR_YELLOW "\tBot: ");
                printf(COLOR_RESET "%d\n", coins2);
                displayMap(map1);
                printf("\n");
                displayMap(map_enemy1); //enemy's map before shooting
                printf("\tEnter the x,y coordinates of the target\n");
                scanf("%d %d", &x, &y);
                system("CLS");
                if (map_enemy1[y][x] == '.' && (map2[y][x] == '.' || map2[y][x] == 'w'))
                {
                    map2[y][x] = 'x';
                    map_enemy1[y][x] = 'w';
                    bonus = false;
                }
                else if (map2[y][x] == 's')
                {
                    //find the target ship in the linked list
                    curr = head2;
                    while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    if (curr->hit != curr->info.length * curr->info.width - 1)
                    { //the ship is not completely exploded
                        map2[y][x] = 'e';
                        map_enemy1[y][x] = 'e';
                        coins1++;
                        curr->hit++;
                    }
                    else
                    {
                        map_enemy1 = complete_explosion(map2, map_enemy1, curr);
                        map2[y][x] = 'e';
                        printf("Complete explosion!\n");
                        //remove the ship from the linked list
                        if (curr == head2)
                            head2 = head2->next;
                        else
                            prev->next = curr->next;
                        int destroyed_ship_area = curr->info.length * curr->info.width;
                        int score = 5 * largest_ship_area / destroyed_ship_area;
                        coins1 += score + 1;
                        if (head2 == NULL)
                            break;
                    }
                }
                else if (map2[y][x] == 'e' || map2[y][x] == 'x')
                    printf("\tYou have already hit this target! Try again\n");
                displayMap(map1);
                displayMap(map_enemy1); //show the result
                Sleep(1000);
            }
        }
        else if (choice == 2)
            ;
        else if (choice == 3)
            ;
        //bot's turn
        bonus = true;
        if (coins2 >= 100)
        {
            choice = rand() % 2 + 1;
        }
        else
            choice = 1;

        if (choice == 1)
        {
            x = rand() % 10;
            y = rand() % 10;
            while (bonus)
            {
                if (head1 == NULL)
                    break;
                system("cls");
                if (map1[y][x] == 'e' || map1[y][x] == 'x')
                {                 //the target has already been hit
                    bonus = true; //the bot can try again so it stays in the loop
                    x = rand() % mapcol;
                    y = rand() % maprow;
                    continue;
                }
                else if (map_enemy2[y][x] == '.' && (map1[y][x] == '.' || map1[y][x] == 'w'))
                { //the target is the ocean
                    map1[y][x] = 'x';
                    map_enemy2[y][x] = 'w';
                    bonus = false;
                }
                else if (map1[y][x] == 's')
                { //the target is a part of a ship
                    //find the target ship in the linked list
                    curr = head1;
                    while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    if (curr->hit != curr->info.length * curr->info.width - 1)
                    { //the ship is not completely exploded
                        map1[y][x] = 'e';
                        map_enemy2[y][x] = 'e';
                        coins2++;
                        curr->hit++;
                    }
                    else
                    {
                        map_enemy2 = complete_explosion(map1, map_enemy2, curr);
                        map1[y][x] = 'e';
                        //unlink the ship from the linked list
                        if (curr == head1)
                            head1 = head1->next;
                        else
                            prev->next = curr->next;
                        int destroyed_ship_area = curr->info.length * curr->info.width;
                        int score = 5 * largest_ship_area / destroyed_ship_area;
                        coins2 += score + 1;
                        if (head1 == NULL)
                            break;
                    }
                    //generate another x,y coordinates as a bonus
                    if (difficulty == 1)
                    {
                        x = rand() % mapcol;
                        y = rand() % maprow;
                    }
                    else if (difficulty == 2)
                    {                       //the bot chooses either the column of the last taget or the row to have a better chance to hit a ship
                        int r = rand() % 2; //randomly chooses vertical or horizontal (sth like missile attack)
                        while (r != 2)
                        {
                            if (r == 0)
                            {
                                for (int i = 0; i < mapcol; i++)
                                {
                                    if (map1[y][i] == '.')
                                    { //the first occurance of a dot
                                        x = i;
                                        r = 2;
                                        break;
                                    }
                                    r = 1;
                                }
                            }
                            else
                            {
                                for (int i = 0; i < maprow; i++)
                                {
                                    if (map1[i][x] == '.')
                                    {
                                        y = i;
                                        r = 2;
                                        break;
                                    }
                                    r = 0;
                                }
                            }
                        }
                    }
                }
                displayMap(map1);
                displayMap(map_enemy1); //show the result
                Sleep(2000);
            }
        }
        else
            ; //missile
    }
    //end of the game
    if (head1 == NULL && head2 != NULL)
    {
        printf("\tBot won!\n");
        users[0].coins += coins1 / 2;
    }
    else if (head2 == NULL && head1 != NULL)
    {
        printf("\t%s won!\n", users[0].name);
        users[0].coins += coins1;
    }
    else
    {
        printf("\tIt's a tie\n");
        users[0].coins += coins1;
    }
    savescores();
}
//incorrect
void missile_attack(node **head, char ***map, char ***map_enemy, int *coins)
{
    //get data
    char direction;
    int point;
    printf("\tEnter the direction in which you like to shoot:\n");
    printf("\tEnter V for vertical or H for horizontal\n");
    scanf("%c", &direction);
    printf("Enter the column/row in which you like to shoot:\n");
    scanf("%d", &point);
    getchar();

    int x, y;
    if (direction == 'V' || direction == 'v')
    {
        for (x = point, y = 0; *map[y][x] == 'w'; y++)
            *map_enemy[y][x] = 'w';
        if (*map[y][x] == 's' || *map[y][x] == 'e')
        {
            node *curr = *head, *prev;
            while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
            {
                prev = curr;
                curr = curr->next;
            }
            if (curr->hit == curr->info.length * curr->info.width - 1)
            { //if the target point is the last point of an exploded ship
                printf("Complete explosion!\n");
                *map_enemy = complete_explosion(*map, *map_enemy, curr);
                //unlink the current ship from the linked list
                if (curr == *head)
                    *head = (*head)->next;
                else
                    prev->next = curr->next;
                int destroyed_ship_area = curr->info.length * curr->info.width;
                int score = 5 * largest_ship_area / destroyed_ship_area;
                *coins += score;
            }
            else
            {
                *map[y][x] = 'e';
                *map_enemy[y][x] = 'e';
                *coins++;
                curr->hit++;
            }
        }
    }
    else if (direction == 'H' || direction == 'h')
    { //horizontal
        for (y = point, x = 0; *map[y][x] == 'w'; x++)
            *map_enemy[y][x] = 'w';
        if (*map[y][x] == 's' || *map[y][x] == 'e')
        {
            //check if you have the right to delete the target ship from the linked list
            node *curr = *head, *prev;
            while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y)
            {
                prev = curr;
                curr = curr->next;
            }
            if (curr->hit == curr->info.length * curr->info.width - 1)
            { //if the target point is the last point of an exploded ship
                printf("Complete explosion!\n");
                *map_enemy = complete_explosion(*map, *map_enemy, curr);
                //unlink the current ship from the linked list
                if (curr == *head)
                    *head = (*head)->next;
                else
                    prev->next = curr->next;
                int destroyed_ship_area = curr->info.length * curr->info.width;
                int score = 5 * largest_ship_area / destroyed_ship_area;
                *coins += score;
            }
            else
            {
                *map[y][x] = 'e';
                *map_enemy[y][x] = 'e';
                *coins++;
                curr->hit++;
            }
        }
    }
}

void savescores()
{
    //better way is to realloc "all" everytime u wanna read smth into it:)
    int i, coin;
    User all[500];
    FILE *fpin = fopen("users.txt", "r");
    for (i = 0; !feof(fpin); i++)
    {
        fscanf(fpin, "%s %s", &all[i].name, &all[i].str_coins);
        if (strcmp(all[i].name, users[0].name) == 0)
            sprintf(all[i].str_coins, "%d", users[0].coins);
        else if (strcmp(all[i].name, users[1].name) == 0)
            sprintf(all[i].str_coins, "%d", users[1].coins);
    }
    int num_of_useres = i;
    fclose(fpin);
    FILE *fpout = fopen("users.txt", "w");
    for (i = 0; i < num_of_useres; i++)
    {
        fprintf(fpout, "%s %s\n", all[i].name, all[i].str_coins);
    }
    fclose(fpout);
}

void savegame(char **map1, char **enemy_map1, char **map2, char **enemy_map2, node *head1, node *head2, bool bot)
{
    char mode[10]; //friend or bot
    /*
    char name[30];
    printf("\tChoose a name for your game\n\tThe game will be saved with the name you enter\n");
    while (1)
    {
        gets(name);
        FILE *fp = fopen("game.bin", "rb");
    }
    */
}
void loadSavedGame ();