# CONTINUATIONS

## STEPS

-[ ] interp needs to interpret the LHS, but it also needs to make the post-it note.
-[ ] create fields in the post it note it needs to remember
-[ ] interp becomes step_interp


## CONTINUE
- *remember/receive balloons are continuations*
-[ ] alwasy created by an interp step

### SET UP

*EXPR will always have a step_interp method*

*CONT will always have a step_continue method*

-[ ] each EXPR subclass needs some number of CONT subclasses

```cpp
//addExpr needs some AddCont
//addExpr has two (addCont and RightThenAddCont), others will have one and other will have none.
```

-[ ] step_interp can set the mode to interp_mode or continue_mode
-[ ] step_continue always sets the mode to interp_mode


### rules for step_interp and step_continue

it should never call

- step_interp
- step_continue
- interp_by_steps
- interp
- optimize
- subst

-[ ] interp_by_steps should call either step_interp or step_continue


## step interp
-[x] AddExpr
-[x] MultExpr
-[x] CompExpr
-[x] FunExpr
-[x] CallExpr
-[x] LetExpr
-[x] NumExpr
-[x] VariableExpr
-[x] BoolExpr
-[x] EqExpr
-[x] IfExpr