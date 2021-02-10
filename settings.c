#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Ship
{
    int length, width;
} Ship;

int maprow = 10, mapcol = 10;
int numOfTotalShips = 10, largest_ship_area = 5;
Ship ships[30];

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
