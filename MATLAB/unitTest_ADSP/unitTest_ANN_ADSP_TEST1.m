%% Clear
clear all; clc;

%% Display format
origFormat = get(0,'Format');
format long

%% Random network
networkname = 'ann_dsp'; % C++ variable name!

L = 3;
NS = [6 5 4 1];
AF = [1 1 1];

SEED = 123;

annM = neuralnetwork(L, NS, AF, 1, SEED);

x_init = zeros(NS(1),1);

%% Generate files
datatype = 'FLOAT';  % 'DOUBLE'
filename = 'MATLAB_GENERATED_FILE_ADSP';
fileEx = 'c';
libname = 'ANN_ARM_lib';
orientation = 'ROWS'; % 'COLUMNS'

PRINT_C_ann_hex(annM, networkname, libname, datatype, filename, fileEx, orientation, x_init);

tspath = '..\AW_ANN_ARM_C\STM32F746G_Discovery_CubeMX_ANN\Src\AW_ANN_ARM_lib';
movefile([filename '.h'], tspath);
movefile([filename '.c'], tspath);

%% Input 
randn('seed',666);
x = randn(NS(1), 10)*10;

PRINT_C_union_hex_data_2D('ANN_INPUT_BUFFER1', 'dat', x, datatype, 'ROWS');      
movefile('ANN_INPUT_BUFFER1.dat', tspath);

%% LOOP
matlab_output = zeros(NS(end), size(x,2)); 
for i = 1 : size(x,2)
    matlab_output(:,i) = annM.compute(x(:,i));
    fprintf('INPUT  %d) [%.18f, %.18f, %.18f, %.18f, %.18f, %.18f]\n', i, x(:,i));
    fprintf('OUTPUT %d) [%.18f, %.18f, %.18f]\n\n', i, matlab_output(:,i));
end
