# MSDscript

## License
Provided under MIT standard license

## INTRODUCTION

MSDscript is an interpreter used for interpreting equations. The interpreter supports the following methods:

- Addition (+)
- Multiplication (*)
- Comparisons (==)
- Functions
- Let and If statements

These statements can be entered in the command line or via linked file (through the command line). They follow a specific format (see USE section below).

## INSTALLATION
Included is a library of all necessary classes, `MSDlib.a`

### Compiling with your own library
Suppose you wanted to use the library with your program, in case called `which_day`. You can compile the library for use with your own project using a command following this syntax:

`c++ -o Which which_day.cpp ./MSDscript/msdScript_main/MSDlib.a`

**NOTE**: point to the location of the downloaded library with a path relative to current location. 

### Compiling and running standalone
If you wanted to run the program by itself, you can compile by entering the directory where you've downloaded the zipped folder, extracting all contents and running the command `make` in your terminal.

### Command Line Arguments

To use this library, the following these command line instructions:

`--help` : will bring a small list of helpful commands.

`--interp` : will run the interpreter. Once in `interp` mode, you can enter an expression following the patterns listed below and it will be interpreted.

`--step` : will run the interpreter in step mode (useful to prevent memory overflows). Similar to `interp`, you may enter expressions to be interpreted.

`--print` : run in print mode. This will return the print value of an equation, with correct spacing and parenthesis as needed.

## DATA TYPES
MSDscript contains two major data types **Expressions (Expr)** and **Values (Val)**. Within each of these classes are subclasses of each data type, explained below:

### Expressions (Expr)

The subclasses of **Expr** are listed below with brief explanation of their purpose. 

- *NumExpr* : expression representation fo a simple number. (Example: `5`)

- *AddExpr* : addition expression (Example: `5 + 5`)

- *MultExpr* : addition expression (Example: `4 * 4`)

- *VariableExpr* : representation of variable, which could be a single character (example `x`) or a string (`myVariable`).

- *LetExpr* : A let expression, giving a **Expr** value to a variable. (Example `_let x = 5 _in 10 + x`). Note that a *LetExpr* takes can accept both simple values (`5`) and full expressions (`5 + 5`) as the expression the value represents. *LetExpr* also requires as **Expr** in which the variable/expression apply to. See [LetExpr](#LetExpr) for more details.
  
- *BoolExpr* : creates a comparison, similar to the `==` in most languages. It compares two expressions. (Example `2 == 2`).  

- *EqExpr* : Creates comparison of *expressions* (not of values of expressions), effecivly seeing if the expressions are equal. (Examples `5 + 5 == 5 + 5` would return `true` whereas `5 + 5 == 8 + 3` would return `false`).

- *IfExpr* : an expression resulting in different outcomes based on the evaluation of the first expression, whether true or false. (Example `_if 5 == 5 _then 10 * 10 _else 3 * 3`).

- *FunExpr* : Function expression that *create* a function to be called by **CallExpr** (see below). (Example `_fun (x) (x+10)`).

- *CallExpr* : Calls an expression previously declared see **FunExpr** above. Takes function to call and the formal argument to pass.

### Expression Methods
All **Expr** subclasses inherit the following methods:

- *equals* : Returns a boolean whether expression matches another expression, using the **EqExpr**.

- *interp* : Interprets the expression to return a **Val**

- *has_variable* (**Depreciated**): Returns a boolean whether the expression has a **VariableExpr** or not. 

- *print* : Prints the expression in readable, string format.

- *pretty_print_at* (**Depreciated**): Prints the expression in readable, string format which strips off unnecessary parenthesis, spaces and spacing.

- *step_interp* : Interprets the expression to return a **Val** by using stack space, rather than heap to prevent memory overflows.


### Implementing Expressions

TODO not sure if this is something we need to include for other developers. would they need to call expressions? Or isn't the point of this interpreter so they don't have to do so? it parses out expressions and gives back values.

#### LetExpr

### Values (Val)
Values are what **Expr** are evaluated to. An **Expr**, when interpreted, returns a value. There are three **Val** types, described below:

- *NumVal* : A numerical value of an expression. It is what an **Expr** would resolve to, when interpreted. For example, `5 + 5` would evaluate to the **Val** `10`. 
- *BoolVal* : A boolean value of an expression. Would result in either `true` or `false`.
- *FunVal* : This is the representation of a function, not the value of an interpreted function. The actual function a variable represents. For example, 

#### Values Methods

## USE

To use the program, you will enter or pass equations in the applicable format for evaluation. 


### Addition
Basic addition equations are written in the following way:

```cpp
5 + 5 //would interpret to 10
```

MSDscript follows basic order of operations, so Multiplication would take presedence over addition. In such cases, addition equations that should be evaluated before multiplication, should be nested inside parenthesis, in the following way:

```cpp
(5 + 5) * 4 //would evaluate to 100
```

### Multiplication

Basic multiplication equations are written in the following way:

```cpp
5 * 5 //would interpret to 25
```

### Let

Basic let equations are written in the following way:

```cpp
_let x = 5
    _in x + 10 //would evaluate to 15
```

*note that spacing is not required and the parser will evaluate correctly even if written on a single line*

### If

Basic if equations are written in the following way:

```cpp
_if 5 == 5 //would evaluate true
    _then 10 * 10 //<--this section would be executed
    _else 3 * 3 
```

In an **IF** statement if the first line evaluates TRUE then the *_then* block will be executed. If it evaluates FALSE then the *_else* block will execute.

### Functions
Basic functions are declared in the following way:

```cpp
_fun(x) x + 5
```

However, functions are usually called within a `_let` statement, as follows:

```cpp
_let myFunction = _fun(x) x + 5 _in myFunction(10) 
        // would evaluated to 15
```
