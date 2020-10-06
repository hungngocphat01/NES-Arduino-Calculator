# Arduino simple calculator
Work-in-progress

## Wiring
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

## How does it work
- The calculator receives input from the 4x4 matrix keypad, parses the tokens immediately as soon as they are sent from the keypad, and categorizes them into ``numbers`` and ``operators``.
- After that, the board converts the original infix expression into a postfix expression, then evaluates it (Shunting-yard algorithm).
- The result will appear on the LCD screen if there was no error reported.

## Source code structure
- ``Calculator.ino``: literally ``main.cpp``.
- ``ExprCalc.h``: shunting-yard algorithm implementation.
- ``Stack.h``: implementations of ``sstack`` (``String`` stack) and ``fstack`` (``float`` stack).
- ``Screen.h``: implementation of LCD printing thingy (almost empty at the moment).
- ``tokenizer.py``: creates an array of tokens, for debugging purpose only.

## Input
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
      <td class="tg-0lax"></td>
    </tr>
  </tbody>
  </table>

## Output
- Evaluated value if there was no error reported.
- Error message if there was at least an error occured:
  - Math Error: divide by 0, or sqrt of negative number.
  - Syntax Error: for instance
    ```
    1 * + 5
    4.2 - ( * 7 )
    etc.
    ```

## What's working
- Input, evaluation, output to serial monitor.

## What's not working/features being added in the future
- Output to LCD display.
- Edit the input expression.

## Debugging
- The calculator will print the converted postfix expression as well as the final result to serial monitor (baud rate 9600).
