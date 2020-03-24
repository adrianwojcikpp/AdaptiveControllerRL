%% Clear
clear all; clc;

%% Display format
origFormat = get(0,'Format');
format long

%% Random network
networkname = 'ann_dsp'; % C++ variable name!

L = 2;
NS = [6 5 2];
AF = [1 1];

SEED = 123;

annM = neuralnetwork(L, NS, AF, 1, SEED);

x_init = zeros(NS(1),1);

%% Generate files
datatype = 'FLOAT';  % 'DOUBLE'
filename = 'MATLAB_GENERATED_FILE_ADSP';
fileEx = 'c';
libname = 'AW_ANN_ADSP_lib';
orientation = 'ROWS'; % 'COLUMNS'

PRINT_C_ann_hex(annM, networkname, libname, datatype, filename, fileEx, orientation, x_init);

tspath = 'C:\Google Drive\Politechnika Poznanska\Praca naukowa\Laboratory\VisualDSP\Torque Drive AW';
movefile([filename '.h'], tspath);
movefile([filename '.c'], tspath);

%% Input 
filename = 'input_test2';

randn('seed',666);
x = randn(NS(1), 1);

PRINT_C_union_hex_data_1D('ANN_INPUT_BUFFER2', 'dat', x, datatype, 'ROWS');      
%movefile('ANN_INPUT_BUFFER2.dat', tspath);

%% LOOP

matlab_output = zeros(2,51);

y_ref = [0.5 0.5]';
matlab_output(:,1) = annM.compute(x);

fprintf('INPUT  %d) [%f, %f, %f, %f, %f, %f]\n', i, x);
fprintf('OUTPUT %2d) [%f, %f]\n', 0, matlab_output(1), matlab_output(2));
for i = 1 : 50
    e = y_ref - matlab_output(:,i);
    annM = annM.backprop_step(e, x, 0.1);
    matlab_output(:,i+1) = annM.compute(x);
    fprintf('OUTPUT %2d) [%f, %f]\n', i, matlab_output(i), matlab_output(i));

end
