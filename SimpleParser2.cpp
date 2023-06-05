#include <iostream>
#include <cctype>	// isdigit(), isalpha(), islower() ����� ���� ����
#include <string>
#include <vector>

using namespace std;

vector<int> to_print;       // print �� �� ������� �ִ� ����

char temp_token;        // while�� �����ϸ鼭 �ӽ÷� ������ ����
int pos = 0;
int cc = 0;
int origin_cc = 0;
char token;
int has_error = 0;  // ���� �߻� ��, has_error = 1
int arr_var[26] = { 0 };        // 'a' ~ 'z' �־��� 26��¥�� arr_var �迭 ����
string str_while;
string copy_str_while;
int cnt = 0;                    // '{' ���� '}'�� �ݴ� �� count ���ִ� ����
bool input_token_flag = true;
bool lex_flag = true;

void getChar();
void error();
void lex(char c);

void store_result(int num);
void print_result();
int countClosing(const string& str);
void getChar_while();
void program();
void declaration();
void statement();
int bexpr();
int relop();
int aexpr();
int term();
int number();
int dec();
char var();

void store_result(int num) {
    to_print.push_back(num);
}

void print_result() {
    for (int i = 0; i < to_print.size(); i++)
        cout << to_print[i] << ' ';
}

int countClosing(const string &str) {
    int num = 0;
    int closing = 0;
    for (int i = str.size() - 1; i >= 0; --i) {
        if (str[i] == '}') {
            num++;
            closing++;
        }
        else if (str[i] == '{') {
            num--;
            if (num == 0) {
                break;
            }
        }
    }
    return closing;
}

void getChar_while() {     
   // token = copy_str_while[0];  // copy_str_while ���ڿ��� �� �տ��� token�� ����
    copy_str_while.erase(0, 1); // index 0���� �����ؼ� 1���� ���� ����
    token = copy_str_while[0];
}
void getChar() {
    token = cin.get();  // ���� �о����
    if (token == ' ' || token == '\t')  // �����̳� tab�� ��� �ǳʶٱ�
        getChar();
}

void error() {  // ���� �߻�
    has_error = 1;  // has_error �÷��� ����
}

void lex(char c) {      // ��·�簣�� lex('~~')�� �ϸ� ���� ���ڰ� while_token�� ����Ǿ����
    if (token == c) // ���� ��ū(token)�� ���� ����(c) ��
        if (lex_flag)   // 1. �Է½�Ʈ�� case ����
            getChar();  // ������ �о����
        else            // 2. string case ����
            getChar_while();
    else
        error();
}

void program() {
    while (token == 'i') {
        lex('i');   // �� ���� ���� ������ (ex: n�̳� =�̳�)
        if (token == 'n') {
            lex('n');
            if (token == 't') {
                lex('t');
                declaration();
            }
            else
                error();
        }
        else if (token == '=') { //����x ����x '='����
            statement();  // token�� '='�� ��� ä�� statement ����
            break;    
        }
        else  // 'i'�ڿ� 'n'�̳� '='�� �ƴ� ���� ��� ����
            error();
    }   // int j; int k; int a;... ó�� �Ϸ�

    if (islower(token)) // '=', print, while, 'a'-'z'�� statement() ����
        statement();
    else if (token == '\n')
        return;
    else
        error();
}

void declaration() { // int a; int b;.. �̷� ���ĸ� ����
    char ch = var();
    lex(ch);
    if (token == ';') {
        lex(';');
        program();
    }
    else
        error();
}

