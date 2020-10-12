#include "Stack.h"
#include "Screen.h"

// Token types
#define UNK 0
#define NUM 1
#define OPR 2

#define E 2.71828182

#define halt while(1)

float Ans = 0;
float X = 0;
bool errflag = false;

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

inline bool isOperator (const String& token) {
    return (getTokenType(token) == OPR);
}

inline bool isNumber(const String& token) {
    return (getTokenType(token) == NUM);
}

inline bool isSingleOp(const String& token) {
    return (token == "sin" || token == "cos" || token == "tan" || token == "sqrt");
}

float convToFloat(const String& token) {
    if (token.length() > 0) {
        // Special constants
        if (token == "pi") return PI;
        if (token == "e") return E;
        if (token == "Ans") return Ans;
        if (token == "X") return X;
        
        // Negative numbers handling since Arduino's .toFloat() cannot handle negative numbers
        if (token[0] == '-') {
            String positive;
            for (int i = 1; i < token.length(); i++) {
                positive += token[i];
            }
            return (-1)*positive.toFloat();
        }
        // Positive number
        return token.toFloat();
    }
    return 0;
}

int precedence(const String& op) {
    if (op == "sin" || op == "cos" || op == "tan") return 4;
    else if (op == "^") return 3;
    else if (op == "*" || op == "/") return 2;
    else if (op == "+" || op == "-") return 1;
    return 0;
}

void showSyntaxErr() {
    Serial.println(F("[ERR] Syntax error"));
    
    lcd.setCursor(0, prntline);
    lcdClrLine(prntline);
    
    lcd.print(F("Syntax error"));
    lcd.noBlink();
    errflag = true;
}

void showMathErr() {
    Serial.println(F("[ERR] Math error"));
    
    lcd.setCursor(0, prntline);
    lcdClrLine(prntline);
    
    lcd.print(F("Math error"));
    lcd.noBlink();
    errflag = true;
}

/* Rules:
- Number:
    + Before: operator, '('
    + After: operator
- Double operator (+ - * / ^):
    + Before: number, ')'.
    + After: number, '(', single operator
- Double operator '-':
    + Before: same, plus '(' or none
    + After: same
- Single operator (sin cos tan sqrt):
    + Before: operator, none
    + After: '(', number
- '(':
    + Before: none, operator, '('
    + After: number, '-', operator, ('
- ')'
    + Before: number, ')'
    + After: none, double operator, ')'

*/

/* If after '-' is a number and:
    + Before it is a '(' or 
    + It is the first element of the expression
then it is a negative sign
*/

void ConvertToPostfix(String* infix, int n, sstack& result, byte prntline = 0) {
    sstack tmp;
    sprintMemoryUsage(F("[STAT] Free mem conv: "));
    
    for (int i = 0; i < n; i++) {
        String& token = infix[i];
        sprintVariable("Processing: ", token);
        if (token == "(") {
            if (i == n - 1) showSyntaxErr();
            if (i - 1 >= 0 && !isOperator(infix[i - 1]) && infix[i - 1] != "(") showSyntaxErr();
            if (i + 1 < n && !isNumber(infix[i + 1]) && !isOperator(infix[i + 1]) && infix[i + 1] != "-" && infix[i + 1] != "(") showSyntaxErr();

            tmp.push(token);
        }
        else if (token == ")") {
            if (i == 0) showSyntaxErr();
            if (i - 1 >= 0 && !isNumber(infix[i - 1]) && infix[i - 1] != ")") showSyntaxErr();
            if (i + 1 < n && (!isOperator(infix[i + 1]) || isSingleOp(infix[i + 1])) && infix[i + 1] != ")") showSyntaxErr();

            String pop;
            do {
                // If ( is missing
                if (tmp.isEmpty()) {
                    showSyntaxErr();
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
                if (i == n - 1) showSyntaxErr();
                if (i - 1 >= 0 && !isOperator(infix[i - 1])) showSyntaxErr();
                if (i + 1 < n && !isNumber(infix[i + 1]) && infix[i + 1] != "(") showSyntaxErr();
            }
            else {
                if (i == 0 || i == n - 1) showSyntaxErr();
                if (i - 1 >= 0 && !isNumber(infix[i - 1]) && infix[i - 1] != ")") showSyntaxErr();
                if (i + 1 < n && !isNumber(infix[i + 1]) && infix[i + 1] != "(" && !isSingleOp(infix[i + 1])) showSyntaxErr();      
            }

            if (!tmp.isEmpty() && precedence(token) <= precedence(tmp.top())) {
                result.push(tmp.pop());
            }
            tmp.push(token);
        }
        else if (isNumber(token)) {
            result.push(token);
        }

        if (errflag) {
            return;
        }
    }
    while (!tmp.isEmpty() && tmp.top() != "(") {
        String pop = tmp.pop();
        if (pop != "(") {
            result.push(pop);
        }
    }
}


float PostfixEvaluate(const sstack& postfix, byte prntline = 0) {
    fstack tmp;
    sprintMemoryUsage(F("[STAT] Free mem eval: "));
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
        if (errflag) {
            return 0;
        }
    }
    Ans = tmp.top();
    return tmp.top();
}
