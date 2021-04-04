# SobokanSolver

Solver of Sobokan game with modification that boxes are pulled instead of being pushed.

## Input
The program reads map from the standard input. Example input in:  
```
6 6
######
#    #
#    #
#-#* #
#O#  #
######
```
where numers in first line are map's height and width respectively and following sybmols means:
- \# -> obstacle
- O -> box
- \- -> worker
- \* -> target position

Assumption is made that the map can be solved and there is only one worker.

## Output 
The output of the program is a single line written to standard output with moves that worker should do in order to pull boxes to their destination.
Moves are denoted with letters u, d, r, l U, D R, L where lowercase means worker's moves without box and uppercase means pulling box. 

For the example input above the output is:
```
UUrdRRdlD
```
