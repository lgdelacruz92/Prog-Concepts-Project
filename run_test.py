import os

TEXT_COLOR_GREEN = '\033[92m'
TEXT_COLOR_RED = '\033[91m'
TEXT_COLOR_END = '\033[0m'

def is_empty(file):
    with open(file, "r") as f:
        if f.read() == '':
            return True
        else:
            return False 

if __name__ == "__main__":
    os.system('make subc')
    for i in range(1, 26):
        number_suffix = '0{number}'.format(number=i) if (i < 10) else '{number}'.format(number=i)
        tiny = 'tiny_{number_suffix}'.format(number_suffix=number_suffix)
        os.system('./subc tiny_test_progs/{tiny} > tests/trees/{tiny}_result.tree 2>/dev/null'.format(tiny=tiny))
        os.system('diff tests/trees/{tiny}_result.tree tiny_test_progs/{tiny}.tree > tests/diff/{tiny}_diff'.format(tiny=tiny))
        if is_empty('{tiny}_diff'.format(tiny=tiny)):
            print(TEXT_COLOR_GREEN + '{tiny} success!'.format(tiny=tiny) + TEXT_COLOR_END)
        else:
            print(TEXT_COLOR_RED + '{tiny} failed!'.format(tiny=tiny) + TEXT_COLOR_END)
