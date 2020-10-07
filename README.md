# Arduino simple calculator
Work-in-progress

## 0. Introduction.
- This is a simple project of NES Academic Club, University of Science - Ho Chi Minh City, Vietnam.
- This project manipulates Arduino into a simple calculator, with the abilities to evaluate basic expressions (sin, cos, tan, sqrt supported); evaluate approximately value of single variable integral (using Riemann middle sum); and solve simple equations.
- Project made from scratch and maintained by hungngocphat01, with the exception of ``Keypad.h`` (courtesy of Mark Stanley, Alexander Brevig) and ``LiquidCrystal_I2C.h`` (courtesy of Frank de Brabander).

## 1. Wiring
<table class="tg">
<thead>
  <tr>
    <th class="tg-1wig">Device</th>
    <th class="tg-1wig">Device pin</th>
    <th class="tg-1wig">Arduino pin</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-1wig">Keypad</td>
    <td class="tg-0lax">1 - 8</td>
    <td class="tg-0lax">5 - 12</td>
  </tr>
  <tr>
    <td class="tg-1wig" rowspan="4">I2C LCD</td>
    <td class="tg-0lax">VCC</td>
    <td class="tg-0lax">3.3V</td>
  </tr>
  <tr>
    <td class="tg-0lax">GND</td>
    <td class="tg-0lax">GND</td>
  </tr>
  <tr>
    <td class="tg-0lax">SDA</td>
    <td class="tg-0lax">A4</td>
  </tr>
  <tr>
    <td class="tg-0lax">SCL</td>
    <td class="tg-0lax">A5</td>
  </tr>
  <tr>
    <td class="tg-1wig" rowspan="2">Shift button</td>
    <td class="tg-0lax">1</td>
    <td class="tg-0lax">GND</td>
  </tr>
  <tr>
    <td class="tg-0lax">2</td>
    <td class="tg-0lax">A0</td>
  </tr>
</tbody>
</table>

## 2. Source code structure
- ``Calculator.ino``: literally ``main.cpp``.
- ``ExprCalc.h``: shunting-yard algorithm implementation.
- ``Stack.h``: implementations of ``sstack`` (``String`` stack) and ``fstack`` (``float`` stack).
- ``Screen.h``: prototype of LCD printing function.
- ``Input.h``: functions to examine and process keystrokes.
- ``Modes.h``: functions to handle supported modes of the calculator.
- ``tokenizer.py``: creates an array of tokens, for debugging purpose only.

## 3. Program flow
- The ``void loop()`` function calls the ``void menuMode()`` function. This function acts as the main function and the program execution will stay in this function under any circumstances. Here, the user chooses the operating mode of the calculator.
  - If the user chooses COMP mode, ``void compMode()`` will be called.
  - The same thing happens with EQN mode (``void eqnMode()``) and INTERGRAL mode (``void intgrlMode()``). More details about these mode are available in [5. Working modes](#51-comp-mode).
  - If the user presses the ``MODE`` button in COMP, EQN or INTEGRAL mode, the function which handles the respective mode will return. The program then goes back to ``void menuMode()``, waiting for another mode to be choosen.

## 4. Input
- 4x4 matrix keypad.
- Shift button (separate button).
- Layout:
  <table class="tg">
    <tbody>
    <tr>
      <th class="tg-0lax">1</th>
      <th class="tg-0lax">2</th>
      <th class="tg-0lax">3</th>
      <th class="tg-0lax">+</th>
    </tr>
    <tr>
      <td class="tg-0lax">4</td>
      <td class="tg-0lax">5</td>
      <td class="tg-0lax">6</td>
      <td class="tg-0lax">-</td>
    </tr>
    <tr>
      <td class="tg-0lax">7</td>
      <td class="tg-0lax">8</td>
      <td class="tg-0lax">9</td>
      <td class="tg-0lax">*</td>
    </tr>
    <tr>
      <td class="tg-0lax">.</td>
      <td class="tg-0lax">0</td>
      <td class="tg-0lax">=</td>
      <td class="tg-0lax">/</td>
    </tr>
  </tbody>
  </table>

- Layout when shift button is pressed:
  <table class="tg">
  <tbody>
    <tr>
      <th class="tg-0lax">^</th>
      <th class="tg-0lax">sqrt</th>
      <th class="tg-0lax">pi</th>
      <th class="tg-0lax">sin</th>
    </tr>
    <tr>
      <td class="tg-0lax">(</td>
      <td class="tg-0lax">)</td>
      <td class="tg-0lax">e</td>
      <td class="tg-0lax">cos</td>
    </tr>
    <tr>
      <td class="tg-0lax">LEFT</td>
      <td class="tg-0lax">RIGHT</td>
      <td class="tg-0lax">Ans</td>
      <td class="tg-0lax">tan</td>
    </tr>
    <tr>
      <td class="tg-0lax"></td>
      <td class="tg-0lax">DEL</td>
      <td class="tg-0lax">=</td>
      <td class="tg-0lax">MODE</td>
    </tr>
  </tbody>
  </table>

## 5. Working modes
### 5.1. COMP mode

#### 5.1.1. How does it work
- The calculator receives input from the 4x4 matrix keypad, parses the tokens immediately as soon as they are sent from the keypad, and categorizes them into ``numbers`` and ``operators``.
- The user can press ``DEL`` (``Shift`` + ``0``) key to pop the last operand/operator out of the expression, or ``=`` to start evaluating.
- After that, the original infix expression is converted into a postfix expression and is evaluated (shunting-yard algorithm).
- Finally, the result is displayed on the LCD screen if there was no error reported.

#### 5.1.2. Output
- Evaluation result if there was no error reported.
- Error message if there was at least one error occured:
  - Math Error: divided by 0, or sqrt of negative number.
  - Syntax Error: for instance
    ```
    1 * + 5
    4.2 - ( * 7 )
    etc.
    ```
### 5.2. EQN mode
- To be added.
### 5.3. Integral mode
- To be added.

## 6. What's working
- Keypad, LCD screen.
- COMP mode.
- Mode changing.

## 7. What's not working/features being added in the future
- Screen scrolling.
- EQN mode.
- Integral mode.

## 8. Debugging
- Following information will be printed to serial monitor:
 - Postfix expression.
 - Evaluated result.
 - New mode (when mode is changed).
 - Coefficients and solutions (EQN mode).
