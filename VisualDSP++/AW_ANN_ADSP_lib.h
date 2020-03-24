#ifndef ANN_LIB_HEADER
#define ANN_LIB_HEADER

#include <math.h>   // exp(x)
#include <string.h> // memcpy

/* DATA TYPE VERSION */
#define DATA_TYPE_FLOAT32  0         // CMSIS compatible
#define DATA_TYPE_DOUBLE64 1
#define DATA_TYPE          DATA_TYPE_FLOAT32

#define MATRIX_ORIENTATION_ROWS    0 // CMSIS compatible
#define MATRIX_ORIENTATION_COLUMNS 1 
#define MATRIX_ORIENTATION		MATRIX_ORIENTATION_ROWS

/* NEURAL NETWORK COMPUTING CONFIGURATION */
#define ANN_COMPUTE_CONFIG_SOFTWARE     0
#define ANN_COMPUTE_CONFIG_ARM_CMSIS    1
#define ANN_COMPUTE_CONFIG_DSP_HARDWARE 2

#define ANN_COMPUTE_CONFIG              ANN_COMPUTE_CONFIG_DSP_HARDWARE

#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_SOFTWARE)
	#include <stdint.h>
#elif (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_ARM_CMSIS)
	#include "arm_math.h"
#endif

/* DEFINES */

#if (MATRIX_ORIENTATION == MATRIX_ORIENTATION_ROWS)
#define MATRIX_GET_ELEMENT(M,I,J)   M.pData[J+M.numCols*I]
#define MATRIX_GET_ELEMENT_PTR(M,I,J)   M->pData[J+M->numCols*I]
#elif
#define MATRIX_GET_ELEMENT(M,I,J)   M.pData[I+M.numRows*J]
#define MATRIX_GET_ELEMENT_PTR(M,I,J)   M->pData[I+M->numRows*J]
#endif

