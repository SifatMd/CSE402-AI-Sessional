#include<bits/stdc++.h>
#include<time.h>
using namespace std;
#define INF 1000000007


int N;
vector<pair<int,int> > coord;
vector<double> distances[100];
int markers[100];
double h1, h2, h3, nncost=0;
int curr_heuristic;


vector<int> nnpath;
vector<int> temp2, copytemp2, savetemp2;
vector<int> H2path, H3path, H4path;
vector<int> DFSpath;
vector<int> optFirst, optSecond, optThird, optBest;

double calcDistance(int x1, int y1, int x2, int y2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}



double NearestNeighbor()
{
    int i, j, k;
    double sum = 0.0;
    srand (time(NULL));
    int start = rand()%N;
    //int start = 0; // index 2 is the starting city
    int visited = 0, current, a, b, c;
    current = start;
    markers[current] = 1;
    nnpath.push_back(current);
    printf("Nearest Neighbor Path: \n%d -> ", current);
    while(true){
        double min_ = INF;
        for(i=0;i<N;i++){
            if(!markers[i] && i!=current && distances[current][i]<min_){
                min_ = distances[current][i];
                k = i;
            }
        }
        nnpath.push_back(k); //this contains the path of nearest neighbor heuristic
        //printf("Min: %f\n", min_);
        current = k;
        markers[current] = 1;
        visited++;
        sum += min_;
        //printf("Min: %f\n", sum);
        if(visited == N-1) break;
        printf("%d -> ", k);
    }
    sum += distances[current][start];
    printf("%d ", k);
    //cout << "Start " << start << ", Sum: " << sum << endl;
    cout << endl;
    return sum;
}


double calcDistanceVector(int ss)
{
    int i, j, k ;
    double cost = 0.0;
    for(i=0;i<ss-1;i++){
        cost += distances[temp2[i]][temp2[i+1]];
    }
    cost += distances[temp2[ss-1]][temp2[0]];
    return cost;
}

double Heuristic2Util()
{
    int i, j, k, ss;
    double cost = 0, min_ = INF;
    ss = temp2.size();
    copytemp2 = vector<int>(ss);
    for(i=0;i<ss;i++){
        for(j=0;j<ss;j++){
            if(i!=j){
                swap(temp2[i], temp2[j]);
                cost = calcDistanceVector(ss);
                if(cost < min_){
                    min_ = cost;
                    for(k=0;k<ss;k++){
                        copytemp2[k] = temp2[k];
                    }
                }
                swap(temp2[i], temp2[j]);
            }
        }
    }
    return min_;
}




double Heuristic3Util()
{
    int i, j, k, ss, one, two;
    double cost = 0, min_ = INF;
    ss = temp2.size();
    copytemp2 = vector<int>(ss);
    for(i=1;i<ss-2;i++){
        for(j=i+2;j<ss;j++){
            one = i; two = j;
            while(one<two){
                swap(temp2[one], temp2[two]);
                one++; two--;
            }

            cost = calcDistanceVector(ss);
            if(cost < min_){
                min_ = cost;
                for(k=0;k<ss;k++){
                    copytemp2[k] = temp2[k];
                }
            }
            one = i; two = j;
            while(one<two){
                swap(temp2[one], temp2[two]);
                one++; two--;
            }

        }
    }
    return min_;

}


void copyIntoTemp2()
{
    int i, j, k;
    for(i=0;i<optThird.size();i++) temp2[i] = optThird[i];
    for(j=0;j<optFirst.size();j++, i++) temp2[i] = optFirst[j];
    for(j=0;j<optSecond.size();j++, i++) temp2[i] = optSecond[j];
}

void doSwap(int mark)
{
    int i, siz;
    if(mark==1){
        siz = optFirst.size();
        for(i=0;i<(siz/2);i++){
            swap(optFirst[i], optFirst[siz-i-1]);
        }
    }
    else if(mark==2){
        siz = optSecond.size();
        for(i=0;i<(siz/2);i++){
            swap(optSecond[i], optSecond[siz-i-1]);
        }
    }
    else if(mark==3){
        siz = optThird.size();
        for(i=0;i<(siz/2);i++){
            swap(optThird[i], optThird[siz-i-1]);
        }
    }
}


