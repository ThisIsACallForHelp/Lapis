#include "Tokenizer.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

// Helper to split text into indentation runs, newlines, identifiers, and multi-char operators
static auto splitIntoPreTokens(const std::string& text) -> std::vector<std::string> {
	std::vector<std::string> words;
	words.reserve(text.size() / 3);
	const size_t n = text.size();
	size_t i = 0;

	while (i < n) {
		char c = text[i];

		// 1. Horizontal whitespace runs (e.g. indentation "  ", "    ", "        ")
		if (c == ' ' || c == '\t') {
			size_t start = i;
			while (i < n && (text[i] == ' ' || text[i] == '\t')) {
				i++;
			}
			words.emplace_back(text.substr(start, i - start));
		}
		// 2. Newlines
		else if (c == '\n' || c == '\r') {
			size_t start = i;
			while (i < n && (text[i] == '\n' || text[i] == '\r')) {
				i++;
			}
			words.emplace_back(text.substr(start, i - start));
		}
		// 3. Alphanumeric / Identifiers
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
			size_t start = i;
			while (i < n && ((text[i] >= 'a' && text[i] <= 'z') || 
			                 (text[i] >= 'A' && text[i] <= 'Z') || 
			                 (text[i] >= '0' && text[i] <= '9') || 
			                 text[i] == '_')) {
				i++;
			}
			words.emplace_back(text.substr(start, i - start));
		}
		// 4. Common multi-character C++ operators and comment delimiters
		else if (i + 1 < n) {
			std::string two = text.substr(i, 2);
			if (two == "::" || two == "->" || two == "==" || two == "!=" || 
			    two == "<=" || two == ">=" || two == "&&" || two == "||" || 
			    two == "<<" || two == ">>" || two == "+=" || two == "-=" || 
			    two == "*=" || two == "/=" || two == "++" || two == "--" || 
			    two == "//" || two == "/*" || two == "*/") {
				words.push_back(two);
				i += 2;
				continue;
			}
			words.emplace_back(1, c);
			i++;
		}
		// 5. Single punctuation or symbol
		else {
			words.emplace_back(1, c);
			i++;
		}
	}
	return words;
}

auto Tokenizer::train(const std::string& text) -> void
{
	this->_encoder_map.clear();
	this->_decoder_map.clear();
	this->_merges.clear();
	this->_counter = 0;

	// 1. Initialize Base Vocabulary (all 256 byte values)
	for (int i = 0; i < 256; ++i)
	{
		std::string ch(1, static_cast<char>(i));
		_encoder_map[ch] = _counter;
		_decoder_map[_counter] = ch;
		_counter++;
	}

	// 2. Pre-tokenize and count word frequencies
	std::vector<std::string> pre_tokens = splitIntoPreTokens(text);
	std::unordered_map<std::string, int> wordCounts;
	for (const auto& w : pre_tokens)
	{
		wordCounts[w]++;
	}

	std::unordered_map<std::string, std::vector<std::string>> vocab_words;
	for (const auto& pair : wordCounts)
	{
		std::vector<std::string> chars;
		for (const auto& c : pair.first)
		{
			chars.push_back(std::string(1, c));
		}
		vocab_words[pair.first] = chars;
	}

	// 3. Iterative BPE Merge Training
	while (_counter < TARGET_VOCAB_SIZE)
	{
		std::unordered_map<std::string, int> pair_freqs;
		for (const auto& item : vocab_words)
		{
			const std::vector<std::string>& symbols = item.second;
			int freq = wordCounts[item.first];
			for (size_t i = 0; i + 1 < symbols.size(); ++i)
			{
				std::string pair = symbols[i] + "\1" + symbols[i + 1];
				pair_freqs[pair] += freq;
			}
		}

		std::string best_pair = "";
		int max_freq = 0;
		for (const auto& pair : pair_freqs)
		{
			if (pair.second > max_freq)
			{
				max_freq = pair.second;
				best_pair = pair.first;
			}
		}

		if (max_freq < 1)
		{
			break;
		}

		size_t sep_pos = best_pair.find("\1");
		std::string leftSymbol = best_pair.substr(0, sep_pos);
		std::string rightSymbol = best_pair.substr(sep_pos + 1);
		std::string new_token = leftSymbol + rightSymbol;

		this->_encoder_map[new_token] = this->_counter;
		this->_decoder_map[this->_counter] = new_token;
		this->_counter++;
		this->_merges.push_back({leftSymbol, rightSymbol});

		for (auto& item : vocab_words)
		{
			std::vector<std::string>& symbols = item.second;
			std::vector<std::string> new_symbols;
			new_symbols.reserve(symbols.size());

			for (size_t i = 0; i < symbols.size(); ++i)
			{
				if (i + 1 < symbols.size() && symbols[i] == leftSymbol && symbols[i + 1] == rightSymbol)
				{
					new_symbols.push_back(new_token);
					i++;
				}
				else
				{
					new_symbols.push_back(symbols[i]);
				}
			}
			symbols = std::move(new_symbols);
		}
	}
}

