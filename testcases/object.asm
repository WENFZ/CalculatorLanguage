pushi 1
jl
# type  info begin
btd 1
int
etd 1
# type info end
lable 1
pushi 2
jl
# type  info begin
btd 2
structure
int
float
etd 2
# type info end
lable 2
{
# global variable declaration: VariableDeclaration:  o 
new 2
pushi 0
wtgm
pop
# end of global variable declaration: VariableDeclaration:  o 
}
{
# expression statement
{
# bianry expression:
# calculate value
pushi 1
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 1
# assignment operation
wthm
# end of binary expression:
}
pop
# end of expression statement
}
{
# print statement:
# calculate value to be printed
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 1
rfhm
printi
# end of print statement: 
}
{
# expression statement
{
# bianry expression:
# calculate value
new 1
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
# assignment operation
wthm
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
pushi 2
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
rfhm
ma 0
# assignment operation
wthm
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
new 1
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
# assignment operation
wthm
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
new 1
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
# assignment operation
wthm
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
pushi 1
# calculate address
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
rfhm
ma 0
# assignment operation
wthm
# end of binary expression:
}
pop
# end of expression statement
}
{
# print statement:
# calculate value to be printed
{
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 0
rfhm
ma 0
rfhm
printi
# end of print statement: 
}
{
pushi 3
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
lable 3
}
# call main function
pushi 0
jl
halt
