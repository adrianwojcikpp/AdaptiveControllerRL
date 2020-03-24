function hex_str = typeConv_double_to_hex(d_val)

d = double(d_val);
byte_array = typecast(d, 'uint64');
hex_str = int2hex(byte_array);
hex_str = hex_str(:)';
hex_str = ['0x' hex_str];
