// choff@riseup.net

#include<bits/stdc++.h>

using namespace std;

/* sweep algorithm
 * this program read first number of nodes, after that
 * read each node as follows:
 * θ,r,C,id
 * where θ is angle, r is the distance from origin, C is
 * weigth or cost of node and id is an identity of each node.
 *
 */
#define UM unordered_map<int, vector<int> >
#define VVd vector<vector<double> >
#define Q 22000    //capacity of car
#define k 8        //# of cars

void printVVd(VVd &x){
    for(int i=0; i<x.size(); ++i){
        cout<<i<<": ";
        for(int j=0; j<x[0].size(); ++j)
            cout<<x[i][j]<<",";
        cout<<endl;
    }
}

void printV(vector<double> &x){
    for(int i=0; i<x.size(); ++i)
        cout<<x[i]<<" ";
    cout<<endl;
}

int main(){
    int n;    //# of nodes
    cin>>n;
    VVd nodes(n,vector<double> (4,0));
    //UM sol;
    vector<double> sol;
    for(int i=0; i<n; ++i)
        cin>>nodes[i][0]>>nodes[i][1]>>nodes[i][2]>>nodes[i][3];
    printVVd(nodes);
    sort(nodes.begin(),nodes.end());

    double capacity=0;

    for (int i=0; i<n; ++i){
        capacity += nodes[i][2];
        if(capacity<Q)
            sol.push_back(nodes[i][3]);
        else{
            sol.push_back(-1);
            sol.push_back(nodes[i][3]);
            capacity=nodes[i][2];
        }
    }
    printV(sol);
    // printVVd(nodes);
    return 0;
}
