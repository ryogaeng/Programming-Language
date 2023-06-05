#include <iostream>
#include <cctype>	// isdigit(), isalpha(), islower() 사용을 위해 선언
#include <string>
#include <vector>

using namespace std;

vector<int> to_print;       // print 해 줄 결과값을 넣는 벡터

char temp_token;        // while문 진행하면서 임시로 저장할 문자
int pos = 0;
int cc = 0;
int origin_cc = 0;
char token;
int has_error = 0;  // 에러 발생 시, has_error = 1
int arr_var[26] = { 0 };        // 'a' ~ 'z' 넣어줄 26개짜리 arr_var 배열 생성
string str_while;
string copy_str_while;
int cnt = 0;                    // '{' 열고 '}'로 닫는 걸 count 해주는 변수
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
   // token = copy_str_while[0];  // copy_str_while 문자열의 맨 앞에를 token에 저장
    copy_str_while.erase(0, 1); // index 0부터 시작해서 1개의 문자 삭제
    token = copy_str_while[0];
}
void getChar() {
    token = cin.get();  // 문자 읽어오기
    if (token == ' ' || token == '\t')  // 공백이나 tab의 경우 건너뛰기
        getChar();
}

void error() {  // 에러 발생
    has_error = 1;  // has_error 플래그 설정
}

void lex(char c) {      // 어쨌든간에 lex('~~')를 하면 다음 문자가 while_token에 저장되어야함
    if (token == c) // 현재 토큰(token)과 예상 문자(c) 비교
        if (lex_flag)   // 1. 입력스트림 case 진행
            getChar();  // 같으면 읽어오기
        else            // 2. string case 진행
            getChar_while();
    else
        error();
}

void program() {
    while (token == 'i') {
        lex('i');   // 그 다음 문자 가져옴 (ex: n이냐 =이냐)
        if (token == 'n') {
            lex('n');
            if (token == 't') {
                lex('t');
                declaration();
            }
            else
                error();
        }
        else if (token == '=') { //숫자x 문자x '='가능
            statement();  // token에 '='가 담긴 채로 statement 실행
            break;    
        }
        else  // 'i'뒤에 'n'이나 '='이 아닌 경우는 모두 에러
            error();
    }   // int j; int k; int a;... 처리 완료

    if (islower(token)) // '=', print, while, 'a'-'z'면 statement() 실행
        statement();
    else if (token == '\n')
        return;
    else
        error();
}

