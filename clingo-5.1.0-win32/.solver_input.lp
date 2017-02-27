number(0..9).
% domain variables X and Y
 #domain number(X;Y).
r(1) :- true.
#hide _new_pred_1(Y).
s(X) :- r(X),not _new_pred_1(Y).
_new_pred_1(Y) :- p(X,Y),q(X,Y).
true.
:- false.
#hide true.
