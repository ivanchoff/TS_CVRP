/*
 * ivanchoff@gmail.com
 *
 */

#include<bits/stdc++.h>

using namespace std;

typedef double db;
typedef vector<db> vd;
typedef vector<vector<db> > vvd;

#define final_node 58
#define init_node 1
#define Q 22000

void load_costs(vvd &costs);
void read_tabu_sol(vd &s);
void load_demand(vd &demand);
void load_time(vvd &time_costs);
db calc_cost(vd n,vd demand, vvd costs);

int main(){
  vvd costs;       //matriz de costos
  vvd time_costs;  //matriz de tiempos
  vd sol;          //solucion tabu
  vd demand;       //vector de demanda

  int k=0,q=0,time=0,cost;

  load_costs(costs);
  read_tabu_sol(sol);
  cout<<"tabu sol size: "<<sol.size()<<endl;
  load_time(time_costs);
  load_demand(demand);
  cout<<"demand size: "<<demand.size()<<endl;

  for(int i=0; i<sol.size(); i++){
    db tmp=sol[i];
    if(tmp==init_node){
      k++;
      q=0;
      time=0;
    }else{
      time += time_costs[tmp][sol[i-1]];
      if(tmp!=final_node){
        q += demand[i];

      }else{
        cout<<"Carro: "<<k<<"  Carga: "<<q<<"  Time: "<<time<<endl;
      }
    }
  }
  cout<<"Costo total: "<<calc_cost(sol,demand,costs)<<endl;
  return 0;
}
//load matrix with distaces data
void load_costs(vvd &costs){
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
  cout<<"costs size: "<<costs[0].size()<<"X"<<costs.size()<<endl;
}
//read aproximated solution vector
void read_tabu_sol(vd &s){
  fstream fs("readSol_input", ios_base::in);
  db x;
  while(fs >> x) s.push_back(x);
}
//load demand vector
void load_demand(vd &demand){
  fstream fs("demand", ios_base::in);
  db tmp;
  while(fs >> tmp){
    demand.push_back(tmp);
  }
}
//load matrix with time data
void load_time(vvd &time_costs){
  fstream fs("tiempos.txt",ios_base::in);
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
    time_costs.push_back(lineData);
  }
  cout<<"time costs size: "<<time_costs[0].size()<<"X"<<time_costs.size()<<endl;
}
//return 0 if is not valid solution otherwise return cost solution
db calc_cost(vd n, vd demand, vvd costs){
  db carga = 0;
  db costo = 0;
  for(db i=0; i<n.size(); i++){
    int to = n[i];
    if(carga > Q){
      cout<<carga<<": supera la carga"<<endl;
      return 0;
    }
    if(to == 1){
      carga = 0;
    }
    else{
      int from = n[i-1];
      carga += demand[to-1];
      costo += costs[from-1][to-1];
    }
  }
  return costo;
}
