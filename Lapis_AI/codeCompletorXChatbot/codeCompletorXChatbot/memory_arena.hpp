#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <new>
static constexpr size_t POOL_SIZE = 64 * 1024 * 1024;


class MemoryArena
{
	uint8_t* _m_data;
	size_t _m_capacity;
	size_t _m_offset;
	public:
		inline MemoryArena(size_t capacity_bytes = POOL_SIZE)
		{
			this->_m_capacity = capacity_bytes;
			this->_m_offset = 0;
			this->_m_data = static_cast<uint8_t*>(_aligned_malloc(this->_m_capacity,64));
			if (!this->_m_data)
			{
				throw std::bad_alloc();
			}
		}

		MemoryArena(const MemoryArena&) = delete;

		auto operator=(const MemoryArena&) -> MemoryArena& = delete;

		inline ~MemoryArena()
		{
			_aligned_free(this->_m_data);
			this->_m_data = nullptr;
		}

		inline auto allocate_bytes(size_t bytes, size_t alignment = 64) -> void*
		{
			size_t aligned_offset = (this->_m_offset + alignment - 1) & ~(alignment - 1);
			if (aligned_offset + bytes > this->_m_capacity)
			{
				throw std::bad_alloc();
			}
			void* ptr = this->_m_data + aligned_offset;
			this->_m_offset = aligned_offset + bytes;
			return ptr;
		}

		inline auto allocate_floats(size_t count) -> float*
		{
			return static_cast<float*>(this->allocate_bytes(count * sizeof(float), 64));
		}

		inline auto reset() -> void {this->_m_offset = 0;}

		inline auto used_bytes() const -> size_t {return this->_m_offset;}

		inline auto remaining_bytes() const -> size_t { return this->_m_capacity - this->_m_offset;}
};