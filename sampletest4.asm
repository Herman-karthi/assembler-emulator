min: ldl a
stl 0
ldl b
stl 1
ldl c
stl 2
ldl d
stl 3
ldc 1;counter
stl 4

loop: ldl 4
ldnl 0
ldl 0
sub 
brlz step
ldl 4
ldnl 0
stl 0
br increment

step: ldl 0
stl 0

increment: ldl 4
ldc 1
add
stl 4
ldl 4
ldc 4
sub
brz exit
br loop

exit: HALT
a: data 10
b: data 2
c: data 3
d: data 4




