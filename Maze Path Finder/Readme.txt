A C program which finds the shortest path in a given maze. I recognise that BFS is a more efficient algorithm, however I wanted to solve it with DFS to solidify my understanding on the subject of backtracking. 

Key features:
- Dynamically allocated memory allows for arbitrarily sized mazes.
- DFS algorithm explores all possible paths and selects the shortest one.
- Path visualization.
- Proper pointer handling to prevent memory leaks.
- Error handling to provide details about incorrect input. 

Usage:
The program looks for a file named "Maze.txt" (which is provided in the repo) in the same folder as the code. In this file, the first line contains two integers which are the number of rows and columns on the maze, and the other lines represent the maze.

Maze representation:
* -> Walls
Space -> Empty cells
@ -> Starting position
# -> Target cell to reach
