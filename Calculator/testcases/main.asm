# global variable declaration: VariableDeclaration: int sum 0
pushi 0
pushi 0
wtgm
pop
# end of global variable declaration: VariableDeclaration: int sum 0
pushi 2
jl
# global function declaration:
# FunctionDeclaration: function takes (int)returning (int) add(i)
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
# move arg i to stack memory
pushi 0
wtsm
pop
# codes for function body
# CompoundStatement
# adjust FP
pushi 1
incfp
# CompoundStatement body
# expression statement
# bianry expression:
# calculate value
# bianry expression:sum + i
# calculate opnd1
# global object access: sum
pushi 0
rfgm
# end of global object access: sum
# calculate opnd2
# local object access: i
pushi -1
rfsm
# end of local object access: i
# calculate result
addi
# end of binary expression:sum + i
# calculate address
# global object access: sum
pushi 0
# end of global object access: sum
# assignment operation
wtgm
# end of binary expression:
pop
# end of expression statement
# if statement:
# calculate condition value:
# bianry expression:i<100
# calculate opnd1
# local object access: i
pushi -1
rfsm
# end of local object access: i
# calculate opnd2
pushi 100
# calculate result
lti
# end of binary expression:i<100
# test condition
jiff 3
# then statement
# expression statement
# function call :
# push params from right to left
# pushing param i + 1
# bianry expression:i + 1
# calculate opnd1
# local object access: i
pushi -1
rfsm
# end of local object access: i
# calculate opnd2
pushi 1
# calculate result
addi
# end of binary expression:i + 1
# adjust FP for the function
pushi 0
incfp
jal 1
# function returned,restore the FP
pushi 0
decfp
# end of function call
pop
# end of expression statement
# jump to endif
pushi 4
jl
# else statement
lable 3
lable 4
# end of if statement
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
# end of CompoundStatement body
# adjust FP
pushi 1
decfp
# end of CompoundStatement
# end of function: FunctionDeclaration: function takes (int)returning (int) add(i)
lable 2
pushi 5
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
# CompoundStatement
# adjust FP
pushi 1
incfp
# CompoundStatement body
# expression statement
# function call :
# push params from right to left
# pushing param 0
pushi 0
# adjust FP for the function
pushi 0
incfp
jal 1
# function returned,restore the FP
pushi 0
decfp
# end of function call
pop
# end of expression statement
# print statement:sum
# calculate value to be printed
# global object access: sum
pushi 0
rfgm
# end of global object access: sum
printi
# end of print statement: sum
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
# end of CompoundStatement body
# adjust FP
pushi 1
decfp
# end of CompoundStatement
# end of function: FunctionDeclaration: function takes (int)returning (int) main(x)
lable 5
pushi 0
jl
halt
