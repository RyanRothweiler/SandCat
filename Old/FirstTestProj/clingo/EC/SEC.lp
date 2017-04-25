%
% Simplified Event Calculus (SEC)
%
% \fullciteA[pp. 252--253, 260]{Shanahan:1997}
%
% @book{Shanahan:1997,
%   author = "Murray Shanahan",
%   year = "1997",
%   title = "Solving the Frame Problem",
%   address = "Cambridge, MA",
%   publisher = "MIT Press",
% }
%
% @techreport{Kowalski:1986,
%   author = "Robert A. Kowalski",
%   year = "1986",
%   title = "Database updates in the event calculus",
%   institution = "London: Imperial College of Science, Technology, and Medicine",
%   number = "DOC 86/12",
% }
%

% (SEC1)
holds_at(F,T2) :- timepoint(T1),
                  timepoint(T2),
                  fluent(F),
                  event(E),
                  happens(E,T1),
                  initiates(E,F,T1),
                  T1<T2,
                  not clipped(T1,F,T2).

% (SEC2)
clipped(T1,F,T2) :- timepoint(T),
                    timepoint(T1),
                    timepoint(T2),
                    fluent(F),
                    event(E),
                    happens(E,T),
                    terminates(E,F,T),
                    T1<T,
                    T<T2.

% (SEC3)
holds_at(F,T) :- timepoint(T),
                 fluent(F),
                 initially(F),
                 not clipped(0,F,T).

% (SEC4) 
holds_at(F2,T2) :- timepoint(T1),
                   timepoint(T2),
                   timepoint(D),
                   fluent(F1),
                   fluent(F2),
                   event(E),
                   happens(E,T1),
                   initiates(E,F1,T1),
                   trajectory(F1,T1,F2,D),
                   T2 = T1+D,
                   not clipped(T1,F1,T2).

% End of file.
