from random import randint

def create_test():
    states = ['A', 'A', 'A', 'H', 'D']
    n = randint(4, 8)
    m = randint(4, 8)
    f.write(str(n) + ' ' + str(m) + '\n')
    for i in range(n):
        v = []
        for j in range(m):
            v.append(states[randint(0, len(states)-1)])
        f.write(' '.join(v) + '\n')

t = 100
f = open('input3.txt', 'w')
f.write(str(t) + '\n')
for i in range(t):
    create_test()