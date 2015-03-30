/*
 * choff@riseup.net
 * tabu_search
 */

#include<bits/stdc++.h>

using namespace std;

#define Q 22000           //capacity of each car
#define k 8               //number of cars
#define tabu_ternure 5    //tabu list size - tabu ternure
#define div_list_size 200 //lenght of diversification list

struct tabu{
  int x;
  int pos_x;
  int y;
  int pos_y;
};

typedef double db;
typedef vector<db> vd;
typedef vector<vector<db> > vvd;
typedef map<db,vd> mvd;
typedef vector<tabu> vt;
typedef set<vector<double> > svd;

void aspiration();
int is_tabu_move(int x, int y, int pos_x, int pos_y);
void add_tabu_nodes(int x, int y, int pos_x, int posy);
void add_to_div_list(vd n, db cost);
db neighbor_cost(vd n);
void exchange(int i, int j, vd &n);
void insertion(int i, int j, db a, vd &n);
void get_neighbor(int i, int &it, int &flag,db &int_best_cost);
void load_costs();
void read_init_sol();
void load_demand();
void print(vd x);
void print_matriz(vvd &x);
void print_map(mvd);
void print_tabu_list(vt tabu_list);

vd s;                //local solution
vt tabu_list;        //list of tabú movements
mvd div_list;        //map that  contain the best 200 solution.
svd div_tabu;        //set with before taken solution.
vd demand;           //vector of demand
vvd costs;           //matrix of costs
vd best_sol;         //global solution
db best_cost=1<<30;  //global solution cost

int main(){

  int best_it=0;
  int max_it=100;
  int i=0;
  int flag=1;

  read_init_sol();                       //load init solution
  //cout<<"sol.size: "<<s.size()<<endl;
  load_costs();                          //load costs
  load_demand();                         //load demand
  //cout<<"demand.size: "<<demand.size()<<endl;
  //cout<<endl<<"init solution cost: "<<neighbor_cost(s)<<endl;

  while(i++ < max_it){
    db int_best_cost;
    if(div_list.size()>0){
      s = div_list.begin()->second;      //get s' for diversification
      div_list.erase(div_list.begin());
    }
    //cout<<"iteration "<<i<<"  cost: "<<neighbor_cost(s)<<endl;
    //print_map(div_list);
    flag=1;
    while(flag){
      div_tabu.insert(s);                          //set s' as tabu move
      int_best_cost=neighbor_cost(s);              //get s' cost
      get_neighbor(i,best_it,flag,int_best_cost);  //get best neighbor of s'
    }
  }

  //cout<<"best solution in it="<<best_it<<endl;
  print(best_sol);
  //cout<<"cost: "<<best_cost;
  return 0;
}


//return 1 if tmp_solution is better that best_solution
int aspiration(db &best_cost, db &n_cost){
  if(n_cost < best_cost) return 1;
  return 0;
}
//eval move and return 1 if is tabu move otherwise return 0
int is_tabu_move(int x, int y, int pos_x, int pos_y){
  for(auto i : tabu_list){
    if( ((i.x==x && i.y==y) || (i.x==y && i.y==x)) &&
        ((i.pos_x==pos_x && i.pos_y==pos_y) || (i.pos_x==pos_y && i.pos_y==pos_x))) return 1;
  }
  return 0;
}
//add 2 nodes to tabu list
void add_tabu_nodes(int x, int y, int pos_x, int pos_y){
  tabu tmp;
  tmp.x = x;
  tmp.y = y;
  tmp.pos_x = pos_x;
  tmp.pos_y = pos_y;
  if(tabu_list.size() < tabu_ternure){
    tabu_list.emplace(tabu_list.begin(),tmp);
  }
  else{
    tabu_list.pop_back();
    tabu_list.emplace(tabu_list.begin(),tmp);
  }
}
//return 0 if is not valid solution otherwise return cost solution
db neighbor_cost(vd n){
  db carga = 0;
  db costo = 0;

  for(db i=0; i<n.size(); i++){
    int to = n[i];
    if(carga > Q){
      //cout<<carga<<": supera la carga"<<endl;
      return 0;
    }
    if(to == 1){
      carga = 0;
    }
    else{
      int from = n[i-1];
      carga += demand[to-1];
      costo += costs[from-1][to-1];
      //cout<<"from: "<<from<<" to:"<<to<<" demanda: "<<demand[to-1]<<" costo:"<< costs[from-1][to-1]<<endl;
      }
  }
  return costo;
}

