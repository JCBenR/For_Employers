# Pointer Refactoring

## prep work

-[ ] make sure all pointers are consistently alligned
 ```c++
Expr* 
//not
Expr *AddExpr
//or
Expr * AddExpr
```

## Parent Classes
-[x] Val*
-[x] Expr*


## Sub Classes

-[x] VariableExpr
-[x] AddExpr
-[x] MultExpr
-[x] LetExpr
-[x] BoolExpr
-[x] EqExpr
-[x] IfExpr
-[x] FunExpr
-[x] CallExpr


## Val
-[x] Val
-[x] NumVal
-[x] BoolVal
-[x] FunVal

## Functions
*should be included in Expr*

## Parse
-[x] Expr*
-[x] VariableExpr*


## new

-[x] VariableExpr
-[x] AddExpr
-[x] MultExpr
-[x] CallExpr
-[x] BoolExpr
-[x] LetExpr
-[x] FunExpr
-[x] NumExpr
-[x] EqExpr
-[x] BoolVal
-[x] IfExpr
-[x] NumVal
-[x] FunVal
-[x] Val
-[x] Expr


## Dynamic Cast
-[x] All Sections with dynamic_cast

## Class

