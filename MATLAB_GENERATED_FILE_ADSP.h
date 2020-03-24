// MATLAB GENERATED FILE. DO NOT CHANGE.
// DATA TYPE: FLOAT 
// DATA ORIENTATION: ROWS 
#ifndef ann_dsp_HEADER_FILE
#define ann_dsp_HEADER_FILE

#include "AW_ANN_ADSP_lib.h" // Artificial neural network library 

#define ann_dsp_LAYERS_NO	2

#define ann_dsp_INPUT_SIZE	6
#define ann_dsp_OUTPUT_SIZE	2

// INPUT
extern hex_to_fp ann_dsp_INPUT_union[ann_dsp_INPUT_SIZE];
#define ann_dsp_INPUT_INIT   &(ann_dsp_INPUT_union[0].fp)

// OUTPUT
extern hex_to_fp ann_dsp_OUTPUT_union[ann_dsp_OUTPUT_SIZE];
#define ann_dsp_OUTPUT_INIT   &(ann_dsp_OUTPUT_union[0].fp)

// LAYER 1
#define ann_dsp_L1_ROWS	5
#define ann_dsp_L1_COLS	6
#define ann_dsp_L1_AF	1
extern hex_to_fp ann_dsp_W1_union[ann_dsp_L1_ROWS*ann_dsp_L1_COLS];
#define ann_dsp_L1_W   &(ann_dsp_W1_union[0].fp)
extern hex_to_fp ann_dsp_B1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_B   &(ann_dsp_B1_union[0].fp)
extern hex_to_fp ann_dsp_Z1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_Z   &(ann_dsp_Z1_union[0].fp)
extern hex_to_fp ann_dsp_DZ1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_DZ   &(ann_dsp_DZ1_union[0].fp)
extern hex_to_fp ann_dsp_A1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_A   &(ann_dsp_A1_union[0].fp)
extern hex_to_fp ann_dsp_AT1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_AT   &(ann_dsp_AT1_union[0].fp)
extern hex_to_fp ann_dsp_D1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_D   &(ann_dsp_D1_union[0].fp)
extern hex_to_fp ann_dsp_WT1_union[ann_dsp_L1_COLS*ann_dsp_L1_ROWS];
#define ann_dsp_L1_WT   &(ann_dsp_WT1_union[0].fp)
extern hex_to_fp ann_dsp_WTxD1_union[ann_dsp_L1_COLS];
#define ann_dsp_L1_WTxD   &(ann_dsp_WTxD1_union[0].fp)
extern hex_to_fp ann_dsp_DW1_union[ann_dsp_L1_ROWS*ann_dsp_L1_COLS];
#define ann_dsp_L1_DW   &(ann_dsp_DW1_union[0].fp)
extern hex_to_fp ann_dsp_DB1_union[ann_dsp_L1_ROWS];
#define ann_dsp_L1_DB   &(ann_dsp_DB1_union[0].fp)

// LAYER 2
#define ann_dsp_L2_ROWS	2
#define ann_dsp_L2_COLS	5
#define ann_dsp_L2_AF	1
extern hex_to_fp ann_dsp_W2_union[ann_dsp_L2_ROWS*ann_dsp_L2_COLS];
#define ann_dsp_L2_W   &(ann_dsp_W2_union[0].fp)
extern hex_to_fp ann_dsp_B2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_B   &(ann_dsp_B2_union[0].fp)
extern hex_to_fp ann_dsp_Z2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_Z   &(ann_dsp_Z2_union[0].fp)
extern hex_to_fp ann_dsp_DZ2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_DZ   &(ann_dsp_DZ2_union[0].fp)
extern hex_to_fp ann_dsp_A2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_A   &(ann_dsp_A2_union[0].fp)
extern hex_to_fp ann_dsp_AT2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_AT   &(ann_dsp_AT2_union[0].fp)
extern hex_to_fp ann_dsp_D2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_D   &(ann_dsp_D2_union[0].fp)
extern hex_to_fp ann_dsp_WT2_union[ann_dsp_L2_COLS*ann_dsp_L2_ROWS];
#define ann_dsp_L2_WT   &(ann_dsp_WT2_union[0].fp)
extern hex_to_fp ann_dsp_WTxD2_union[ann_dsp_L2_COLS];
#define ann_dsp_L2_WTxD   &(ann_dsp_WTxD2_union[0].fp)
extern hex_to_fp ann_dsp_DW2_union[ann_dsp_L2_ROWS*ann_dsp_L2_COLS];
#define ann_dsp_L2_DW   &(ann_dsp_DW2_union[0].fp)
extern hex_to_fp ann_dsp_DB2_union[ann_dsp_L2_ROWS];
#define ann_dsp_L2_DB   &(ann_dsp_DB2_union[0].fp)

// NETWORK 
extern mat_type ann_dsp_INPUT;
extern mat_type ann_dsp_OUTPUT;

extern ann_layer_type ann_dsp_LAYERS[ann_dsp_LAYERS_NO];
 extern ann_type ann_dsp;

#endif
