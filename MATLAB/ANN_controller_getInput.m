function [ann,x] = ANN_controller_getInput(ann_vec)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   w(1) = L -> number of network layers
%   w(2 : L+1) -> size of input (s1) and sizes of layers (s2 ... sL)
%   w(L+2)     -> 1. layer activation function type
%   w(L+3 : L+2+s2*s1)          -> 1. layer weigth
%   w(L+3+s2*s1 : L+2+s2*s1+s1) -> 1. layer bias
%   [etc]
%   w(...)       -> max output
%   w(... : end) -> network input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

L = ann_vec(1);              % number of layers

[i_start, i_end] = ANN_getIndex(2, (1+L));
NS = ann_vec(i_start:i_end); % network size: input & layers sizes

ann = neuralnetwork(L, NS);

for l = 1 : ann.L
    % activation function
    [i_start, i_end] = ANN_getIndex(i_end + 1, 1);
    
    [ann.f{l}, ann.df{l}] = ann.af_selector(ann_vec(i_start));
    
    % weigths matrix
    [i_start, i_end] = ANN_getIndex(i_end + 1, ann.NS(l+1)*ann.NS(l));
    W = ann_vec(i_start:i_end);
    
    ann.w{l} = reshape(W, [ann.NS(l+1), ann.NS(l)]);
    
    % bias vector
    [i_start, i_end] = ANN_getIndex(i_end + 1, ann.NS(l+1));
    B = ann_vec(i_start:i_end);
    
    ann.b{l} = B;  
end

% max output
[i_start, i_end] = ANN_getIndex(i_end + 1, 1);
ann.mxOut = ann_vec(i_start);

% network input (if present)
i_start = ANN_getIndex(i_end + 1, 1);
if length(ann_vec) < i_start
    x = [];
else
    x = ann_vec(i_start:end);
end