#define LAYER_CONSTRUCTOR(L)    { L##_ROWS, L##_COLS, L##_W }, \
                                { L##_ROWS, 1, L##_B }, \
                                { L##_ROWS, 1, L##_Z }, \
							    { L##_ROWS, 1, L##_DZ }, \
                                { L##_ROWS, 1, L##_A }, \
                                { 1, L##_ROWS, L##_AT }, \
                                { L##_ROWS, 1, L##_D }, \
								{ L##_COLS, L##_ROWS, L##_WT }, \
								{ L##_COLS, 1, L##_WTxD }, \
								{ L##_ROWS, L##_COLS, L##_DW }, \
                                { L##_ROWS, 1, L##_DB } 

#define TANSIG_LAYER(L)	 { LAYER_CONSTRUCTOR(L), tansig,  dtansig  }
#define LOGSIG_LAYER(L)	 { LAYER_CONSTRUCTOR(L), logsig,  dlogsig  }
#define PURELIN_LAYER(L) { LAYER_CONSTRUCTOR(L), purelin, dpurelin }

/* DATA TYPES Begin */
typedef union {
	uint32_t h;
	float fp;
} hex_to_float;

typedef union {
	uint64_t h;
	double fp;
} hex_to_double;

typedef struct
{
	uint32_t numRows;
	uint32_t numCols;
	float* 	pData;
} matrix_instance_f32;

typedef struct
{
	uint32_t numRows;
	uint32_t numCols;
	double*  pData;
} matrix_instance_d64;

// MATRIX TYPE
#if (DATA_TYPE == DATA_TYPE_FLOAT32)
	#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_SOFTWARE)
	typedef float				   scalar_type; // probably unnecessary in hardware implementation
	typedef matrix_instance_f32    mat_type;
	typedef hex_to_float           hex_to_fp;
	#elif (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_ARM_CMSIS)
	typedef float32_t			       scalar_type; // probably unnecessary in hardware implementation
	typedef arm_matrix_instance_f32    mat_type;
	typedef hex_to_float               hex_to_fp;
	#endif
#elif (DATA_TYPE == DATA_TYPE_DOUBLE64) && (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_SOFTWARE)
	typedef double				   scalar_type; // probably unnecessary in hardware implementation
	typedef matrix_instance_d64    mat_type;
	typedef hex_to_double          hex_to_fp;
#endif

typedef void(*activation_func)(const mat_type*, mat_type*);
typedef void(*activation_func_derivative)(const mat_type*, mat_type*);

/* (!) -> can by replaced by global dump array (or two) */
typedef struct
{
	mat_type w;		 // weigths matrix 
	mat_type b;      // bias vector
	mat_type z;      // (!) linear output vector
	mat_type dz;	 // (!) activation function derivative output
	mat_type a;      // (!) activation function output vector
	mat_type aT;     // (!) activation function output transposed vector
	mat_type d;      // (!) error vector
	mat_type wT;     // (!) weigths transposed matrix 
	mat_type wTxd;   // (!) weigths transposed matrix multiplied by error vector
	mat_type dw;     // (!) weigths matrix change
	mat_type db;     // (!) bias vector change
	activation_func f;			    // activation function
	activation_func_derivative df;  // activation function derivative
} ann_layer_type;

typedef struct
{
	uint32_t numLayers;			     // number of layers
	uint32_t inputSize;              // input size
	uint32_t outputSize;             // output size
	ann_layer_type* pLayers; // layers array
} ann_type;

/* FUNCTIONS Begin */

// Activation functions 
scalar_type tansig_s(scalar_type);
void tansig(const mat_type*, mat_type*);
scalar_type logsig_s(scalar_type);
void logsig(const mat_type*, mat_type*);
scalar_type purelin_s(scalar_type);
void purelin(const mat_type*, mat_type*);

// Activation functions derivative
scalar_type dtansig_s(scalar_type);
void dtansig(const mat_type*, mat_type*);
scalar_type dlogsig_s(scalar_type);
void dlogsig(const mat_type*, mat_type*);
scalar_type dpurelin_s(scalar_type);
void dpurelin(const mat_type*, mat_type*);

// Matrix/vector operations
#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_SOFTWARE)

void matrix_multiplication_soft(const mat_type*, const mat_type*, mat_type*);
void matrix_addition_soft(const mat_type*, const mat_type*, mat_type*);
void matrix_scale_soft(const mat_type*, const scalar_type, mat_type*);
void matrix_transpose_soft(const mat_type*, mat_type*);

#define matrix_multiplication matrix_multiplication_soft
#define matrix_addition       matrix_addition_soft
#define matrix_scale          matrix_scale_soft
#define matrix_transpose      matrix_transpose_soft

void vector_multiplication_soft(const scalar_type*, const scalar_type*, scalar_type*, uint32_t);
void vector_addition_soft(const scalar_type*, const scalar_type*, scalar_type*, uint32_t);
void vector_scale_soft(const scalar_type*, const scalar_type, scalar_type*, uint32_t);
void vector_transpose_soft(const mat_type*, mat_type*);

#define vector_multiplication vector_multiplication_soft
#define vector_addition       vector_addition_soft
#define vector_scale          vector_scale_soft
#define vector_transpose      vector_transpose_soft

#elif (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_ARM_CMSIS) && (DATA_TYPE == DATA_TYPE_FLOAT32)

#define matrix_multiplication arm_mat_mult_f32
#define matrix_addition       arm_mat_add_f32
#define matrix_scale          arm_mat_scale_f32
#define matrix_transpose      arm_mat_trans_f32

void vector_transpose_soft(const mat_type*, mat_type*);

#define vector_multiplication arm_mult_f32
#define vector_addition       arm_add_f32
#define vector_scale          arm_scale_f32
#define vector_transpose      vector_transpose_soft

#elif (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE) && (DATA_TYPE == DATA_TYPE_FLOAT32)

void vector_multiplication_soft(const scalar_type*, const scalar_type*, scalar_type*, uint32_t);
void vector_transpose_soft(const mat_type*, mat_type*);

#define vector_multiplication vector_multiplication_soft
#define vector_transpose      vector_transpose_soft

#endif

// Neural network algorithm
//void ann_compute(const ann_type* pAnn, const mat_type* pInput);
//void ann_compute_output(const ann_type* pAnn, const mat_type* pInput, const mat_type* pOutput);
void ann_compute(ann_type* pAnn, mat_type* pInput);
void ann_compute_output(ann_type* pAnn, mat_type* pInput, mat_type* pOutput);

void ann_backpropagation(ann_type* pAnn, mat_type* pInput, mat_type* pError, scalar_type eta);

// Printing
//void print_matrix(mat_type*);
//void print_layer(ann_layer_type*);

/* FUNCTIONS End */

#endif