void declaration() { // int a; int b;.. 이런 형식만 가능
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
    if (token == '=') {     // 처음부터 i = ~ 이 나온 경우, 즉 i 갱신 하는 if문
        lex('=');    
        arr_var[8] = aexpr();  // 'i' - 97 = 9번째(arr_var[8]) 갱신
        if (token == ';')
            lex(';');
        else
            error();
    }
    while (1) {
        int result;
        if (token == 'p') { // p의 경우
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
                                if (token == '}' && cnt > 0) { // 앞에 '{'가 존재하고 이 '{'는 while의 유효한 '{'인 경우
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
                                else if (islower(token)) {  // while문의 statement가 아직 안끝났다는 소리
                                    statement();
                                    break;
                                }
                                else if (token == '\n') break;
                                else error();
                            }
                            else error();   // print <aexpr> 후에 ';'없으면 error()
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
                int temp = aexpr();     // 임시로 temp에다가 = 뒤에 나오는 <aexpr>연산 결과 값 저장
                if (token == ';') {
                    arr_var[15] = temp;  // 'p' - 97 = 16번째(arr_var[15]) 갱신
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // 앞에 {가 존재하고 이 {는 while의 유효한 {인 경우임.
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
                    else if (islower(token)) {  // while문의 statement가 아직 안끝났다는 소리
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

        else if (token == 'w') { // w의 경우 -> while (<bexpr>) { {<statemetn>} } 
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
                                lex('(');   // 그러면 token은 bexpr의 첫글자에게
                                if (bexpr()) {    // statement 실행
                                    if (token == ')') {
                                        if (input_token_flag)    str_while.push_back(token);
                                        lex(')');
                                        if (token == '{') {
                                            if (input_token_flag)    str_while.push_back(token);
                                            if (input_token_flag)    cnt++;          // while문이 실행되고, 정상적으로 진행되어 '{' 까지 도착하면 '}' 판별을 위한 cnt++
                                            lex('{');
                                            if (token == '=')  
                                                error();  // statement() 초반 부분의 = 처리 부분에 대한 사전 에러 처리
                                            statement();     // while의 {statement}실행
                                            if (input_token_flag) {
                                                cc = countClosing(str_while);       // 뒤에서부터 센 '}'의 갯수 -> 이만큼 while 실행
                                                origin_cc = cc;
                                                copy_str_while.assign(str_while);   //copy_str_while에 string에 저장되어있는거 복사
                                                input_token_flag = false;     // str_while 문자열에 token 삽입 중지
                                                lex_flag = false;       // lex()에서 입력스트림이 아닌 string으로 진행하도록
                                                pos = copy_str_while.length();
                                                pos = copy_str_while.rfind('(', pos - 1);   // 뒤에서부터 '(' 찾기, pos에는 해당 '('의 index가 할당
                                                copy_str_while.erase(0, pos);  // '(' 앞에 있는거 다 날리기 0번째 문자가 '('
                                                token = copy_str_while[0];      // token에 '(' 넣기
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
                                                copy_str_while.assign(str_while);   // copy_str_while에 string에 저장되어있는거 복사
                                                pos = copy_str_while.rfind('(', pos - 1);   // 뒤에서부터 '(' 찾기, pos에는 해당 '('의 index가 할당
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
                            else error(); // while 다음에 '(' 아니면 error
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
                int temp = aexpr();     // 임시로 temp에다가 = 뒤에 나오는 <aexpr>연산 결과 값 저장
                if (token == ';') {
                    arr_var[22] = temp;  // 'w' - 97 = 23번째(arr_var[22]) 갱신
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // 앞에 {가 존재하고 이 {는 while의 유효한 {인 경우임.
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
                    else if (islower(token)) {  // while문의 statement가 남은 경우
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

        else if (islower(token)){      // 'p'와 'w'를 제외한 나머지 소문자 알파벳의 경우
            char ch = token;
            if (input_token_flag)    str_while.push_back(token);
            lex(ch);
            if (token == '=') {
                if (input_token_flag)    str_while.push_back(token);
                lex('=');
                int temp = ch - 97;
                arr_var[temp] = aexpr();     // 'a'-'z'까지의 배열에 값 집어 넣기
                if (token == ';') {
                    if (input_token_flag)    str_while.push_back(token);
                    lex(';');
                    if (token == '}' && cnt > 0) { // 앞에 '{'가 존재하고 이 '{'는 while의 유효한 '{'인 경우
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
                    else if (islower(token)) {  // while문의 statement가 남은 경우
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
    /* term() -> '+' or '-' -> term() 순서로 진행
       term()에서 문자면 해당 문자 순서에 해당하는 arr_var배열의 인덱스에 저장된 값 가져오고, 
                  숫자면 number()의 return 값 가져와서 연산 */
    int result = term();   
    while (token == '+' || token == '-') {
        if (input_token_flag)     str_while.push_back(token);         // aexpr 연산에서 사용하는 +,-기호를 while에서 사용할 char배열 뒤에 집어넣기
        if (token == '+') {
            lex('+');
            result += term();
        }
        else if (token == '-') {
            lex('-');
            result -= term();
        }
    }
    return result;      // '+', '-' 연산 결과 return
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
    if (input_token_flag)     str_while.push_back(token);         // while문에 쓰일 숫자 0~9 char배열에 뒤에서 집어넣기
    int result = token - '0';
    lex(token);
    return result;
}

char var() {                
    if (islower(token)) {
        if (input_token_flag)    
            str_while.push_back(token);         // while문에 쓰일 소문자 'a'~'z' char배열에 뒤에서 집어넣기
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
        for (int i = 0; i < 26; i++)        // 값 담는 배열 초기화
            arr_var[i] = 0;
        to_print.clear();       // 벡터 초기화
        has_error = 0;          // error flag 0으로 초기화
        str_while.clear();
        cout << endl;
    }
    return 0;
}

