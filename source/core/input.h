#pragma once

#include "core/hash.h"
#include "core/math.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>

namespace tt
{
	class c_input
	{
	public:
		void add(sf::Keyboard::Key key, c_hash action);
		void add(sf::Mouse::Button button, c_hash action);
		bool operator[](c_hash action) const;
		bool on(sf::Event const& event);
		void reset();
		c_vec2i mouse() const;

	private:
		std::unordered_map<sf::Keyboard::Key, c_hash> m_keys;
		std::unordered_map<sf::Mouse::Button, c_hash> m_mouse_buttons;
		std::unordered_map<c_hash, bool, s_hash_hasher> m_state;
		c_vec2i m_mouse;
	};
}