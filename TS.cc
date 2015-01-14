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

#define Q 20000 //capacidad del carro
#define k 8  //cantidad de carros
#define tabu_ternure 5  //tabu list size tabu ternure

struct tabu{
  int x;
  int pos_x;
  int y;
  int pos_y;
};
typedef double db;
typedef vector<db> vd;
typedef vector<vector<db>> vvd;
typedef vector<tabu> vt;

void aspiration();
int is_tabu_move(int x, int y, int pos_x, int pos_y, vt &tabu_list);
void print_tabu_list(vt tabu_list);
void add_tabu_nodes(int x, int y, int pos_x, int posy, vt &tabu_list);
db neighbor_cost(vd n, vd &demand, vvd &costs);
void exchange(int i, int j, vd &n);
void insertion(int i, int j, db a, vd &n);
void get_neighbor(vd s, vd &demand, vvd &costs, db &best_cost, vd &best_s, vt &tabu_list, int i, int &it);
void load_costs(vvd &c);
void read_init_sol(vd &s);
void load_demand(vd &x);
void print(vd x);
void print_matriz(vvd &x);


int main(){
  vd s;
  vt tabu_list;
  vd demand;
  vvd costs;
  //db tabu_iter=5;

  db best_cost=1<<30;
  vd best_sol;
  int best_it=0;
  int max_it=1000000;
  int i=0;
  
  read_init_sol(s);
  cout<<"sol.size: "<<s.size()<<endl;
  load_costs(costs);
  load_demand(demand);
  cout<<"demand.size: "<<demand.size()<<endl;
  cout<<"init solution:"<<endl;
  print(s);
  cout<<endl<<"cost: "<<neighbor_cost(s,demand,costs)<<endl;

  /*  
  while(i++ < max_it){
    get_neighbor(s,demand,costs,best_cost,best_sol,tabu_list,i,best_it);
    cout<<"iteration "<<i<<" : ";
    //print(s);
  }
  
  cout<<"best solution in it="<<best_it<<endl;
  print(best_sol);
  cout<<"cost: "<<best_cost;
  */
  return 0;
}

//return 1 if tmp solution is accepted
int aspiration(db &best_cost, db &n_cost){
  if(n_cost < best_cost) return 1;
  return 0;
}
//return 1 if is tabu move otherwise return 0
int is_tabu_move(int x, int y, int pos_x, int pos_y, vt &tabu_list){
  for(auto i : tabu_list){
    if( ((i.x==x && i.y==y) || (i.x==y && i.y==x)) &&  
        ((i.pos_x==pos_x && i.pos_y==pos_y) || (i.pos_x==pos_y && i.pos_y==pos_x))) return 1;
  }
  return 0;
}

void print_tabu_list(vt tabu_list){
  cout<<"Tabu_list: (x,y) (pos_x,pos_y)"<<endl;
  for(auto i : tabu_list){
    cout<<"("<<i.x<<","<<i.y<<")("<<i.pos_x<<","<<i.pos_y<<")"<<endl;
  }
}
//add 2 nodes to tabu list
void add_tabu_nodes(int x, int y, int pos_x, int pos_y, vt &tabu_list){
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
db neighbor_cost(vd n, vd &demand, vvd &costs){
  db carga = 0;
  db costo = 0;
  
  for(db i=0; i<n.size(); i++){
    int to = n[i];
    if(carga > Q){
      cout<<carga<<": supera la carga ";
      return 0;
    }
    if(to == 1){
      carga = 0;
    }
    else{
      int from = n[i-1];
      carga += demand[to-1];
      costo += costs[from-1][to-1];
      cout<<"from: "<<from<<" to:"<<to<<" demanda: "<<demand[to-1]<<" costo:"<< costs[from-1][to-1]<<endl;
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
  //cout<<"exchanging "<<i<<" "<<j<<endl;
}

//generate all neighbors and evuale each of them
void get_neighbor(vd s, vd &demand, vvd &costs, db &best_cost, vd &best_s, vt &tabu_list, int i, int &it){
  db local_best_cost=1<<30;
  db x=0,y=0; //nodes involucrated in best neighbor
  int pos_x=0, pos_y=0; //dir of x and y
  vector<db>best_neighbor;
  
  for(db i=0; i<s.size(); i++){
    for(db j=0; j<s.size();j++){
      vector<db> tmp = s;
      db a = tmp[i];
      db b = tmp[j];
      if( a!=1 && b!=1  && b != 58 && a!=58 && a!=b){
        //insertion(i,j,a,tmp);
        exchange(i,j,tmp);
        db tmp_cost = neighbor_cost(tmp,demand,costs);
        if(!is_tabu_move(a,b,i,j,tabu_list)){
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
    best_s = best_neighbor;
    it = i;
  }
  //cout<<"best neighbor: "<<endl;
  //print(best_neighbor);
  cout<<"cost: "<<local_best_cost<<"nodes: ("<<x<<","<<y<<")("<<pos_x<<","<<pos_y<<")"<<endl;
  s = best_neighbor;
  add_tabu_nodes(x,y,pos_x,pos_y,tabu_list);
}


//load matriz of costs
void load_costs(vvd &c){
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
    c.push_back(lineData);
  }
  cout<<"costs size: "<<c[0].size()<<"X"<<c.size()<<endl;
}


//read aproximated solution vector
void read_init_sol(vd &s){
  fstream fs("init_sol", ios_base::in);
  db x;
  while(fs >> x){
    s.push_back(x);
  }
}
//load demand vector
void load_demand(vd &x){
  fstream fs("demand", ios_base::in);
  db tmp;
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
