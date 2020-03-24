function result_str = PRINT_C_union_hex_data_2D(filename, fileEx, data, datatype, dataorient)
%% Select data type conversion method
switch datatype
    case 'FLOAT'
        floating_point_to_hex = @typeConv_float_to_hex;
    case 'DOUBLE'
        floating_point_to_hex = @typeConv_double_to_hex;
end

%% Check data orientation (if matrix)
if strcmp(dataorient,'ROWS')
    data = data';    % CMSIS compatible
end

%% Create string from data
result_str = ''; % empsty string

R = size(data, 1);
C = size(data, 2);

for i = 1 : R
    result_str = char(strcat({result_str}, {'{'}));
    for j = 1 : C-1
        str_tmp = sprintf('{%s}, ',floating_point_to_hex(data(i,j)));
        result_str = char(strcat({result_str}, {str_tmp}));
    end
    if i < R
        str_tmp = sprintf('{%s}},\n', floating_point_to_hex(data(i,end)));
    else
        str_tmp = sprintf('{%s}}', floating_point_to_hex(data(i,end)));
    end
    result_str = char(strcat({result_str}, {str_tmp}));
end
   
%% Save to file
if ~isempty(filename)
fileID = fopen([filename '.' fileEx], 'w');
fprintf(fileID, '%s', result_str); 
fclose(fileID);
end