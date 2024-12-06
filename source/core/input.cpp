#include "input.h"

namespace tt
{
	void c_input::add(sf::Keyboard::Key key, c_hash action)
	{
		m_keys[key] = action;
		m_state[action] = false;
	}

	void c_input::add(sf::Mouse::Button button, c_hash action)
	{
		m_mouse_buttons[button] = action;
		m_state[action] = false;
	}

	bool c_input::operator[](c_hash action) const
	{
		auto it = m_state.find(action);
		if (it != m_state.end())
		{
			return it->second;
		}
		return false;
	}

	bool c_input::on(sf::Event const& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			auto it = m_keys.find(event.key.code);
			if (it != m_keys.end())
			{
				m_state[it->second] = true;
				return true;
			}
			return false;
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			auto it = m_keys.find(event.key.code);
			if (it != m_keys.end())
			{
				m_state[it->second] = false;
				return true;
			}
			return false;
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			auto it = m_mouse_buttons.find(event.mouseButton.button);
			if (it != m_mouse_buttons.end())
			{
				m_state[it->second] = true;
				return true;
			}
			return false;
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			auto it = m_mouse_buttons.find(event.mouseButton.button);
			if (it != m_mouse_buttons.end())
			{
				m_state[it->second] = false;
				return true;
			}
			return false;
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			m_mouse = { event.mouseMove.x, event.mouseMove.y };
			return true;
		}
		return false;
	}

	void c_input::reset()
	{
		for (auto& [_, state] : m_state)
		{
			state = false;
		}
	}

	c_vec2i c_input::mouse() const
	{
		return m_mouse;
	}
}