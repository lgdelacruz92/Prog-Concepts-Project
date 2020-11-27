#!/usr/bin/python
import sys
import json
import os

if len(sys.argv) == 1:
    raise Exception('Error. No filename.')

filename = sys.argv[1]

label = 'build ' + filename + '.cpp'

# Add task.json setting
with open('./.vscode/tasks.json') as task_dot_json:
    task_dot_json_dict = json.load(task_dot_json)
    new_task = {
        "type": "shell",
        "label": "build " + filename,
        "command": "/usr/bin/clang++",
        "args": [
                "-std=c++17",
                "-stdlib=libc++",
                "-g",
                "./tests/" + filename + '.cpp',
                "parser.cpp",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
        ],
        "options": {
            "cwd": "${workspaceFolder}"
        },
        "problemMatcher": [
            "$gcc"
        ],
        "group": {
            "kind": "build",
            "isDefault": True
        }
    }
    task_dot_json_dict['tasks'].insert(0, new_task)
    with open('new_tasks.json', "w") as new_task_dot_json:
        json.dump(task_dot_json_dict, new_task_dot_json)

with open('./.vscode/launch.json') as launch_dot_json:
    launch_dot_json_dict = json.load(launch_dot_json)
    new_launch = {
        "name": "run " + filename,
        "type": "cppdbg",
        "request": "launch",
        "program": "${fileDirname}/${fileBasenameNoExtension}",
        "args": [],
        "stopAtEntry": True,
        "cwd": "${workspaceFolder}",
        "environment": [],
        "externalConsole": False,
        "MIMode": "lldb",
        "preLaunchTask": "build " + filename
    }
    launch_dot_json_dict['configurations'].insert(0, new_launch)
    with open('new_launch.json', "w") as new_launch_dot_json:
        json.dump(launch_dot_json_dict, new_launch_dot_json)

with open("Makefile") as make_file:
    with open("new_make_file", "w") as new_make_file:
        line_number = 0
        for line in make_file:
            if line_number == 3:
                new_make_file.write(
                    filename + ': ./tests/' + filename + '.cpp parser.o')
                new_make_file.write(
                    '\n\tg++ --std=c++17 ./tests/' + filename + '.cpp parser.o -o ' + filename)
                new_make_file.write('\n')
                new_make_file.write('\n')
            new_make_file.write(line)
            line_number += 1

def classify(name):
    tokens = name.split('_')
    classname = ''
    for token in tokens:
        classname += token.capitalize()
    return classname

with open("./tests/" + filename + ".cpp", "w") as test_file:
    classname = classify(filename)
    base_test_template = [
        '#include <iostream>\n',
        '#include <sstream>\n',
        '#include "../parser.h"\n',
        '#include "./test_utils.h"\n',
        'using namespace std;\n\n',

        'class ' + classname + ': public Parser {\n',
        'public:\n',
        '\t' + classname + '(istream* _fin) {\n',
        '\t\tfin = _fin;\n',
        '\t\tline = 0;\n',
        '\t}\n',
        '};\n\n',
        'int main() {\n',
        '\treturn 0;\n',
        '}'
    ]
    test_file.write(''.join(base_test_template))

os.system('rm ./.vscode/tasks.json')
os.system('rm ./.vscode/launch.json')
os.system('rm Makefile')

os.system('mv new_tasks.json ./.vscode/tasks.json')
os.system('mv new_launch.json ./.vscode/launch.json')
os.system('mv new_make_file Makefile')
