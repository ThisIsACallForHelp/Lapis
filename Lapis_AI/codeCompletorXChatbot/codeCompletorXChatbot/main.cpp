#include <iostream>
#include <iomanip>
#include "memory_arena.hpp"
#include "tensor.hpp"
#include "model.hpp"

int main()
{
	MemoryArena arena((long long)2 * 1024 * 1024 * 1024);
	ModelConfig cfg;
	TransformerModel tm = TransformerModel::create(arena, cfg);
	tm.init_weights(1337);
	size_t B = 1;
	size_t T = 8;
	size_t M = B * T;
	std::vector<LayerActivations> layer_acts;
	layer_acts.reserve(cfg.num_layers);
	for (size_t i = 0; i < cfg.num_layers; ++i)
	{
		layer_acts.push_back(LayerActivations::create(arena, cfg, B, T));
	}
	BackwardWorkspace bw = BackwardWorkspace::create(arena, cfg, B, T);
	Tensor token_ids = Tensor::create_2d(arena, B, T);
	Tensor targets = Tensor::create_1d(arena, M);
	Tensor x_0 = Tensor::create_2d(arena, M, cfg.dim);
	Tensor x_final = Tensor::create_2d(arena, M, cfg.dim);
	Tensor rstd_final = Tensor::create_1d(arena, M);
	Tensor logits = Tensor::create_2d(arena, M, cfg.vocab_size);
	Tensor d_logits = Tensor::create_2d(arena, M, cfg.vocab_size);
	for (size_t t = 0; t < T; ++t)
	{
		token_ids(0, t) = static_cast<float>(t);
		targets(t) = static_cast<float>((t + 1) % T);
	}
	for (size_t step = 1; step <= 10; ++step)
	{
		tm.zero_grad();
		tm.forward(token_ids, layer_acts, x_0, x_final, rstd_final, logits);
		float loss = cross_entropy_forward(logits, targets);
		std::cout << "Step " << step << " | Loss: " << loss << std::endl;
		cross_entropy_backward(logits, targets, d_logits);
		tm.backward(token_ids, layer_acts, x_0, x_final, rstd_final, d_logits, bw);
		tm.step_optimizer(1e-3f, step);
	}
	return 0;
}