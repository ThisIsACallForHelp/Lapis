#pragma once
#include <vector>
#include <cstddef>
#include <cassert>
#include <random>
#include "tensor.hpp"
#include "kernels.hpp"
#include "loss.hpp"
#include "optimizer.hpp"

struct ModelConfig {
	size_t dim = 576;
	size_t num_layers = 24;
	size_t num_heads = 9;
	size_t ffn_dim = 1536;
	size_t context_length = 512;
	size_t vocab_size = 1024;
	float eps = 1e-5f;
	auto head_dim() const -> size_t 
	{	
		assert(dim % num_heads == 0);
		return dim/num_heads;
	}
};

struct TransformerBlock {
	Tensor gamma_attn;
	Tensor W_q;
	Tensor W_k;
	Tensor W_v;
	Tensor W_o;
	Tensor d_gamma_attn;
	Tensor dW_q;
	Tensor dW_k;
	Tensor dW_v;
	Tensor dW_o;
	Tensor m_gamma_attn;
	Tensor m_W_q;
	Tensor m_W_k;
	Tensor m_W_v;
	Tensor m_W_o;
	Tensor v_gamma_attn;
	Tensor v_W_q;
	Tensor v_W_k;
	Tensor v_W_v;
	Tensor v_W_o;
	Tensor gamma_ffn;
	Tensor W_gate;
	Tensor W_up;
	Tensor W_down;
	Tensor d_gamma_ffn;
	Tensor dW_gate;
	Tensor dW_up;
	Tensor dW_down;
	Tensor m_gamma_ffn;
	Tensor m_W_gate;
	Tensor m_W_up;
	Tensor m_W_down;
	Tensor v_gamma_ffn;
	Tensor v_W_gate;
	Tensor v_W_up;
	Tensor v_W_down;
	static auto create(MemoryArena& arena, const ModelConfig& cfg) -> TransformerBlock
	{
		TransformerBlock tb;
		size_t D = cfg.dim;
		size_t D_ffn = cfg.ffn_dim;
		tb.gamma_attn = Tensor::create_1d(arena, D);
		tb.v_gamma_attn = Tensor::create_1d(arena, D);
		tb.m_gamma_attn = Tensor::create_1d(arena, D);
		tb.d_gamma_attn = Tensor::create_1d(arena, D);
		tb.gamma_ffn = Tensor::create_1d(arena, D);
		tb.d_gamma_ffn = Tensor::create_1d(arena, D);
		tb.m_gamma_ffn = Tensor::create_1d(arena, D);
		tb.v_gamma_ffn = Tensor::create_1d(arena, D);
		tb.W_k = Tensor::create_2d(arena, D, D);
		tb.W_o = Tensor::create_2d(arena, D, D);
		tb.W_q = Tensor::create_2d(arena, D, D);
		tb.W_v = Tensor::create_2d(arena, D, D);
		tb.dW_k = Tensor::create_2d(arena, D, D);
		tb.dW_o = Tensor::create_2d(arena, D, D);
		tb.dW_q = Tensor::create_2d(arena, D, D);
		tb.dW_v = Tensor::create_2d(arena, D, D);
		tb.m_W_k = Tensor::create_2d(arena, D, D);
		tb.m_W_o = Tensor::create_2d(arena, D, D);
		tb.m_W_q = Tensor::create_2d(arena, D, D);
		tb.m_W_v = Tensor::create_2d(arena, D, D);
		tb.v_W_k = Tensor::create_2d(arena, D, D);
		tb.v_W_o = Tensor::create_2d(arena, D, D);
		tb.v_W_q = Tensor::create_2d(arena, D, D);
		tb.v_W_v = Tensor::create_2d(arena, D, D);
		tb.W_gate = Tensor::create_2d(arena, D, D_ffn);
		tb.W_up = Tensor::create_2d(arena, D, D_ffn);
		tb.dW_gate = Tensor::create_2d(arena, D, D_ffn);
		tb.dW_up = Tensor::create_2d(arena, D, D_ffn);
		tb.m_W_gate = Tensor::create_2d(arena, D, D_ffn);
		tb.m_W_up = Tensor::create_2d(arena, D, D_ffn);
		tb.v_W_gate = Tensor::create_2d(arena, D, D_ffn);
		tb.v_W_up = Tensor::create_2d(arena, D, D_ffn);
		tb.W_down = Tensor::create_2d(arena, D_ffn, D);
		tb.dW_down = Tensor::create_2d(arena, D_ffn, D);
		tb.m_W_down = Tensor::create_2d(arena, D_ffn, D);
		tb.v_W_down = Tensor::create_2d(arena, D_ffn, D);
		return tb;}};