//insertion operator for neighbor selection
void insertion(int i, int j, db a, vd &n){
  n.erase(n.begin()+i);
  n.emplace(n.begin()+j,a);
}
//exchange operator for neighbor selection
void exchange(int i, int j, vd &n){
  db tmp = n[i];
  n[i] = n[j];
  n[j] = tmp;
}

//add neighbor to divesification list
void add_to_div_list(vd n, db cost){
  if(!div_tabu.count(n)){
    if(!div_list.count(cost)){
      if(div_list.size()>div_list_size)
        div_list.erase(++div_list.rbegin().base());
      else
        div_list[cost]=n;
    }
  }
}
//generate all neighbors and evuale each of them
void get_neighbor(int i, int &it, int &flag,db &int_best_cost){
  db local_best_cost=1<<30;
  db x=0,y=0;                //nodes involucrated in best neighbor
  int pos_x=0, pos_y=0;      //dir of x and y
  vector<db>best_neighbor;
  int l=1;

  //exchange all positions evaluating and choosing best neigbor
  for(db i=0; i<s.size(); i++){
    for(db j=0; j<s.size();j++){
      vector<db> tmp = s;
      db a = tmp[i];
      db b = tmp[j];
      if( a!=1 && b!=1  && b != 58 && a!=58 && a!=b){
        exchange(i,j,tmp);                  //exchange positions
        db tmp_cost = neighbor_cost(tmp);   //get cost of neighbor

        if(tmp_cost!=0) add_to_div_list(tmp,tmp_cost); //add posible solution to diversification list

        if(!is_tabu_move(a,b,i,j)){
          if(tmp_cost < local_best_cost && tmp_cost!=0){
            local_best_cost = tmp_cost;
            best_neighbor = tmp;
            x=a,y=b;
            pos_x=i, pos_y=j;
          }
        }
        else if(aspiration(best_cost,tmp_cost)){
          local_best_cost = tmp_cost;
          best_neighbor = tmp;
          x=a,y=b;
          pos_x=i, pos_y=j;
        }
      }
    }
  }
  if(local_best_cost < best_cost){
    best_cost = local_best_cost;
    best_sol = best_neighbor;
    it = i;
  }
  if(local_best_cost < int_best_cost){
    s = best_neighbor;
    add_tabu_nodes(x,y,pos_x,pos_y);
    //cout<<"----> cost: "<<local_best_cost<<" nodes: ("<<x<<","<<y<<")("<<pos_x<<","<<pos_y<<")"<<endl;
  }else {
    flag=0;
    //cout<<"---->no tiene mejor vecino, local_best_cost: "<<local_best_cost<<endl;
  };

}
//load matrix with distaces data
void load_costs(){
  fstream fs("costs",ios_base::in);
  string line;
  while(!getline(fs, line, '\n').eof()) {
    istringstream reader(line);
    vector<db> lineData;
    string::const_iterator i = line.begin();
    while(!reader.eof()) {
      db val;
      reader >> val;
      if(reader.fail())
        break;
      lineData.push_back(val);
    }
    costs.push_back(lineData);
  }
  //cout<<"costs size: "<<costs[0].size()<<"X"<<costs.size()<<endl;
}
//read aproximated solution vector
void read_init_sol(){
  fstream fs("TS_init_sol_input", ios_base::in);
  db x;
  while(fs >> x) s.push_back(x);
}
//load demand vector
void load_demand(){
  fstream fs("demand", ios_base::in);
  db tmp;
  while(fs >> tmp){
    demand.push_back(tmp);
  }
}

/*
//########################################
//##### functions for debug algorithm ####
//########################################
*/

//print vector
void print(vd x){
  for(auto i : x){
    cout<<i<<" ";
  }
  cout<<endl;
}
//print costs matriz
void print_matriz(vvd &x){
  cout<<"matriz content: "<<endl;
  for(auto i : x){
    for(auto j : i){
      cout<<j<<" ";
    }
    cout<<endl;
  }
}
void print_tabu_list(vt tabu_list){
  cout<<"Tabu_list: (x,y) (pos_x,pos_y)"<<endl;
  for(auto i : tabu_list){
    cout<<"("<<i.x<<","<<i.y<<")("<<i.pos_x<<","<<i.pos_y<<")"<<endl;
  }
}
//print map of posible solutions for diversification
void print_map(mvd x){
  for(auto i : x){
    cout<<i.first<<endl;
  }
}
