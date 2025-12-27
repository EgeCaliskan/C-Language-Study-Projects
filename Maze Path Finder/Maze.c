#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum state {
    FREE,
    WALL,
    PLAYER,
    VISITED,
    EXIT

} state;


state** read_maze(char* fileloc, int* start_i, int* start_j, int* nr, int* nc) {
    FILE* fp = fopen(fileloc, "r");
    if (fp == NULL) {
        printf("Cannot read file.");
        exit(-1);
    }
    char c;
    fscanf(fp, "%d %d", nr, nc);
    state** maze = malloc(sizeof(state*) * *nr);
    for (int i = 0; i < *nr; i++)
        maze[i] = malloc(sizeof(state) * *nc);
    for (int i = 0; i < *nr; i++) {
        for (int j = 0; j < *nc; j++) {
            while ((c = fgetc(fp)) == '\n');

            switch (c) {
                case '*':
                    maze[i][j] = WALL;
                    break;
                case '#':
                    maze[i][j] = EXIT;
                    break;
                case ' ':
                    maze[i][j] = FREE;
                    break;
                case '@':
                    maze[i][j] = PLAYER;
                    *start_i = i;
                    *start_j = j;
                    break;
                default:
                    printf("Foreign character:%c", c);
                    break;
            }


        }
    }

    fclose(fp);
    return maze;
}
int to_1d(int i, int j, int n_row, int n_col) {
    return i * n_col + j;

}
void to_2d(int p, int* i, int* j, int n_col) {
    *i = p / n_col;
    *j = p % n_col;
}
void find_shortest_path(state** maze, int loc_i, int loc_j, int n_row, int n_col, int* shortest_path, int* len_shortest_path, int* path, int len_path) {

    if (maze[loc_i][loc_j] == EXIT) {


        if (len_path < *len_shortest_path || *len_shortest_path == -1) {
            *len_shortest_path = len_path;

            for (int i = 0; i < len_path; i++)
                shortest_path[i] = path[i];


        }
        return;
    }
    maze[loc_i][loc_j] = VISITED;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++ ) {
            int n_zero = 0;
            if (j == 0)
                n_zero++;
            if (i == 0)
                n_zero++;

            if (n_zero != 1)
                continue;
            int new_i = loc_i + i;
            int new_j = loc_j + j;
            int inside_bounds = new_i >= 0 && new_i < n_row && new_j >= 0 && new_j < n_col;
            if ((inside_bounds == 1) && (maze[new_i][new_j] == FREE || maze[new_i][new_j] == EXIT)) {
                path[len_path] = to_1d(new_i, new_j, n_row, n_col);
                find_shortest_path(maze, new_i, new_j, n_row, n_col, shortest_path, len_shortest_path, path, len_path + 1);

            }
        }
    }
    maze[loc_i][loc_j] = FREE;
}

void free_maze(state** maze, int nr, int nc) {
    for (int i = 0; i < nr; i++) {
            free(maze[i]);
    }
    free(maze);
}
void print_shortest_path(state** maze, int nr, int nc, int *shortest_path, int len_path) {
    int i,j;
    for (int n = 0; n < len_path; n++) {

        to_2d(shortest_path[n],&i,&j,nc);
        maze[i][j] = PLAYER;
    }
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {

            switch (maze[i][j]) {
                case PLAYER:
                    printf("@");
                    break;
                case WALL:
                    printf("*");
                    break;
                case EXIT:
                    printf("#");
                    break;
                case FREE:
                    printf(" ");
                    break;
                case VISITED:
                    printf("V");
                    break;
                default:
                    printf("O");
                    break;
            }
            printf(" ");
        }
        printf("\n");
    }
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Program must be called with a single argument: The name of the maze file in the same directory as the c file.");
        return -1;
    }
    char file_loc[100] = "../";
    strcat(file_loc, argv[1]);
    int start_i, start_j, nr, nc;
    state** maze = read_maze(file_loc, &start_i, &start_j, &nr, &nc);

    int* shortest_path = calloc(nr* nc * 2,sizeof(int));
    int len_shortest_path = -1;
    int path[2000] = {0};
    find_shortest_path(maze, start_i, start_j, nr, nc, shortest_path, &len_shortest_path, path, 0);

    maze[start_i][start_j] = -1;
    printf("\n");
    print_shortest_path(maze, nr, nc, shortest_path, len_shortest_path);
    printf("Length of the shortest path is:%d ", len_shortest_path);
    free_maze(maze, nr, nc);
    free(shortest_path);
}