void SwapOpt2and3()
{
    int i, j, k, s1, s2;
    s1 = optFirst.size();
    s2 = optSecond.size();
    int *over1 = new int[s1];
    int *over2 = new int[s2];

    for(i=0;i<s1;i++) over1[i] = optFirst[i];
    for(i=0;i<s2;i++) over2[i] = optSecond[i];

    optFirst.clear(); optSecond.clear();
    optFirst = vector<int>(s2);
    optSecond = vector<int>(s1);

    for(i=0;i<s2;i++) optFirst[i] = over2[i];
    for(i=0;i<s1;i++) optSecond[i] = over1[i];

    delete over1;
    delete over2;
}

double doManipulationFor3Opt(int ss)
{
    int i, jj=0;
    double cost = 0, min_ = INF;

    while(jj<2){
        //reverse order of optfirst
        doSwap(1);
        copyIntoTemp2();
        doSwap(1); //returning everything to normal
        cost = calcDistanceVector(ss);
        if(cost<min_){
            min_ = cost;
            for(i=0;i<ss;i++) optBest[i] = temp2[i];
        }

        //reverse order of optSecond
        doSwap(2);
        copyIntoTemp2();
        doSwap(2); //returning everything to normal
        cost = calcDistanceVector(ss);
        if(cost<min_){
            min_ = cost;
            for(i=0;i<ss;i++) optBest[i] = temp2[i];
        }

        //reverse order of both
        doSwap(1);
        doSwap(2);
        copyIntoTemp2();
        doSwap(1); //returning everything to normal
        doSwap(2);
        cost = calcDistanceVector(ss);
        if(cost<min_){
            min_ = cost;
            for(i=0;i<ss;i++) optBest[i] = temp2[i];
        }

        if(jj==1) break;
        //swapping first and second segment into one another
        SwapOpt2and3();
        copyIntoTemp2();
        cost = calcDistanceVector(ss);
        if(cost<min_){
            min_ = cost;
            for(i=0;i<ss;i++) optBest[i] = temp2[i];
        }

        jj++;
    }


    return min_;
}





double Heuristic4Util()
{
    int i, j, k, ss, one, two, ii, three, four, five, six;
    double cost = 0, min_ = INF;
    ss = temp2.size();
    copytemp2 = vector<int>(ss);
    savetemp2 = vector<int>(ss);
    optBest = vector<int>(ss);
    for(i=0;i<ss;i++){
        for(j=i+2;j<ss;j++){
            for(k=j+2;k<ss;k++){
                for(ii=0;ii<ss;ii++) savetemp2[ii] = temp2[ii];

                //copy the separate segments into separate vectors
                for(ii=i+1;ii<=j;ii++) optFirst.push_back(temp2[ii]);
                for(ii=j+1;ii<=k;ii++) optSecond.push_back(temp2[ii]);
                for(ii=k+1;ii<ss;ii++) optThird.push_back(temp2[ii]);
                for(ii=0;ii<=i;ii++) optThird.push_back(temp2[ii]);

                cost = doManipulationFor3Opt(ss);

                if(cost < min_){
                    min_ = cost;
                    for(ii=0;ii<ss;ii++){
                        copytemp2[ii] = optBest[ii];
                    }
                }
                for(ii=0;ii<ss;ii++) temp2[ii] = savetemp2[ii];
                optFirst.clear(); optSecond.clear(); optThird.clear();
            }
        }
    }
    return min_;
}



void Heuristic2and3()
{
    double min_, cost, currcost;
    int i, j, k, ss;
    ss = nnpath.size();
    temp2 = vector<int>(ss);
    printf("Initial: ---------------------------\n");
    for(i=0;i<ss;i++){
        temp2[i] = nnpath[i];
        if(i!=ss-1) printf("%d->", temp2[i]);
        else printf("%d", temp2[i]);
    }
    currcost = calcDistanceVector(ss);
    printf("\nCurrcost: %f\n------------------------------------\n", currcost);

    while(true){
        if(curr_heuristic==2) cost = Heuristic2Util();
        else if(curr_heuristic==3) cost = Heuristic3Util();
        else if(curr_heuristic==4) cost = Heuristic4Util();
        if(currcost>cost){
            for(i=0;i<ss;i++){
                temp2[i] = copytemp2[i];
                //printf("%d->", temp2[i]);
            }
            //printf("\nPresent Cost: %f\n", cost);
            currcost = cost;
        }
        else break;
    }

    if(curr_heuristic==2) printf("Cost of Vertex Swap : %f\n", currcost);
    if(curr_heuristic==3) printf("Cost of Edge Swap with 2-opt: %f\n", currcost);
    if(curr_heuristic==4) printf("Cost of Edge Swap with 3-opt: %f\n", currcost);
}



