#include "Stack.h"
#include "Screen.h"

#define SIN 1
#define COS 2
#define TAN 3
#define SQRT 4

#define UNK 0
#define NUM 1
#define OPR 2

#define E 2.71828182

#define halt while(1)

int getTokenType(String token) {
  if (token.length() == 0) {
    return UNK;
  }
  if (token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos" || token == "tan" || token == "^" || token == "sqrt" ||
    token == "(" || token == ")" || token == "=") {
    return OPR;
  }
  else {
    return NUM;
  }
}

inline bool isOperator (String token) {
    return (getTokenType(token) == OPR);
}

inline bool isNumber(String token) {
    return (getTokenType(token) == NUM);
}

inline bool isSingleOp(String token) {
    return (token == "sin" || token == "cos" || token == "tan" || token == "sqrt");
}

float convToFloat(String token) {
    if (token.length() > 0) {
        if (token[0] == '-') {
            String positive;
            for (int i = 1; i < token.length(); i++) {
                positive += token[i];
            }
            return (-1)*positive.toFloat();
        }
        return token.toFloat();
    }
    return 0;
}

int precedence(String op) {
    if (op == "sin" || op == "cos" || op == "tan") return 4;
    else if (op == "^") return 3;
    else if (op == "*" || op == "/") return 2;
    else if (op == "+" || op == "-") return 1;
    return 0;
}

void showSyntaxErr(int pos) {
    Serial.println(F("Syntax error"));
    halt;
}

void showMathErr() {
    Serial.println(F("Math error"));
    halt;
}

/* Rules:
- Dot: after and before must be a number.
- Double operator:
    + Before: number, ')'.
    + After: number, '(', single operator
- Double operator '-':
    + Before: same, plus '(' or none
    + After: same
- Single operator:
    + Before: operator, none
    + After: '(', number
- '(':
    + Before: none, operator
    + After: number, '-', operator
- ')'
    + Before: number
    + After: none, double operator

*/

/* If after '-' is a number and:
    + Before it is a '(' or 
    + It is the first element of the expression
then it is a negative sign
*/


sstack ConvertToPostfix(String* infix, int n) {   
    sstack result;
    sstack tmp;
    
    for (int i = 0; i < n; i++) {
        String& token = infix[i];
        if (token == "(") {
            if (i == n - 1) showSyntaxErr(i);
            if (i - 1 >= 0 && !isOperator(infix[i - 1])) showSyntaxErr(i);
            if (i + 1 < n && !isNumber(infix[i + 1]) && !isOperator(infix[i + 1]) && infix[i + 1] != "-") showSyntaxErr(i);

            tmp.push(token);
        }
        else if (token == ")") {
            if (i == 0) showSyntaxErr(i);
            if (i - 1 >= 0 && !isNumber(infix[i - 1])) showSyntaxErr(i);
            if (i + 1 < n && (!isOperator(infix[i + 1]) || isSingleOp(infix[i + 1]))) showSyntaxErr(i);

            String pop;
            do {
                // If ( is missing
                if (tmp.isEmpty()) {
                    showSyntaxErr(i);
                }
                pop = tmp.pop();
                if (pop != "(") {
                    result.push(pop);
                }
            } while (pop != "(");
        }
        else if (isOperator(token)) {
            // Is negative
            if (token == "-" && i + 1 < n && isNumber(infix[i + 1]) && (i == 0 || (i - 1 >= 0 && infix[i - 1] == "("))) {
                infix[i + 1] = "-" + infix[i + 1];
                continue;
            }
            // Check syntax if not negative sign
            if (isSingleOp(token)) {
                if (i == n - 1) showSyntaxErr(i);
                if (i - 1 >= 0 && !isOperator(infix[i - 1])) showSyntaxErr(i);
                if (i + 1 < n && !isNumber(infix[i + 1]) && infix[i + 1] != "(") showSyntaxErr(i);
            }
            else {
                if (i == 0 || i == n - 1) showSyntaxErr(i);
                if (i - 1 >= 0 && !isNumber(infix[i - 1]) && infix[i - 1] != ")") showSyntaxErr(i);
                if (i + 1 < n && !isNumber(infix[i + 1]) && infix[i + 1] != "(" && !isSingleOp(infix[i + 1])) showSyntaxErr(i);      
            }

            if (!tmp.isEmpty() && precedence(token) <= precedence(tmp.top())) {
                result.push(tmp.pop());
            }
            tmp.push(token);
        }
        else if (isNumber(token)) {
            if (token == ".") {
                if (i == 0 || i == n - 1) showSyntaxErr(i);
                if (i - 1 >= 0 && !isNumber(infix[i - 1])) showSyntaxErr(i);
                if (i + 1 < n && !isNumber(infix[i + 1])) showSyntaxErr(i);
            }

            result.push(token);
        }
    }
    while (!tmp.isEmpty() && tmp.top() != "(") {
        String pop = tmp.pop();
        if (pop != "(") {
            result.push(pop);
        }
    }
    return result;
}


float PostfixEvaluate(sstack postfix) {
    fstack tmp;
    for (int i = 0; i <= postfix.index; i++) {
        String token = postfix[i];
        if (isNumber(token)) {
            tmp.push(convToFloat(token));
        }
        else if (isSingleOp(token)) {
            float operand = tmp.pop();
            if (token == "sin") tmp.push(sin(operand));
            else if (token == "cos") tmp.push(cos(operand));
            else if (token == "tan") tmp.push(tan(operand));
            else if (token == "sqrt") {
                if (operand < 0) showMathErr();
                tmp.push(sqrt(operand));
            }
        }
        else {
            float operand2 = tmp.pop();
            float operand1 = tmp.pop();

            if (token == "+") tmp.push(operand1 + operand2);
            else if (token == "-") tmp.push(operand1 - operand2);
            else if (token == "*") tmp.push(operand1 * operand2);
            else if (token == "/") {
                if (operand2 == 0) showMathErr();
                tmp.push(operand1 / operand2);
            } 
            else if (token == "^") tmp.push(pow(operand1, operand2));
        }
    }
    return tmp.top();
}
