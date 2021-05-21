from rich.console import Console
from rich.table import Column, Table

from enum import Enum

console = Console()
print = console.print


class CompilerException(Exception):
    pass


class StatusEnum(Enum):
    NON_TESTED = -1
    PASS = 0
    FAIL = 1


class TestStatus:
    def __init__(self, test_name: str, status=StatusEnum.NON_TESTED):
        self.test_name = test_name
        self.status = status

    def fail(self):
        self.status = StatusEnum.FAIL

    def success(self):
        self.status = StatusEnum.PASS


class StudentStatus:
    def __init__(self, student_id: str):
        self.sid = student_id
        self.tests = []

    def add_test(self, test_name):
        self.tests.append(TestStatus(test_name))

    def update_last(self, success):
        if success:
            self.tests[-1].success()
        else:
            self.tests[-1].fail()


class JudgeInfo:
    def __init__(self):
        self.student_ids = []
        self.student_status = []

    def add_student(self, student_id: str):
        self.student_ids.append(student_id)
        self.student_status.append(StudentStatus(student_id))

    def __getitem__(self, item):
        return self.student_status[item]

    def summary(self):
        print('Summary:', style='bold green')
        table = Table(show_header=True, header_style="bold magenta")

        table.add_column('SID', style='dim')
        for test in self.student_status[0].tests:
            table.add_column(test.test_name, style='dim', no_wrap=True)

        for student in self.student_status:
            row = [student.sid]
            for test in student.tests:
                if test.status == StatusEnum.PASS:
                    row.append("[bold green]PASS[/bold green]")
                elif test.status == StatusEnum.FAIL:
                    row.append("[bold red]FAIL[/bold red]")
                else:
                    row.append("[bold grey]NON-TESTED[/bold grey]")

            table.add_row(*row)
        print(table)


def contains(target: str, *sub_strs):
    return any(s in target for s in sub_strs)
