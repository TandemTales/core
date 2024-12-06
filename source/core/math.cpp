#include "math.h"
#include <cmath>
#include <corecrt_math_defines.h>

namespace stk
{
    constexpr std::uint32_t next_prime(std::uint32_t x)
    {
        if (x < 2)
            return 2;

        if (x == 2)
            return 3;

        if (x >= 4294960003) // Largest prime number that fits in a uint32_t.
            return 0;

        uint32_t prime = (x + 1) | 1;
        while (true)
        {
            bool is_prime = true;
            for (uint32_t divisor = 3; divisor * divisor <= prime; divisor += 2)
            {
                if (prime % divisor == 0)
                {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime)
                return prime;

            prime += 2;
        }
    }

    c_rand::c_rand() : m_engine(m_device()) {}

    constexpr c_angle c_angle::from_rad(long double angle_rad)
    {
        return static_cast<std::int16_t>(angle_rad * static_cast<long double>(deg_180) / M_PI);
    }

    constexpr c_angle c_angle::from_deg(long double angle_deg)
    {
        return static_cast<std::int16_t>(angle_deg * static_cast<long double>(deg_45) / 45.0L);
    }

    constexpr c_angle::c_angle() : m_angle(0) {}
    constexpr c_angle::c_angle(std::int16_t angle) : m_angle(angle) {}

    constexpr std::int16_t c_angle::angle() const { return m_angle; }
    constexpr std::int16_t& c_angle::angle() { return m_angle; }

    constexpr long double c_angle::angle_rad() const
    {
        return static_cast<long double>(m_angle) * M_PI / static_cast<long double>(deg_180);
    }

    constexpr long double c_angle::angle_deg() const
    {
        return static_cast<long double>(m_angle) / static_cast<long double>(deg_45) * 45.0L;
    }

    void c_angle::set_rad(float angle_rad)
    {
        m_angle = static_cast<std::int16_t>(angle_rad * static_cast<float>(deg_180) / M_PI);
    }

    void c_angle::set_deg(float angle_deg)
    {
        m_angle = static_cast<std::int16_t>(angle_deg * static_cast<float>(deg_45) / 45.0f);
    }

    constexpr bool c_angle::operator==(c_angle const& other) const { return m_angle == other.m_angle; }
    constexpr bool c_angle::operator!=(c_angle const& other) const { return m_angle != other.m_angle; }
    constexpr c_angle c_angle::operator+(c_angle const& other) const { return c_angle(m_angle + other.m_angle); }
    constexpr c_angle c_angle::operator-(c_angle const& other) const { return c_angle(m_angle - other.m_angle); }
    constexpr c_angle c_angle::operator-() const { return c_angle(-m_angle); }

    c_angle& c_angle::operator+=(c_angle const& other)
    {
        m_angle += other.m_angle;
        return *this;
    }

    c_angle& c_angle::operator-=(c_angle const& other)
    {
        m_angle -= other.m_angle;
        return *this;
    }

    c_vec2f c_angle::rot(c_vec2f const vec) const
    {
        float x = vec.x();
        float y = vec.y();
        long double rad = angle_rad();
        auto cos_val = gcem::cos(rad);
        auto sin_val = gcem::sin(rad);
        return { static_cast<float>(cos_val * x - sin_val * y), static_cast<float>(sin_val * x + cos_val * y) };
    }

    c_vec2i c_angle::rot(c_vec2i const vec) const
    {
        int32_t x = vec.x();
        int32_t y = vec.y();
        long double rad = angle_rad();
        auto cos_val = gcem::cos(rad);
        auto sin_val = gcem::sin(rad);
        return { static_cast<int32_t>(cos_val * x - sin_val * y), static_cast<int32_t>(sin_val * x + cos_val * y) };
    }

    bool overlaps(c_vec2i a_center, c_vec2i a_extents, c_vec2i b_center, c_vec2i b_extents)
    {
        return std::abs(a_center.x() - b_center.x()) < a_extents.x() / 2 + b_extents.x() / 2
            && std::abs(a_center.y() - b_center.y()) < a_extents.y() / 2 + b_extents.y() / 2;
    }

    constexpr c_angle operator "" _deg(long double degrees)
    {
        return c_angle::from_deg(degrees);
    }

    constexpr c_angle operator "" _deg(unsigned long long degrees)
    {
        return c_angle::from_deg(static_cast<long double>(degrees));
    }

    constexpr c_angle operator "" _rad(long double radians)
    {
        return c_angle::from_rad(radians);
    }

    constexpr c_angle operator "" _rad(unsigned long long radians)
    {
        return c_angle::from_rad(static_cast<long double>(radians));
    }
}