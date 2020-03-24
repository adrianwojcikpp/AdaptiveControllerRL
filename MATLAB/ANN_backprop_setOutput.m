function ann_vec = ANN_backprop_setOutput(ann)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   w(1) = L -> number of network layers
%   w(2 : L+1) -> size of input (s1) and sizes of layers (s2 ... sL)
%   w(L+2)     -> 1. layer activation function type
%   w(L+3 : L+2+s2*s1)          -> 1. layer weigths
%   w(L+3+s2*s1 : L+2+s2*s1+s1) -> 1. layer bias
%   [etc]
%   w(...)       -> max output
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% compute size
s = 1 + (1+ann.L); % layer number + network size vector
for l = 1 : ann.L
    s = s + 1;                     % activation function
    s = s + ann.NS(l+1)*ann.NS(l); % weights
    s = s + ann.NS(l+1);           % bias
end
s = s + 1; % max out
    
% allocate vector
ann_vec = zeros(s,1);

% fill vector
ann_vec(1) = ann.L; % number of layers

[i_start, i_end] = ANN_getIndex(2, (1+ann.L)); 
ann_vec(i_start:i_end) = ann.NS; % network size: input & layers sizes

% layer data: activation func. index, weigths matrix, bias vector
for l = 1 : ann.L
   layer_data = [ann.af_index(ann.f{l}); ann.w{l}(:); ann.b{l}];
 
   [i_start, i_end] = ANN_getIndex(i_end + 1, length(layer_data));  
   ann_vec(i_start:i_end) = layer_data; 
end

% max output
i_start = ANN_getIndex(i_end + 1, 1); 
ann_vec(i_start) = ann.mxOut;