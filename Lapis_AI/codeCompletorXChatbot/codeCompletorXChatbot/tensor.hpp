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
		Tensor()
		{
			this->_mem_addr = nullptr;
			this->_rank = 0;
		}
		Tensor(float* data, size_t d0) : _mem_addr(data), _rank(1)
		{
			this->_shapes[0] = d0;
			this->_strides[0] = 1;
		}
		Tensor(float* data, size_t d0, size_t d1) : _mem_addr(data), _rank(2)
		{
			this->_shapes[0] = d0;
			this->_shapes[1] = d1;
			this->_strides[0] = d1;
			this->_strides[1] = 1;
		}
		Tensor(float* data, size_t d0, size_t d1, size_t d2) : _mem_addr(data), _rank(3)
		{
			this->_shapes[0] = d0;
			this->_shapes[1] = d1;
			this->_shapes[2] = d2;
			this->_strides[0] = d1 * d2;
			this->_strides[1] = d2;
			this->_strides[2] = 1;
		}
		Tensor(float* data, size_t d0, size_t d1, size_t d2, size_t d3) : _mem_addr(data), _rank(4)
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
		float& operator()(size_t i)
		{
			size_t index = i * this->_strides[0];
			return this->_mem_addr[index];
		}
		float& operator()(size_t r, size_t c)
		{
			size_t index = r * this->_strides[0] + c * this->_strides[1];
			return this->_mem_addr[index];
		}
		float& operator()(size_t b, size_t t, size_t d)
		{
			size_t index = b * this->_strides[0] + t * this->_strides[1] + d * this->_strides[2];
			return this->_mem_addr[index];
		}
		float& operator()(size_t b, size_t h, size_t t, size_t d)
		{
			size_t index = b * this->_strides[0] + h * this->_strides[1] + t * this->_strides[2] + d * this->_strides[3];
			return this->_mem_addr[index];
		}

		const float& operator()(size_t i) const
		{
			size_t index = i * this->_strides[0];
			return this->_mem_addr[index];
		}
		const float& operator()(size_t r, size_t c) const
		{
			size_t index = r * this->_strides[0] + c * this->_strides[1];
			return this->_mem_addr[index];
		}
		const float& operator()(size_t b, size_t t, size_t d) const
		{
			size_t index = b * this->_strides[0] + t * this->_strides[1] + d * this->_strides[2];
			return this->_mem_addr[index];
		}
		const float& operator()(size_t b, size_t h, size_t t, size_t d) const
		{
			size_t index = b * this->_strides[0] + h * this->_strides[1] + t * this->_strides[2] + d * this->_strides[3];
			return this->_mem_addr[index];
		}

		float* data() {return this->_mem_addr;}
		const float* data() const {return this->_mem_addr;}

		size_t rank() const {return this->_rank;}
		size_t shape(size_t dim) const {return this->_shapes[dim];}
		size_t stride(size_t dim) const {return this->_strides[dim];}
		size_t numel() const
		{
			if (!this->_rank) {return 0;}
			size_t res = 1;
			for (size_t i = 0; i < this->_rank; i++)
			{
				res *= this->_shapes[i];
			}
			return res;
		}
		size_t bytes() const {return numel() * sizeof(float);}

		static Tensor create_1d(MemoryArena& arena, size_t d0)
		{
			Tensor d1(arena.allocate_floats(d0), d0);
			return d1;
		}
		static Tensor create_2d(MemoryArena& arena, size_t d0, size_t d1)
		{
			Tensor d2(arena.allocate_floats(d0 * d1), d0, d1);
			return d2;
		}
		static Tensor create_3d(MemoryArena& arena, size_t d0, size_t d1, size_t d2)
		{
			Tensor d3(arena.allocate_floats(d0 * d1 * d2), d0, d1, d2);
			return d3;
		}
		static Tensor create_4d(MemoryArena& arena, size_t d0, size_t d1, size_t d2, size_t d3)
		{
			Tensor d4(arena.allocate_floats(d0 * d1 * d2 * d3), d0, d1, d2, d3);
			return d4;
		}
		auto Zero() -> void
		{
			if (this->_mem_addr == nullptr || !this->_rank) {return;}
			std::memset(this->_mem_addr, 0, this->bytes());
		}
};