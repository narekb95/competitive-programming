python3 gen.py

g++ -std=c++14 -o online online.cpp
g++ -std=c++14 -o sol sol.cpp

./online < in3.txt > online.out
./sol < in3.txt > my.out

diff my.out online.out