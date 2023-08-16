d = {1:'e', 2:'t', 3:'i', 4:'n', 5:'a', 6:'m', 7:'s',
8:'d', 9:'r', 10:'g', 11:'u',
12:'k', 13:'w', 14:'o', 15:'h', 16:'b', 17:'l', 18:'z', 19:'f',
20:'c', 21:'p', 22:'j', 23:'q', 24:'v', 25:'x', 26:'y', 27:'1',
28:'2', 29:'3', 30:'4', 31:'5', 32:'6'};

s = ''
for i in range(1, 20):
    for j in range(1, 11):
        s += d[i]
        s += d[j]
        s += '000'
        string = 'readyFiles\\' + s + '.txt'
        g = open(string, 'w')
        for k in range(1, 100 * i + j * 10):
            g.write('е')
        g.close()
        s = ''
