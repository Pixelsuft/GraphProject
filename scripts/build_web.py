import os
import subprocess

cwd = os.path.dirname(os.path.dirname(__file__)) or os.getcwd()
out_dir = os.path.join(cwd, 'embuild')
if not os.path.isdir(out_dir):
    os.mkdir(out_dir)

input_src = [
    os.path.join(os.path.join(cwd, 'src', x))
    for x in os.listdir(os.path.join(cwd, 'src'))
    if x.endswith('.cpp')
]

cmdline = [
    'em++',
    *input_src,
    '-sUSE_SDL=3',
    '-sUSE_SDL_TTF=3',
    '-o',
    os.path.join(out_dir, 'index.html'),
]

subprocess.call(cmdline, encoding='utf-8', shell=True)
