#pragma once
#include "tensor.hpp"

inline auto cross_entropy_forward(Tensor& logits, Tensor& targets) -> float
{
	float max_logit = 0.0f;
	float sum = 0.0f;
	float log_sum_exp = 0.0f;
	float curr = 0.0;
	float token_loss = 0.0f;
	float batch_loss = 0.0f;
	float raw_target = 0.0f;
	size_t target = 0;
	size_t M = logits.shape(0);
	size_t V = logits.shape(1);
	for (size_t i = 0; i < M; ++i)
	{
		max_logit = logits(i, 0);
		for(size_t v = 0; v < V; ++v)
		{
			curr = logits(i,v);
			max_logit = (max_logit < curr) ? curr : max_logit;
		}
		sum = 0.0f;
		for (size_t v = 0; v < V; ++v)
		{
			curr = logits(i, v);
			sum += expf(curr - max_logit);
		}
		log_sum_exp = max_logit + logf(sum);
		target = targets(i);
		raw_target = logits(i,target);
		token_loss = log_sum_exp - raw_target;
		batch_loss += token_loss;
	}
	batch_loss /= M;
	return batch_loss;
}

inline auto cross_entropy_backward(Tensor& logits, Tensor& targets, Tensor& dLogits) -> void
{
	float max_logit = 0.0f;
	float sum = 0.0f;
	float curr = 0.0;
	float raw_target = 0.0f;
	float normalized_softmax = 0.0f;
	float gradient = 0.0f;
	size_t target = 0;
	size_t M = logits.shape(0);
	size_t V = logits.shape(1);
	for (size_t i = 0; i < M; ++i)
	{
		max_logit = logits(i, 0);
		for (size_t v = 0; v < V; ++v)
		{
			curr = logits(i, v);
			max_logit = (max_logit < curr) ? curr : max_logit;
		}
		sum = 0.0f;
		for (size_t v = 0; v < V; ++v)
		{
			curr = logits(i, v);
			sum += expf(curr - max_logit);
		}
		target = targets(i);
		for (size_t v = 0; v < V; ++v)
		{
			normalized_softmax = expf(logits(i,v) - max_logit) / sum;
			if (v == target) {gradient = (normalized_softmax - 1.0f) / M;}
			else{gradient = normalized_softmax / M;}
			dLogits(i, v) = gradient;
		}
	}
}