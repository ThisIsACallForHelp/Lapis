#pragma once
#include <cmath>
#include <cassert>
#include "tensor.hpp"

inline auto adamw_step(Tensor& W, Tensor& m, Tensor& v, const Tensor& dW, float lr, size_t step, float beta1 = 0.9f, float beta2 = 0.999f, float eps = 1e-8f, float weight_decay = 0.01f) -> void
{
	assert(W.numel() == dW.numel() && W.numel() == m.numel() && W.numel() == v.numel() && step >= 1);
	float* W_data = W.data();
	const float* dW_data = dW.data();
	float* m_ptr = m.data();
	float* v_ptr = v.data();
	float bias_correction1 = 1.0f - powf(beta1, static_cast<float>(step));
	float bias_correction2 = 1.0f - powf(beta2, static_cast<float>(step));
	float m_hat = 0.0f;
	float v_hat = 0.0f;
	float g = 0.0f;
	float w = 0.0f;
	for (size_t i = 0; i < W.numel(); ++i)
	{
		g = dW_data[i];
		w = W_data[i];
		m_ptr[i] = beta1 * m_ptr[i] + (1.0f - beta1) * g;
		v_ptr[i] = beta2 * v_ptr[i] + (1.0f - beta2) * g * g;
		m_hat = m_ptr[i] / bias_correction1;
		v_hat = v_ptr[i] / bias_correction2;
		w *= 1.0f - lr * weight_decay;
		W_data[i] = w - ((lr * m_hat) / (sqrtf(v_hat) + eps));
	}

}