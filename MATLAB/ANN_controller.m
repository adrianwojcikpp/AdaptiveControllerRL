function y = ANN_controller(INPUT)
%% INPUT vector to network structure & network input
[ann,x] = ANN_controller_getInput(INPUT);

%% Neural network algorthim
y = ann.compute(x);