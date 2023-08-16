import os
d = {" ":'0', "е":'e', "т":'t', "и":'i', "н":'n', "а":'a', "м":'m', "с":'s',
"д":'d', "р":'r', "г":'g', "у":'u',
"к":'k', "в":'w', "о":'o', "х":'h', "б":'b', "л":'l', "з":'z', "ф":'f',
"ц":'c', "п":'p', "й":'j', "щ":'q', "ж":'v', "ь":'x', "ы":'y', "ю":'1',
"э":'2', "ъ":'3', "ч":'4', "ш":'5', "я":'6'};

f = open('f.txt')
s = f.read()
f.close()
s1 = s.lower()
s2 = s1.replace('\n', ' ')
s3 = s2.replace('.', '')
s4 = s3.replace(',', '')
l1 = s4.split('%')
for sine in l1:
    sine2 = sine.strip()
    l = sine2.split(' ')
    sf = l.pop(0)
    while len(sf) < 5:
        sf += ' '
    start = sf
    for line in l:
        sf += line[:4]
        sf += ' '
    k = sf.rstrip()
    s5 = ''
    for i in range(5):
        a = d[start[i]]
        s5 += a
    os.mkdir('readyFiles\\' + s5)
    for i in range(len(k) // 55 + 1):
        string = 'readyFiles\\' + s5 + '\\' + str(i) + '.txt'
        g = open(string, 'w')
        for j in range(i * 55, (i + 1) * 55):
            if j != len(k):
                g.write(k[j])
            else:
                break;
        g.close()




