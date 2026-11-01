#pragma once
#include <cstddef>
#include <cassert>
#include "tensor.hpp"
#include <cmath>

inline auto gemm_forward(const Tensor& X, const Tensor& W, Tensor& Y) -> void
{
	size_t M = X.shape(0);
	size_t K = X.shape(1);
	size_t N = W.shape(1);
	assert(K == W.shape(0) && M == Y.shape(0) && N == Y.shape(1));
	Y.Zero();
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < K; j++)
		{
			float curr_elem = X(i, j);
			for (size_t k = 0; k < N; k++)
			{
				Y(i, k) += curr_elem * W(j, k);
			}
		}
	}
}

inline auto gemm_backward(const Tensor& X, const Tensor& W, const Tensor& dY, Tensor& dX, Tensor& dW) -> void
{
	size_t M = X.shape(0);
	size_t K = X.shape(1);
	size_t N = W.shape(1);
	assert(W.shape(0) == K && dY.shape(0) == M && dY.shape(1) == N && dX.shape(0) == M && dX.shape(1) == K && dW.shape(0) == K && dW.shape(1) == N);
	dX.Zero();
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			float curr_val = dY(i, j);
			for (size_t k = 0; k < K; k++)
			{
				dX(i, k) += curr_val * W(k, j);
			}
		}
	}

	for (size_t i = 0; i < K; i++)
	{
		for (size_t j = 0; j < M; j++)
		{
			float curr_elem = X(j, i);
			for (size_t k = 0; k < N; k++)
			{
				dW(i, k) += curr_elem * dY(j, k);
			}
		}
	}

}

inline auto rmsnorm_forward(const Tensor& X, const Tensor& gamma, Tensor& Y, Tensor& rstd, float eps = 1e-5f) -> void
{
	size_t M = X.shape(0);
	size_t D = X.shape(1);
	assert(Y.shape(0) == M && Y.shape(1) == D && gamma.shape(0) == D && rstd.shape(0) == M);
	for (size_t i = 0; i < M; i++)
	{
		float sum = 0.0f;
		for (size_t j = 0; j < D; j++){sum += X(i, j) * X(i, j);}
		float inv_rms = 1.0f / sqrt((sum/D) + eps);
		rstd(i) = inv_rms;
		for (size_t j = 0; j < D; j++) {Y(i,j) = X(i,j) * inv_rms * gamma(j);}
	}
}

inline auto rmsnorm_backward(const Tensor& X, const Tensor& gamma, const Tensor& dY, const Tensor& rstd, Tensor& dX, Tensor& dgamma) -> void
{
	size_t M = X.shape(0);
	size_t D = X.shape(1);
	assert(dX.shape(0) == M && dX.shape(1) == D && dY.shape(0) == M && 
		dY.shape(1) == D && dgamma.shape(0) == D && gamma.shape(0) == D && rstd.shape(0) == M);
	dX.Zero();
	for (size_t i = 0; i < M; i++)
	{
		float inv_rms = rstd(i);
		float sum_dx_x = 0;
		float dhat_x = 0;
		for (size_t j = 0; j < D; j++) 
		{
			dgamma(j) += dY(i, j) * X(i, j) * inv_rms;
			sum_dx_x += dY(i, j) * gamma(j) * X(i, j);
		}
		for (size_t j = 0; j < D; j++)
		{
			dhat_x = dY(i, j) * gamma(j);
			dX(i, j) = inv_rms * (dhat_x - ((inv_rms*inv_rms/D) * X(i, j) * sum_dx_x));
		}
	}
}

inline auto rope_forward(Tensor& X, size_t num_heads, size_t pos_offset = 0) -> void
{
	size_t B = X.shape(0);
	size_t T = X.shape(1);
	size_t D = X.shape(2);
	size_t d_k = D / num_heads;
	assert(D % num_heads == 0 && (d_k % 2 == 0));	
	for (size_t b = 0; b < B; b++)
	{
		for (size_t t = 0; t < T; t++)
		{
			size_t pos = t + pos_offset;
			for (size_t h = 0; h < num_heads; h++)
			{
				size_t start_head = h * d_k;
				for (size_t p = 0; p < d_k / 2; p++)
				{
					size_t idx0 = start_head + (2 * p);
					size_t idx1 = start_head + (2 * p) + 1;
					float exponent = (2.0f * p) / static_cast<float>(d_k);
					float frequency = 1.0f / powf(10000.0f, exponent);
					float theta = pos * frequency;
					float cos_theta = cosf(theta);
					float sin_theta = sinf(theta);
					float v0 = X(b,t,idx0);
					float v1 = X(b,t,idx1);
					X(b, t, idx0) = v0 * cos_theta - v1 * sin_theta;
					X(b, t, idx1) = v1 * cos_theta + v0 * sin_theta;

				}
			}
		}
	}
}

inline auto rope_backward(Tensor& dX, size_t num_heads, size_t pos_offset = 0) -> void
{
	size_t B = dX.shape(0);
	size_t T = dX.shape(1);
	size_t D = dX.shape(2);
	size_t d_k = D / num_heads;
	assert(D % num_heads == 0 && (d_k % 2 == 0));
	for (size_t b = 0; b < B; b++)
	{
		for (size_t t = 0; t < T; t++)
		{
			size_t pos = t + pos_offset;
			for (size_t h = 0; h < num_heads; h++)
			{
				size_t start_head = h * d_k;
				for (size_t p = 0; p < d_k / 2; p++)
				{
					size_t idx0 = start_head + (2 * p);
					size_t idx1 = start_head + (2 * p) + 1;
					float exponent = (2.0f * p) / static_cast<float>(d_k);
					float frequency = 1.0f / powf(10000.0f, exponent);
					float theta = pos * frequency;
					float cos_theta = cosf(theta);
					float sin_theta = sinf(theta);
					float v0 = dX(b, t, idx0);
					float v1 = dX(b, t, idx1);
					dX(b, t, idx0) = v0 * cos_theta + v1 * sin_theta;
					dX(b, t, idx1) = v1 * cos_theta - v0 * sin_theta;

				}
			}
		}
	}
}

