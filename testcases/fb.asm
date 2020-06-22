{
# global variable declaration: VariableDeclaration: int x 3
pushi 3
pushi 0
wtgm
pop
# end of global variable declaration: VariableDeclaration: int x 3
}
{
pushi 2
jl
# global function declaration:
# FunctionDeclaration: function takes (int)returning (int) f(x)
# function address
lable 1
# save return address
pushi 0
wtsm
# pop up return address on the opnd stack
pop
# shadow the fp for return address
pushi 1
incfp
# move arg x to stack memory
pushi 0
wtsm
pop
# codes for function body
{
# if statement:
# calculate condition value:
{
# bianry expression:x<2
# calculate opnd1
{
# local object access: x
pushi 0
rfsm
# end of local object access: x
}
# calculate opnd2
pushi 2
# calculate result
lti
# end of binary expression:x<2
}
# test condition
jiff 3
# then statement
{
# return statement: 
# calculate the return value
{
# local object access: x
pushi 0
rfsm
# end of local object access: x
}
# adjust the FP for potential variables declared within the function
pushi 0
decfp
# fetch return address from stack memory
pushi 1
decfp
pushi 0
rfsm
# jump to return address
jump
# end of return statement
}
# jump to endif
pushi 4
jl
# else statement
lable 3
{
# return statement: 
# calculate the return value
{
# bianry expression: + 
# calculate opnd1
{
# function call :
# push params from right to left
# pushing param x - 1
{
# bianry expression:x - 1
# calculate opnd1
{
# local object access: x
pushi 0
rfsm
# end of local object access: x
}
# calculate opnd2
pushi 1
# calculate result
subi
# end of binary expression:x - 1
}
# adjust FP for the function
pushi 1
incfp
jal 1
# function returned,restore the FP
pushi 1
decfp
# end of function call
}
# calculate opnd2
{
# function call :
# push params from right to left
# pushing param x - 2
{
# bianry expression:x - 2
# calculate opnd1
{
# local object access: x
pushi 0
rfsm
# end of local object access: x
}
# calculate opnd2
pushi 2
# calculate result
subi
# end of binary expression:x - 2
}
# adjust FP for the function
pushi 1
incfp
jal 1
# function returned,restore the FP
pushi 1
decfp
# end of function call
}
# calculate result
addi
# end of binary expression: + 
}
# adjust the FP for potential variables declared within the function
pushi 0
decfp
# fetch return address from stack memory
pushi 1
decfp
pushi 0
rfsm
# jump to return address
jump
# end of return statement
}
lable 4
# end of if statement
}
# end of function: FunctionDeclaration: function takes (int)returning (int) f(x)
lable 2
}
{
pushi 5
jl
# global function declaration:
# FunctionDeclaration: function takes (int)returning (int) main(param)
# function address
lable 0
# save return address
pushi 0
wtsm
# pop up return address on the opnd stack
pop
# shadow the fp for return address
pushi 1
incfp
# move arg param to stack memory
pushi 0
wtsm
pop
# codes for function body
{
# CompoundStatement
# adjust FP
pushi 1
incfp
# CompoundStatement body
{
# local variable declaration: VariableDeclaration: int x 10
# codes for initializer
pushi 10
# move variable value from opnd stack to stack memory
pushi 0
wtsm
pop
# end of local variable declaration VariableDeclaration: int x 10
}
{
# print statement:
# calculate value to be printed
{
# function call :
# push params from right to left
# pushing param x
{
# local object access: x
pushi 0
rfsm
# end of local object access: x
}
# adjust FP for the function
pushi 1
incfp
jal 1
# function returned,restore the FP
pushi 1
decfp
# end of function call
}
printi
# end of print statement: 
}
{
# return statement: 
# calculate the return value
pushi 0
# adjust the FP for potential variables declared within the function
pushi 1
decfp
# fetch return address from stack memory
pushi 1
decfp
pushi 0
rfsm
# jump to return address
jump
# end of return statement
}
# end of CompoundStatement body
# adjust FP
pushi 1
decfp
# end of CompoundStatement
}
# end of function: FunctionDeclaration: function takes (int)returning (int) main(param)
lable 5
}
# call main function
pushi 0
jl
halt
