function result_str = PRINT_C_union_hex_data_1D(filename, fileEx, data, datatype, dataorient)
%% Select data type conversion method
switch datatype
    case 'FLOAT'
        floating_point_to_hex = @typeConv_float_to_hex;
    case 'DOUBLE'
        floating_point_to_hex = @typeConv_double_to_hex;
end

%% Check data orientation (if matrix)
if ismatrix(data) && strcmp(dataorient,'ROWS')
    data = data';    % CMSIS compatible
end

%% Create string from data
data = data(:);  % matrix to vector
result_str = ''; % empsty string

for i = 1 : length(data)-1
    str_tmp = sprintf('{%s}, ',floating_point_to_hex(data(i)));
    result_str = char(strcat({result_str}, {str_tmp}));
end
str_tmp = sprintf('{%s}', floating_point_to_hex(data(end)));
result_str = char(strcat({result_str}, {str_tmp}));
   
%% Save to file
if ~isempty(filename)
fileID = fopen([filename '.' fileEx], 'w');
fprintf(fileID, '%s', result_str); 
fclose(fileID);
end