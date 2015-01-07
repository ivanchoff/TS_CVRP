#include<bits/stdc++.h>

/*
Principios de TS:
->uso de estructuras de memoria flexibles
->mecanismo asociado de control
->funciones de memoria de diferentes lapsos de tiempo
  -->memoria a corto plazo (lista tabu): historial de los
     los ultimos movimientos realizados.
  -->memoria mediano plazo: atributos mas comunes de un 
     conjunto de soluciones
  -->memoria largo plazo:registro de zonas que no han sido
     exploradas aun.

->Estrategia de vecindad:
* operador lambda-intercambio
* mutacion
* insercion
* 2-opt -intercambio


-->CARACTERISTICAS:

---->vector de solucion 
---->matriz de costos
---->lista de demanda
---->lista tabu:
     -soluciones visitadas recientemente ||
     -movimientos realizados recientemente ||
     -atributos o caracteristicas que tenian soluciones visitadas
---->criterio de aspiracion

*/


/*

 * Definir criterio de aspiracion y correr el problema del pdf
 * agregar estrategia de intensificacion

*/



using namespace std;

#define Q 90 //capacidad del carro
#define k 3  //cantidad de carros
#define tabu_size 4  //tabu list size

typedef vector<double> vd;
typedef vector<vector<double>> vvd;
typedef vector<pair<int,int>> vp;

void aspiration();
int is_tabu_move(int x, int y, vp &tabu_list);
void print_tabu_list(vp tabu_list);
void add_tabu_nodes(int x, int y, vp &tabu_list);
double neighbor_cost(vd n, vd &demand, vvd &costs);
void insertion(vd s, vd &demand, vvd &costs, double &best_cost, vp &tabu_list);
void load_costs(vvd &c);
void read_init_sol(vd &s);
void load_demand(vd &x);
void print(vd x);
void print_matriz(vvd &x);


int main(){
  vd s;
  vd s_prima;
  vd s0;
  vp tabu_list;
  vd demand;
  vvd costs;
  double tabu_iter=5;
  
  double T = 0;
  double n_iter=0;
  double k_iter=0;
  double max_iter=100;

  double best_cost=3000000;

  int i=14000;
  
  read_init_sol(s);
  load_costs(costs);
  load_demand(demand);
  //cout<<"init solution: "<<endl;
  //print(s);
  //cout<<endl<<"demand: "<<endl;
  //print(demand);
  //cout<<endl;
  //cout<<endl<<"costs: "<<endl;
  //print_matriz(costs);
  //cout<<"cost: "<<neighbor_cost(s,demand,costs);
  //insertion(s,demand, costs);
  
  //while(n_iter-k_iter>= max_iter){
  //  n_iter++;
  //  s0=s;
    //seleccionar s
    /*
      if(f(s0)<f(s*)){
        s_prima = s;
        k_iter = n_iter;
      }
    */
  //}
  while(i--){
    insertion(s,demand,costs,best_cost,tabu_list);
    cout<<"iteration "<<i<<" ";
    print(s);
    //add_tabu_nodes(x++,y++,tabu_list);
    // print_tabu_list(tabu_list);
  }


  return 0;
}

//return 1 if tmp solution is accepted
int aspiration(double &best_cost, double &n_cost){
  if(n_cost < best_cost) return 1;
  return 0;
}

int is_tabu_move(int x, int y, vp &tabu_list){
  for(auto i : tabu_list){
    if(i.first == x && i.second == y) return 1;
  }
  return 0;
}

void print_tabu_list(vp tabu_list){
  cout<<"Tabu_list"<<endl;
  for(auto i : tabu_list){
    cout<<i.first<<" "<<i.second<<endl;
  }
}
//add 2 nodes to tabu list
void add_tabu_nodes(int x, int y, vp &tabu_list){
  if(tabu_list.size() < tabu_size){
    tabu_list.emplace(tabu_list.begin(),make_pair(x,y));
  }
  else{
    tabu_list.pop_back();
    tabu_list.emplace(tabu_list.begin(),make_pair(x,y));
  }
}
//return 0 if is not valid solution otherwise return cost solution
double neighbor_cost(vd n, vd &demand, vvd &costs){
  double carga = 0;
  double costo = 0;
  
  for(double i=0; i<n.size(); i++){
    int to = n[i];
    if(carga > Q){
      return 0;
    }
    if(to==16){
      carga = 0;
      if(i>0){
        int from = n[i-1];
        carga += demand[to-1];
        costo += costs[from-1][to-1];
        //cout<<"from: "<<from<<" to:"<<to<<" demanda: "<<demand[to-1]<<" costo:"<< costs[from-1][to-1]<<endl;
      }
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

//generate all neighbors and evuale each of them
void insertion(vd s, vd &demand, vvd &costs, double &best_cost, vp &tabu_list){
  double local_best_cost=2000000;
  double x=0,y=0; //nodos afectados en el vecino
  vector<double>best_neighbor;

  for(double i=0; i<s.size(); i++){
    for(double j=0; j<s.size();j++){
      vector<double> tmp = s;
      double a = tmp[i];
      double b = tmp[j];
      if(a != 16 && b != 16 && (a!=b)){      
        tmp.erase(tmp.begin()+i);
        tmp.emplace(tmp.begin()+j,a);
        //print(tmp);
        double tmp_cost = neighbor_cost(tmp,demand,costs);
        
        if(!is_tabu_move(a,b,tabu_list)){
          if(tmp_cost < local_best_cost && tmp_cost!=0){
            local_best_cost = tmp_cost;
            best_neighbor = tmp;
            x=a,y=b;
          }
        }
        else if(aspiration(best_cost,tmp_cost)){
          local_best_cost = tmp_cost;
          best_neighbor = tmp;
          x=a,y=b;
        }
      }
    }
  }
  if(local_best_cost < best_cost) best_cost = local_best_cost;
  cout<<"best neighbor: ";
  print(best_neighbor);
  cout<<"cost: "<<local_best_cost<<"nodes: "<<x<<" "<<y<<endl;
  s = best_neighbor;
  add_tabu_nodes(x,y,tabu_list);
}


//load matriz of costs
void load_costs(vvd &c){
  fstream fs("costos",ios_base::in);
  string line;

  while(!getline(fs, line, '\n').eof()) {
    istringstream reader(line);
    vector<double> lineData;
    string::const_iterator i = line.begin();
    while(!reader.eof()) {
      double val;
      reader >> val;
      if(reader.fail())
        break;
      lineData.push_back(val);
    }
    c.push_back(lineData);
  }
  cout<<"costs size: "<<c[0].size()<<"X"<<c.size()<<endl;
}


//read aproximated solution vector
void read_init_sol(vd &s){
  fstream fs("in", ios_base::in);
  double x;
  while(fs >> x){
    s.push_back(x);
  }
}
//load demand vector
void load_demand(vd &x){
  fstream fs("demand", ios_base::in);
  double tmp;
  while(fs >> tmp){
    x.push_back(tmp);
  }
}  

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
