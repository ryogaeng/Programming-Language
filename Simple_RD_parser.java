import java.util.Scanner;

public class Simple_RD_parser{
    static int judge_bexpr = 0;
    static char token;
    static boolean has_error = false;
    static String line;
    static int index = 0;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.print(">> ");
            if (!scanner.hasNextLine()) break;
            line = scanner.nextLine();
            index = 0;
            if (line.isEmpty()) break;

            getChar();
            print();
            while (index < line.length()) {
                getChar();
            }
        }
        scanner.close();
    }

    static void getChar() {
        if (index < line.length()) {
            token = line.charAt(index++);
            if (token == ' ' || token == '\t')
                getChar();
        } else {
            token = '\n';
        }
    }

    static void error() {
        System.out.println(">> syntax error!!");
        has_error = true;
    }

    static void lex(char c) {
        if (token == c) {
            getChar();
        } else {
            error();
        }
    }

    static int expr() {
        return bexpr();
    }

    static int bexpr() {
        int left = aexpr();

        if (token == '=' || token == '!' || token == '<' || token == '>') {
            int op = relop();
            int right = aexpr();
            judge_bexpr = 1;

            switch (op) {
                case 0:
                    return left == right ? 1 : 0;
                case 1:
                    return left != right ? 1 : 0;
                case 2:
                    return left < right ? 1 : 0;
                case 3:
                    return left > right ? 1 : 0;
                case 4:
                    return left <= right ? 1 : 0;
                case 5:
                    return left >= right ? 1 : 0;
                default:
                    error();
                    return 0;
            }
        } else {
            judge_bexpr = -1;
            return left;
        }
    }

    static int relop() {
        switch (token) {
            case '=':
                lex('=');
                if (token == '=') {
                    lex('=');
                    return 0;
                } else {
                    error();
                    return -1;
                }
            case '!':
                lex('!');
                if (token == '=') {
                    lex('=');
                    return 1;
                }
                return -1;
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
                return -1;
        }
    }

    static int aexpr() {
        int result = term();
        while (!has_error && (token == '*' || token == '/')) {
            if (token == '*') {
                lex('*');
                result *= term();
            } else if (token == '/') {
                lex('/');
                result /= term();
            }
        }
        return result;
    }

    static int term() {
        int result = factor();
        while (!has_error && (token == '+' || token == '-')) {
            if (token == '+') {
                lex('+');
                result += factor();
            } else if (token == '-') {
                lex('-');
                result -= factor();
            }
        }
        return result;
    }
    static int factor() {
        int result;
        if (token == '(') {
            lex('(');
            result = aexpr();
            if (token == ')') {
                lex(')');
            } else {
                error();
            }
        } else {
            result = number();
        }
        return result;
    }

    static int number() {
        int result = dec();
        while (!has_error && Character.isDigit(token)) {
            result = 10 * result + dec();
        }
        return result;
    }

    static int dec() {
        if (!Character.isDigit(token)) {
            error();
            return 0;
        }
        int result = Character.getNumericValue(token);
        lex(token);
        return result;
    }

    static void print() {
        has_error = false;
        int result = expr();
        if (has_error) {
            return;
        }
        if (token == '\n') {
            if (judge_bexpr == 1 && result == 1) {
                System.out.println(">> true");
            } else if (judge_bexpr == 1 && result == 0) {
                System.out.println(">> false");
            } else {
                System.out.println(">> " + result);
            }
        } else {
            error();
        }
    }
}



