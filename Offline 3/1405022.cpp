#include<bits/stdc++.h>
using namespace std;
int rows, columns, startx, starty, destx, desty, percent_hurdle;
const int HURDLE = 5;

const double THRESHOLD = 0.01;

const int MANHATTEN = 1;
const int EUCLIDEAN = 2;
const int MAXXANDY = 3;
int current_heuristic;

long long depth, depth_one, depth_two, depth_three;
int N;

class Grid{
public:
    vector<vector<int> >grid;
    double hn, gn, fn;
    int currx, curry;
    Grid* parent;

    Grid(){
        hn = 0;
        gn = 0;
        fn = 0;
        parent = NULL;
    }
};



class Compare
{
public:
    bool operator() (Grid a, Grid b)
    {
        if(a.fn < b.fn){
            return false;
        }
    }
};
vector<Grid> closedList;
priority_queue<Grid, vector<Grid>, Compare > openList;


void printAllGrids(Grid current)
{
    long long cost = 0;
    Grid *now = &current;
    while(true){
        Grid *parent = now->parent;
        for(int i=0;i<rows;i++){
            for(int j=0;j<columns;j++){
                printf("%d ", now->grid[i][j]);
            }
            cout <<endl;
        }
        cost++;
        if(parent == NULL){
            if(current_heuristic == EUCLIDEAN){
                depth_one = cost;
            }
            else if(current_heuristic == MANHATTEN){
                depth_two = cost;
            }
            else if(current_heuristic == MAXXANDY){
                depth_three = cost;
            }
            break;
        }
        now = parent;
        cout << endl <<endl;
    }
}


//------------------------------------------------------------------------------------------------

int calculateManhatten(int curr_x, int curr_y)
{
    int sum = 0;
    sum = abs(destx - curr_x) + abs(desty - curr_y);
    return sum;
}

double calculateEuclidean(int curr_x, int curr_y)
{
    double sum = 0;
    sum = sqrt(pow((destx - curr_x),2) + pow((desty - curr_y),2));
    return sum;
}

int calculateMaxOfXAndY(int curr_x, int curr_y)
{
    return max(abs(destx - curr_x), abs(desty - curr_y));
}


void makeNewGrid(Grid current, int a, int b)
{
    int i, j, c;
    double x, y, z;
    if(a>=0 && b>=0 && a<rows && b<columns && current.grid[a][b]!=HURDLE && current.grid[a][b]!=1)
    {
        Grid *copy_current = new Grid;
        copy_current->hn = current.hn;
        copy_current->gn = current.gn;
        copy_current->fn = current.fn;
        copy_current->currx = current.currx;
        copy_current->curry = current.curry;
        copy_current->parent = current.parent;

        copy_current->grid = vector<vector<int> >(rows);
        for(i=0;i<rows;i++){
            for(j=0;j<columns;j++){
                copy_current->grid[i].push_back(0);
            }
        }
        for(i=0;i<rows;i++){
            for(j=0;j<columns;j++){
                copy_current->grid[i][j] = current.grid[i][j];
            }
        }

        if(current_heuristic == MANHATTEN){
            x = double(calculateManhatten(a,b));
        }
        else if(current_heuristic == EUCLIDEAN){
            x = double(calculateEuclidean(a,b));
        }
        else if(current_heuristic == MAXXANDY){
            x = double(calculateMaxOfXAndY(a,b));
        }
        Grid new_grid;
        new_grid.currx = a;
        new_grid.curry = b;
        new_grid.hn = x;
        new_grid.gn = current.gn + 1;
        new_grid.fn = new_grid.gn + new_grid.hn;
        new_grid.parent = copy_current;

        new_grid.grid = vector<vector<int> >(rows);
        for(i=0;i<rows;i++){
            for(j=0;j<columns;j++){
                new_grid.grid[i].push_back(0);
            }
        }
        for(i=0;i<rows;i++){
            for(j=0;j<columns;j++){
                new_grid.grid[i][j] = current.grid[i][j];
            }
        }
        new_grid.grid[a][b] = 1;
        openList.push(new_grid);
    }
}

