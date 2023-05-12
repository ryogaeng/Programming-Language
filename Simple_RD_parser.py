import sys

judge_bexpr = 0
token = ''
has_error = False

def getChar():
    global token
    token = sys.stdin.read(1)
    if token == ' ' or token == '\t':
        getChar()

def error():
    global has_error
    print(">> Syntax error!!")
    has_error = True

def lex(c):
    if token == c:
        getChar()
    else:
        error()

def expr():
    return bexpr()

def bexpr():
    global judge_bexpr
    left = aexpr()
    if token in ['=', '!', '<', '>']:
        op = relop()
        right = aexpr()
        judge_bexpr = 1
        if op == 0:
            return int(left == right)
        elif op == 1:
            return int(left != right)
        elif op == 2:
            return int(left < right)
        elif op == 3:
            return int(left > right)
        elif op == 4:
            return int(left <= right)
        elif op == 5:
            return int(left >= right)
        else:
            error()
            return 0
    else:
        judge_bexpr = -1
        return left

def relop():
    global token
    if token == '=':
        lex('=')
        if token == '=':
            lex('=')
            return 0
        else:
            error()
    elif token == '!':
        lex('!')
        if token == '=':
            lex('=')
            return 1
    elif token == '<':
        lex('<')
        if token == '=':
            lex('=')
            return 4
        return 2
    elif token == '>':
        lex('>')
        if token == '=':
            lex('=')
            return 5
        return 3
    else:
        error()

def aexpr():
    result = term()
    while not has_error and token in ['*', '/']:
        if token == '*':
            lex('*')
            result *= term()
        elif token == '/':
            lex('/')
            result //= term()
    return result

def term():
    result = factor()
    while not has_error and token in ['+', '-']:
        if token == '+':
            lex('+')
            result += factor()
        elif token == '-':
            lex('-')
            result -= factor()
    return result

def factor():
    global token
    if token == '(':
        lex('(')
        result = aexpr()
        if token == ')':
            lex(')')
        else:
            error()
    else:
        result = number()
    return result

def number():
    result = dec()
    while not has_error and token.isdigit():
        result = 10 * result + dec()
    return result

def dec():
    global token
    if not token.isdigit():
        error()
        return 0
    result = int(token)
    lex(token)
    return result

def print_expr():
    global has_error
    has_error = False
    result = expr()
    if has_error:
        return 1
    if token == '\n':
        if judge_bexpr == 1 and result == 1:
            print(">> true")
        elif judge_bexpr == 1 and result == 0:
            print(">> false")
        else:
            print(">>", result)
        return 0
    else:
        error()

def main():
    while True:
        print(">> ", end='')
        getChar()
        if token == '\n':
            break
        print_expr()
        while token != '\n' and token != '':
            getChar()  # discard the rest of the line

if __name__ == "__main__":
    main()