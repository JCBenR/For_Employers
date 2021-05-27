Add _true, _false, ==, and _if..._then..._else to your MSD script implementation.

Some details on the forms:

-[x] The boolean values for true and false must really be distinct from number values. That is, don't use the same value to represent the number 0 and false, for example. That's why you'll need a new Val subclass.
-[x] When + or * gets a boolean value, then it should complain at run time. Note that they complain about values, not expressions. The left and right subexpressions in a + expression can be anything, and it's only when you interp them that you know whether the results are integers.
-[x] The == operation can work on any kinds of values, and it's even allowed to compare a boolean to a number — but only booleans are equal to booleans and only numbers are equal to numbers. That is, unlike * and +, == never complains about its arguments, and it always produces a boolean value.
-[x] The if..._then..._else form requires its first subexpression to produce a boolean value. (Again, it can only complain after interp produces a non-boolean value, no matter what the expression is). Also, only the “then” or “else” branch should be interpreted, not both.
-[x] The print method should always put parentheses around a == or _if..._then..._else form, 
-[x] and never around _true or _false. 
-[x] Make print include no space around ==, 
-[x] while pretty_print includes a space on both sides of ==. 
-[x] Both printing modes include a single space after _if, _then, and _else. 
-[ ] For parentheses, the pretty_print method on expressions should be consistent with the usual goals: minimal parentheses as needed to make the expression unambiguous, parentheses added to the smallest expressions when necessary, and all consistent with the parser. 
-[x] For line breaks, pretty_print should start _then and _else on new lines and aligned under the _if, 
-[x] while print simply emits a space before _then and _else.
As an example, the MSDscript program

```cpp
_let same = 1 == 2
_in _if 1 == 2
    _then _false + 5
    _else 88
```

should interp to the value 88, and it should not complain about false being added to 5.

The example:

```cpp
_if 4 + 1
_then 2
_else 3
```

does not have a value; interpreting it should raise an exception, because 5 is not a boolean.



This is a big change to your MSDscript code base, involving a lot of method and function definitions and well as tests. Fortunately, with the possible exception of pretty printing (see further below), the changes should be straightforward; they follow recipes and patterns that we've used already.

You should break down this big change into manageable steps, and here are some suggested steps:

-[x] Add a new Val class to represent boolean values. You can implement and test the new class without changing anything else about your MSDcript implementation, except that you can't complete a to_expr method to convert a value back to an expression (because you haven't added boolean expressions, yet). For to_expr, just return NULL and save it for testing later — but make sure you have good test coverage for the new code otherwise.
-[x] Add new Expr classes for booleans, equality expressions, and conditionals. Add one class at a time, and get back to good test coverage before writing more. Save pretty printing for last, perhaps just using print as the operation temporarily.
-[ ] Change your parser to recognize the new forms. Again, you can add them one at a time and test along the way.
-[ ] After you're finished, consider updating your random tester to try to new language forms.
Here are some observations and advice on pretty-print with the expanded language:

- The == form has lower precedence than +, which has lower precedence than *. So, to handle parenthesization for those forms, the “accumulated” mode might which level of precedence needs parentheses.
- The keyworded forms _let and _if end up having the same parenthesization rules. Instead of trying to combine the keyword-form printing mode with an operation-precedence level, you'll probably find it easier to make ”keyword forms need parentheses" a separate accumulator. When it's split this way, note that unparethesized operators tend to pass the keyword-form mode down to subexpressions on the right-hand side, while parenthesized operators can always clear that mode. (On the left-hand side of a operator, the mode can be always set to parenthesize keyword forms.)

## other items:
-[ ] check todos

## terms
### conditionals
this is like a 
```javascript
isMetric ? setMetric() : setImperial()
```
it's an if this: do this, else do that.

### expressions
(1 + 2 ) == 3 is an *expression*

-that expression has a **value** determined by interp().

the value of of (1 + 2) == 3 is *_true*

it's through interp when an expression goes to a value

1+2 is an *expression*. it has a *value*, which is 3. you can get that value by running interp() on the expression.

3 is both and *expression* and a *value*. it is a NumExpr *expression* and a NumVal *value*.

*_true* is an expression and it has the value *_true*