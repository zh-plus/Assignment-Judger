import re
import shutil
import subprocess
import os

from functools import partial

from tools import CompilerException, JudgeInfo, print, contains

CORRECT_DISTANCE = 12965

info = JudgeInfo()


def check_and_save(out_str: str):
    if f'{CORRECT_DISTANCE}' in out_str:
        print('PASS', style='bold green')
        info[-1].update_last(success=True)
    else:
        print('FAIL', style='bold red')
        info[-1].update_last(success=False)

    print('\n\n')

    return


def print_process(p: subprocess.CompletedProcess, message: str):
    print(f"{message}:")
    print("Stdout:", style='bold green')

    if p.stdout:
        if len(p.stdout) > 20000:
            print(f'Stdout length too long: {len(p.stdout)}, only print firt 2000 letters.', style='bold red')
            print(p.stdout[:2000].decode('utf-8'))
        else:
            print(p.stdout)

    if p.stderr:
        print("Stderr (Can be warning here!):", style='bold red')

        if len(p.stderr) > 20000:
            print(f'Stderr length too long: {len(p.stdout)}, only print firt 2000 letters.', style='bold red')
            print(p.stderr[:2000].decode('utf-8'))
        else:
            print(p.stderr, style='red')

        if 'compil' in message.lower() and 'error' in p.stderr.lower():
            raise CompilerException()


def run(command: str, input_content=None, print_msg=""):
    partial_run = partial(subprocess.run, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd="assignments", timeout=5, encoding='utf-8')

    try:
        p = partial_run(command.split(" "), input=input_content)
        print_process(p, print_msg)

        return p.stdout
    except subprocess.TimeoutExpired as e:
        print("Time Out! Continue to next file.", style='bold red')
        print_process(e, "Error message:")

        return e.stdout.decode('utf-8')


def compile_test(clang_path, file, objective_path):
    info[-1].add_test('compile')

    try:
        run(f'{clang_path} {file} -o {objective_path}', print_msg="Compiling output")
    except CompilerException as e:
        info[-1].update_last(success=False)
        raise e
    else:
        info[-1].update_last(success=True)


def change_name_test(objective_path):
    info[-1].add_test('change_name')

    shutil.move('assignments/world_cities.csv', 'assignments/world_cities.tmp')

    run_input = """New York City
Hong Kong
bye"""

    print('[0] Change Name Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    shutil.move('assignments/world_cities.tmp', 'assignments/world_cities.csv')

    # Check output
    if contains(out_str, 'world_cities.csv', 'miss', 'file'):
        print('PASS', style='bold green')
        info[-1].update_last(success=True)
    else:
        print('FAIL', style='bold red')
        info[-1].update_last(success=False)

    print('\n\n')


def correctness_test(objective_path):
    info[-1].add_test('correctness')

    run_input = """New York City
Hong Kong
bye"""

    print('[1] Correctness Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    check_and_save(out_str)


def auto_select_test(objective_path):
    info[-1].add_test('auto_select')

    run_input = """New York
Hong Kong
bye"""

    print('[2] Auto Select Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    check_and_save(out_str)


def manual_select_test(objective_path):
    info[-1].add_test('manual_select')

    run_input = """New
Hong Kong
bye"""

    print('[3] Manually Select Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    has_numbers = lambda s: any(char.isdigit() for char in s)

    # The correct distance is CORRECT_DISTANCE
    if out_str and f'{CORRECT_DISTANCE}' not in out_str:
        if 'New York City' in out_str:
            str_index = out_str.find('New York City')

            match = re.search(r"\d", out_str[:str_index][::-1])

            if match:
                index = str_index - match.start() - 1

            if match and abs(index - str_index) < 5:
                run_input = f"""New
{out_str[index]}
Hong Kong
bye"""
            else:
                print('No number before New York City! Using full NYC name.', style='bold magenta')
                run_input = f"""New
New York City
Hong Kong
bye"""

            print('[3.1] Manually Select Test with Full NYC Name\n'
                  'Run with input:', style='bold magenta')
            print(run_input)
            out_str = run(f'./{objective_path}', input_content=run_input)

    check_and_save(out_str)


def ignore_whitespace_test(objective_path):
    info[-1].add_test('ignore_ws')

    run_input = """  New York City 
Hong Kong
bye"""

    print('[4] Ignore Whitespace Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    check_and_save(out_str)


def case_insensitive_test(objective_path):
    info[-1].add_test('ignore_whitespace')

    run_input = """  New York City 
Hong Kong
bye"""

    print('[4] Ignore Whitespace Test\n'
          'Run with input:', style='bold magenta')
    print(run_input)

    out_str = run(f'./{objective_path}', input_content=run_input, print_msg="Run output")

    check_and_save(out_str)


def judge(file: str):
    print(f'Judging {file}', style='bold blue')
    info.add_student(file.replace('.cpp', ''))

    clang_path = "/home/linuxbrew/.linuxbrew/bin/clang++"
    objective_path = file.replace(".cpp", ".o")

    try:
        compile_test(clang_path, file, objective_path)
    except CompilerException as e:
        print('Compiler Error!', style='bold red')
        print('Continue to next file.', style='bold red')
        return

    change_name_test(objective_path)
    correctness_test(objective_path)
    auto_select_test(objective_path)
    manual_select_test(objective_path)
    ignore_whitespace_test(objective_path)


if __name__ == '__main__':
    # judge('11911626.cpp')
    for root, dirs, files in os.walk('assignments'):
        valid_names = list(filter(lambda s: s.endswith('.cpp'), files))
        for filename in valid_names:
            judge(filename)
            print('===============================================================', style='bold yellow')

    info.summary()
