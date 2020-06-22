{
pushi 1
jl
# global function declaration:
# FunctionDeclaration: function takes (int)returning (int) main(x)
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
# move arg x to stack memory
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
# local variable declaration: VariableDeclaration: int i 0
# codes for initializer
pushi 0
# move variable value from opnd stack to stack memory
pushi 0
wtsm
pop
# end of local variable declaration VariableDeclaration: int i 0
}
{
# local variable declaration: VariableDeclaration: int sum 0
# codes for initializer
pushi 0
# move variable value from opnd stack to stack memory
pushi 1
wtsm
pop
# end of local variable declaration VariableDeclaration: int sum 0
}
{
# while statement:
lable 2
# while condition:
{
# bianry expression:i<100
# calculate opnd1
{
# local object access: i
pushi 0
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 100
# calculate result
lti
# end of binary expression:i<100
}
jiff 3
# end of while condition
# while body
{
# CompoundStatement
# adjust FP
pushi 2
incfp
# CompoundStatement body
{
# expression statement
{
# bianry expression:
# calculate value
{
# bianry expression:i + 1
# calculate opnd1
{
# local object access: i
pushi -2
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 1
# calculate result
addi
# end of binary expression:i + 1
}
# calculate address
{
# local object access: i
pushi -2
# end of local object access: i
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# expression statement
{
# bianry expression:
# calculate value
{
# bianry expression:sum + i
# calculate opnd1
{
# local object access: sum
pushi -1
rfsm
# end of local object access: sum
}
# calculate opnd2
{
# local object access: i
pushi -2
rfsm
# end of local object access: i
}
# calculate result
addi
# end of binary expression:sum + i
}
# calculate address
{
# local object access: sum
pushi -1
# end of local object access: sum
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
# end of CompoundStatement body
# adjust FP
pushi 2
decfp
# end of CompoundStatement
}
# end of while body
pushi 2
jl
lable 3
# end of while statement
}
{
# print statement:sum
# calculate value to be printed
{
# local object access: sum
pushi 1
rfsm
# end of local object access: sum
}
printi
# end of print statement: sum
}
{
# expression statement
{
# bianry expression:
# calculate value
pushi 0
# calculate address
{
# local object access: i
pushi 0
# end of local object access: i
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# while statement:
lable 4
# while condition:
{
# bianry expression:i<500
# calculate opnd1
{
# local object access: i
pushi 0
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 500
# calculate result
lti
# end of binary expression:i<500
}
jiff 5
# end of while condition
# while body
{
# CompoundStatement
# adjust FP
pushi 2
incfp
# CompoundStatement body
{
# expression statement
{
# bianry expression:
# calculate value
{
# bianry expression:i + 1
# calculate opnd1
{
# local object access: i
pushi -2
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 1
# calculate result
addi
# end of binary expression:i + 1
}
# calculate address
{
# local object access: i
pushi -2
# end of local object access: i
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# if statement:
# calculate condition value:
{
# bianry expression:i>10
# calculate opnd1
{
# local object access: i
pushi -2
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 10
# calculate result
gti
# end of binary expression:i>10
}
# test condition
jiff 6
# then statement
pushi 2
decfp
pushi 5
jl
# jump to endif
pushi 7
jl
# else statement
lable 6
lable 7
# end of if statement
}
# end of CompoundStatement body
# adjust FP
pushi 2
decfp
# end of CompoundStatement
}
# end of while body
pushi 4
jl
lable 5
# end of while statement
}
{
# print statement:i
# calculate value to be printed
{
# local object access: i
pushi 0
rfsm
# end of local object access: i
}
printi
# end of print statement: i
}
{
# expression statement
{
# bianry expression:
# calculate value
pushi 0
# calculate address
{
# local object access: i
pushi 0
# end of local object access: i
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# expression statement
{
# bianry expression:
# calculate value
pushi 0
# calculate address
{
# local object access: sum
pushi 1
# end of local object access: sum
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# local variable declaration: VariableDeclaration: bool isOdd false
# codes for initializer
pushb false
# move variable value from opnd stack to stack memory
pushi 2
wtsm
pop
# end of local variable declaration VariableDeclaration: bool isOdd false
}
{
# while statement:
lable 8
# while condition:
{
# bianry expression:i<10
# calculate opnd1
{
# local object access: i
pushi 0
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 10
# calculate result
lti
# end of binary expression:i<10
}
jiff 9
# end of while condition
# while body
{
# CompoundStatement
# adjust FP
pushi 3
incfp
# CompoundStatement body
{
# expression statement
{
# bianry expression:
# calculate value
{
# bianry expression:i + 1
# calculate opnd1
{
# local object access: i
pushi -3
rfsm
# end of local object access: i
}
# calculate opnd2
pushi 1
# calculate result
addi
# end of binary expression:i + 1
}
# calculate address
{
# local object access: i
pushi -3
# end of local object access: i
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# expression statement
{
# bianry expression:
# calculate value
{
# unary expression:!isOdd
# calculate opnd
{
# local object access: isOdd
pushi -1
rfsm
# end of local object access: isOdd
}
# calculate result
not
# end of unary expression:!isOdd
}
# calculate address
{
# local object access: isOdd
pushi -1
# end of local object access: isOdd
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# if statement:
# calculate condition value:
{
# local object access: isOdd
pushi -1
rfsm
# end of local object access: isOdd
}
# test condition
jiff 10
# then statement
pushi 3
decfp
pushi 8
jl
# jump to endif
pushi 11
jl
# else statement
lable 10
lable 11
# end of if statement
}
{
# expression statement
{
# bianry expression:
# calculate value
{
# bianry expression:sum + i
# calculate opnd1
{
# local object access: sum
pushi -2
rfsm
# end of local object access: sum
}
# calculate opnd2
{
# local object access: i
pushi -3
rfsm
# end of local object access: i
}
# calculate result
addi
# end of binary expression:sum + i
}
# calculate address
{
# local object access: sum
pushi -2
# end of local object access: sum
}
# assignment operation
wtsm
# end of binary expression:
}
pop
# end of expression statement
}
{
# print statement:sum
# calculate value to be printed
{
# local object access: sum
pushi -2
rfsm
# end of local object access: sum
}
printi
# end of print statement: sum
}
# end of CompoundStatement body
# adjust FP
pushi 3
decfp
# end of CompoundStatement
}
# end of while body
pushi 8
jl
lable 9
# end of while statement
}
{
# print statement:i
# calculate value to be printed
{
# local object access: i
pushi 0
rfsm
# end of local object access: i
}
printi
# end of print statement: i
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
# end of function: FunctionDeclaration: function takes (int)returning (int) main(x)
lable 1
}
# call main function
pushi 0
jl
halt
