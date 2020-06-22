pushi 1
jl
# type  info begin
btd 1
bool
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
# global object access: o
pushi 0
rfgm
# end of global object access: o
}
ma 1
rfhm
pop
# end of expression statement
}
{
# expression statement
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
ma 1
# assignment operation
wrhm
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
wrhm
# end of binary expression:
}
pop
# end of expression statement
}
# call main function
pushi 0
jl
halt
