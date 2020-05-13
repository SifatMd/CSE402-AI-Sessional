#include<bits/stdc++.h>
using namespace std;

const double THRESHOLD = 0.01;

const int NOOFMISPLACEDTILES = 1;
const int EUCLIDEAN = 2;
const int MANHATTEN = 3;
const int NOOFTILEOUTOF = 4;
const int NMAXSWAP = 5;
int current_heuristic;
long int depth_one, depth_two, depth_three, depth_four, depth_five;
long int depth;
int N;

class Board{
  public:
    int aray[16];
    double fn, gn, hn;
    Board* parent;

    Board(){
        fn = 0; gn = 0; hn = 0;
        parent = NULL;
        for(int i=0;i<16;i++) aray[i] = 0;
    }


};

class Compare
{
public:
    bool operator() (Board a, Board b)
    {
        if(a.fn < b.fn){
            return false;
        }
    }
};
vector<Board> closedList;
priority_queue<Board, vector<Board>, Compare> openList;


struct compareBoards {
    bool operator()(const Board a, const Board b) const{
        for(int i=0;i<16;i++){
            if(a.aray[i]!=b.aray[i]){
                return a.aray[i] < b.aray[i];
            }
        }
        return false;
    }
};
map<Board, int, compareBoards> myClosedMap;
map<Board, int, compareBoards> myOpenMap;

void print_board(Board curr)
{
    for(int i=0;i<16;i++){
        cout << curr.aray[i] <<"  ";
        if(i%4==3) cout <<endl;
    }
}

void print_all_boards(Board curr){
    Board *now = &curr;
    long int cost = 0;
    while(true){
        Board *parent = now->parent;
        for(int i=0;i<16;i++){
            cout<< now->aray[i]<< "  ";
            if(i%4==3) cout << endl;
        }
        cout << endl << endl;

        cost++;
        if(parent == NULL){
            //printf("Cost: %d\n", cost);
            if(current_heuristic == NOOFMISPLACEDTILES){
                depth_one = cost;
            }
            else if(current_heuristic == EUCLIDEAN){
                depth_two = cost;
            }
            else if(current_heuristic == MANHATTEN){
                depth_three = cost;
            }
            else if(current_heuristic == NOOFTILEOUTOF){
                depth_four = cost;
            }
            else if(current_heuristic == NMAXSWAP){
                depth_five = cost;
            }
            break;
        }
        //cout << "fn: " << now->fn <<endl;
        now = parent;
    }
}

bool checkGoalState(Board b)
{
    int i, j, k;
    for(i=0;i<16;i++){
        if(b.aray[i] != i){
            return false;
        }
    }
    return true;
}


int NoOfMisplacedTilesCost(Board B){
    int cost = 0;
    for(int i=0;i<16;i++){
        if(B.aray[i]!=0 && B.aray[i]!=i){
            cost++;
        }
    }
    return cost;
}