struct LayerActivations
{
	Tensor x_norm1, rstd1, Q, K, V, attn_probs, attn_out, x_mid,
		x_norm2, rstd2, gate, up, swiglu_out, ffn_down, x_out, attn_proj;
	static auto create(MemoryArena& arena, const ModelConfig& cfg, size_t B, size_t T) -> LayerActivations
	{
		size_t M = B * T;
		size_t D = cfg.dim;
		size_t H = cfg.num_heads;
		size_t D_ffn = cfg.ffn_dim;
		LayerActivations la;
		la.x_norm1 = Tensor::create_2d(arena, M, D);
		la.rstd1 = Tensor::create_1d(arena, M);
		la.Q = Tensor::create_3d(arena, B, T, D);
		la.K = Tensor::create_3d(arena, B, T, D);
		la.V = Tensor::create_3d(arena, B, T, D);
		la.attn_probs = Tensor::create_4d(arena, B, H, T, T);
		la.attn_out = Tensor::create_3d(arena, B, T, D);
		la.x_mid = Tensor::create_2d(arena, M, D);
		la.x_norm2 = Tensor::create_2d(arena, M, D);
		la.rstd2 = Tensor::create_1d(arena, M);
		la.gate = Tensor::create_2d(arena, M, D_ffn);
		la.up = Tensor::create_2d(arena, M, D_ffn);
		la.swiglu_out = Tensor::create_2d(arena, M, D_ffn);
		la.ffn_down = Tensor::create_2d(arena, M, D);
		la.x_out = Tensor::create_2d(arena, M, D);
		la.attn_proj = Tensor::create_2d(arena, M, D);
		return la;
	}
};
struct BackwardWorkspace
{
	Tensor dx_final, dx_curr, dx_prev, dx_mid, dswiglu_out, dgate, dup, dx_norm2, dattn_proj, dattn_out, dQ, dK, dV, dA, dS, dx_norm1;
	static auto create(MemoryArena& arena, const ModelConfig& cfg, size_t B, size_t T) -> BackwardWorkspace
	{
		size_t M = B * T;
		size_t D = cfg.dim;
		size_t D_ffn = cfg.ffn_dim;
		BackwardWorkspace bw;
		bw.dx_final = Tensor::create_2d(arena, M, D);
		bw.dx_curr = Tensor::create_2d(arena, M, D);
		bw.dx_prev = Tensor::create_2d(arena, M, D);
		bw.dx_mid = Tensor::create_2d(arena, M, D);
		bw.dswiglu_out = Tensor::create_2d(arena, M, D_ffn);
		bw.dgate = Tensor::create_2d(arena, M, D_ffn);
		bw.dup = Tensor::create_2d(arena, M, D_ffn);
		bw.dx_norm2 = Tensor::create_2d(arena, M, D);
		bw.dattn_proj = Tensor::create_2d(arena, M, D);
		bw.dattn_out = Tensor::create_3d(arena, B, T, D);
		bw.dQ = Tensor::create_3d(arena, B, T, D);
		bw.dK = Tensor::create_3d(arena, B, T, D);
		bw.dV = Tensor::create_3d(arena, B, T, D);
		bw.dA = Tensor::create_1d(arena, T);
		bw.dS = Tensor::create_1d(arena, T);
		bw.dx_norm1 = Tensor::create_2d(arena, M, D);
		return bw;
	}
};

