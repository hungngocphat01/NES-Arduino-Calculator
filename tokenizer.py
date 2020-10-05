expr = "( 1 / 2 ) ^ 7 + 9.5 - 3 + ( - 1 / 2 ) * ( - 3 / 4 )"
tokens = expr.split()

for token in tokens:
    print(f"\"{token}\", ", end = "")
print()
print(len(tokens))
print(eval(expr))