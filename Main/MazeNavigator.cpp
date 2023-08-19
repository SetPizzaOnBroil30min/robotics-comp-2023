#include <queue>
#include <iostream>
#include <array>

using namespace std;

enum Direction {UP, DOWN, RIGHT, LEFT};
enum Colour {UNKNOWN, BW, RED, OTHER};
enum WallType{MAYBEWALL, NOWALL, YESWALL};

class Cell {
    public:
        
        Cell () {}
        int x;
        int y;

        int distance;

        Colour colour = UNKNOWN;

        WallType upWall, downWall, rightWall, leftWall = MAYBEWALL;
    
};

class Maze {
    public:
        Maze () {}
        Cell startCell;
        Cell endCell;

        Cell cellMap[6][6];

};

class Robot {
    public:
        Robot () {}

        int x = 5;
        int y = 5;

        Direction direc = UP;
};

Robot robot;
Maze maze;

// pre initialised to avoid stupid errors
void initMaze();
void PerformNextTurn(Direction currentDirection);
void RemoveColourWalls(Colour toRemove);
void SetMazeStartAndEnd(int startX, int startY, int endX, int endY);
void NavigateMaze(int startX, int startY, int endX, int endY, bool reset);
void TurnAnticlockwise();
void TurnClockwise();
bool ScanForWall();
void MoveForward();
Colour ReadColour();
void UpdateDistances();
void UpdateCellDist(int distance, int x, int y);
Cell GetCell(Cell cell, Direction direction);
Direction GetClockwise(Direction direction);
Direction GetAnticlockwise(Direction direction);
void SetWall(Direction direction, int x, int y);
WallType GetWall(int x, int y, Direction direction);


// globals

void initMaze() {
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            Cell cell;
            cell.x = i;
            cell.y = j;

            maze.cellMap[i][j] = cell;
            
            if (i == 0) {   
                SetWall(LEFT, cell.x, cell.y);
            } if (i == 5) {
                SetWall(RIGHT, cell.x, cell.y);
            } if (j == 0) {
                SetWall(UP, cell.x, cell.y);
            } if (j == 5) {
                SetWall(DOWN, cell.x, cell.y);
            }

        }
    }    
};

void PerformNextTurn(Direction currentDirection) {
    cout << "performing Next turn\n";
    UpdateDistances();

    int thisDist = maze.cellMap[robot.x][robot.y].distance;

    if (GetWall(robot.x, robot.y, robot.direc) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], currentDirection).distance == thisDist-1) {
        cout << "Decided to move forward\n";
        return;
    }
    else if (GetWall(robot.x, robot.y, GetClockwise(robot.direc)) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], GetClockwise(currentDirection)).distance == thisDist-1) {
        cout << "Decided to turn clockwise\n";
        robot.direc = GetClockwise(currentDirection);
        TurnClockwise();
    }
    else if (GetWall(robot.x, robot.y, GetAnticlockwise(robot.direc)) != YESWALL && GetCell(maze.cellMap[robot.x][robot.y], GetAnticlockwise(currentDirection)).distance == thisDist-1) {
        cout << "Decided to turn anticlockwise\n";

        robot.direc = GetAnticlockwise(currentDirection);
        TurnClockwise();
    }
    else {
        cout << "Decided to turn clockwise twice\n";

        robot.direc = GetClockwise(GetClockwise(currentDirection));

        TurnClockwise();
        TurnClockwise();

    }
}

void RemoveColourWalls(Colour toRemove) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            // check for boundary conditions
            if (maze.cellMap[i][j].colour == OTHER) {
                // set wall to MAYBEWALL
            }
        }
    }
}

void SetMazeStartAndEnd(int startX, int startY, int endX, int endY) {
    maze.startCell = maze.cellMap[startX][startY];


    maze.endCell = maze.cellMap[endX][endY];

}


void NavigateMaze(int startX, int startY, int endX, int endY, bool reset) {
    if (reset) {
        initMaze();
    }
    SetMazeStartAndEnd(startX, startY, endX, endY);

    while(robot.x != maze.endCell.x && robot.y != maze.endCell.y) {

        maze.cellMap[robot.x][robot.y].colour = ReadColour();

        PerformNextTurn(robot.direc);
        
        WallType wall = GetWall(robot.x, robot.y, robot.direc);

        if (wall == NOWALL || ScanForWall() == 0) {
            robot.x = GetCell(maze.cellMap[robot.x][robot.y], robot.direc).x;
            robot.y = GetCell(maze.cellMap[robot.x][robot.y], robot.direc).y;
            MoveForward();

        }
        else {
            SetWall(robot.direc, robot.x, robot.y);
        }
    }
}


