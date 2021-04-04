// Your First C++ Program

#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

/*
axis:
     ----> y
    |
    |
    |
   x\/
    
*/

int EMPTY = 0;
int WALL = 1;
int BOX = 2;
int TARGET = 3;
int WORKER = 4;

char LEFT = 'l';
char RIGHT = 'r';
char UP = 'u';
char DOWN = 'd';
char LEFT_BOX = 'L';
char RIGHT_BOX = 'R';
char UP_BOX = 'U';
char DOWN_BOX = 'D';

struct Position
{
    int x;
    int y;
};

struct State
{
    vector<vector<int>> map;
    Position workerPosition;
    Position lastWorkerPostion;
    vector<char> moves;
};

int height, width;
vector<Position> targetsPositions;

int global_worker_x, global_worker_y;

State processInput();
void printMatrix(vector<vector<int>> matrix);

bool inMap(int x, int y)
{
    return x >= 0 && x < height && y >= 0 && y < width;
}

bool canMoveRight(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x, y + 1) && (matrix[x][y + 1] == EMPTY || matrix[x][y + 1] == TARGET);
}

bool canMoveLeft(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x, y - 1) && (matrix[x][y - 1] == EMPTY || matrix[x][y - 1] == TARGET);
}

bool canMoveUp(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x - 1, y) && (matrix[x - 1][y] == EMPTY || matrix[x - 1][y] == TARGET);
}

bool canMoveDown(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x + 1, y) && (matrix[x + 1][y] == EMPTY || matrix[x + 1][y] == TARGET);
}

bool canMoveRightBox(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x, y + 1) && inMap(x, y - 1) && (matrix[x][y + 1] == EMPTY || matrix[x][y + 1] == TARGET) && matrix[x][y - 1] == BOX;
}

bool canMoveLeftBox(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x, y - 1) && inMap(x, y + 1) && (matrix[x][y - 1] == EMPTY || matrix[x][y - 1] == TARGET) && matrix[x][y + 1] == BOX;
}

bool canMoveUpBox(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x - 1, y) && inMap(x + 1, y) && (matrix[x - 1][y] == EMPTY || matrix[x - 1][y] == TARGET) && matrix[x + 1][y] == BOX;
}

bool canMoveDownBox(int x, int y, vector<vector<int>> matrix)
{
    return inMap(x + 1, y) && inMap(x - 1, y) && (matrix[x + 1][y] == EMPTY || matrix[x + 1][y] == TARGET) && matrix[x - 1][y] == BOX;
}


State move(State state, Position newWorkerPosition)
{
    State next = state;
    Position workerPos = next.workerPosition;

    next.map[newWorkerPosition.x][newWorkerPosition.y] = WORKER;
    next.map[workerPos.x][workerPos.y] = EMPTY;

    next.lastWorkerPostion = workerPos;
    next.workerPosition = newWorkerPosition;

    return next;
}

State moveBox(State state, Position newWorkerPosition, Position boxPostion)
{
    State next = state;
    Position workerPos = next.workerPosition;

    next.map[newWorkerPosition.x][newWorkerPosition.y] = WORKER;
    next.map[workerPos.x][workerPos.y] = BOX;
    next.map[boxPostion.x][boxPostion.y] = EMPTY;

    next.lastWorkerPostion = workerPos;
    next.workerPosition = newWorkerPosition;

    return next;
}

bool isSolved(State state)
{
    bool solved = true;

    for (auto it = targetsPositions.begin(); it != targetsPositions.end(); it++)
    {
        if (state.map[it->x][it->y] != BOX)
            solved = false;
        
    }

    return solved;
}

vector<char> legalMoves(State state)
{
    vector<char> moves;
    Position workerPos = state.workerPosition;
    vector<vector<int>> map = state.map;

    if (canMoveRightBox(workerPos.x, workerPos.y, map))
        moves.push_back(RIGHT_BOX);

    if (canMoveLeftBox(workerPos.x, workerPos.y, map))
        moves.push_back(LEFT_BOX);


    if (canMoveUpBox(workerPos.x, workerPos.y, map))
        moves.push_back(UP_BOX);

    
    if (canMoveDownBox(workerPos.x, workerPos.y, map))
        moves.push_back(DOWN_BOX);


    
    if (canMoveRight(workerPos.x, workerPos.y, map))
        moves.push_back(RIGHT);
    

    if (canMoveLeft(workerPos.x, workerPos.y, map))
        moves.push_back(LEFT);
    

    if (canMoveUp(workerPos.x, workerPos.y, map))
        moves.push_back(UP);
    

    if (canMoveDown(workerPos.x, workerPos.y, map))
        moves.push_back(DOWN);
    

    return moves;
}

//for debug
void printState(State state){
    cout<< "====================" << endl;
    cout << state.workerPosition.x << " " << state.workerPosition.y << endl;
    cout << state.lastWorkerPostion.x << " " << state.lastWorkerPostion.y << endl;
    for (auto it = state.moves.begin(); it != state.moves.end(); it++)
        cout << *it << " ";
    cout<< endl;
    printMatrix(state.map);
    cout<< "====================" << endl;

}

