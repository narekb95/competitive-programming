python3 gen.py

# g++ -std=c++14 -o other other.cpp
# g++ -std=c++14 -o sol sol.cpp

./other < ingen.txt > other.out
./sol < ingen.txt > my.out

diff my.out other.out