auto Tokenizer::encode(const std::string& text) -> std::vector<int>
{
	std::vector<int> res;
	std::unordered_map<std::string, std::vector<int>> word_cache;

	std::vector<std::string> words = splitIntoPreTokens(text);
	res.reserve(words.size());

	for (const auto& word : words)
	{
		auto c_it = word_cache.find(word);
		if (c_it != word_cache.end())
		{
			res.insert(res.end(), c_it->second.begin(), c_it->second.end());
			continue;
		}

		// Fast path: if the whole word is directly in the vocabulary
		auto it = _encoder_map.find(word);
		if (it != _encoder_map.end())
		{
			std::vector<int> enc = { it->second };
			word_cache[word] = enc;
			res.push_back(it->second);
			continue;
		}

		// BPE decomposition & merge pass
		std::vector<std::string> symbols;
		for (char c : word) symbols.push_back(std::string(1, c));

		for (const auto& rule : _merges)
		{
			std::vector<std::string> new_symbols;
			for (size_t i = 0; i < symbols.size(); ++i)
			{
				if (i + 1 < symbols.size() && symbols[i] == rule.first && symbols[i + 1] == rule.second)
				{
					new_symbols.push_back(rule.first + rule.second);
					i++;
				}
				else
				{
					new_symbols.push_back(symbols[i]);
				}
			}
			symbols = std::move(new_symbols);
			if (symbols.size() <= 1) break;
		}

		std::vector<int> enc;
		enc.reserve(symbols.size());
		for (const auto& sym : symbols)
		{
			auto s_it = _encoder_map.find(sym);
			if (s_it != _encoder_map.end())
			{
				enc.push_back(s_it->second);
			}
			else
			{
				for (unsigned char uc : sym)
				{
					enc.push_back(static_cast<int>(uc));
				}
			}
		}

		word_cache[word] = enc;
		res.insert(res.end(), enc.begin(), enc.end());
	}

	return res;
}

auto Tokenizer::decode(const std::vector<int>& encoded) -> std::string
{
	std::string res;
	for (auto id : encoded)
	{
		auto it = _decoder_map.find(id);
		if (it != _decoder_map.end())
		{
			res += it->second;
		}
	}
	return res;
}

auto Tokenizer::get_vocab_size() -> int
{
	return this->_counter;
}

bool Tokenizer::save(const std::string& path)
{
	std::ofstream out(path, std::ios::binary);
	if (!out.is_open()) return false;

	// Write counter
	out.write(reinterpret_cast<const char*>(&_counter), sizeof(_counter));

	// Write encoder map size
	size_t map_size = _encoder_map.size();
	out.write(reinterpret_cast<const char*>(&map_size), sizeof(map_size));

	// Write encoder map
	for (const auto& pair : _encoder_map)
	{
		size_t str_len = pair.first.size();
		out.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
		out.write(pair.first.c_str(), str_len);
		out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
	}

	// Write merges size
	size_t merges_size = _merges.size();
	out.write(reinterpret_cast<const char*>(&merges_size), sizeof(merges_size));

	// Write merges
	for (const auto& rule : _merges)
	{
		size_t left_len = rule.first.size();
		out.write(reinterpret_cast<const char*>(&left_len), sizeof(left_len));
		out.write(rule.first.c_str(), left_len);

		size_t right_len = rule.second.size();
		out.write(reinterpret_cast<const char*>(&right_len), sizeof(right_len));
		out.write(rule.second.c_str(), right_len);
	}

	out.close();
	return true;
}

bool Tokenizer::load(const std::string& path)
{
	std::ifstream in(path, std::ios::binary);
	if (!in.is_open()) return false;

	_encoder_map.clear();
	_decoder_map.clear();
	_merges.clear();

	// Read counter
	in.read(reinterpret_cast<char*>(&_counter), sizeof(_counter));

	// Read encoder map
	size_t map_size;
	in.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));
	for (size_t i = 0; i < map_size; ++i)
	{
		size_t str_len;
		in.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
		std::string str(str_len, '\0');
		in.read(&str[0], str_len);
		int id;
		in.read(reinterpret_cast<char*>(&id), sizeof(id));
		
		_encoder_map[str] = id;
		_decoder_map[id] = str;
	}

	// Read merges
	size_t merges_size;
	in.read(reinterpret_cast<char*>(&merges_size), sizeof(merges_size));
	for (size_t i = 0; i < merges_size; ++i)
	{
		size_t left_len;
		in.read(reinterpret_cast<char*>(&left_len), sizeof(left_len));
		std::string left(left_len, '\0');
		in.read(&left[0], left_len);

		size_t right_len;
		in.read(reinterpret_cast<char*>(&right_len), sizeof(right_len));
		std::string right(right_len, '\0');
		in.read(&right[0], right_len);

		_merges.push_back({left, right});
	}

	in.close();
	return true;
}
