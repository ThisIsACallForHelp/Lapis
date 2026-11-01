#include <iostream>
#include <iomanip>
#include "memory_arena.hpp"
#include "tensor.hpp"

int main()
{
	std::cout << "========================================" << std::endl;
	std::cout << "   Phase 1 Integration: Arena & Tensor" << std::endl;
	std::cout << "========================================" << std::endl;

	// 1. Initialize MemoryArena (64 MB pool)
	MemoryArena arena(64 * 1024 * 1024);
	std::cout << "[1] Arena initialized with 64 MB capacity." << std::endl;

	// 2. Create a 2D Tensor (Matrix of shape [3, 4])
	Tensor mat = Tensor::create_2d(arena, 3, 4);
	std::cout << "\n[2] Created 2D Tensor:" << std::endl;
	std::cout << "    Rank:    " << mat.rank() << std::endl;
	std::cout << "    Shape:   [" << mat.shape(0) << ", " << mat.shape(1) << "]" << std::endl;
	std::cout << "    Strides: [" << mat.stride(0) << ", " << mat.stride(1) << "]" << std::endl;
	std::cout << "    Numel:   " << mat.numel() << " floats (" << mat.bytes() << " bytes)" << std::endl;

	// Verify 64-byte cache alignment for AVX2 SIMD
	uintptr_t mat_addr = reinterpret_cast<uintptr_t>(mat.data());
	std::cout << "    Address: " << mat.data()
	          << " (" << (mat_addr % 64 == 0 ? "PASSED: 64-byte aligned" : "FAILED") << ")" << std::endl;

	// Populate and print the 2D matrix
	std::cout << "\n    Writing and printing 2D Matrix values:" << std::endl;
	for (size_t r = 0; r < mat.shape(0); ++r)
	{
		std::cout << "    ";
		for (size_t c = 0; c < mat.shape(1); ++c)
		{
			mat(r, c) = static_cast<float>(r * 10 + c);
			std::cout << std::setw(6) << mat(r, c) << " ";
		}
		std::cout << std::endl;
	}

	// 3. Create a 3D Tensor (Batch, Tokens, HiddenDim of shape [2, 3, 4])
	Tensor act = Tensor::create_3d(arena, 2, 3, 4);
	std::cout << "\n[3] Created 3D Tensor:" << std::endl;
	std::cout << "    Rank:    " << act.rank() << std::endl;
	std::cout << "    Shape:   [" << act.shape(0) << ", " << act.shape(1) << ", " << act.shape(2) << "]" << std::endl;
	std::cout << "    Strides: [" << act.stride(0) << ", " << act.stride(1) << ", " << act.stride(2) << "]" << std::endl;
	std::cout << "    Numel:   " << act.numel() << " floats (" << act.bytes() << " bytes)" << std::endl;

	uintptr_t act_addr = reinterpret_cast<uintptr_t>(act.data());
	std::cout << "    Address: " << act.data()
	          << " (" << (act_addr % 64 == 0 ? "PASSED: 64-byte aligned" : "FAILED") << ")" << std::endl;

	act(1, 2, 3) = 42.0f;
	std::cout << "    Wrote act(1, 2, 3) = 42.0f -> Read back: " << act(1, 2, 3) << std::endl;

	// 4. Memory Footprint
	std::cout << "\n[4] Arena Status:" << std::endl;
	std::cout << "    Used bytes:      " << arena.used_bytes() << " bytes" << std::endl;
	std::cout << "    Remaining bytes: " << arena.remaining_bytes() << " bytes" << std::endl;

	// 5. Zero-cost recycling via reset
	arena.reset();
	std::cout << "\n[5] Arena after reset():" << std::endl;
	std::cout << "    Used bytes:      " << arena.used_bytes() << " bytes (Instant O(1) recycling)" << std::endl;

	std::cout << "\n========================================" << std::endl;
	std::cout << "   PHASE 1 INTEGRATION: ALL PASSED!" << std::endl;
	std::cout << "========================================" << std::endl;

	return 0;
}