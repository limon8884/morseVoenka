import os
d = {" ":'0', "е":'e', "т":'t', "и":'i', "н":'n', "а":'a', "м":'m', "с":'s',
"д":'d', "р":'r', "г":'g', "у":'u',
"к":'k', "в":'w', "о":'o', "х":'h', "б":'b', "л":'l', "з":'z', "ф":'f',
"ц":'c', "п":'p', "й":'j', "щ":'q', "ж":'v', "ь":'x', "ы":'y', "ю":'1',
"э":'2', "ъ":'3', "ч":'4', "ш":'5', "я":'6'};


t = 's.txt'
f = open(t)
s1 = f.read(5)
s2 = ''
for i in range(5):
    a = d[s1[i]]
    s2 += a
os.mkdir('readyFiles\\' + s2)
s = f.read()
f.close()
for i in range(len(s) // 55 + 1):
	string = 'readyFiles\\' + s2 + '\\' + str(i) + '.txt'
	g = open(string, 'w')
	if i == 0:
		g.write(s1)	
	for j in range(i * 55, (i + 1) * 55):
		if j != len(s):
			g.write(s[j])
		else:
			break;
	g.close()

