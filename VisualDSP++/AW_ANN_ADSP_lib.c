#include "AW_ANN_ADSP_lib.h"

/* Activation function BEGIN */
scalar_type tansig_s(scalar_type z)
{
#if (DATA_TYPE == DATA_TYPE_FLOAT32)
	return (2.0f / (1.0f + expf(-2.0f * z)) - 1.0f);
#elif (DATA_TYPE == DATA_TYPE_DOUBLE64)
	return (2.0 / (1.0 + exp(-2.0 * z)) - 1.0);
#endif
}

void tansig(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = tansig_s(pInput->pData[i]);
}

scalar_type logsig_s(scalar_type z)
{
#if (DATA_TYPE == DATA_TYPE_FLOAT32)
	return (1.0f / (1.0f + expf(-z)));
#elif (DATA_TYPE == DATA_TYPE_DOUBLE64)
	return (1.0 / (1.0 + exp(-z)));
#endif
}

void logsig(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = logsig_s(pInput->pData[i]);
}

scalar_type purelin_s(scalar_type z)
{
	return z;
}

void purelin(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = purelin_s(pInput->pData[i]);
}
/* Activation function END */

/* Activation function derivative BEGIN */
scalar_type dtansig_s(scalar_type z)
{
	return (1.0 - tansig_s(z)*tansig_s(z));
}

void dtansig(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = dtansig_s(pInput->pData[i]);
}

scalar_type dlogsig_s(scalar_type z)
{
	return logsig_s(z)*(1.0 - logsig_s(z));
}

void dlogsig(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = dlogsig_s(pInput->pData[i]);
}

scalar_type dpurelin_s(scalar_type z)
{
	return 1;
}
void dpurelin(const mat_type* pInput, mat_type* pOutput)
{
	uint32_t i;
	for (i = 0; i < (pInput->numRows*pInput->numCols); i++)
		pOutput->pData[i] = dpurelin_s(pInput->pData[i]);
}
/* Activation function derivative END */

/* Matrix/vector operations BEGIN */
#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_SOFTWARE)
void matrix_multiplication_soft(const mat_type *pSrcA, const mat_type *pSrcB, mat_type *pDst)
{
	uint32_t i, j, k;
	// TODO: size validation
	for (i = 0; i < pSrcA->numRows; i++)
	{
		for (j = 0; j < pSrcB->numCols; j++)
		{
			MATRIX_GET_ELEMENT_PTR(pDst, i, j) = 0;
			for (k = 0; k < pSrcA->numCols; k++)
			{
				MATRIX_GET_ELEMENT_PTR(pDst, i, j) += MATRIX_GET_ELEMENT_PTR(pSrcA, i, k) * MATRIX_GET_ELEMENT_PTR(pSrcB, k, j);
			}
		}
	}
}

void matrix_addition_soft(const mat_type *pSrcA, const mat_type *pSrcB, mat_type *pDst)
{
	uint32_t i, j;
	// TODO: size validation
	for (i = 0; i < pSrcA->numRows; i++)
	{
		for (j = 0; j < pSrcA->numCols; j++)
		{
			MATRIX_GET_ELEMENT_PTR(pDst, i, j) = MATRIX_GET_ELEMENT_PTR(pSrcA, i, j) + MATRIX_GET_ELEMENT_PTR(pSrcB, i, j);
		}
	}
}

void matrix_scale_soft(const mat_type *pSrc, const scalar_type s, mat_type *pDst)
{
	uint32_t i, j;
	// TODO: size validation
	for (i = 0; i < pSrc->numRows; i++)
	{
		for (j = 0; j < pSrc->numCols; j++)
		{
			MATRIX_GET_ELEMENT_PTR(pDst, i, j) = MATRIX_GET_ELEMENT_PTR(pSrc, i, j) * s;
		}
	}
}

