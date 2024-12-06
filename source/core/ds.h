#pragma once

#include <array>
#include <utility>
#include <iterator>
#include <cstddef>

namespace tt
{
	template<class T, size_t N>
	class c_fixed_vector
	{
	public:
		c_fixed_vector()
			: m_count(0)
		{
		}

		bool append(T const& val)
		{
			if (m_count < N)
			{
				m_arr[m_count++] = val;
				return true;
			}
			return false;
		}

		template<class... Args>
		bool emplace(Args&&... args)
		{
			if (m_count < N)
			{
				m_arr[m_count++] = T(std::forward<Args>(args)...);
				return true;
			}
			return false;
		}

		bool remove_at_ordered(size_t i)
		{
			if (i < m_count)
			{
				for (size_t j = i; j < m_count - 1; ++j)
				{
					m_arr[j] = std::move(m_arr[j + 1]);
				}
				m_arr[m_count - 1].~T();
				--m_count;
				return true;
			}
			return false;
		}

		bool remove_at_unordered(size_t i)
		{
			if (i < m_count)
			{
				m_arr[i].~T();
				if (i != m_count - 1)
				{
					m_arr[i] = std::move(m_arr[m_count - 1]);
				}
				--m_count;
				return true;
			}
			return false;
		}

		T const& operator[](size_t i) const
		{
			return m_arr[i];
		}

		T& operator[](size_t i)
		{
			return m_arr[i];
		}

		size_t count() const
		{
			return m_count;
		}

		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			iterator(T* ptr) : ptr(ptr) {}

			reference operator*()
			{
				return *ptr;
			}

			pointer operator->()
			{
				return ptr;
			}

			iterator& operator++()
			{
				++ptr;
				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp = *this;
				++ptr;
				return tmp;
			}

			bool operator==(const iterator& other) const
			{
				return ptr == other.ptr;
			}

			bool operator!=(const iterator& other) const
			{
				return ptr != other.ptr;
			}

		private:
			T* ptr;
		};

		class const_iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;

			const_iterator(const T* ptr) : ptr(ptr) {}

			reference operator*() const
			{
				return *ptr;
			}

			pointer operator->() const
			{
				return ptr;
			}

			const_iterator& operator++()
			{
				++ptr;
				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++ptr;
				return tmp;
			}

			bool operator==(const const_iterator& other) const
			{
				return ptr == other.ptr;
			}

			bool operator!=(const const_iterator& other) const
			{
				return ptr != other.ptr;
			}

		private:
			const T* ptr;
		};

		iterator begin()
		{
			return iterator(&m_arr[0]);
		}

		iterator end()
		{
			return iterator(&m_arr[0] + m_count);
		}

		const_iterator begin() const
		{
			return const_iterator(&m_arr[0]);
		}

		const_iterator end() const
		{
			return const_iterator(&m_arr[0] + m_count);
		}

		const_iterator cbegin() const
		{
			return const_iterator(&m_arr[0]);
		}

		const_iterator cend() const
		{
			return const_iterator(&m_arr[0] + m_count);
		}

	private:
		std::array<T, N> m_arr;
		size_t m_count;
	};

} // namespace tt