inline auto attention_forward(const Tensor& Q, const Tensor& K, const Tensor& V, Tensor& attn_probs, Tensor& Out, size_t num_heads) -> void
{
	size_t B = Q.shape(0);
	size_t T = Q.shape(1);
	size_t D = Q.shape(2);
	size_t dk = D / num_heads;
	float csm = 1.0f / sqrt(dk);
	Out.Zero();

	for (size_t h = 0; h < num_heads; h++)
	{
		for (size_t b = 0; b < B; b++)
		{
			size_t start = h * dk;
			for (size_t t = 0; t < T; t++)
			{
				for (size_t i = 0; i <= t; i++)
				{
					float score = 0.0f;
					for (size_t d = 0; d < dk; d++)
					{
						score += Q(b, t, start + d) * K(b, i, start + d);
					}
					attn_probs(b, h, t, i) = score * csm;
				}
				float max_val = attn_probs(b, h, t, 0);
				for (size_t i = 0; i <= t; i++)
				{
					max_val = (attn_probs(b, h, t, i) > max_val) ? attn_probs(b, h, t, i) : max_val;
				}
				float sum_exp = 0.0f;
				for (size_t i = 0; i <= t; i++)
				{
					float exp_val = expf(attn_probs(b, h, t, i) - max_val);
					attn_probs(b, h, t, i) = exp_val;
					sum_exp += exp_val;
				}
				for (size_t i = 0; i <= t; i++)
				{
					attn_probs(b, h, t, i) /= sum_exp;
				}
				for (size_t i = t + 1; i < T; i++)
				{
					attn_probs(b, h, t, i) = 0.0f;
				}
				for (size_t d = 0; d < dk; d++)
				{
					float val = 0.0f;
					for (size_t i = 0; i <= t; i++)
					{
						val += attn_probs(b, h, t, i) * V(b, i, start + d);
					}
					Out(b, t, start + d) = val;
				}
			}
		}
	}
}

inline auto attention_backward(const Tensor& Q, const Tensor& K, const Tensor& V, const Tensor& attn_probs, const Tensor& dOut, Tensor& dQ, Tensor& dK, Tensor& dV, size_t num_heads, Tensor& dA, Tensor& dS) -> void
{
	size_t B = Q.shape(0);
	size_t T = Q.shape(1);
	size_t D = Q.shape(2);
	size_t dk = D / num_heads;
	float csm = 1.0f / sqrt(dk);
	dQ.Zero();
	dK.Zero();
	dV.Zero();
	size_t start = 0;
	for (size_t h = 0; h < num_heads; h++)
	{
		for (size_t b = 0; b < B; b++)
		{
			start = h * dk;
			for (size_t t = 0; t < T; t++)
			{
				for (size_t i = 0; i <= t; i++)
				{
					float da = 0.0f;
					for (size_t d = 0; d < dk; d++)
					{
						dV(b, i, start + d) += attn_probs(b, h, t, i) * dOut(b, t, start + d);
						da += dOut(b, t, start + d) * V(b, i, start + d);
					}
					dA(i) = da;
				}
				float sum_dA_A = 0.0f;
				for (size_t i = 0; i <= t; i++)
				{
					sum_dA_A += dA(i) * attn_probs(b, h, t, i);
				}
				for (size_t i = 0; i <= t; i++)
				{
					dS(i) = csm * (dA(i) - sum_dA_A) * attn_probs(b, h, t, i);
				}
				for (size_t i = 0; i <= t; i++)
				{
					for (size_t d = 0; d < dk; d++)
					{
						dQ(b, t, start + d) += dS(i) * K(b, i, start + d);
						dK(b, i, start + d) += dS(i) * Q(b, t, start + d);
					}
				}
			}
		}
	}
}

inline auto swiglu_forward(const Tensor& gate, const Tensor& up, Tensor& out) -> void
{
	const float* g_ptr = gate.data();
	const float* u_ptr = up.data();
	float g = 0.0f;
	float u = 0.0f;
	float* out_ptr = out.data();
	float sigmoid = 0.0f;
	float swish = 0.0f;
	for (size_t i = 0; i < gate.numel(); i++)
	{
		g = g_ptr[i];
		u = u_ptr[i];
		sigmoid = 1.0f/ (1.0f+expf(-g));
		swish = g * sigmoid;
		out_ptr[i] = swish * u;
	}
}

inline auto swiglu_backward(const Tensor& gate, const Tensor& up, const Tensor& dOut, Tensor& dgate, Tensor& dup) -> void
{
	const float* g_ptr = gate.data();
	const float* u_ptr = up.data();
	const float* dout_ptr = dOut.data();
	float* dup_ptr = dup.data();
	float* dgate_ptr = dgate.data();
	float sigmoid = 0.0f;
	float swish = 0.0f;

	for (size_t i = 0; i < gate.numel(); i++)
	{
		sigmoid = 1.0f / (1.0f + expf(-g_ptr[i]));
		swish = g_ptr[i] * sigmoid;
		dup_ptr[i] = dout_ptr[i] * swish;
		dgate_ptr[i] = dout_ptr[i] * u_ptr[i] * sigmoid * (1.0f + g_ptr[i] * (1.0f - sigmoid));
	}
}