#include "research.h"

int main(void)
{
    int rows[] = { 50, 75, 100, 125, 150, 175, 200 };
    int columns[] = { 50, 75, 100, 125, 150, 175, 200 };
    int persents[] = { 1, 3, 5, 7, 10, 12, 15, 20, 25, 35, 50, 60 };

    make_research(rows, columns, sizeof(rows) / sizeof(rows[0]), persents, sizeof(persents) / sizeof(persents[0]));
    
    return 0;
}
