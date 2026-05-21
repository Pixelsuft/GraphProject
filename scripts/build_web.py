import os
import subprocess

cwd = os.path.dirname(os.path.dirname(__file__)) or os.getcwd()
out_dir = os.path.join(cwd, 'embuild')
if not os.path.isdir(out_dir):
    os.mkdir(out_dir)

cmdline = [
    'em++',
    os.path.join(cwd, 'jumbo', 'main.cpp'),
    '-sUSE_SDL=3',
    '-sUSE_SDL_TTF=3',
    '-fno-exceptions',
    '-fno-rtti',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-Wno-unused-parameter',
    '-O3',
    '-o',
    os.path.join(out_dir, 'index.html'),
]

subprocess.call(cmdline, encoding='utf-8', shell=True)
