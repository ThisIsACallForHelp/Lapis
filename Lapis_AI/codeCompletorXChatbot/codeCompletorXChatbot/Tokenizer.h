#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#define TARGET_VOCAB_SIZE 5000

class Tokenizer 
{
    std::unordered_map<std::string, int> _encoder_map;
    std::unordered_map<int, std::string> _decoder_map;
    int _counter = 0;
	std::vector<std::pair<std::string, std::string>> _merges;
public:
	auto train(const std::string& text) -> void;
	auto encode(const std::string& text) -> std::vector<int>;
	auto decode(const std::vector<int>& encoded) -> std::string;
	auto get_vocab_size() -> int;
	bool save(const std::string& path);
	bool load(const std::string& path);
};
