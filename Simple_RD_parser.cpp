#include <iostream>
#include <cctype>   // isdigit()을 위한 cctype 선언

using namespace std;

int judge_bexpr = 0;
char token;
int has_error = 0;  // 에러 발생 시, has_error = 1

void getChar();
void error();
int print();
void lex(char c);
int expr();
int bexpr();
int relop();
int aexpr();
int term();
int factor();
int number();
int dec();

void getChar() {
    token = cin.get();
    if (token == ' ' || token == '\t')
        getChar();
}

void error() {  // 에러 발생
    cout << ">> syntax error!! \n";
    has_error = 1; 
}

void lex(char c) {
    if (token == c)
        getChar();
    else
        error();
}

int expr() {
    return bexpr();
}

int bexpr() {
    int left = aexpr();

    if (token == '=' || token == '!' || token == '<' || token == '>') {
        int op = relop();
        int right = aexpr();
        judge_bexpr = 1;    // bexpr()

        switch (op) {
        case 0:
            return left == right;
        case 1:
            return left != right;
        case 2:
            return left < right;
        case 3:
            return left > right;
        case 4:
            return left <= right;
        case 5:
            return left >= right;
        default:
            error();
            return 0;
        }
    }
    else {
        judge_bexpr = -1;   // aexpr()
        return left;
    }
}

int relop() {
    switch (token) {
    case '=':
        lex('=');
        if (token == '=') {
            lex('=');
            return 0;
        }
        else
            error();
    case '!':
        lex('!');
        if (token == '=') {
            lex('=');
            return 1;
        }
    case '<':
        lex('<');
        if (token == '=') {
            lex('=');
            return 4;
        }
        return 2;
    case '>':
        lex('>');
        if (token == '=') {
            lex('=');
            return 5;
        }
        return 3;
    default:
        error();
    }
}

int aexpr() {
    int result = term();
    while (!has_error && (token == '*' || token == '/')) {
        if (token == '*') {
            lex('*');
            result *= term();
        }
        else if (token == '/') {
            lex('/');
            result /= term();
        }
    }
    return result;
}

int term() {
    int result = factor();
    while (!has_error && (token == '+' || token == '-')) {
        if (token == '+') {
            lex('+');
            result += factor();
        }
        else if (token == '-') {
            lex('-');
            result -= factor();
        }
    }
    return result;
}

int factor() {
    int result;
    if (token == '(') {
        lex('(');
        result = aexpr();
        if (token == ')') {
            lex(')');
        }
        else {
            error();
        }
    }
    else {
        result = number();
    }
    return result;
}

int number() {
    int result = dec();
    while (!has_error && isdigit(token))
        result = 10 * result + dec();
    return result;
}

int dec() {
    if (!isdigit(token)) {
        error();
        return 0;
    }
    int result = token - '0';
    lex(token);
    return result;
}

int print() {
    has_error = 0;
    int result = expr();
    if (has_error)
        return 1;
    if (token == '\n') {
        if (judge_bexpr == 1 && result == 1)
            cout << ">> true" << "\n";
        else if (judge_bexpr == 1 && result == 0)
            cout << ">> false" << "\n";
        else
            cout << ">> " << result << "\n";
        return 0;
    }
    else {
        error();
    }
}

int main() {
    while (1) {
        cout << ">> ";
        getChar();
        if (token == EOF || token == '\n')
            break;
        print();
        while (token != '\n' && token != EOF)   // 혹시라도 남은 문자 날리기
            getChar(); 
    }
    return 0;
}


