function result_str = PRINT_C_union_hex_array_1D(filename, fileEx, arrayname, arraytype, arraysize, data, datatype, dataorient)
%% Create string
result_str = '';
str_data_tmp = PRINT_C_union_hex_data_1D([],[], data, datatype, dataorient);
str_array_tmp = sprintf('%s %s[%s] = {%s};',arraytype, arrayname, arraysize, str_data_tmp);
result_str = char(strcat({result_str}, {str_array_tmp}));

%% Save to file
if ~isempty(filename)
fileID = fopen([filename '.' fileEx], 'w');
fprintf(fileID, '%s', result_str); 
fclose(fileID);
end