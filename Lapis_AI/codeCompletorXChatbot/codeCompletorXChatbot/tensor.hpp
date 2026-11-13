#pragma once
#include <cstddef>
#include <cstdint>
#include "memory_arena.hpp"
#include <cstring>

class Tensor
{
	float* _mem_addr;
	uint8_t _rank;
	size_t _shapes[4] = {0};
	size_t _strides[4] = {0};

	public:
		explicit Tensor()
		{
			this->_mem_addr = nullptr;
			this->_rank = 0;
		}
		explicit Tensor(float* data, size_t d0) : _mem_addr(data), _rank(1)
		{
			this->_shapes[0] = d0;
			this->_strides[0] = 1;
		}
		explicit Tensor(float* data, size_t d0, size_t d1) : _mem_addr(data), _rank(2)
		{
			this->_shapes[0] = d0;
			this->_shapes[1] = d1;
			this->_strides[0] = d1;
			this->_strides[1] = 1;
		}
		explicit Tensor(float* data, size_t d0, size_t d1, size_t d2) : _mem_addr(data), _rank(3)
		{
			this->_shapes[0] = d0;
			this->_shapes[1] = d1;
			this->_shapes[2] = d2;
			this->_strides[0] = d1 * d2;
			this->_strides[1] = d2;
			this->_strides[2] = 1;
		}
		explicit Tensor(float* data, size_t d0, size_t d1, size_t d2, size_t d3) : _mem_addr(data), _rank(4)
		{
			this->_shapes[0] = d0;
			this->_shapes[1] = d1;
			this->_shapes[2] = d2;
			this->_shapes[3] = d3;
			this->_strides[0] = d1 * d2 * d3;
			this->_strides[1] = d3 * d2;
			this->_strides[2] = d3;
			this->_strides[3] = 1;
		}
		[[nodiscard]] auto operator()(size_t i) noexcept -> float&
		{
			size_t index = i * this->_strides[0];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t r, size_t c) noexcept -> float&
		{
			size_t index = r * this->_strides[0] + c * this->_strides[1];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t b, size_t t, size_t d) noexcept -> float&
		{
			size_t index = b * this->_strides[0] + t * this->_strides[1] + d * this->_strides[2];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t b, size_t h, size_t t, size_t d) noexcept -> float&
		{
			size_t index = b * this->_strides[0] + h * this->_strides[1] + t * this->_strides[2] + d * this->_strides[3];
			return this->_mem_addr[index];
		}

		[[nodiscard]] auto operator()(size_t i) const noexcept -> const float&
		{
			size_t index = i * this->_strides[0];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t r, size_t c) const noexcept -> const float&
		{
			size_t index = r * this->_strides[0] + c * this->_strides[1];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t b, size_t t, size_t d) const noexcept -> const float&
		{
			size_t index = b * this->_strides[0] + t * this->_strides[1] + d * this->_strides[2];
			return this->_mem_addr[index];
		}
		[[nodiscard]] auto operator()(size_t b, size_t h, size_t t, size_t d) const noexcept -> const float&
		{
			size_t index = b * this->_strides[0] + h * this->_strides[1] + t * this->_strides[2] + d * this->_strides[3];
			return this->_mem_addr[index];
		}

		[[nodiscard]] auto data() noexcept -> float* {return this->_mem_addr;}
		[[nodiscard]] auto data() const noexcept -> const float* {return this->_mem_addr;}

		[[nodiscard]] auto rank() const noexcept -> size_t {return this->_rank;}
		[[nodiscard]] auto shape(size_t dim) const noexcept -> size_t {return this->_shapes[dim];}
		[[nodiscard]] auto stride(size_t dim) const noexcept -> size_t {return this->_strides[dim];}
		[[nodiscard]] auto numel() const noexcept -> size_t
		{
			if (!this->_rank) {return 0;}
			size_t res = 1;
			for (size_t i = 0; i < this->_rank; i++) {res *= this->_shapes[i];}
			return res;
		}
		[[nodiscard]] auto bytes() const noexcept -> size_t {return numel() * sizeof(float);}

		[[nodiscard]] static auto create_1d(MemoryArena& arena, size_t d0) -> Tensor
		{
			Tensor d1(arena.allocate_floats(d0), d0);
			return d1;
		}
		[[nodiscard]] static auto create_2d(MemoryArena& arena, size_t d0, size_t d1) -> Tensor
		{
			Tensor d2(arena.allocate_floats(d0 * d1), d0, d1);
			return d2;
		}
		[[nodiscard]] static auto create_3d(MemoryArena& arena, size_t d0, size_t d1, size_t d2) -> Tensor
		{
			Tensor d3(arena.allocate_floats(d0 * d1 * d2), d0, d1, d2);
			return d3;
		}
		[[nodiscard]] static auto create_4d(MemoryArena& arena, size_t d0, size_t d1, size_t d2, size_t d3) -> Tensor
		{
			Tensor d4(arena.allocate_floats(d0 * d1 * d2 * d3), d0, d1, d2, d3);
			return d4;
		}
		auto Zero() noexcept -> void
		{
			if (this->_mem_addr == nullptr || !this->_rank) {return;}
			std::memset(this->_mem_addr, 0, this->bytes());
		}
};