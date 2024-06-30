#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void make_goal(int s, int *puzzle) {
    int ts = s * s;
    int cur = 1, x = 0, ix = 1, y = 0, iy = 0;

    for (int i = 0; i < ts; i++) {
        puzzle[i] = -1;
    }

    while (1) {
        puzzle[x + y * s] = cur;
        if (cur == 0) break;
        cur += 1;
        if (x + ix == s || x + ix < 0 || (ix != 0 && puzzle[x + ix + y * s] != -1)) {
            iy = ix;
            ix = 0;
        } else if (y + iy == s || y + iy < 0 || (iy != 0 && puzzle[x + (y + iy) * s] != -1)) {
            ix = -iy;
            iy = 0;
        }
        x += ix;
        y += iy;
        if (cur == ts) cur = 0;
    }
}

void swap_empty(int s, int *p) {
    int idx = 0;
    int poss[4], poss_count = 0;

    for (int i = 0; i < s * s; i++) {
        if (p[i] == 0) {
            idx = i;
            break;
        }
    }

    if (idx % s > 0) poss[poss_count++] = idx - 1; // Left
    if (idx % s < s - 1) poss[poss_count++] = idx + 1; // Right
    if (idx / s > 0) poss[poss_count++] = idx - s; // Up
    if (idx / s < s - 1) poss[poss_count++] = idx + s; // Down

    int swi = poss[rand() % poss_count];
    p[idx] = p[swi];
    p[swi] = 0;
}

void make_puzzle(int s, int solvable, int iterations, int *p) {
    make_goal(s, p);

    for (int i = 0; i < iterations; i++) {
        swap_empty(s, p);
    }

    if (!solvable) {
        if (p[0] == 0 || p[1] == 0) {
            int temp = p[s * s - 1];
            p[s * s - 1] = p[s * s - 2];
            p[s * s - 2] = temp;
        } else {
            int temp = p[0];
            p[0] = p[1];
            p[1] = temp;
        }
    }
}

void print_puzzle(int s, int *p) {
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < s; j++) {
            printf("%2d ", p[i * s + j]);
        }
        printf("\n");
    }
}

int is_number(char *str)
{
    while (*str != 0)
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

int ft_atoi(char *str)
{
    if (is_number(str))
    {
        return atoi(str);
    }
    dprintf(2, "%s must be a valid number\n", str);
    exit(1);
}

int main(int ac, char **av) {
    int s;
    int solvable = -1;
    int iterations = -1;


    srand(time(NULL));
    solvable = rand() % 2;
    iterations = 10000;

    if (ac < 2)
    {
        dprintf(2, "The first argument should be the size of the puzzle\n");
        exit(1);
    }

    s = ft_atoi(av[1]);
    if (s < 3)
    {
        dprintf(2, "size of the puzzle must be greater than 2\n");
        exit(1);
    }

    for (int i = 2; i < ac; i++)
    {
        if (strncmp(av[i], "-s", 2) == 0) {
            solvable = 1;
        } else if (strncmp(av[i], "-u", 2) == 0) {
            solvable = 0;
        } else if (strncmp(av[i], "-i", 2) == 0) {
            if (i + 1 >= ac)
            {
                dprintf(2, "Should provide the number of iteration\n");
                exit(1);
            }
            iterations = ft_atoi(av[i + 1]);
        }
    }

    


    int *puzzle = (int *)malloc(s * s * sizeof(int));
    if (puzzle == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    make_puzzle(s, solvable, iterations, puzzle);

    if (solvable)
        printf("# This puzzle is solvable\n");
    else
        printf("# This puzzle is unsolvable\n");
    printf("%d\n", s);
    print_puzzle(s, puzzle);

    free(puzzle);
    return 0;
}
