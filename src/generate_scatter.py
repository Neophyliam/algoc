import random

scatter = []
for i in range(256):
    scatter.append(random.randint(0, 0xffffffff))
with open('scatter.c', 'w') as f:
    f.write('unsigned long scatter[] = {\n')
    for i in range(256):
        f.write('%d,\n' % scatter[i])
    f.write('};\n')