int main()
{
    cin >> height >> width;

    State initialState = processInput();

    queue<State> q;
    vector<char> result;
    bool solved = false;
    q.push(initialState);

    while (!solved)
    {

        State actualState = q.front();
        q.pop();

        vector<char> moves = legalMoves(actualState);
        for (auto it = moves.begin(); it != moves.end(); it++)
        {
            State next;
            Position worker;
            Position nextPos;
            Position lastPos;
            Position boxPos;

            switch (*it)
            {
            case 'r':
                worker = actualState.workerPosition;
                nextPos = {worker.x, worker.y + 1};
                lastPos = actualState.lastWorkerPostion;
                //it is pointless to back
                if (nextPos.x != lastPos.x || nextPos.y != lastPos.y) {
                    next = move(actualState, nextPos);
                    next.moves.push_back('r');
                    if (isSolved(next)) {
                        result = next.moves;
                        solved = true;
                        break;
                    }
                    else {
                        q.push(next);
                    }
                }

                break;
            case 'l':
                worker = actualState.workerPosition;
                nextPos = {worker.x, worker.y - 1};
                lastPos = actualState.lastWorkerPostion;

                //it is pointless to back
                if (nextPos.x != lastPos.x || nextPos.y != lastPos.y) {                    
                    next = move(actualState, nextPos);
                    next.moves.push_back('l');
                    if (isSolved(next)) {
                        result = next.moves;
                        solved = true;
                        break;
                    }
                    else {
                        q.push(next);
                    }
                }
                break;
            case 'u':
                worker = actualState.workerPosition;
                nextPos = {worker.x - 1, worker.y};
                lastPos = actualState.lastWorkerPostion;

                //it is pointless to back
                if (nextPos.x != lastPos.x || nextPos.y != lastPos.y) {
                    next = move(actualState, nextPos);
                    next.moves.push_back('u');
                    if (isSolved(next)) {
                        result = next.moves;
                        solved = true;
                        break;
                    }
                    else {
                        q.push(next);
                    }
                }
                break;
            case 'd':
                worker = actualState.workerPosition;
                nextPos = {worker.x + 1, worker.y };
                lastPos = actualState.lastWorkerPostion;

                //it is pointless to back
                if (nextPos.x != lastPos.x || nextPos.y != lastPos.y) {
                    next = move(actualState, nextPos);
                    next.moves.push_back('d');
                    if (isSolved(next)) {
                        result = next.moves;
                        solved = true;
                        break;
                    }
                    else {
                        q.push(next);
                    }
                }
                
                break;
            case 'R':
                worker = actualState.workerPosition;
                nextPos = {worker.x, worker.y + 1};
                boxPos = {worker.x, worker.y - 1};
                
                next = moveBox(actualState, nextPos, boxPos);
                next.moves.push_back('R');
                if (isSolved(next)) {
                    result = next.moves;
                    solved = true;

                    break;
                }
                else {
                    q.push(next);
                }

                break;
            case 'L':
                worker = actualState.workerPosition;
                nextPos = {worker.x, worker.y - 1};
                boxPos = {worker.x, worker.y + 1};

                next = moveBox(actualState, nextPos, boxPos);
                next.moves.push_back('L');
                if (isSolved(next)) {
                    result = next.moves;
                    solved = true;
                    break;
                }
                else {
                    q.push(next);
                }
                break;
            case 'U':
                worker = actualState.workerPosition;
                nextPos = {worker.x - 1, worker.y};
                boxPos = {worker.x + 1, worker.y};

                next = moveBox(actualState, nextPos, boxPos);
                next.moves.push_back('U');
                if (isSolved(next)) {
                    result = next.moves;
                    solved = true;
                    break;
                }
                else {
                    q.push(next);
                }
                break;
            case 'D':
                worker = actualState.workerPosition;
                nextPos = {worker.x + 1, worker.y};
                boxPos = {worker.x - 1, worker.y};
                
                next = moveBox(actualState, nextPos, boxPos);
                next.moves.push_back('D');
                if (isSolved(next)) {
                    result = next.moves;
                    solved = true;
                    break;
                }
                else {
                    q.push(next);
                }
                break;
            default:
                break;
            }
        }
    }

    for (auto it = result.begin(); it != result.end(); it++){
            cout << *it;
    }
        


    return 0;
}


State processInput()
{
    string line;
    getline(cin, line); //read new line from first line

    State initialState;
    vector<vector<int>> map;

    char ch;

    for (int i = 0; i < height; i++)
    {
        getline(cin, line);
        vector<int> e;
        map.push_back(e);
        for (int j = 0; j < width; j++)
        {
            switch (line[j])
            {
            case '#':
                map[i].push_back(WALL);
                break;
            case 'O':
                map[i].push_back(BOX);
                break;
            case '*':
                Position target;
                target.x = i;
                target.y = j;
                targetsPositions.push_back(target);
                map[i].push_back(TARGET);
                break;
            case '-':
                Position pos;
                pos.x = i;
                pos.y = j;
                initialState.workerPosition = pos;
                map[i].push_back(WORKER);
                break;
            case ' ':
                map[i].push_back(EMPTY);
                break;
            default:
                break;
            }
        }
    }

    initialState.map = map;
    initialState.lastWorkerPostion = {-1, -1};

    return initialState;
}

//for debug
void printMatrix(vector<vector<int>> matrix)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
