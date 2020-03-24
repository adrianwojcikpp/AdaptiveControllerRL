function ann_vec = ANN_backprop(INPUT)

persistent ann backprop_params

%% INPUT vector conversion
if isempty(ann)
    % to cost function, network input, init network & training params
    [Cf, x, ann_init, backprop_params] = ANN_backprop_getInput(INPUT, 'full');
    ann = ANN_controller_getInput(ann_init);
else
    % to cost function & network input
    [Cf, x] = ANN_backprop_getInput(INPUT);
end

%% Local variables
% algorithm parameters
eta = backprop_params(1);

%% Backprop step
ann = ann.backprop_step(Cf, x, eta);

%% Training output
ann_vec = ANN_backprop_setOutput(ann);