void statement() {
    if (token == '=') {     // ó������ i = ~ �� ���� ���, �� i ���� �ϴ� if��
        lex('=');    
        arr_var[8] = aexpr();  // 'i' - 97 = 9��°(arr_var[8]) ����
        if (token == ';')
            lex(';');
        else
            error();
    }
    while (1) {
        int result;
        if (token == 'p') { // p�� ���
            if (input_token_flag)    str_while.push_back(token);
            lex('p');
            if (token == 'r') {
                if (input_token_flag)    str_while.push_back(token);
                lex('r');
                if (token == 'i') {
                    if (input_token_flag)    str_while.push_back(token);
                    lex('i');
                    if (token == 'n') {
                        if (input_token_flag)    str_while.push_back(token);
                        lex('n');
                        if (token == 't') {
                            if (input_token_flag)    str_while.push_back(token);
                            lex('t');
                            result = aexpr();
                            if (token == ';') {
                                if (input_token_flag)    str_while.push_back(token);
                                lex(';');
                                store_result(result);
                                if (token == '}' && cnt > 0) { // �տ� '{'�� �����ϰ� �� '{'�� while�� ��ȿ�� '{'�� ���
                                    while (token == '}' && cnt != 0) {
                                        if (input_token_flag)   str_while.push_back(token);
                                        if (input_token_flag)   cnt--;
                                        lex('}');
                                        if (input_token_flag)
                                            temp_token = token;
                                        else if (!input_token_flag)
                                            break;
                                    }
                                    break;
                                }
                                else if (token == '}' && !input_token_flag) {
                                    break;
                                }
                                else if (islower(token)) {  // while���� statement�� ���� �ȳ����ٴ� �Ҹ�
                                    statement();
                                    break;
                                }
                                else if (token == '\n') break;
                                else error();
                            }
                            else error();   // print <aexpr> �Ŀ� ';'������ error()
                        }
                        else error();
                    }
                    else error();
                }
                else error();
            }
            else if (token == '=') {    // p = <aexpr>;
                if (input_token_flag)    str_while.push_back(token);
                lex('=');
                int temp = aexpr();     // �ӽ÷� temp���ٰ� = �ڿ� ������ <aexpr>���� ��� �� ����
                if (token == ';') {
                    arr_var[15] = temp;  // 'p' - 97 = 16��°(arr_var[15]) ����
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // �տ� {�� �����ϰ� �� {�� while�� ��ȿ�� {�� �����.
                        while (token == '}' && cnt != 0) {
                            if (input_token_flag)   str_while.push_back(token);
                            if (input_token_flag)   cnt--;
                            lex('}');
                            if (input_token_flag)
                                temp_token = token;
                            else if (!input_token_flag)
                                break;
                        }
                        break;
                    }
                    else if (token == '}' && !input_token_flag) {
                        break;
                    }
                    else if (islower(token)) {  // while���� statement�� ���� �ȳ����ٴ� �Ҹ�
                        statement();
                        break;
                    }
                    else if (token == '\n')
                        break;
                    else error();
                }
                else    error();
            }
            else  error();
        }

        else if (token == 'w') { // w�� ��� -> while (<bexpr>) { {<statemetn>} } 
            if (input_token_flag)    str_while.push_back(token);
            lex('w');
            if (token == 'h') {
                if (input_token_flag)    str_while.push_back(token);
                lex('h');
                if (token == 'i') {
                    if (input_token_flag)    str_while.push_back(token);
                    lex('i');
                    if (token == 'l') {
                        if (input_token_flag)    str_while.push_back(token);
                        lex('l');
                        if (token == 'e') {
                            if (input_token_flag)    str_while.push_back(token);
                            lex('e');
                            if (token == '(') {
                                if (input_token_flag)    str_while.push_back(token);
                                lex('(');   // �׷��� token�� bexpr�� ù���ڿ���
                                if (bexpr()) {    // statement ����
                                    if (token == ')') {
                                        if (input_token_flag)    str_while.push_back(token);
                                        lex(')');
                                        if (token == '{') {
                                            if (input_token_flag)    str_while.push_back(token);
                                            if (input_token_flag)    cnt++;          // while���� ����ǰ�, ���������� ����Ǿ� '{' ���� �����ϸ� '}' �Ǻ��� ���� cnt++
                                            lex('{');
                                            if (token == '=')  
                                                error();  // statement() �ʹ� �κ��� = ó�� �κп� ���� ���� ���� ó��
                                            statement();     // while�� {statement}����
                                            if (input_token_flag) {
                                                cc = countClosing(str_while);       // �ڿ������� �� '}'�� ���� -> �̸�ŭ while ����
                                                origin_cc = cc;
                                                copy_str_while.assign(str_while);   //copy_str_while�� string�� ����Ǿ��ִ°� ����
                                                input_token_flag = false;     // str_while ���ڿ��� token ���� ����
                                                lex_flag = false;       // lex()���� �Է½�Ʈ���� �ƴ� string���� �����ϵ���
                                                pos = copy_str_while.length();
                                                pos = copy_str_while.rfind('(', pos - 1);   // �ڿ������� '(' ã��, pos���� �ش� '('�� index�� �Ҵ�
                                                copy_str_while.erase(0, pos);  // '(' �տ� �ִ°� �� ������ 0��° ���ڰ� '('
                                                token = copy_str_while[0];      // token�� '(' �ֱ�
                                                lex('(');
                                            }
                                            while (cc) {
                                                if (bexpr()) {
                                                    do {
                                                        lex(')');
                                                        int a = origin_cc - cc;
                                                        int b = a;
                                                        if (token == '{' && (origin_cc != cc)) {
                                                            while (a) {
                                                                while (token != '}') {
                                                                    lex(token);
                                                                }
                                                                if (token == '}') lex('}');
                                                                a--;
                                                            }
                                                        }
                                                        if (!b)
                                                            lex('{');
                                                        statement();
                                                        copy_str_while.assign(str_while);
                                                        copy_str_while.erase(0, pos);
                                                        token = copy_str_while[0];
                                                        lex('(');
                                                    } while (bexpr());
                                                }
                                                copy_str_while.assign(str_while);   // copy_str_while�� string�� ����Ǿ��ִ°� ����
                                                pos = copy_str_while.rfind('(', pos - 1);   // �ڿ������� '(' ã��, pos���� �ش� '('�� index�� �Ҵ�
                                                if (!(pos > 0)) {
                                                    cc--;
                                                    break;
                                                }
                                                copy_str_while.erase(0, pos);
                                                token = copy_str_while[0];
                                                lex('(');
                                                cc--;
                                            }   
                                            copy_str_while.clear();
                                            pos = 0;
                                            cc = 0;
                                            origin_cc = 0;
                                            input_token_flag = true;
                                            lex_flag = true;
                                            token = temp_token;
                                        }
                                        else error();
                                    }
                                    else error();
                                }
                                else {
                                    if (input_token_flag && token == ')') {
                                        while (token != '}') {
                                            str_while.push_back(token);
                                            lex(token);
                                        }
                                        while (token == '}') {
                                            str_while.push_back(token);
                                            lex('}');
                                        }
                                    }
                                    if (!input_token_flag) {
                                        int temp = origin_cc - cc + 1;
                                        while (temp) {
                                            while (token != '}') {
                                                lex(token);
                                            }
                                            if (token == '}') {
                                                lex('}');
                                            }
                                            temp--;
                                        }
                                    }
                                    else
                                        error();
                                }
                            }
                            else error(); // while ������ '(' �ƴϸ� error
                        }
                        else error();
                    }
                    else error();
                }
                else error();
            }
            else if (token == '=') {    // w = <aexpr>;
                if (input_token_flag)    str_while.push_back(token);
                lex('=');
                int temp = aexpr();     // �ӽ÷� temp���ٰ� = �ڿ� ������ <aexpr>���� ��� �� ����
                if (token == ';') {
                    arr_var[22] = temp;  // 'w' - 97 = 23��°(arr_var[22]) ����
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // �տ� {�� �����ϰ� �� {�� while�� ��ȿ�� {�� �����.
                        while (token == '}' && cnt != 0) {
                            if (input_token_flag)   str_while.push_back(token);
                            if (input_token_flag)   cnt--;
                            lex('}');
                            if (input_token_flag)
                                temp_token = token;
                            else if (!input_token_flag)
                                break;
                        }
                        break;
                    }
                    else if (token == '}' && !input_token_flag) {
                        break;
                    }
                    else if (islower(token)) {  // while���� statement�� ���� ���
                        statement();
                        break;
                    }
                    else if (token == '\n')
                        break;
                    else error();
                }
                else
                    error();
            }
            else
                error();
        }

        else if (islower(token)){      // 'p'�� 'w'�� ������ ������ �ҹ��� ���ĺ��� ���
            char ch = token;
            if (input_token_flag)    str_while.push_back(token);
            lex(ch);
            if (token == '=') {
                if (input_token_flag)    str_while.push_back(token);
                lex('=');
                int temp = ch - 97;
                arr_var[temp] = aexpr();     // 'a'-'z'������ �迭�� �� ���� �ֱ�
                if (token == ';') {
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // �տ� '{'�� �����ϰ� �� '{'�� while�� ��ȿ�� '{'�� ���
                        while (token == '}' && cnt != 0) {
                            if (input_token_flag)   str_while.push_back(token);
                            if (input_token_flag)   cnt--;
                            lex('}');
                            if (input_token_flag)
                                temp_token = token;
                            else if (!input_token_flag)
                                break;
                        }
                        break;
                    }
                    else if (token == '}' && !input_token_flag) {
                        break;
                    }
                    else if (islower(token)) {  // while���� statement�� ���� ���
                        statement();
                        break;
                    }
                    else if (token == '\n')
                        break;
                    else error();
                }
                else error();
            }
            else error();
        }
        else if (token == '\n') break;
        else {
            error();
            break;
        } 
    }
}

