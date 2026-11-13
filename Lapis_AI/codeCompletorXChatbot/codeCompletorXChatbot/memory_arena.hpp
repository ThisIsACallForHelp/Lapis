#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <new>
inline static constexpr size_t POOL_SIZE = 2ULL * 1024 * 1024 * 1024;


class MemoryArena
{
	uint8_t* _m_data;
	size_t _m_capacity;
	size_t _m_offset;
	public:
		inline explicit MemoryArena(size_t capacity_bytes = POOL_SIZE)
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

		inline ~MemoryArena() noexcept
		{
			_aligned_free(this->_m_data);
			this->_m_data = nullptr;
		}

		[[nodiscard]] inline auto allocate_bytes(size_t bytes, size_t alignment = 64) -> void*
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

		[[nodiscard]] inline auto allocate_floats(size_t count) -> float*
		{
			return static_cast<float*>(this->allocate_bytes(count * sizeof(float), 64));
		}

		inline auto reset() noexcept -> void {this->_m_offset = 0;}

		[[nodiscard]] inline auto used_bytes() const noexcept -> size_t {return this->_m_offset;}

		[[nodiscard]] inline auto remaining_bytes() const noexcept -> size_t { return this->_m_capacity - this->_m_offset;}
};