int minKey(bool included[], int keys[], int n)
{
    int i, index, min_ = INF;
    for(i=0;i<n;i++){
        if(included[i]==false && keys[i]<min_){
            min_ = keys[i];
            index = i;
        }
    }
    return index;
}


int* MST(int n)
{
    int i, j, k;
    bool included[n];
    int keys[n];
    int *parent = new int[n];

    for(i=0;i<n;i++) {
        included[i] = false;
        keys[i] = INF;
    }

    keys[0] = 0;
    parent[0] = -1;

    for(i=0;i<n-1;i++){
        int minkey = minKey(included, keys, n);
        included[minkey] = true;
        for(j=0;j<n;j++){
            if(distances[minkey][j]>0 && included[j]==false && distances[minkey][j]<keys[j]){
                parent[j] = minkey;
                keys[j] = distances[minkey][j];
            }
        }
    }
    /*
    printf("MST Path: \n");
    for(i=0;i<n;i++){
        printf("%d %d\n", parent[i], i);
    }*/
    return parent;
}


void DFS(vector<int> mstPaths[], int n, int *parent, int *color, int curr_node)
{
    int i, j, k;
    DFSpath.push_back(curr_node);
    color[curr_node] = 1;
    for(i=0;i<mstPaths[curr_node].size();i++){
        k = mstPaths[curr_node][i];
        if(color[k]==0) DFS(mstPaths, n, parent, color, k);
    }
    color[curr_node] = 2;
    return ;
}




int main()
{
    int i, j, k, a, b, c;
    double d;
    scanf("%d", &N);
    for(i=0;i<N;i++){
        scanf("%d %d", &a, &b);
        coord.push_back(make_pair(a,b));
    }
    for(i=0;i<N;i++){
        distances[i] = vector<double>(N);
    }

    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(i!=j && distances[i][j]==0){
                d = calcDistance(coord[i].first, coord[i].second, coord[j].first, coord[j].second);
                distances[i][j] = d;
                distances[j][i] = d;
                //cout << distances[i][j] <<endl;
            }
        }
    }

    /*for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            printf("%f ", distances[i][j]);
        }
        cout << endl;
    }*/

    printf("******************* Construction Algorithm: ************************\n\n");

    int *parent = MST(N);
    vector<int> mstPaths[N];
    for(i=0;i<N;i++){
        a = parent[i];
        if(a!=-1) mstPaths[a].push_back(i);
    }
    int *color = new int[N];
    for(i=0;i<N;i++) color[i] = 0;

    DFS(mstPaths, N, parent, color, 0);
    DFSpath.push_back(0);

    printf("DFS path: \n");
    h1 = 0;
    for(i=0;i<DFSpath.size();i++){
        if(i!=DFSpath.size()-1) {
            printf("%d -> ", DFSpath[i]);
            h1 += distances[DFSpath[i]][DFSpath[i+1]];
        }
        else printf("%d ", DFSpath[i]);

    }
    printf("\nCost of MST+DFS: %f\n\n\n", h1);


    //Nearest Neighbor
    nncost = NearestNeighbor();
    printf("Cost of Nearest Neighbor: %f\n", nncost);


    printf("\n\n******************* Improvement Algorithm: ************************\n\n");
    //Vertex Swap
    curr_heuristic = 2;
    Heuristic2and3();
    printf("\nPath: ");
    for(i=0;i<N;i++){
        H2path.push_back(temp2[i]);
        if(i!=N-1) printf("%d-> ", temp2[i]);
        else printf("%d \n\n", temp2[i]);
    }

    //Edge Swap - 2-opt
    curr_heuristic = 3;
    Heuristic2and3();
    printf("\nPath: ");
    for(i=0;i<N;i++){
        H3path.push_back(temp2[i]);
        if(i!=N-1) printf("%d-> ", temp2[i]);
        else printf("%d \n\n", temp2[i]);
    }


    //Edge Swap - 3 - opt
    curr_heuristic = 4;
    Heuristic2and3();
    printf("\nPath: ");
    for(i=0;i<N;i++){
        H4path.push_back(temp2[i]);
        if(i!=N-1) printf("%d-> ", temp2[i]);
        else printf("%d \n\n", temp2[i]);
    }



    return 0;
}