struct TransformerModel
{
	Tensor token_embeddings;
	Tensor d_token_embeddings;
	Tensor m_token_embeddings;
	Tensor v_token_embeddings;
	std::vector<TransformerBlock> layers;
	Tensor gamma_final;
	Tensor d_gamma_final;
	Tensor m_gamma_final;
	Tensor v_gamma_final;
	Tensor lm_head;
	Tensor d_lm_head;
	Tensor m_lm_head;
	Tensor v_lm_head;
	ModelConfig config;
	static auto create(MemoryArena& arena, const ModelConfig& cfg) -> TransformerModel
	{
		TransformerModel tm;
		tm.config = cfg;
		size_t V = cfg.vocab_size;
		size_t D = cfg.dim;
		tm.token_embeddings = Tensor::create_2d(arena, V, D);
		tm.d_token_embeddings = Tensor::create_2d(arena, V, D);
		tm.m_token_embeddings = Tensor::create_2d(arena, V, D);
		tm.v_token_embeddings = Tensor::create_2d(arena, V, D);
		tm.layers.reserve(cfg.num_layers);
		for (size_t i = 0; i < cfg.num_layers; ++i)
		{
			tm.layers.push_back(TransformerBlock::create(arena, cfg));
		}
		tm.gamma_final = Tensor::create_1d(arena, D);
		tm.d_gamma_final = Tensor::create_1d(arena, D);
		tm.m_gamma_final = Tensor::create_1d(arena, D);
		tm.v_gamma_final = Tensor::create_1d(arena, D);
		tm.lm_head = Tensor::create_2d(arena, D, V);
		tm.d_lm_head = Tensor::create_2d(arena, D, V);
		tm.m_lm_head = Tensor::create_2d(arena, D, V);
		tm.v_lm_head = Tensor::create_2d(arena, D, V);
		return tm;
	}
	auto init_weights(uint64_t seed = 1337) -> void
	{
		std::mt19937_64 rng(seed);
		float sigma_embed = 0.02f;
		float sigma_proj = sigma_embed / sqrtf(2.0f * static_cast<float>(this->config.num_layers));
		std::normal_distribution<float> dist_embed(0.0f, sigma_embed);
		std::normal_distribution<float> dist_proj(0.0f, sigma_proj);
		auto fill_normal = [&](Tensor& t, std::normal_distribution<float>& dist)
		{
				float* ptr = t.data();
				for (size_t i = 0; i < t.numel(); ++i) {ptr[i] = dist(rng);}
		};
		auto fill_const = [&](Tensor& t, float val)
		{
				float* ptr = t.data();
				for (size_t i = 0; i < t.numel(); ++i) {ptr[i] = val;}
		};
		fill_normal(this->token_embeddings, dist_embed);
		this->d_token_embeddings.Zero();
		this->m_token_embeddings.Zero();
		this->v_token_embeddings.Zero();
		fill_const(this->gamma_final, 1.0f);
		this->d_gamma_final.Zero();
		this->m_gamma_final.Zero();
		this->v_gamma_final.Zero();
		fill_normal(this->lm_head, dist_proj);
		this->d_lm_head.Zero();
		this->m_lm_head.Zero();
		this->v_lm_head.Zero();
		for (auto& b : this->layers)
		{
			fill_const(b.gamma_attn, 1.0f);
			b.d_gamma_attn.Zero();
			b.m_gamma_attn.Zero();
			b.v_gamma_attn.Zero();
			fill_normal(b.W_q, dist_proj);
			b.dW_q.Zero();
			b.m_W_q.Zero();
			b.v_W_q.Zero();
			fill_normal(b.W_k, dist_proj);
			b.dW_k.Zero();
			b.m_W_k.Zero();
			b.v_W_k.Zero();
			fill_normal(b.W_v, dist_proj);
			b.dW_v.Zero();
			b.m_W_v.Zero();
			b.v_W_v.Zero();
			fill_normal(b.W_o, dist_proj);
			b.dW_o.Zero();
			b.m_W_o.Zero();
			b.v_W_o.Zero();
			fill_const(b.gamma_ffn, 1.0f);
			b.d_gamma_ffn.Zero();
			b.m_gamma_ffn.Zero();
			b.v_gamma_ffn.Zero();
			fill_normal(b.W_gate, dist_proj);
			b.dW_gate.Zero();
			b.m_W_gate.Zero();
			b.v_W_gate.Zero();
			fill_normal(b.W_up, dist_proj);
			b.dW_up.Zero();
			b.m_W_up.Zero();
			b.v_W_up.Zero();
			fill_normal(b.W_down, dist_proj);
			b.dW_down.Zero();
			b.m_W_down.Zero();
			b.v_W_down.Zero();
		}
	}
	auto step_optimizer(float lr, size_t step, float beta1 = 0.9f, float beta2 = 0.999f, float eps = 1e-8f, float weight_decay = 0.01f) -> void
	{
		adamw_step(this->token_embeddings, this->m_token_embeddings, this->v_token_embeddings, d_token_embeddings, lr, step, beta1, beta2, eps, weight_decay);
		adamw_step(this->gamma_final, this->m_gamma_final, this->v_gamma_final, this->d_gamma_final, lr, step, beta1, beta2, eps, 0.0f);
		adamw_step(this->lm_head, this->m_lm_head, this->v_lm_head, this->d_lm_head, lr, step, beta1, beta2,eps, weight_decay);
		for (auto& b : this->layers)
		{
			adamw_step(b.gamma_attn, b.m_gamma_attn, b.v_gamma_attn, b.d_gamma_attn, lr, step, beta1, beta2, eps, 0.0f);
			adamw_step(b.W_q, b.m_W_q, b.v_W_q, b.dW_q, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.W_k, b.m_W_k, b.v_W_k, b.dW_k, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.W_v, b.m_W_v, b.v_W_v, b.dW_v, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.W_o, b.m_W_o, b.v_W_o, b.dW_o, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.gamma_ffn, b.m_gamma_ffn, b.v_gamma_ffn, b.d_gamma_ffn, lr, step, beta1, beta2, eps, 0.0f);
			adamw_step(b.W_gate, b.m_W_gate, b.v_W_gate, b.dW_gate, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.W_up, b.m_W_up, b.v_W_up, b.dW_up, lr, step, beta1, beta2, eps, weight_decay);
			adamw_step(b.W_down, b.m_W_down, b.v_W_down, b.dW_down, lr, step, beta1, beta2, eps, weight_decay);
		}
	}
	auto forward(const Tensor& token_ids, std::vector<LayerActivations>& layer_acts, Tensor& x_0, Tensor& x_final, Tensor& rstd_final, Tensor& logits) -> void
	{
		size_t B = token_ids.shape(0);
		size_t T = token_ids.shape(1);
		size_t M = B * T;
		size_t D = config.dim;
		for (size_t m = 0; m < M; ++m)
		{
			size_t id = static_cast<size_t>(token_ids.data()[m]);
			for (size_t d = 0; d < D; ++d)
			{
				x_0(m, d) = this->token_embeddings(id, d);
			}
		}
		for (size_t l = 0; l < config.num_layers; ++l)
		{
			TransformerBlock& layer = this->layers[l];
			LayerActivations& activation = layer_acts[l];
			const Tensor& x_in = (l == 0) ? x_0 : layer_acts[l - 1].x_out;
			rmsnorm_forward(x_in, layer.gamma_attn, activation.x_norm1, activation.rstd1, config.eps);
			Tensor Q_2d(activation.Q.data(), M, D);
			Tensor K_2d(activation.K.data(), M, D);
			Tensor V_2d(activation.V.data(), M, D);
			gemm_forward(activation.x_norm1, layer.W_q, Q_2d);
			gemm_forward(activation.x_norm1, layer.W_k, K_2d);
			gemm_forward(activation.x_norm1, layer.W_v, V_2d);
			rope_forward(activation.Q, config.num_heads);
			rope_forward(activation.K, config.num_heads);
			attention_forward(activation.Q, activation.K, activation.V, activation.attn_probs, activation.attn_out, config.num_heads);
			Tensor attn_out_2d(activation.attn_out.data(), M, D);
			gemm_forward(attn_out_2d, layer.W_o, activation.attn_proj);
			size_t elem_count = activation.x_mid.numel();
			for (size_t j = 0; j < elem_count; ++j)
			{
				activation.x_mid.data()[j] = x_in.data()[j] + activation.attn_proj.data()[j];
			}
			rmsnorm_forward(activation.x_mid, layer.gamma_ffn, activation.x_norm2, activation.rstd2, config.eps);
			gemm_forward(activation.x_norm2, layer.W_gate, activation.gate);
			gemm_forward(activation.x_norm2, layer.W_up, activation.up);
			swiglu_forward(activation.gate, activation.up, activation.swiglu_out);
			gemm_forward(activation.swiglu_out, layer.W_down, activation.ffn_down);
			elem_count = activation.x_out.numel();
			for (size_t j = 0; j < elem_count; ++j)
			{
				activation.x_out.data()[j] = activation.x_mid.data()[j] + activation.ffn_down.data()[j];
			}
		}
		rmsnorm_forward(layer_acts.back().x_out, this->gamma_final, x_final, rstd_final, config.eps);
		gemm_forward(x_final, this->lm_head, logits);
	}