void TurnAnticlockwise() {
    cout << "turning anticlockwise";
}
void TurnClockwise() {
    cout << "turning anticlockwise";
}

bool ScanForWall() {
    cout << "Is there a wall? (y/n)";
    string input;
    cin >> input;
    if (input == "y") {
        cout << "There is a wall\n";
        return true;

    }
    else {
        cout << "There is not a wall\n";
        return false;
    }
    
}

void MoveForward() {
    cout << "current location is: x = " << robot.x << ", y = " << robot.y << "\n";
}


Colour ReadColour() {
    return RED;
}

void UpdateDistances () {
    // clear all distances
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            maze.cellMap[i][j].distance = 100;
        }
    }

    // call the recursive flood fill function
    UpdateCellDist(0, maze.endCell.x, maze.endCell.y);

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cout << maze.cellMap[i][j].distance << " ";
        }
        cout << "\n";
    }

}

void UpdateCellDist(int distance, int x, int y) {
    // cout << "UpdateCellDist() Called\n";
    maze.cellMap[x][y].distance = distance;
    
    if (y > 0 && maze.cellMap[x][y].upWall != YESWALL && maze.cellMap[x][y-1].distance > distance + 1) {
        UpdateCellDist(distance + 1, x, y-1);
    }
    if (y < 5 && maze.cellMap[x][y].downWall != YESWALL && maze.cellMap[x][y+1].distance > distance + 1) {
        UpdateCellDist(distance + 1, x, y+1);
    }
    if (x < 5 && maze.cellMap[x][y].rightWall != YESWALL && maze.cellMap[x+1][y].distance > distance + 1) {
        UpdateCellDist(distance + 1, x + 1, y);
    }
    if (x > 0 && maze.cellMap[x][y].leftWall != YESWALL && maze.cellMap[x-1][y].distance > distance + 1) {
        UpdateCellDist(distance + 1, x - 1, y);
    } 
}

// Does not account for running off array edges
Cell GetCell(Cell cell, Direction direction) {
    if (direction == UP) {return maze.cellMap[cell.x][cell.y + 1];}
    if (direction == DOWN) {return maze.cellMap[cell.x][cell.y - 1];}
    if (direction == RIGHT) {return maze.cellMap[cell.x + 1][cell.y];}
    else {return maze.cellMap[cell.x - 1][cell.y];}
}

Direction GetClockwise(Direction direction) {
    // returns the clockwise of this direction
    if (direction == UP) {return RIGHT;}
    if (direction == DOWN) {return LEFT;}
    if (direction == RIGHT) {return DOWN;}
    else {return UP;}
}
Direction GetAnticlockwise(Direction direction) {
    // returns the clockwise of this direction
    if (direction == UP) {return LEFT;}
    if (direction == DOWN) {return RIGHT;}
    if (direction == RIGHT) {return UP;}
    else {return DOWN;}
}

void SetWall(Direction direction, int x, int y) {
    if (direction == UP) {
        maze.cellMap[x][y].upWall = YESWALL;

        if (y != 0) {
            maze.cellMap[x][y-1].downWall = YESWALL;
        }
    }
    else if (direction == DOWN) {
        maze.cellMap[x][y].downWall = YESWALL;

        if (y != 5) {
            maze.cellMap[x][y+1].upWall = YESWALL;
        }
    }
    else if (direction == RIGHT) {
        maze.cellMap[x][y].rightWall = YESWALL;

        if (x != 5) {
            maze.cellMap[x+1][y].leftWall = YESWALL;
        }
    }
    else if (direction == LEFT) {
        maze.cellMap[x][y].leftWall = YESWALL;

        if (x != 0) {
            maze.cellMap[x-1][y].rightWall = YESWALL;
        }
    }
}

WallType GetWall(int x, int y, Direction direction) {
    if (direction == UP) {return maze.cellMap[x][y].upWall;}
    if (direction == DOWN) {return maze.cellMap[x][y].downWall;}
    if (direction == RIGHT) {return maze.cellMap[x][y].rightWall;}
    else{return maze.cellMap[x][y].leftWall;}
}


int main(int argc, char const *argv[])
{
    NavigateMaze(5,5,1,1,true);
}


// lastDistance
// check current cell dist > lastdist + 1
// set dist = last dist + 1;
// for eahc cell that doenst have a wall, and dist < last dist + 1:


// start from dest
// if it hits the start cell dist 
// follow the number 


// get firsection
// check wall
// if wall == true?
//  update!
//  recalculate
// else:
//  move:
//  recalculate / find next lowest?

// 0,0 top left
//
//[(0,0),(1,0),(2,0),(3,0),(4,0)]
//[(0,1),(1,1),(2,1),(3,1),(4,1)]

// start = 5,5
// end: 1,1

//6x 6 grid