void matrix_transpose_soft(const mat_type *pSrc, mat_type *pDst)
{
	uint32_t i, j;
	// TODO: size validation
	for (i = 0; i < pSrc->numRows; i++)
	{
		for (j = 0; j < pSrc->numCols; j++)
		{
			MATRIX_GET_ELEMENT_PTR(pDst, j, i) = MATRIX_GET_ELEMENT_PTR(pSrc, i, j);
		}
	}
}

void vector_addition_soft(const scalar_type *pSrcA, const scalar_type *pSrcB, scalar_type *pDst, uint32_t blocksize)
{
	uint32_t i;
	// TODO: size validation
	for (i = 0; i < blocksize; i++)
	{
		pDst[i] = pSrcA[i]+pSrcB[i];
	}
}

void vector_multiplication_soft(const scalar_type *pSrcA, const scalar_type *pSrcB, scalar_type *pDst, uint32_t blocksize)
{
	uint32_t i;
	// TODO: size validation
	for (i = 0; i < blocksize; i++)
	{
		pDst[i] = pSrcA[i]*pSrcB[i];
	}
}

void vector_scale_soft(const scalar_type *pSrc, const scalar_type scale, scalar_type *pDst, uint32_t blocksize)
{
	uint32_t i;
	// TODO: size validation
	for (i = 0; i < blocksize; i++)
	{
		pDst[i] = pSrc[i]*scale;
	}
}
#endif
void vector_transpose_soft(const mat_type *pSrc, mat_type *pDst)
{
	if(pSrc->pData != pDst->pData)
		memcpy(pDst->pData, pSrc->pData, pSrc->numRows*sizeof(scalar_type));

	pDst->numRows = pSrc->numCols;
	pDst->numCols = pSrc->numRows;
}

/* Matrix/vector operations END */

/* Neural network algorithm BEGIN */
#define PRINT_COMPUTE 0

void ann_compute(ann_type* pAnn, mat_type* pInput)
{
	uint16_t L;

	// z(0) = w(0)*Input
	#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
	matmmltf(pAnn->pLayers[0].z.pData, 
			 pAnn->pLayers[0].w.pData, 
			 pInput->pData, 
			 pAnn->pLayers[0].w.numRows,
			 pAnn->pLayers[0].w.numCols,
			 pInput->numCols);
	#else
	matrix_multiplication(&(pAnn->pLayers[0].w), 
						  pInput, 
						  &(pAnn->pLayers[0].z));
	#endif

	// z(0) = b(0)+z(0)
	vector_addition(pAnn->pLayers[0].b.pData,
					pAnn->pLayers[0].z.pData, 
					pAnn->pLayers[0].z.pData, 
					pAnn->pLayers[0].b.numRows);
	
	// a(0) = f0(z(0))
	pAnn->pLayers[0].f(&(pAnn->pLayers[0].z), &(pAnn->pLayers[0].a));

	for (L = 1; L < pAnn->numLayers; L++)
	{
		// z(l) = w(l)*a(l-1)
		#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
		matmmltf(pAnn->pLayers[L].z.pData,
				 pAnn->pLayers[L].w.pData,
				 pAnn->pLayers[L-1].a.pData,
				 pAnn->pLayers[L].w.numRows,
				 pAnn->pLayers[L].w.numCols,
				 pAnn->pLayers[L-1].a.numCols); 
		#else
		matrix_multiplication(&(pAnn->pLayers[L].w),
							  &(pAnn->pLayers[L-1].a),
							  &(pAnn->pLayers[L].z));
		#endif
		
		// z(l) = b(l)+z(l)
		vector_addition(pAnn->pLayers[L].b.pData, 
						pAnn->pLayers[L].z.pData,
						pAnn->pLayers[L].z.pData,
						pAnn->pLayers[L].b.numRows);

		// a(l) = fl(z(l))
		pAnn->pLayers[L].f(&(pAnn->pLayers[L].z), &(pAnn->pLayers[L].a));
	}
}

