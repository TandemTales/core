#pragma once

#include <cstdint>
#include <random>
#include <gcem.hpp>
#include <limits>

namespace tt
{
    // Returns the next prime number after x. Returns 0 if there is no prime number after uX that fits in a uint32_t.
    constexpr std::uint32_t next_prime(std::uint32_t x);

    // constexpr and deterministic random number generator.
    template<class T>
    constexpr T det_rand_int(T seed, T max = std::numeric_limits<T>::max())
    {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        if (max == 0)
        {
            return 0;
        }
        return (seed * 1664525 + 1013904223) % max;
    }

    // Uses the device to generate a non-deterministic random number
    class c_rand
    {
    public:
        c_rand();

        template<class T>
        T rand_int(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
        {
            static_assert(std::is_integral_v<T>, "T must be an integral type");
            if (min > max)
            {
                std::swap(min, max);
            }
            std::uniform_int_distribution<T> dist(min, max);
            return dist(m_engine);
        }

        template<class T>
        T rand_float(T min, T max)
        {
            static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");
            if (min > max)
            {
                std::swap(min, max);
            }
            std::uniform_real_distribution<T> dist(min, max);
            return dist(m_engine);
        }

    private:
        std::random_device m_device;
        std::mt19937 m_engine;
    };

    template<class T>
    class c_vec2
    {
    public:
        constexpr c_vec2() : m_x(0), m_y(0) {}
        constexpr c_vec2(T x, T y) : m_x(x), m_y(y) {}

        template<typename U>
        constexpr c_vec2(const c_vec2<U>& other) : m_x(static_cast<T>(other.x())), m_y(static_cast<T>(other.y())) {}

        constexpr T x() const { return m_x; }
        constexpr T y() const { return m_y; }
        constexpr T& x() { return m_x; }
        constexpr T& y() { return m_y; }
        void x(T x) { m_x = x; }
        void y(T y) { m_y = y; }

        constexpr c_vec2<T> operator+(const c_vec2<T>& other) const
        {
            return c_vec2<T>(m_x + other.m_x, m_y + other.m_y);
        }

        constexpr c_vec2<T> operator-(const c_vec2<T>& other) const
        {
            return c_vec2<T>(m_x - other.m_x, m_y - other.m_y);
        }

        constexpr c_vec2<T> operator-() const
        {
            return c_vec2<T>(-m_x, -m_y);
        }

        constexpr c_vec2<T> operator*(const c_vec2<T>& other) const
        {
            return c_vec2<T>(m_x * other.m_x, m_y * other.m_y);
        }

        constexpr c_vec2<T> operator/(const c_vec2<T>& other) const
        {
            return c_vec2<T>(m_x / other.m_x, m_y / other.m_y);
        }

        constexpr c_vec2<T> operator*(T scalar) const
        {
            return c_vec2<T>(m_x * scalar, m_y * scalar);
        }

        constexpr c_vec2<T> operator/(T scalar) const
        {
            return c_vec2<T>(m_x / scalar, m_y / scalar);
        }

        constexpr c_vec2<T>& operator+=(const c_vec2<T>& other)
        {
            m_x += other.m_x;
            m_y += other.m_y;
            return *this;
        }

        constexpr c_vec2<T>& operator-=(const c_vec2<T>& other)
        {
            m_x -= other.m_x;
            m_y -= other.m_y;
            return *this;
        }

        constexpr c_vec2<T>& operator*=(const c_vec2<T>& other)
        {
            m_x *= other.m_x;
            m_y *= other.m_y;
            return *this;
        }

        constexpr c_vec2<T>& operator/=(const c_vec2<T>& other)
        {
            m_x /= other.m_x;
            m_y /= other.m_y;
            return *this;
        }

        constexpr c_vec2<T>& operator*=(T scalar)
        {
            m_x *= scalar;
            m_y *= scalar;
            return *this;
        }

        constexpr c_vec2<T>& operator/=(T scalar)
        {
            m_x /= scalar;
            m_y /= scalar;
            return *this;
        }

        constexpr bool operator==(const c_vec2<T>& rhs) const
        {
            return m_x == rhs.m_x && m_y == rhs.m_y;
        }

    private:
        T m_x;
        T m_y;
    };

    using c_vec2i = c_vec2<std::int32_t>;
    using c_vec2f = c_vec2<float>;

    class c_angle
    {
    public:
        static constexpr std::int16_t deg_0 = 0;
        static constexpr std::int16_t deg_45 = 8192;
        static constexpr std::int16_t deg_90 = 16384;
        static constexpr std::int32_t deg_180 = 32768;

        static constexpr c_angle from_rad(long double angle_rad);
        static constexpr c_angle from_deg(long double angle_deg);

        constexpr c_angle();
        constexpr c_angle(std::int16_t angle);

        constexpr std::int16_t angle() const;
        constexpr std::int16_t& angle();
        constexpr long double angle_rad() const;
        constexpr long double angle_deg() const;

        void set_rad(float angle_rad);
        void set_deg(float angle_deg);

        constexpr bool operator==(c_angle const& other) const;
        constexpr bool operator!=(c_angle const& other) const;
        constexpr c_angle operator+(c_angle const& other) const;
        constexpr c_angle operator-(c_angle const& other) const;
        constexpr c_angle operator-() const;
        c_angle& operator+=(c_angle const& other);
        c_angle& operator-=(c_angle const& other);

        [[nodiscard]] c_vec2f rot(c_vec2f const vec) const;
        [[nodiscard]] c_vec2i rot(c_vec2i const vec) const;

    private:
        std::int16_t m_angle;
    };

    bool overlaps(c_vec2i a_center, c_vec2i a_extents, c_vec2i b_center, c_vec2i b_extents);

    constexpr c_angle operator "" _deg(long double degrees);
    constexpr c_angle operator "" _deg(unsigned long long degrees);
    constexpr c_angle operator "" _rad(long double radians);
    constexpr c_angle operator "" _rad(unsigned long long radians);

    enum class e_compass_direction : std::uint8_t
    {
        n,
        ne,
        e,
        se,
        s,
        sw,
        w,
        nw,
        count
    };
}