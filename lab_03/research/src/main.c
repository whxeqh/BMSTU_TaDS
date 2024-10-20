#include "research.h"

int main(void)
{
    int rows[] = { 10, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
    int columns[] = { 10, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
    int persents[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };

    make_research(rows, columns, sizeof(rows) / sizeof(rows[0]), persents, sizeof(persents) / sizeof(persents[0]));
    
    return 0;
}
