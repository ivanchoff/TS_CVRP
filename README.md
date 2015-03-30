This repos contain some programs for solve **CVRP** (Capacitated Vehicle Routing Problem) specifically for **"Collectors garbage  trucks"** using **sweep algorithm and Tabu Search** in C++ with C++11 standard.

**table of contents**

- [Sweep algorithm](#sweep.cc)
- [Tabu Search](#TS.cc)
- [View solution](#readSol.cc)

#sweep.cc
![sweep algorithm](http://w1.cirrelt.ca/~vidalt/en/VRPExample.png)

This program recieve from standar input the number of nodes, after that read info of each node as follows:

r Θ C id

where:

**r** is the radial coordinate (double)

**Θ** is the angular coordinate (double)

**C** is the capacity of each node (for this problem is the weight of garbage) (double)

**id** is an unique id of each node.

the input info is on file sweep_input.

for compile

```bash
g++ sweep.cc -o sweep
```

for run

```bash
 ./sweep < sweep_input
```

for save solution of sweep algorithm as input of Tabu Search

```bash
./sweep < sweep_input > TS_init_sol_input
```

#TS.cc

This program requires these files:

**demand**: matriz with nodes demand (double)

**costs**: matriz with costs between all nodes (double)

**TS_init_sol_input**: init solution for algorithm

for compile:

```bash
g++ TS.cc -o TS -std=c++11
```

for run:

```bash
./TS < TS_init_sol_input
```
for run and save solution in file:
```bash
./TS < TS_init_sol_input > readSol_input
```

#readSol.cc

This program only show data solution more friendly so readSol.cc requires these files:

**costs** matriz of nodes costs

**readSol_input** output solution vector of TS.cc

**demand** matriz of nodes demand

**tiempos.txt** matriz of nodes times

for compile

```bash
g++ readSol.cc -o readSol
```

for run

```bash
./readSol < readSol_input
```
