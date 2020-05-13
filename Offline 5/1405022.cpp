#include<bits/stdc++.h>
using namespace std;

int THRESHOLD_DEPTH = 10;
char Player = 'X';
char Opponent = 'O';
//char board[4][4];
int counter = 0;
bool fullSearch;

long long reached_nodes;

int minvalues[20];
int maxvalues[20];


void takeInput(char board[4][4])
{
    int i, j, k, input, row, colomn;
    while(true){
        printf("Present Configuration: \n");
        for(i=0;i<4;i++){
            for(j=0;j<4;j++) printf("%c ", board[i][j]);
            cout << endl;
        }
        cout << endl;
        printf("Enter move (Your input is O) : ");
        scanf("%d", &input);
        row = input/4;
        colomn = input%4;

        if(board[row][colomn]=='_') break;
        printf("Error!! Enter Again!!\n\n");
    }
    board[row][colomn] = 'O';
}


int getScore(char board[4][4])
{
    int i, j, k;

    for(i=0;i<4;i++){
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][2]==board[i][3]){
            if(board[i][0]==Player) return 10;
            else if(board[i][0]==Opponent) return -10;
        }
    }

    for(j=0;j<4;j++){
        if(board[0][j]==board[1][j] && board[1][j]==board[2][j] && board[2][j]==board[3][j]){
            if(board[0][j]==Player) return 10;
            else if(board[0][j]==Opponent) return -10;
        }
    }

    if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[2][2]==board[3][3]){
        if(board[0][0]==Player) return 10;
        else if(board[0][0]==Opponent) return -10;
    }

    if(board[0][3]==board[1][2] && board[1][2]==board[2][1] && board[2][1]==board[3][0]){
        if(board[0][3]==Player) return 10;
        else if(board[0][3]==Opponent) return -10;
    }

    return 0;
}



int getScoreHeuristic(char board[4][4])
{
    int i, j, k, a, b, c;

    //row check
    for(i=0;i<4;i++){
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][3]=='_'){
            if(board[i][0]==Player) return 10;
            else if(board[i][0]==Opponent) return -10;
        }
        else if(board[i][0]==board[i][1] && board[i][1]==board[i][3] && board[i][2]=='_'){
            if(board[i][0]==Player) return 10;
            else if(board[i][0]==Opponent) return -10;
        }
        else if(board[i][0]==board[i][2] && board[i][2]==board[i][3] && board[i][1]=='_'){
            if(board[i][0]==Player) return 10;
            else if(board[i][0]==Opponent) return -10;
        }
        else if(board[i][1]==board[i][2] && board[i][2]==board[i][3] && board[i][0]=='_'){
            if(board[i][1]==Player) return 10;
            else if(board[i][1]==Opponent) return -10;
        }
    }

    //colomn check
    for(j=0;j<4;j++){
        if(board[0][j]==board[1][j] && board[1][j]==board[2][j] && board[3][j]=='_'){
            if(board[0][j]==Player) return 10;
            else if(board[0][j]==Opponent) return -10;
        }
        else if(board[0][j]==board[1][j] && board[1][j]==board[3][j] && board[2][j]=='_'){
            if(board[0][j]==Player) return 10;
            else if(board[0][j]==Opponent) return -10;
        }
        else if(board[0][j]==board[2][j] && board[2][j]==board[3][j] && board[1][j]=='_'){
            if(board[0][j]==Player) return 10;
            else if(board[0][j]==Opponent) return -10;
        }
        else if(board[1][j]==board[2][j] && board[2][j]==board[3][j] && board[0][j]=='_'){
            if(board[1][j]==Player) return 10;
            else if(board[1][j]==Opponent) return -10;
        }
    }


    if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[3][3]=='_'){
        if(board[0][0]==Player) return 10;
        else if(board[0][0]==Opponent) return -10;
    }
    else if(board[0][0]==board[1][1] && board[1][1]==board[3][3] && board[2][2]=='_'){
        if(board[0][0]==Player) return 10;
        else if(board[0][0]==Opponent) return -10;
    }
    else if(board[0][0]==board[2][2] && board[3][3]==board[2][2] && board[1][1]=='_'){
        if(board[0][0]==Player) return 10;
        else if(board[0][0]==Opponent) return -10;
    }
    else if(board[2][2]==board[1][1] && board[3][3]==board[2][2] && board[0][0]=='_'){
        if(board[1][1]==Player) return 10;
        else if(board[1][1]==Opponent) return -10;
    }



    if(board[0][3]==board[1][2] && board[1][2]==board[2][1] && board[3][0]=='_'){
        if(board[0][3]==Player) return 10;
        else if(board[0][3]==Opponent) return -10;
    }
    else if(board[0][3]==board[1][2] && board[1][2]==board[3][0] && board[2][1]=='_'){
        if(board[0][3]==Player) return 10;
        else if(board[0][3]==Opponent) return -10;
    }
    else if(board[0][3]==board[2][1] && board[3][0]==board[2][1] && board[1][2]=='_'){
        if(board[0][3]==Player) return 10;
        else if(board[0][3]==Opponent) return -10;
    }
    else if(board[2][1]==board[1][2] && board[1][2]==board[3][0] && board[0][3]=='_'){
        if(board[1][2]==Player) return 10;
        else if(board[1][2]==Opponent) return -10;
    }



    int x=0, o=0;
    for(i=1;i<3;i++){
        for(j=1;j<3;j++){
            if(board[i][j]==Player) x++;
            else if(board[i][j]==Opponent) o++;
        }
    }
    if(x>o) return 10;
    else if(o>x) return -10;


    return 0;
}





