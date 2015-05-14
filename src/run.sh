#!/usr/bin/bash
echo -e "it need c++11 standard"
echo -e '\033[1;36mCompilando TS.cc'
g++ TS.cc -o TS -std=c++11
echo -e '\033[1;32mOK!'
echo -e '\033[1;36mCompilando sweep.cc'
g++ sweep.cc -o sweep
echo -e '\033[1;32mOK!'
echo -e '\033[1;36mCompilando readSol.cc'
g++ readSol.cc -o readSol
echo -e '\033[1;32mOK!'
echo -e '\033[1;36mRuning sweep algorithm'
./sweep < sweep_input > TS_init_sol_input
echo -e '\033[1;36mRuning TS algorithm'
./TS < TS_init_sol_input > readSol_input
echo -e '\033[0;36mSolution:'
./readSol < readSol_input > readSol_output
cat readSol_output
