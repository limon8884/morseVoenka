f = open('f.txt')
s = f.read()
f.close()
s1 = s.lower()
s2 = s1.replace('\n', ' ')
s3 = s2.replace('.', '')
s4 = s3.replace(',', '')
l = s4.split(' ')
sf = l.pop(0)
while len(sf) < 5:
    sf += ' '
for line in l:
    sf += line[:4]
    sf += ' '
s = sf.rstrip()
g = open('s.txt', 'w')
#g.clear()
g.write(s)
g.close()