void ann_compute_output(ann_type* pAnn, mat_type* pInput, mat_type* pOutput)
{
	int i;
	ann_compute(pAnn, pInput);
	memcpy(pOutput->pData, pAnn->pLayers[pAnn->numLayers-1].a.pData, sizeof(scalar_type)*(pAnn->outputSize));
}

#define PRINT_BACKPROP 0

void ann_backpropagation(ann_type* pAnn, mat_type* pInput, mat_type* pError, scalar_type eta)
{
	// local temp
	int16_t L, Lmax;
	mat_type InputT; 
	
	InputT.pData = pInput->pData;
	//vector_transpose(pInput, &InputT);
	InputT.numRows = pInput->numCols;
	InputT.numCols = pInput->numRows;

/* input forward propagation */
	ann_compute(pAnn, pInput);

/* error backpropagation  */
	Lmax = (pAnn->numLayers - 1); // last layer index

	// dz(Lmax) = dfL(z(Lmax))
	pAnn->pLayers[Lmax].df(&(pAnn->pLayers[Lmax].z), &(pAnn->pLayers[Lmax].dz));

	// d(Lmax) = E .* dz(Lmax)
	vector_multiplication(pError->pData, 
						  pAnn->pLayers[Lmax].dz.pData,
						  pAnn->pLayers[Lmax].d.pData, 
						  pError->numRows); 

	for (L = Lmax-1; L >= 0; L--)
	{
		/* d(L) = (w(L+1)' * d(L+1)) .* dfL(z(L) */

		// wT(L+1) = transpose(w(L+1))
		matrix_transpose(&(pAnn->pLayers[L + 1].w), &(pAnn->pLayers[L + 1].wT));

		// dz(L) = dfL(z(L))
        pAnn->pLayers[L].df(&(pAnn->pLayers[L].z), &(pAnn->pLayers[L].dz));

		// wTxd(L+1) = wT(L+1) * d(L+1)
		#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
		matmmltf(pAnn->pLayers[L + 1].wTxd.pData, 
				 pAnn->pLayers[L + 1].wT.pData, 
				 pAnn->pLayers[L + 1].d.pData, 
				 pAnn->pLayers[L + 1].wT.numRows, 
				 pAnn->pLayers[L + 1].wT.numCols, 
				 pAnn->pLayers[L + 1].d.numCols);
		#else
		matrix_multiplication(&(pAnn->pLayers[L + 1].wT), 
							  &(pAnn->pLayers[L + 1].d), 
							  &(pAnn->pLayers[L + 1].wTxd));
		#endif
		 

		// d(L) = wTxd(L+1) .* dz(L)
		vector_multiplication(pAnn->pLayers[L + 1].wTxd.pData, 
							  pAnn->pLayers[L].dz.pData,
							  pAnn->pLayers[L].d.pData,  
							  pAnn->pLayers[L + 1].wTxd.numRows); 
	}

/* weigths update */

	// dw(0) = d(0) * xT
	#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
	matmmltf(pAnn->pLayers[0].dw.pData, 
			 pAnn->pLayers[0].d.pData,
			 InputT.pData,
			 pAnn->pLayers[0].d.numRows,
			 pAnn->pLayers[0].d.numCols,
			 InputT.numCols); 
	#else
	matrix_multiplication(&(pAnn->pLayers[0].d), 
						  &InputT, 
						  &(pAnn->pLayers[0].dw));
	#endif
	
	// dw(0) = eta * dw(0)
	#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
	matsmltf(pAnn->pLayers[0].dw.pData, 
			 pAnn->pLayers[0].dw.pData, 
			 eta, 
			 pAnn->pLayers[0].dw.numRows,
			 pAnn->pLayers[0].dw.numCols);
	#else
	matrix_scale(&(pAnn->pLayers[0].dw), 
				 eta,
				 &(pAnn->pLayers[0].dw));
	#endif
	
	// w(0) = w(0) + dw(0)
	#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
	matmaddf(pAnn->pLayers[0].w.pData, 
			 pAnn->pLayers[0].w.pData,
			 pAnn->pLayers[0].dw.pData,
			 pAnn->pLayers[0].w.numRows,
			 pAnn->pLayers[0].w.numCols);
	#else
	matrix_addition(&(pAnn->pLayers[0].w), 
					&(pAnn->pLayers[0].dw), 
					&(pAnn->pLayers[0].w));
	#endif
	
	for (L = 1; L < pAnn->numLayers; L++)
	{
		// aT(L-1) = transpose(a(L-1))
		vector_transpose(&(pAnn->pLayers[L - 1].a), &(pAnn->pLayers[L - 1].aT));

		// dw(L) = d(L) * aT(L-1)
		#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
		matmmltf(pAnn->pLayers[L].dw.pData,
				 pAnn->pLayers[L].d.pData,
				 pAnn->pLayers[L - 1].aT.pData,
				 pAnn->pLayers[L].d.numRows,
				 pAnn->pLayers[L].d.numCols,
				 pAnn->pLayers[L - 1].aT.numCols); 
		#else
		matrix_multiplication(&(pAnn->pLayers[L].d),
							  &(pAnn->pLayers[L - 1].aT),
							  &(pAnn->pLayers[L].dw));
		#endif
		
		// dw(L) = eta * dw(L)
		#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
		matsmltf(pAnn->pLayers[L].dw.pData, 
				 pAnn->pLayers[L].dw.pData, 
				 eta,
				 pAnn->pLayers[L].dw.numRows,
				 pAnn->pLayers[L].dw.numCols);
		#else
		matrix_scale(&(pAnn->pLayers[L].dw),
					 eta,
					 &(pAnn->pLayers[L].dw));
		#endif
	
		// w(L) = w(L) + dw(L)
		#if (ANN_COMPUTE_CONFIG == ANN_COMPUTE_CONFIG_DSP_HARDWARE)
		matmaddf(pAnn->pLayers[L].w.pData,
				 pAnn->pLayers[L].w.pData,
				 pAnn->pLayers[L].dw.pData,
				 pAnn->pLayers[L].w.numRows,
				 pAnn->pLayers[L].w.numCols);
		#else
		matrix_addition(&(pAnn->pLayers[L].w), 
						&(pAnn->pLayers[L].dw), 
						&(pAnn->pLayers[L].w));
		#endif
	
	}

/* biases update */

	for (L = 0; L < pAnn->numLayers; L++)
	{
		// db(L) = eta * d(L)
		vector_scale(pAnn->pLayers[L].d.pData, 
					 eta, 
					 pAnn->pLayers[L].db.pData,
					 pAnn->pLayers[L].d.numRows);

		// b(L) = b(L) + db(L)
		vector_addition(pAnn->pLayers[L].b.pData,
						pAnn->pLayers[L].db.pData,
						pAnn->pLayers[L].b.pData,
						pAnn->pLayers[L].b.numRows);
	}
	
}

/* Neural network algorithm END */

/* Printing BEGIN */
//void print_matrix(mat_type* mat)
//{
//	scalar_type value;
//	for (int i = 0; i < mat->numRows; i++)
//	{
//		for (int j = 0; j < mat->numCols; j++)
//		{
//			value = MATRIX_GET_ELEMENT_PTR(mat, i, j);
//			printf("%.15f, ", value);
//		}
//		printf("\n");
//	}
//	printf("\n");
//}
//
//void print_layer(ann_layer_type* layer)
//{
//	scalar_type value;
//	for (int i = 0; i < layer->w.numRows; i++)
//	{
//		for (int j = 0; j < layer->w.numCols; j++)
//		{
//			value = MATRIX_GET_ELEMENT(layer->w, i, j);
//			printf("%.15f, ", value);
//		}
//		printf("\n");
//	}
//	printf("\n");
//}
/* Printing END */
