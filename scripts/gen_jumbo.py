import os

cwd = os.path.dirname(os.path.dirname(__file__)) or os.getcwd()
if not os.path.isdir(os.path.join(cwd, 'jumbo')):
    os.mkdir(os.path.join(cwd, 'jumbo'))
out = open(os.path.join(cwd, 'jumbo', 'main.cpp'), 'w')

for fn in os.listdir(os.path.join(cwd, 'src')):
    if not fn.endswith('.cpp'):
        continue
    out.write('// ' + fn + '\n')
    data = open(os.path.join(cwd, 'src', fn)).read()
    data = data.replace('#include "', '#include "../src/')
    out.write(data + '\n')