int bexpr() {
    int left = aexpr();
    if (token == '=' || token == '!' || token == '<' || token == '>') {
        int op = relop();
        int right = aexpr();

        switch (op) {
        case 0:
            return left == right;   // True 1, False 0
        case 1:
            return left != right;   // True 1, False 0
        case 2:
            return left < right;    // True 1, False 0
        case 3:
            return left > right;    // True 1, False 0
        case 4:
            return left <= right;   // True 1, False 0
        case 5:
            return left >= right;   // True 1, False 0
        default:
            error();
            return 0;
        }
    }
}

int relop() {
    switch (token) {
    case '=':
        if (input_token_flag)    str_while.push_back(token);
        lex('=');
        if (token == '=') {
            if (input_token_flag)    str_while.push_back(token);
            lex('=');
            return 0;
        }
        else
            error();
    case '!':
        if (input_token_flag)    str_while.push_back(token);
        lex('!');
        if (token == '=') {
            if (input_token_flag)    str_while.push_back(token);
            lex('=');
            return 1;
        }
        else
            error();
    case '<':
        if (input_token_flag)    str_while.push_back(token);
        lex('<');
        if (token == '=') {
            if (input_token_flag)    str_while.push_back(token);
            lex('=');
            return 4;
        }
        return 2;
    case '>':
        if (input_token_flag)    str_while.push_back(token);
        lex('>');
        if (token == '=') {
            if (input_token_flag)    str_while.push_back(token);
            lex('=');
            return 5;
        }
        return 3;
    default:
        error();
    }
}