void applyHeuristics()
{
    int i, j, a, b, c, d, x, y, curr_cost, turns=0;
    while(true){
        Grid current;
        current = openList.top();
        openList.pop();
        closedList.push_back(current);

        x = current.currx;
        y = current.curry;
        if(x==destx && y==desty) {
            printAllGrids(current);
            break;
        }

        //Up
        a = x;
        b = y-1;
        makeNewGrid(current, a, b);

        //Down
        a = x;
        b = y+1;
        makeNewGrid(current, a, b);

        //Left
        a = x-1;
        b = y;
        makeNewGrid(current, a, b);

        //Right
        a = x+1;
        b = y;
        makeNewGrid(current, a, b);
    }

    //cout << "\n\nSuccess\n\n";
}


void clearAll()
{
    openList = priority_queue<Grid, vector<Grid>, Compare>();
    closedList.clear();
}


long double functionCost(double b)
{
    long double a = pow(b, depth+1) - b - (b*(N+1));
    //cout << "----" << a << "----" <<endl;
    return a;
}

double bisection(double a, double b)
{
    if(functionCost(a)*functionCost(b)>=0.0){
        cout << "what  " << functionCost(a) <<endl;
        cout << "what  " << functionCost(b) <<endl;
        cout << "what  " << functionCost(b)*functionCost(a) <<endl;
        printf("Problem in limits\n");
        return -1;
    }
    double c = a;
    while((b-a)>=THRESHOLD){
        c = (a+b)/2;

        if(functionCost(c) == 0.0) break;
        else if(functionCost(c)*functionCost(a)<0) b = c;
        else a = c;
    }
    cout << "Branching factor : " << c << endl << endl <<endl;
    return c;
}





int main()
{
    int i, j, k, a, b, curr_hurdle = 0, expected_hurdle = 0;
    printf("Enter number of rows and columns: ");
    cin>> rows >> columns;
    printf("Enter starting position: ");
    cin >> startx >> starty;
    printf("Enter ending position: ");
    cin >> destx >> desty;
    printf("Enter percentage of hurdle expected: ");
    cin >> percent_hurdle;

    Grid input;
    input.grid = vector<vector<int> >(rows);
    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            input.grid[i].push_back(0);
        }
    }

    srand(time(NULL));
    expected_hurdle = (rows*columns)*((double)percent_hurdle/100);

    input.grid[startx][starty] = 1;

    while(true){
        for(i=0;i<rows;i++){
            for(j=0;j<columns;j++){
                if((i!=startx && j!=starty) && (i!=destx && j!=desty) && input.grid[i][j]==0){
                    a = rand()%2;
                    if(a==1){
                        input.grid[i][j] = HURDLE;
                        curr_hurdle++;
                        if(curr_hurdle == expected_hurdle) break;
                    }
                }
            }
            if(curr_hurdle == expected_hurdle) break;
        }
        if(curr_hurdle == expected_hurdle) break;
    }

    printf("\nStarting Board Config: \n");
    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            printf("%d ", input.grid[i][j]);
        }
        cout << endl;
    }
    cout << "------------------------------------------------" <<endl << endl;

    input.hn = 0;
    input.gn = 0;
    input.fn = 0;
    input.currx = startx;
    input.curry = starty;
    input.parent = NULL;
    openList.push(input);

    current_heuristic = EUCLIDEAN;
    applyHeuristics();
    printf("\nLength of closed list: %d\n", closedList.size());
    printf("Depth: %lld\n", depth_one);
    N = closedList.size();
    depth = depth_one;
    bisection(0.5,10);


    clearAll();
    openList.push(input);
    current_heuristic = MANHATTEN;
    applyHeuristics();
    printf("\nLength of closed list: %d\n", closedList.size());
    printf("Depth: %lld\n", depth_two);
    N = closedList.size();
    depth = depth_two;
    bisection(0.5,10);


    clearAll();
    openList.push(input);
    current_heuristic = MAXXANDY;
    applyHeuristics();
    printf("\nLength of closed list: %d\n", closedList.size());
    printf("Depth: %lld\n", depth_three);
    N = closedList.size();
    depth = depth_three;
    bisection(0.5,10);


    return 0;
}












