# refactoring Homework

Refactor your MSDscript implementation so that Expr::interp returns PTR(Val) instead of int. For now, your only implementation of Val will be NumVal for numbers (but you'll add boolean values in the next assignment). You can use whatever name you prefer for the classes (e.g., virtual base class Value with subclass NumberValue); the point is just to change over to value objects instead of using int for values.

For clarity, you might:
-[x] rename existing classes like AddExpr to AddExpr, etc., but that's up to you. Although it's optional, renaming those classes is easy compared to the refactoring of Expr::interp, so consider starting with that renaming.

    -[x] Add
    -[x] Mult
    -[x] NumExpr
    -[x] VariableExpr
    -[x] LetExpr

## More recommendations:

Read these bullets before you start, but also go back and read all them again after you think you're done.
-[x] Create new files "rep.cpp" and "rep.h" (or whatever file names you prefer) for the new Val classes.
-[x] In "expr.h", use a class Val; declaration near the top so that the declaration of Expr::interp can refer to PTR(Val). Do not use #include "rep.h", because the declaration of Expr classes should not need the implementation of Val classes.
-[x] In "expr.cpp", use #include "rep.h", because the implementation of Expr methods will need to use the implementation of Val classes and methods.
-[x] In "rep.h", similarly use class Expr; to allow Val::to_expr's return type to be Expr*. In "rep.cpp", use #include "expr.h" in addition to #include "rep.h".
-[x] You'll need to update your existing Expr::interp tests to use number-value objects instead of integers. Remember to change == for comparing integers to ->equals for comparing value objects.
-[ ] ~~If you're using Xcode, don't forget to add "rep.cpp" to your test target as well as your main target.~~
-[x] To full test a method like NumVal::equals, you'll need a pointer of type PTR(Val) that is not a pointer to a NumVal object — but NumVal is the only instantiable Val class at first! To work around that problem, use NULL in tests for now, since it can have type PTR(Val) and is not a pointer to a NumVal object.
-[x] Don't forget to update "Makefile".