	auto backward(const Tensor& token_ids, std::vector<LayerActivations>& layer_acts, Tensor& x_0, Tensor& x_final, Tensor& rstd_final, Tensor& d_logits, BackwardWorkspace& bw) -> void
	{
		gemm_backward(x_final, this->lm_head, d_logits, bw.dx_final, this->d_lm_head);
		rmsnorm_backward(layer_acts.back().x_out, this->gamma_final, bw.dx_final, rstd_final, bw.dx_curr, this->d_gamma_final);
		size_t num_layers = config.num_layers;
		for (int l = static_cast<int>(num_layers) - 1; l >= 0; --l)
		{
			TransformerBlock& layer = this->layers[l];
			LayerActivations& activation = layer_acts[l];
			const Tensor& x_in = (l == 0) ? x_0: layer_acts[l-1].x_out;
			size_t elem_count = bw.dx_mid.numel();
			for (size_t j = 0; j < elem_count; ++j)
			{
				bw.dx_mid.data()[j] = bw.dx_curr.data()[j];
			}
			gemm_backward(activation.swiglu_out, layer.W_down, bw.dx_curr, bw.dswiglu_out, layer.dW_down);
			swiglu_backward(activation.gate, activation.up, bw.dswiglu_out, bw.dgate, bw.dup);
			gemm_backward(activation.x_norm2, layer.W_gate, bw.dgate, bw.dx_norm2, layer.dW_gate);
			gemm_backward(activation.x_norm2, layer.W_up, bw.dup, bw.dx_prev, layer.dW_up);
			size_t num_elem = activation.x_norm2.numel();
			for (size_t j = 0; j < num_elem; ++j) {bw.dx_norm2.data()[j] += bw.dx_prev.data()[j];}
			rmsnorm_backward(activation.x_mid, layer.gamma_ffn, bw.dx_norm2, activation.rstd2, bw.dx_prev, layer.d_gamma_ffn);
			for (size_t j = 0; j < num_elem; ++j) {bw.dx_mid.data()[j] += bw.dx_prev.data()[j];}
			for (size_t j = 0; j < num_elem; ++j) {bw.dattn_proj.data()[j] = bw.dx_mid.data()[j];}
			size_t B = activation.attn_out.shape(0);
			size_t T = activation.attn_out.shape(1);
			size_t D = activation.attn_out.shape(2);
			size_t M = B * T;
			Tensor attn_out_2d(activation.attn_out.data(), M, D);
			Tensor dattn_out_2d(bw.dattn_out.data(), M, D);
			gemm_backward(attn_out_2d, layer.W_o, bw.dattn_proj, dattn_out_2d, layer.dW_o);
			attention_backward(activation.Q, activation.K, activation.V, activation.attn_probs, bw.dattn_out, bw.dQ, bw.dK, bw.dV, config.num_heads, bw.dA, bw.dS);
			rope_backward(bw.dQ, config.num_heads);
			rope_backward(bw.dK, config.num_heads);
			Tensor dQ_2d(bw.dQ.data(), M, D);
			Tensor dK_2d(bw.dK.data(), M, D);
			Tensor dV_2d(bw.dV.data(), M, D);
			gemm_backward(activation.x_norm1, layer.W_q, dQ_2d, bw.dx_norm1, layer.dW_q);
			gemm_backward(activation.x_norm1, layer.W_k, dK_2d, bw.dx_prev, layer.dW_k);
			for (size_t j = 0; j < num_elem; ++j) {bw.dx_norm1.data()[j] += bw.dx_prev.data()[j];}
			gemm_backward(activation.x_norm1, layer.W_v, dV_2d, bw.dx_prev, layer.dW_v);
			for (size_t j = 0; j < num_elem; ++j) {bw.dx_norm1.data()[j] += bw.dx_prev.data()[j];}
			rmsnorm_backward(x_in, layer.gamma_attn, bw.dx_norm1, activation.rstd1, bw.dx_prev, layer.d_gamma_attn);
			for (size_t j = 0; j < num_elem; ++j) {bw.dx_curr.data()[j] = bw.dx_prev.data()[j] + bw.dx_mid.data()[j];}
		}
		size_t M = token_ids.numel();
		size_t D = this->config.dim;
		for (size_t m = 0; m < M; ++m)
		{
			size_t id = static_cast<size_t>(token_ids.data()[m]);
			for (size_t d = 0; d < D; ++d) {this->d_token_embeddings(id, d) += bw.dx_curr(m, d);}
		}
	}
};