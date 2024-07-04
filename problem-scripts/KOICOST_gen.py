from random import randint

def unique_rand_int(s,e,D):
    x = randint(s, e)
    while x in D:
        x = randint(s, e)
    D[x] = True
    return x

D = {}
n = 1000
m = 10000
Max = 100000
a = [(randint(1, n), randint(1, n), unique_rand_int(1, 100000, D)) for _ in range(m)]
print('{} {}'.format(n ,m))
for x in a:
    print('{} {} {}'.format(x[0], x[1], x[2]))