double EuclideanCost(Board B)
{
    double cost = 0.0, sum = 0.0;
    int val, ax, ay, bx, by;
    for(int i=0;i<16;i++){
        if(B.aray[i]!=0){
            val = B.aray[i];
            ax = i/4; ay = i%4;
            bx = val/4; by = val%4;

            cost = double(sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
            sum += cost;
        }
    }
    return sum;
}

int ManhattenCost(Board B)
{
    int cost=0, sum=0, i, ax, ay, bx, by, val;
    for(i=0;i<16;i++){
        if(B.aray[i]!=0){
            val = B.aray[i];
            ax = i/4; ay = i%4;
            bx = val/4; by = val%4;

            cost = abs(ax-bx)+abs(ay-by);
            sum += cost;
        }
    }
    return sum;
}

int NoOfTilesOutOfCost(Board B)
{
    int sum=0, i, j, k, ax, ay, bx, by, val;
    for(i=0;i<16;i++){
        if(B.aray[i]!=0){
            val = B.aray[i];
            ax = i/4; ay = i%4;
            bx = val/4; by = val%4;

            if(ax!=bx) sum++;
            if(ay!=by) sum++;
        }
    }
    return sum;
}


int NMaxSwapCost(Board B, int pos) //pos is the position of zero
{
    int i, j, k, total_swaps = 0, val;
    while(true){
        if(checkGoalState(B)){
            break;
        }
        val = pos;
        if(val==0){
            for(i=1;i<16;i++){
                if(B.aray[i]!=i){
                    B.aray[0] = B.aray[i];
                    B.aray[i] = 0;
                    pos = i;
                    break;
                }
            }
            total_swaps++;
        }

        //shuffle with element which should sit here rightfully
        for(i=0;i<16;i++){
            if(B.aray[i]==val){
                B.aray[pos] = val;
                B.aray[i] = 0;
                pos = i;
                break;
            }
        }
        total_swaps++;
    }
    //printf("Total swaps: %d\n", total_swaps);
    return total_swaps;
}



bool doInsertion(Board current, int new_pos, int pos)
{
    int i, val;

    Board *copy_current = new Board;
    for(i=0;i<16;i++) copy_current->aray[i] = current.aray[i];
    copy_current->fn = current.fn;
    copy_current->gn = current.gn;
    copy_current->hn = current.hn;
    copy_current->parent = current.parent;

    //cout << "here"<<endl;

    Board new_board;
    for(i=0;i<16;i++) new_board.aray[i] = current.aray[i];
    val = new_board.aray[new_pos];
    new_board.aray[new_pos] = 0;
    new_board.aray[pos] = val;
    new_board.parent = copy_current;
    new_board.gn = current.gn + 1;
    if(current_heuristic == NOOFMISPLACEDTILES){
        new_board.hn = double(NoOfMisplacedTilesCost(new_board));
    }
    else if(current_heuristic == EUCLIDEAN){
        new_board.hn = EuclideanCost(new_board);
    }
    else if(current_heuristic == MANHATTEN){
        new_board.hn = double(ManhattenCost(new_board));
    }
    else if(current_heuristic == NOOFTILEOUTOF){
        new_board.hn = double(NoOfTilesOutOfCost(new_board));
    }
    else if(current_heuristic == NMAXSWAP){
        new_board.hn = double(NMaxSwapCost(new_board, new_pos));
    }
    new_board.fn = new_board.gn + new_board.hn;
    //print_board(new_board);

    if(checkGoalState(new_board)){
        print_all_boards(new_board);
        return true;
    }
    //cout << "there"<<endl;
    openList.push(new_board);

    return false;
}



bool doMovement(Board current, int pos) //pos is the position of 0
{
    int i, j, k, posx, posy, ax, ay, new_pos, val;
    posx = pos/4; posy = pos%4;
    bool fact = false;

    //up
    ax = posx;
    ay = posy;
    ax = ax-1;
    if(ax>=0 && ay>=0 && ax<=3 && ay<=3){
        new_pos = 4*ax + ay;
        fact = doInsertion(current, new_pos, pos);
        if(fact==true) return true;
    }

    //down
    ax = posx;
    ay = posy;
    ax = ax+1;
    if(ax>=0 && ay>=0 && ax<=3 && ay<=3){
        new_pos = 4*ax + ay;
        fact = doInsertion(current, new_pos, pos);
        if(fact==true) return true;
    }

    //right
    ax = posx;
    ay = posy;
    ay = ay+1;
    if(ax>=0 && ay>=0 && ax<=3 && ay<=3){
        new_pos = 4*ax + ay;
        fact = doInsertion(current, new_pos, pos);
        if(fact==true) return true;
    }

    //left
    ax = posx;
    ay = posy;
    ay = ay - 1;
    if(ax>=0 && ay>=0 && ax<=3 && ay<=3){
        new_pos = 4*ax + ay;
        fact = doInsertion(current, new_pos, pos);
        if(fact==true) return true;
    }
    return false;
}

void applyHeuristics()
{
    int i, a, b, c, d, x, y, curr_cost, turns=0, pos;
    bool fact = false;
    while(true){
        Board current = openList.top();
        openList.pop();
        map<Board,int>::iterator it;
        if(!myOpenMap.empty()){
            it = myOpenMap.find(current);
            myOpenMap.erase(it);
        }

        pair<map<Board,int>::iterator,bool> ret;
        ret = myClosedMap.insert( pair<Board,int>(current, current.gn));
        if(ret.second == true){ // if the node can be inserted into the closed list meaning this is a new node.
            closedList.push_back(current);
            for(i=0;i<16;i++){
                if(current.aray[i]==0){
                    pos = i;
                    break;
                }
            }
            fact = doMovement(current, pos);
            if(fact==true) break;
        }
    }


    //printf("Success :D\n");
}

void clearAll()
{
    openList = priority_queue<Board, vector<Board>, Compare>();
    closedList.clear();
    myOpenMap.clear();
    myClosedMap.clear();
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
    int i, j, k, a, b, c;
    Board input;

    cout <<"Enter configuration: "<<endl;
    for(i=0;i<16;i++){
        cin >> a;
        input.aray[i] = a;
    }
    cout << "--------------------------------------" <<endl;
    input.fn = 0;
    input.gn = 0;
    input.hn = 0;
    input.parent = NULL;

    openList.push(input);
    myOpenMap.insert(pair<Board,int>(input,0));

    if(!checkGoalState(input)){
        current_heuristic = NOOFMISPLACEDTILES;
        applyHeuristics();
        printf("Length of closed list: %d\n", closedList.size());
        printf("Depth: %ld\n", depth_one);
        N = closedList.size();
        depth = depth_one;
        bisection(0.5, 10);


        clearAll();
        openList.push(input);
        myOpenMap.insert(pair<Board,int>(input,0));
        current_heuristic = EUCLIDEAN;
        applyHeuristics();
        printf("Length of closed list: %d\n", closedList.size());
        printf("Depth: %ld\n", depth_two);
        N = closedList.size();
        depth = depth_one;
        bisection(0.5, 10);


        clearAll();
        openList.push(input);
        myOpenMap.insert(pair<Board,int>(input,0));
        current_heuristic = MANHATTEN;
        applyHeuristics();
        printf("Length of closed list: %d\n", closedList.size());
        printf("Depth: %ld\n", depth_three);
        N = closedList.size();
        depth = depth_one;
        bisection(0.5, 10);


        clearAll();
        openList.push(input);
        myOpenMap.insert(pair<Board,int>(input,0));
        current_heuristic = NOOFTILEOUTOF;
        applyHeuristics();
        printf("Length of closed list: %d\n", closedList.size());
        printf("Depth: %ld\n", depth_four);
        N = closedList.size();
        depth = depth_one;
        bisection(0.5, 10);


        clearAll();
        openList.push(input);
        myOpenMap.insert(pair<Board,int>(input,0));
        current_heuristic = NMAXSWAP;
        applyHeuristics();
        printf("Length of closed list: %d\n", closedList.size());
        printf("Depth: %ld\n", depth_five);
        N = closedList.size();
        depth = depth_one;
        bisection(0.5, 10);

    }
    else{
        cout << "Input was the Destination Board" <<endl;
    }


    return 0;
}


















