# global function declaration:
# FunctionDeclaration: function takes (int,int)returning (int) add(sum,i)
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
# move arg sum to stack memory
pushi 0
wtsm
pop
# move arg i to stack memory
pushi 1
wtsm
pop
# codes for function body
# CompoundStatement
# adjust FP
pushi 2
incfp
# CompoundStatement body
# if statement:
# calculate condition value:
# bianry expression:i == 100
# calculate opnd1
# local object access: i
pushi -1
rfsm
# end of local object access: i
# calculate opnd2
pushi 100
# calculate result
eqi
# end of binary expression:i == 100
# test condition
jiff 2
# then statement
# return statement: 
# calculate the return value
# bianry expression:sum + 100
# calculate opnd1
# local object access: sum
pushi -2
rfsm
# end of local object access: sum
# calculate opnd2
pushi 100
# calculate result
addi
# end of binary expression:sum + 100
# adjust the FP for potential variables declared within the function
pushi 2
decfp
# fetch return address from stack memory
pushi 1
decfp
pushi 0
rfsm
# jump to return address
jump
# end of return statement
# jump to endif
pushi 3
jl
# else statement
lable 2
# return statement: 
# calculate the return value
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
# pushing param sum + i
# bianry expression:sum + i
# calculate opnd1
# local object access: sum
pushi -2
rfsm
# end of local object access: sum
# calculate opnd2
# local object access: i
pushi -1
rfsm
# end of local object access: i
# calculate result
addi
# end of binary expression:sum + i
# adjust FP for the function
pushi 0
incfp
jal 1
# function returned,restore the FP
pushi 0
decfp
# end of function call
# adjust the FP for potential variables declared within the function
pushi 2
decfp
# fetch return address from stack memory
pushi 1
decfp
pushi 0
rfsm
# jump to return address
jump
# end of return statement
lable 3
# end of if statement
# end of CompoundStatement body
# adjust FP
pushi 2
decfp
# end of CompoundStatement
# end of function: FunctionDeclaration: function takes (int,int)returning (int) add(sum,i)
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
# print statement:
# calculate value to be printed
# function call :
# push params from right to left
# pushing param 0
pushi 0
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
printi
# end of print statement: 
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