int aexpr() {  
    /* term() -> '+' or '-' -> term() ������ ����
       term()���� ���ڸ� �ش� ���� ������ �ش��ϴ� arr_var�迭�� �ε����� ����� �� ��������, 
                  ���ڸ� number()�� return �� �����ͼ� ���� */
    int result = term();   
    while (token == '+' || token == '-') {
        if (input_token_flag)     str_while.push_back(token);         // aexpr ���꿡�� ����ϴ� +,-��ȣ�� while���� ����� char�迭 �ڿ� ����ֱ�
        if (token == '+') {
            lex('+');
            result += term();
        }
        else if (token == '-') {
            lex('-');
            result -= term();
        }
    }
    return result;      // '+', '-' ���� ��� return
}

int term() { 
    int result;
    if (isdigit(token)) {
        result = number();
        return result;
    }
    else if (islower(token)){
        char ch = var();     
        int temp = ch - 97;  // 'a'~'z' -> 0~25
        result = arr_var[temp];
        lex(ch);
        return result;
    }
    else {
        error();
    }
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
    }
    if (input_token_flag)     str_while.push_back(token);         // while���� ���� ���� 0~9 char�迭�� �ڿ��� ����ֱ�
    int result = token - '0';
    lex(token);
    return result;
}

char var() {                
    if (islower(token)) {
        if (input_token_flag)    
            str_while.push_back(token);         // while���� ���� �ҹ��� 'a'~'z' char�迭�� �ڿ��� ����ֱ�
        return token;
    }
    else    
        error();
}

int main() {
    while (1) {
        cout << ">> ";
        getChar();
        if (token == EOF || token == '\n') {
            cout << ">> ";
            lex('\n');
            if (token == EOF || token == '\n') 
                break;
        }
        program();          
        if (has_error) {
            cout << ">> Syntax Error!";
            while (token != '\n') {
                lex(token);
            }
        }
        else if (has_error == 0){
			cout << ">> ";
            print_result();
        }
        for (int i = 0; i < 26; i++)        // �� ��� �迭 �ʱ�ȭ
            arr_var[i] = 0;
        to_print.clear();       // ���� �ʱ�ȭ
        has_error = 0;          // error flag 0���� �ʱ�ȭ
        str_while.clear();
        cout << endl;
    }
    return 0;
}