bool isGameOver(char board[4][4])
{
    int i, j, k;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(board[i][j] == '_') return false;
        }
    }

    return true;
}




int minimax(char board[4][4], int depth, bool turn)
{
    int i, j, k, optimum;
    reached_nodes++;

    int val = getScore(board);
    if(val==10) return val;
    else if(val==-10) return val;


    if(isGameOver(board)) return 0;


    if(turn){
        optimum = INT_MIN;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                if(board[i][j]=='_'){
                    board[i][j] = Player;

                    if(fullSearch) {
                        optimum = max(optimum, minimax(board, depth+1, !turn));
                    }
                    else{
                        if(depth < THRESHOLD_DEPTH){
                            optimum = max(optimum, minimax(board, depth+1, !turn));
                        }
                        else{
                            optimum = max(optimum, getScoreHeuristic(board));
                        }
                    }

                    board[i][j] = '_';
                    if(maxvalues[depth] < optimum) break;
                }
            }
            if(maxvalues[depth] < optimum) break;
        }
        if(optimum<maxvalues[depth]){
            maxvalues[depth] = optimum;
        }

        return optimum;
    }
    else{
        optimum = INT_MAX;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                if(board[i][j]=='_'){
                    board[i][j] = Opponent;

                    if(fullSearch) {
                        optimum = min(optimum, minimax(board, depth+1, !turn));
                    }
                    else{
                        if(depth < THRESHOLD_DEPTH){
                            optimum = min(optimum, minimax(board, depth+1, !turn));
                        }
                        else{
                            optimum = min(optimum, getScoreHeuristic(board));
                        }
                    }
                    board[i][j] = '_';

                    if(optimum<=minvalues[depth]) break;
                }
            }
            if(optimum<=minvalues[depth]) break;
        }

        if(optimum>minvalues[depth]){
            minvalues[depth] = optimum;
        }

        return optimum;
    }

}







int main()
{
    int i, j, k, a, b, c, mode;
    int maxval, maxx, curr_move, value;
    reached_nodes = 0;

    printf("Enter Mode: \n1 for Full Search\n2 for Shallow Search\n");
    scanf("%d", &mode);
    if(mode==1){
        fullSearch = true;
    }
    else if(mode==2){
        fullSearch = false;
    }

    char board[4][4];
    for(i=0;i<4;i++){
        for(j=0;j<4;j++) board[i][j]='_';
    }

    /*char board[4][4] = {{'_','_','_','X'},
                        {'_','O','_','O'},
                        {'_','_','_','_'},
                        {'X','_','_','_'}};*/




    while(!isGameOver(board)){
        takeInput(board);

        value = getScore(board);
        if(value==-10) break;

        maxval = INT_MIN;
        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                if(board[i][j]=='_'){
                    board[i][j] = Player;
                    reached_nodes++;
                    for(int ii=0;ii<20;ii++) minvalues[ii] = -20, maxvalues[ii] = 20;

                    maxx = minimax(board, 0,false);
                    if(maxx > maxval){
                        maxval = maxx;
                        //printf("%d\n", maxval);
                        curr_move = (i*4) + j;
                    }

                    board[i][j] = '_';
                }
            }
        }
        printf("\n\nNumber of Function Calls: %d\n\n", reached_nodes);
        reached_nodes = 0;

        board[curr_move/4][curr_move%4] = Player;
        value = getScore(board);
        if(value==10) break;

    }

    for(i=0;i<4;i++){
        for(j=0;j<4;j++) printf("%c ", board[i][j]);
        cout << endl;
    }

    k = getScore(board);
    printf("\n\n------------------------------------------\n\n");
    if(k>0) printf("Computer won!!");
    else if(k<0) printf("Opponent won :( ");
    else printf("Draw ");


    return 0;
}













