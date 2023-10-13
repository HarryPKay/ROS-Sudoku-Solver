% ENCODING

% adjust grid size
%#const size = 16.
#const gridDimension = 4.%<GRID_DIMENSION>
#const sqrtGridDimension = 2.%<SQRT_GRID_DIMENSION>

col(1..gridDimension).
row(1..gridDimension).
num(1..gridDimension).
subCol(0..sqrtGridDimension - 1).
subRow(0..sqrtGridDimension - 1).

% fmentions, each cell mentions number once (cardinality of 1 for that number).
{ gridNum(R, C, N) : num(N) } = 1 :- row(R), col(C).

% each row and col mentions 1 of each number
{ gridNum(R, C, N) : row(R) } = 1 :- col(C), num(N).
{ gridNum(R, C, N) : col(C) } = 1 :- row(R), num(N).

% each subggrid also has numbers, offset by 1 for proper allignment 
subGridNum(S_R, S_C, R, C, N) :- gridNum(R, C, N), S_R = (R - 1)/sqrtGridDimension, S_C = (C - 1)/sqrtGridDimension.

% constraints to enforce unique numbers for subgrids.
:- subGridNum(S_R, S_C, R, C, N), subGridNum(S_R_2, S_C_2, R_2, C_2, N), S_R == S_R_2, S_C == S_C_2, R != R_2, C != C_2.

% Map to solution
gridNumSol(R, C, N) :- gridNum(R, C, N).

% filter for gridNumSol
#show gridNumSol/3.
%#show subGridNum/5.
