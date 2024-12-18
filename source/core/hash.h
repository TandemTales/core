#pragma once
#include <cstdint>
#include <string>

namespace tt
{
	// Forward declarations
	constexpr std::uint32_t murmur_hash3(const char* key, std::uint32_t len, std::uint32_t seed = 0);
	constexpr std::uint32_t fnv1a_hash(const char* key, std::uint32_t len, std::uint32_t seed = 0);
	constexpr std::uint32_t universal_hash(const char* str, std::uint32_t seed = 0);

	class c_hash
	{
	public:
		constexpr c_hash() : m_hash(0) {}
		constexpr c_hash(uint32_t h) : m_hash(h) {}
		constexpr c_hash(char const* key, std::uint32_t len);
		constexpr c_hash(std::string const& key);

		operator uint32_t() const { return m_hash; }
		constexpr bool operator==(const c_hash& rhs) const;
		constexpr bool operator==(std::uint32_t rhs) const;
		constexpr c_hash operator+(std::string const& key) const;

	public:
		// Intentionally make this public to allow CHashes to be used as non-type template parameters.
		uint32_t m_hash;
	};

	// Hash function implementation (needs to be in header for constexpr)
	constexpr std::uint32_t murmur_hash3(const char* key, std::uint32_t len, std::uint32_t seed)
	{
		constexpr std::uint32_t c1 = 0xcc9e2d51;
		constexpr std::uint32_t c2 = 0x1b873593;
		constexpr std::uint32_t r1 = 15;
		constexpr std::uint32_t r2 = 13;
		constexpr std::uint32_t m = 5;
		constexpr std::uint32_t n = 0xe6546b64;

		std::uint32_t hash = seed;

		const int numBlocks = len / 4;
		const std::uint32_t* blocks = (const std::uint32_t*)key;

		for (int i = 0; i < numBlocks; i++) {
			std::uint32_t k = blocks[i];
			k *= c1;
			k = (k << r1) | (k >> (32 - r1));
			k *= c2;

			hash ^= k;
			hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
		}

		const std::uint8_t* tail = (const std::uint8_t*)(key + numBlocks * 4);
		std::uint32_t k1 = 0;

		switch (len & 3) {
		case 3: k1 ^= tail[2] << 16;
		case 2: k1 ^= tail[1] << 8;
		case 1: k1 ^= tail[0];
			k1 *= c1; k1 = (k1 << r1) | (k1 >> (32 - r1)); k1 *= c2; hash ^= k1;
		}

		hash ^= len;
		hash ^= (hash >> 16);
		hash *= 0x85ebca6b;
		hash ^= (hash >> 13);
		hash *= 0xc2b2ae35;
		hash ^= (hash >> 16);

		return hash;
	}

	constexpr std::uint32_t fnv1a_hash(const char* key, std::uint32_t len, std::uint32_t seed)
	{
		constexpr std::uint32_t fnv_prime = 16777619;
		std::uint32_t hash = seed;

		for (std::uint32_t i = 0; i < len; i++)
		{
			hash ^= key[i];
			hash *= fnv_prime;
		}

		return hash;
	}

	constexpr std::uint32_t universal_hash(const char* str, std::uint32_t seed)
	{
		constexpr std::uint32_t mod = 1'000'000'009;
		constexpr std::uint32_t base = 257;

		std::uint32_t hash = seed;
		for (std::uint32_t i = 0; str[i] != '\0'; ++i)
		{
			hash = (hash * base + str[i]) % mod;
		}
		return hash;
	}

	// c_hash inline implementations
	constexpr c_hash::c_hash(char const* key, std::uint32_t len)
		: c_hash(fnv1a_hash(key, len))
	{
	}

	constexpr c_hash::c_hash(std::string const& key)
		: c_hash(fnv1a_hash(key.c_str(), static_cast<uint32_t>(key.size())))
	{
	}

	constexpr bool c_hash::operator==(const c_hash& rhs) const
	{
		return m_hash == rhs.m_hash;
	}

	constexpr bool c_hash::operator==(std::uint32_t rhs) const
	{
		return m_hash == rhs;
	}

	constexpr c_hash c_hash::operator+(std::string const& key) const
	{
		return c_hash(fnv1a_hash(key.c_str(), static_cast<uint32_t>(key.size()), m_hash));
	}

	// Hash functor for STL containers
	struct s_hash_hasher
	{
		std::size_t operator()(const c_hash& h) const
		{
			return h.m_hash;
		}
	};

	// User-defined literal
	constexpr c_hash operator "" _h(const char* key, size_t len)
	{
		return c_hash(key, static_cast<uint32_t>(len));
	}

	// Template specialization declaration
	template<class T>
	constexpr c_hash hash()
	{
		static_assert(std::is_same<T, T>{} == false, "Invalid type for hash");
	}

	// Static assertions (can be moved to a test file if preferred)
	constexpr c_hash foo_hash = c_hash("foo", 3);
	static_assert(foo_hash == fnv1a_hash("foo", 3));
	static_assert(c_hash("foobaz", 6) == c_hash("foo", 3) + std::string("baz"));
	static_assert(c_hash("foobaz", 6) == c_hash("foobaz"));
	static_assert("foo"_h == c_hash("foo"));
}