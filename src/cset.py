'''
A script to generate `cset` variable in text.c. Use :read !python3 cset.py to
modify text.c.
'''

cset_str = ''
for i in range(256):
    if i % 16 == 0:
        cset_str += '"'
    cset_str += r'\{:03o}'.format(i)
    if i % 16 == 15:
        cset_str += '"\n'
print(cset_str)
