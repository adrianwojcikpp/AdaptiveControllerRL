function PRINT_C_ann_hex(ann, networkname, libname, datatype, filename, fileEx, dataorient, input)
%% Create .h file
fileID = fopen([filename '.h'],'w');

typename = 'hex_to_fp'; % custom union for uint/floating point conversion
getUnionPointer = @(name)(['&(' name '[0].fp)']);

header = sprintf('%s_HEADER_FILE', networkname);
fprintf(fileID,'// MATLAB GENERATED FILE. DO NOT CHANGE.\n');
fprintf(fileID,'// DATA TYPE: %s \n', datatype);
fprintf(fileID,'// DATA ORIENTATION: %s \n', dataorient);
fprintf(fileID,'#ifndef %s\n', header);
fprintf(fileID,'#define %s\n\n', header);

fprintf(fileID,'#include "%s" // Artificial neural network library \n\n', [libname '.h']);

% basic network data
str_layer_no = sprintf('%s_LAYERS_NO', networkname);
fprintf(fileID,'#define %s	%d\n\n', str_layer_no, ann.L);
str_input_size = sprintf('%s_INPUT_SIZE', networkname);
fprintf(fileID,'#define %s	%d\n', str_input_size, ann.NS(1));
str_output_size = sprintf('%s_OUTPUT_SIZE', networkname);
fprintf(fileID,'#define %s	%d\n\n', str_output_size, ann.NS(end));

% input vector
fprintf(fileID,'// INPUT\n');
str_array_input = sprintf('%s',[networkname '_INPUT' '_union']);
fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_input, str_input_size);
fprintf(fileID,'#define %s_INPUT_INIT   %s\n\n', networkname, getUnionPointer(str_array_input));

% output vector
fprintf(fileID,'// OUTPUT\n');
str_array_output = sprintf('%s',[networkname '_OUTPUT' '_union']);
fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_output, str_output_size);
fprintf(fileID,'#define %s_OUTPUT_INIT   %s\n', networkname, getUnionPointer(str_array_output));

fprintf(fileID,'\n');
    
str_rows = cell(ann.L,1);                     % rows number
str_cols = cell(ann.L,1);                     % columns number
str_array_weights = cell(ann.L,1);            % weigths matrix 
str_array_biases = cell(ann.L,1);             % bias vector
str_array_linout = cell(ann.L,1);             % linear output vector
str_array_afdout = cell(ann.L,1);             % activation function derivative output
str_array_netout = cell(ann.L,1);             % activation function output vector
str_array_netout_transp = cell(ann.L,1);      % activation function output transposed vector
str_array_error  = cell(ann.L,1);             % error vector
str_array_weights_transp = cell(ann.L,1);     % weigths matrix transposed  
str_array_weights_times_error = cell(ann.L,1);% weigths matrix transposed multiplied by error vector
str_array_weights_change = cell(ann.L,1);     % weigths matrix change
str_array_biases_change = cell(ann.L,1);      % bias vector change

