
# Simple Math Domain-Specific Language (DSL)

> **_NOTE:_**  This project is still in progress, therefore its repository is not completed. The descriptions below outline the intended goals and features of the repository.

The presented DSL (Domain-Specific Language) is designed for creating and executing basic mathematical programs. The grammar includes the following elements:

- `var`: Initializes a variable.
- `int, float`: Define data types.
- `while`: Indicates the start of a while loop.
- `if`: Indicates the start of an if statement.
- `elif`:Used in if statements for additional conditions.
- `else`: Used in if statements for the else condition.
- `+, -, *, /`: Represents simple mathematical operations.
- `(, ), :, =`: Symbols for miscellaneous use.
- `<, >, ?, !`: Are comparators. (? is ==, and ! is !=)
- `print` : Prints a variable or a string in quotes.
- `\n` : New line or Enter key, represents the end of a line (like semi-colon ’;’ in C++).
- `run`: Executes the code.

## Instructions:
Step-by-step instructions on how to create a program using this Domain-Specific Language (DSL):

1. Variable Declaration:

   - Start the program by declaring a variable using the keyword `var`.
   - Choose a data type (`int` or `float`) for the variable.
   - Assign a name to the variable using a valid identifier.
   - Use the equals sign `=` to initialize the variable with an expression.

   Example:
   ```dsl
   var int myVariable = 42
   ```

2. Mathematical Expression:
   - Create a mathematical expression using the variables declared.
   - Use the operators +, -, *, or / to perform operations.
   - Parentheses ( and ) can be used to control the order of operations.

   Example:
   ```dsl
   var float result = myVariable * (2 + 3)
   ```
3. Print Statement:
   - Use the print keyword to display the result or any variable.
   - You can print a literal, an identifier, or a string enclosed in double quotes.

   Example:
   ```dsl
   print result
   ```

4. Conditional Statement (Optional):
   - Optionally, conditional statement can be used to control the flow of the program.
   - Use the if, elif, and else keywords for this purpose.
   - Create comparisons using the <, >, ? (equivalent of ==), or ! (equivalent of !=) comparators.
   - Indent the code block to be executed within the statement.

   Example:
   ```dsl
   if (result > 10):
       print "Result is greater than 10"
   elif (result ? 10):
       print "Result is exactly 10"
   else:
       print "Result is less than 10"
   ```

5. While Loop (Optional):
   - Optionally, a while loop can be implemented to repeat a block of code while a certain condition is met.
   - Use the while keyword followed by the condition in parentheses.
   - Indent the code block to be executed within the loop.
   
   Example:
   ```dsl
   var int counter = 0
   while (counter < 5):
       print "Counter is:"
       print counter
       counter = counter + 1

   ```

6. Run the Program.
   - Finish your program with the keyword run followed by a newline character.

   Example
   ```dsl
   run
   ```

Putting all together, a simple program could look like this:
``` dsl
var int myVariable = 42
var float result = myVariable * (2 + 3)
print result

if (result > 10):
    print "Result is greater than 10"
elif (result ? 10):
    print "Result is exactly 10"
else:
    print "Result is less than 10"

var int counter = 0
while (counter < 5):
    print "Counter is: "
    print counter
    counter = counter + 1

run
```

## Examples:
**Input**
```dsl
var int x = 5
var int y = 2
var int z = x * y * (3 + 4)
z = z + 2
var int a = z - 40 / 2 + x
print a + 1
run
```

**Output**
```
> 58
```

**Input**
```dsl
print "Hello world!"
run
```

**Output**:
```
> Hello world!
```

## The Grammar
The full grammar can be found [here](/grammar.txt)


## References

1. [Python Sample DSL Code](https://github.com/davidcallanan/py-myopl-code/)
3. [Python DSL Video Tutorial](https://www.youtube.com/watch?v=Eythq9848Fg&list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD&index=1)
