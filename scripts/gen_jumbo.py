import os

cwd = os.path.dirname(os.path.dirname(__file__)) or os.getcwd()
if not os.path.isdir(os.path.join(cwd, 'jumbo')):
    os.mkdir(os.path.join(cwd, 'jumbo'))
out = open(os.path.join(cwd, 'jumbo', 'main.cpp'), 'w')
out.write('// Automatically generated with script, do not touch!\n')
out.write('#define JUMBO_BUILD\n')

asset_arrays = ''
asset_logic = ''

for fn in sorted(os.listdir(os.path.join(cwd, 'assets'))):
    var_name = f'asset_{fn.replace(".", "_").replace("-", "_")}'
    data = open(os.path.join(os.path.join(cwd, 'assets'), fn), 'rb').read()
    joined = ', '.join(map(hex, data))
    asset_arrays += f'static const unsigned char {var_name}[] = {{ {joined} }};\n'
    asset_logic += f'    else if (fn == "{fn}")\n'
    asset_logic += f'        return SDL_IOFromConstMem({var_name}, {len(data)});\n'

out.write(asset_arrays + '\n')

for fn in sorted(os.listdir(os.path.join(cwd, 'src'))):
    if not fn.endswith('.cpp'):
        continue
    out.write('// ' + fn + '\n')
    data = open(os.path.join(cwd, 'src', fn)).read()
    data = data.replace('#include "', '#include "../src/').replace(
        '// JUMBO_FILL_ASSETS', asset_logic
    )
    out.write(data + '\n')
