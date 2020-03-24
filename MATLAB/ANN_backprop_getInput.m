function [Cf, x, ann_init, backprop_params] = ANN_backprop_getInput(INPUT, op)
%% Check options
if nargin > 1
   out_full = strcmp(op, 'full'); 
else
   out_full = 0;
end

%% Convert INPUT
% Cost function (size) [scalar]
[i_start, i_end] = ANN_getIndex(1, 1);
Cf_size = INPUT(i_start : i_end);

% Cost function [scalar]
[i_start, i_end] = ANN_getIndex(i_end + 1, Cf_size);
Cf = INPUT(i_start : i_end);

% Input (size) [scalar]
[i_start, i_end] = ANN_getIndex(i_end + 1, 1);
x_size = INPUT(i_start : i_end);

% Input [vector]
[i_start, i_end] = ANN_getIndex(i_end + 1, x_size);
x = INPUT(i_start : i_end);

if out_full
% ANN (size) [scalar]
[i_start, i_end] = ANN_getIndex(i_end + 1, 1);
ann_init_size = INPUT(i_start : i_end);

% ANN [vector]
[i_start, i_end] = ANN_getIndex(i_end + 1, ann_init_size);
ann_init = INPUT(i_start : i_end);

% Backprop params  [scalar]
[i_start, i_end] = ANN_getIndex(i_end + 1, 1);
backprop_params = INPUT(i_start : i_end);
else
ann_init = [];
backprop_params = [];
end