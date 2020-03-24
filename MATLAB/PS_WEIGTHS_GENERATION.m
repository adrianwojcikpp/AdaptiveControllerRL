function [w,b] = PS_WEIGTHS_GENERATION(rows, cols)

W = (randn(cols, rows)/100)';
w = W(:, 1 : end-1);
b = W(:, end);