#include<bits/stdc++.h>
using namespace std;

int color[9][9]; //0 = white, 1 = grey, 2 = black
int result;
ofstream file;


void DFS(int x, int y, int len){
    int a, b, c, d, e, f, i, j, times=0, hold=0;
    color[x][y] = 1;

    a = x+1; b = y; //right
    c = x; d = y-1; //down
    e = x-1; f = y+1; //diagonal up left

    if(a>=1 && a<=len && b>=1 && b<=len && color[a][b]==0){ //right
        DFS(a,b,len);
        if(result==1) return ;
    }
    else times++;

    if(c>=1 && c<=len && d>=1 && d<=len && color[c][d]==0){ //down
        DFS(c,d,len);
        if(result==1) return ;
    }
    else times++;

    if(e>=1 && e<=len && f>=1 && f<=len && color[e][f]==0){ //left diagonal up
        DFS(e,f,len);
        if(result==1) return ;
    }
    else times++;

    if(times == 3){
        hold = 0;
        for(i=1;i<=len;i++){
            for(j=1;j<=len;j++){
                if(color[i][j] == 1) hold++;
            }
        }


        if(hold == (len*len)){
            result = 1;
            return ;
        }
        else{
            if(result==0) result = -1;
        }
    }
    color[x][y] = 0;

    return ;

}


void createBoard(int len){
    int x, y, cnt;

    for(y = len;y > 0;y--){
         for(x = 1;x <= len;x++){
            result = 0;

            int i, j;
            for(i=1;i<=len;i++){
                for(j=1;j<=len;j++){
                    color[i][j] = 0;
                }
            }

            DFS(x, y, len);

            if(result == 1) {
                //cout<<"Y  ";
                file << "Y  ";
            }
            else if(result==-1){
                //cout<<"N  ";
                file << "N  ";
            }

         }
         //cout<<endl;
         file <<"\n";
    }

}


int main(){
    int boardlen, i, j, k, x, y;


    file.open("output.txt");

    for(boardlen=2;boardlen<=8;boardlen++){
        //cout<<"--------------------------------------"<<endl;

        file << "--------------------------------------\n";
        file << boardlen <<endl;

        createBoard(boardlen);
        //cout<<"--------------------------------------"<<endl<<endl;

        file << "--------------------------------------\n\n";
    }

    file.close();

    return 0;
}



