for i = 1 : ann.L
    fprintf(fileID,'// LAYER %d\n',i);
    
    % rows number definition
    str_rows{i} = sprintf('%s_L%d_ROWS', networkname, i);
    fprintf(fileID,'#define %s	%d\n',str_rows{i}, ann.NS(i+1));
    
    % cols number definition
    str_cols{i} = sprintf('%s_L%d_COLS', networkname, i);
    fprintf(fileID,'#define %s	%d\n',str_cols{i}, ann.NS(i));
    
    % activation function number definition
    fprintf(fileID,'#define %s_L%d_AF	%d\n', networkname, i, ann.af_index(ann.f{i}));
    
    % weigths matrix 
    str_array_weights{i} = sprintf('%s', [networkname '_W' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_weights{i}, [str_rows{i} '*' str_cols{i}]);
    fprintf(fileID,'#define %s_L%d_W   %s\n', networkname, i, getUnionPointer(str_array_weights{i}));
    
    % bias vector
    str_array_biases{i} = sprintf('%s', [networkname '_B' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_biases{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_B   %s\n', networkname, i, getUnionPointer(str_array_biases{i}));
    
    % linear output vector
    str_array_linout{i} = sprintf('%s', [networkname '_Z' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_linout{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_Z   %s\n', networkname, i, getUnionPointer(str_array_linout{i}));
    
    % activation function derivative output
    str_array_afdout{i} = sprintf('%s', [networkname '_DZ' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_afdout{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_DZ   %s\n', networkname, i, getUnionPointer(str_array_afdout{i}));
            
    % activation function output vector
    str_array_netout{i} = sprintf('%s', [networkname '_A' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_netout{i},str_rows{i});
    fprintf(fileID,'#define %s_L%d_A   %s\n', networkname, i, getUnionPointer(str_array_netout{i}));
    
     % activation function output transposed vector
    str_array_netout_transp{i} = sprintf('%s',[networkname '_AT' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_netout_transp{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_AT   %s\n', networkname, i, getUnionPointer(str_array_netout_transp{i}));
    
    % output error vector
    str_array_error{i} = sprintf('%s',[networkname '_D' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_error{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_D   %s\n', networkname, i, getUnionPointer(str_array_error{i}));
    
    % weigths transposed matrix 
    str_array_weights_transp{i} = sprintf('%s',[networkname '_WT' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_weights_transp{i}, [str_cols{i} '*' str_rows{i}]);
    fprintf(fileID,'#define %s_L%d_WT   %s\n', networkname, i, getUnionPointer(str_array_weights_transp{i}));
    
    % weigths transposed matrix multiplied by error vector
    str_array_weights_times_error{i} = sprintf('%s',[networkname '_WTxD' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_weights_times_error{i},str_cols{i});
    fprintf(fileID,'#define %s_L%d_WTxD   %s\n', networkname, i, getUnionPointer(str_array_weights_times_error{i}));
    
    % weigths matrix change
    str_array_weights_change{i} = sprintf('%s',[networkname '_DW' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_weights_change{i}, [str_rows{i} '*' str_cols{i}]);
    fprintf(fileID,'#define %s_L%d_DW   %s\n', networkname, i, getUnionPointer(str_array_weights_change{i}));
    
    % bias vector change
    str_array_biases_change{i} = sprintf('%s',[networkname '_DB' num2str(i) '_union']);
    fprintf(fileID,'extern %s %s[%s];\n', typename, str_array_biases_change{i}, str_rows{i});
    fprintf(fileID,'#define %s_L%d_DB   %s\n', networkname, i, getUnionPointer(str_array_biases_change{i}));
    
    fprintf(fileID,'\n');
end

% input, output & network declaration
fprintf(fileID,'// NETWORK \n');
fprintf(fileID,'extern mat_type %s_INPUT;\n', networkname);
fprintf(fileID,'extern mat_type %s_OUTPUT;\n\n', networkname);

fprintf(fileID,'extern ann_layer_type %s_LAYERS[%s];\n ', networkname, str_layer_no);
fprintf(fileID,'extern ann_type %s;\n', networkname);

fprintf(fileID,'\n#endif\n');
fclose(fileID);

%% Create .c/.cpp file
fileID = fopen([filename '.' fileEx],'w');
fprintf(fileID,'// MATLAB GENERATED FILE. DO NOT CHANGE.\n');
fprintf(fileID,'// DATA TYPE: %s \n', datatype);
fprintf(fileID,'// DATA ORIENTATION: %s \n', dataorient);
fprintf(fileID,'#include "%s"\n\n',[filename '.h']);

% input vector
fprintf(fileID,'// INPUT\n');
str_input_tmp = PRINT_C_union_hex_array_1D([],[], str_array_input, typename, str_input_size, input, datatype, dataorient);
fprintf(fileID,'%s\n\n', str_input_tmp);

% output vector
fprintf(fileID,'// OUTPUT\n');
out_init = zeros(ann.O, 1);
str_output_tmp = PRINT_C_union_hex_array_1D([],[], str_array_output, typename, str_output_size, out_init, datatype, dataorient);
fprintf(fileID,'%s\n\n', str_output_tmp);

for i = 1 : ann.L
    fprintf(fileID,'// LAYER %d\n',i);
    
    % weigths matrix  
    str_weigths_tmp = PRINT_C_union_hex_array_1D([],[], str_array_weights{i}, typename, [str_rows{i} '*' str_cols{i}], ann.w{i}, datatype, dataorient);
    fprintf(fileID,'%s\n', str_weigths_tmp);

    % bias vector
    str_biases_tmp = PRINT_C_union_hex_array_1D([],[], str_array_biases{i}, typename, str_rows{i}, ann.b{i}, datatype, dataorient);
    fprintf(fileID,'%s\n', str_biases_tmp);
     
    % linear output vector
    z_init = zeros(ann.NS(i+1), 1);
    str_linout_tmp = PRINT_C_union_hex_array_1D([],[], str_array_linout{i}, typename, str_rows{i}, z_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_linout_tmp);
    
    % activation function derivative output
    dz_init = zeros(ann.NS(i+1), 1);
    str_afdout_tmp = PRINT_C_union_hex_array_1D([],[], str_array_afdout{i}, typename, str_rows{i}, dz_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_afdout_tmp);
    
    % activation function output vector
    a_init = zeros(ann.NS(i+1), 1);
    str_netout_tmp = PRINT_C_union_hex_array_1D([],[], str_array_netout{i}, typename, str_rows{i}, a_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_netout_tmp);
    
    % activation function output transposed vector
    aT_init = zeros(ann.NS(i+1), 1);
    str_netout_transp_tmp = PRINT_C_union_hex_array_1D([],[], str_array_netout_transp{i}, typename, str_rows{i}, aT_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_netout_transp_tmp);
    
    % output error vector
    d_init = zeros(ann.NS(i+1), 1);
    str_error_tmp = PRINT_C_union_hex_array_1D([],[], str_array_error{i}, typename, str_rows{i}, d_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_error_tmp);
    
    % weigths matrix transposed 
    str_weigths_transp_tmp = PRINT_C_union_hex_array_1D([],[], str_array_weights_transp{i}, typename, [str_cols{i} '*' str_rows{i}], ann.w{i}', datatype, dataorient);
    fprintf(fileID,'%s\n', str_weigths_transp_tmp);
    
    % weigths matrix transposed multiplied by error vector
    wtxd_init = zeros(ann.NS(i), 1);
    str_weights_times_error_tmp = PRINT_C_union_hex_array_1D([],[], str_array_weights_times_error{i}, typename, str_cols{i}, wtxd_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_weights_times_error_tmp);
    
    % weigths matrix change
    dw_init = zeros(size(ann.w{i}));
    str_weights_change_tmp = PRINT_C_union_hex_array_1D([],[], str_array_weights_change{i}, typename, [str_rows{i} '*' str_cols{i}], dw_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_weights_change_tmp);

    % bias vector change
    db_init = zeros(size(ann.b{i}));
    str_biases_change_tmp = PRINT_C_union_hex_array_1D([],[], str_array_biases_change{i}, typename, str_rows{i}, db_init, datatype, dataorient);
    fprintf(fileID,'%s\n', str_biases_change_tmp);
   
    fprintf(fileID,'\n');
end

fprintf(fileID,'\n');

% input, output & network definition
fprintf(fileID,'// NETWORK \n');
fprintf(fileID,'mat_type %s_INPUT  = { %s_INPUT_SIZE,  1, %s_INPUT_INIT  };\n', networkname, networkname, networkname);
fprintf(fileID,'mat_type %s_OUTPUT = { %s_OUTPUT_SIZE, 1, %s_OUTPUT_INIT };\n\n', networkname, networkname, networkname);

fprintf(fileID,'ann_layer_type %s_LAYERS[%s] = { ', networkname, str_layer_no);
for k = 1 : ann.L-1
    afname = functions(ann.f{k});
    afname = upper(afname.function);
    fprintf(fileID,'%s_LAYER(%s_L%d), ', afname, networkname, k);
end
afname = functions(ann.f{end});
afname = upper(afname.function);
fprintf(fileID,'%s_LAYER(%s_L%d)};\n', afname, networkname, ann.L);

fprintf(fileID,'ann_type %s = { %s_LAYERS_NO, %s_INPUT_SIZE, %s_OUTPUT_SIZE, %s_LAYERS };\n', ...
        networkname, networkname, networkname, networkname, networkname);

fprintf(fileID,'\n');

fclose(fileID);