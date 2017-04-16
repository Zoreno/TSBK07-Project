#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <map>

namespace userinterface {
	namespace detail {

		//=================================================================
		// Vector
		//=================================================================

		template <typename T>
		class Compare
		{
		public:
			bool operator()(const T& first, const T& second);
		};

		template <typename T, size_t SIZE, class CMP = Compare<T>>
		class Vector
		{
			typedef CMP compare_type;

		public:
			//=================================================================
			// Constructors
			//=================================================================

			Vector();
			Vector(std::initializer_list<T> args);
			template <size_t OTHER_SIZE, typename = typename std::enable_if<SIZE >= OTHER_SIZE>::type>
			explicit Vector(const Vector<T, OTHER_SIZE, CMP>& other);
			explicit Vector(Vector<T, SIZE, CMP>&& other) noexcept;

			//=================================================================
			// Destructor
			//=================================================================

			virtual ~Vector();

			//=================================================================
			// Assignment Operators
			//=================================================================

			template <size_t OTHER_SIZE, typename = typename std::enable_if<SIZE >= OTHER_SIZE>::type>
			Vector <T, SIZE, CMP>& operator=(const Vector<T, OTHER_SIZE, CMP>& other) &;

			Vector<T, SIZE, CMP>& operator=(Vector<T, SIZE, CMP>&& other) & noexcept;

			//=================================================================
			// Swap
			//=================================================================

			void swap(Vector<T, SIZE, CMP>& other) noexcept;
			friend void swap(Vector<T, SIZE, CMP>& first, Vector<T, SIZE, CMP>& second) noexcept;

			//=================================================================
			// Member Access
			//=================================================================

			T at(size_t i) const;
			
			T X() const;

			T Y() const;

			T Z() const;				

			//=================================================================
			// Relational Operators
			//=================================================================

			bool operator==(const Vector<T, SIZE, CMP>& rhs);

		protected:
			T data[SIZE];
		};

		//=================================================================
		// Vector type specializations
		//=================================================================

		typedef Vector<int, 2> Vector2i;
		typedef Vector<int, 3> Vector3i;

		typedef Vector<int, 2> Point2i;
		typedef Vector<int, 3> Point3i;

		typedef Vector<unsigned int, 2> Vector2u;
		typedef Vector<unsigned int, 3> Vector3u;

		typedef Vector<unsigned int, 2> Point2u;
		typedef Vector<unsigned int, 3> Point3u;

		typedef Vector<float, 2> Vector2f;
		typedef Vector<float, 3> Vector3f;

		typedef Vector<float, 2> Point2f;
		typedef Vector<float, 3> Point3f;

		typedef Vector<double, 2> Vector2d;
		typedef Vector<double, 3> Vector3d;

		typedef Vector<double, 2> Point2d;
		typedef Vector<double, 3> Point3d;

		//=================================================================
		// Vector implementation
		//=================================================================

		template <typename T>
		bool Compare<T>::operator()(const T& first, const T& second)
		{
			return first == second;
		}

		template <typename T, size_t SIZE, class CMP>
		Vector<T, SIZE, CMP>::Vector()
		{
			for (size_t i{ 0 }; i < SIZE; ++i)
			{
				data[i] = T{};
			}
		}

		template <typename T, size_t SIZE, class CMP>
		Vector<T, SIZE, CMP>::Vector(std::initializer_list<T> args)
		{
			if (args.size() > SIZE)
				throw std::out_of_range{ "Too many arguments" };

			size_t counter{ 0 };
			for (T i : args)
			{
				data[counter++] = i;
			}
		}

		template <typename T, size_t SIZE, class CMP>
		template <size_t OTHER_SIZE, typename>
		Vector<T, SIZE, CMP>::Vector(const Vector<T, OTHER_SIZE, CMP>& other)
		{
			size_t i{ 0 };
			for (; i < OTHER_SIZE; ++i)
			{
				data[i] = other.data[i];
			}
			for (;i < SIZE; ++i)
			{
				data[i] = T{};
			}
		}

		template <typename T, size_t SIZE, class CMP>
		Vector<T, SIZE, CMP>::Vector(Vector<T, SIZE, CMP>&& other) noexcept
		{
			swap(*this, other);
		}

		template <typename T, size_t SIZE, class CMP>
		Vector<T, SIZE, CMP>::~Vector() {}

		template <typename T, size_t SIZE, class CMP>
		template <size_t OTHER_SIZE, typename>
		Vector<T, SIZE, CMP>& Vector<T, SIZE, CMP>::operator=(const Vector<T, OTHER_SIZE, CMP>& other) &
		{
			size_t i{ 0 };
			for (; i < OTHER_SIZE; ++i)
			{
				data[i] = other.data[i];
			}
			for (;i < SIZE; ++i)
			{
				data[i] = T{};
			}

			return *this;
		}

		template <typename T, size_t SIZE, class CMP>
		Vector<T, SIZE, CMP>& Vector<T, SIZE, CMP>::operator=(Vector<T, SIZE, CMP>&& other) & noexcept
		{
			swap(*this, other);

			return *this;
		}

		template <typename T, size_t SIZE, class CMP>
		void Vector<T, SIZE, CMP>::swap(Vector<T, SIZE, CMP>& other) noexcept
		{
			swap(*this, other);
		}

		template <typename T, size_t SIZE, class CMP>
		T Vector<T, SIZE, CMP>::at(size_t i) const
		{
			if (i >= SIZE)
				throw std::out_of_range("Index out of range");

			return data[i];
		}

		template <typename T, size_t SIZE, class CMP>
		T Vector<T, SIZE, CMP>::X() const
		{
			if (SIZE < 1)
				throw std::out_of_range("Vector too small");

			return data[0];
		}

		template <typename T, size_t SIZE, class CMP>
		T Vector<T, SIZE, CMP>::Y() const
		{
			if (SIZE < 2)
				throw std::out_of_range("Vector too small");

			return data[1];
		}

		template <typename T, size_t SIZE, class CMP>
		T Vector<T, SIZE, CMP>::Z() const
		{
			if (SIZE < 3)
				throw std::out_of_range("Vector too small");

			return data[2];
		}

		template <typename T, size_t SIZE, class CMP>
		bool Vector<T, SIZE, CMP>::operator==(const Vector<T, SIZE, CMP>& rhs)
		{
			for (size_t i{ 0 }; i < SIZE; ++i)
			{
				if (!CMP(data[i], rhs.data[i]))
					return false;
			}

			return true;
		}

		template<typename T, size_t SIZE, class CMP>
		void swap(Vector<T, SIZE, CMP>& first, Vector<T, SIZE, CMP>& second) noexcept
		{
			std::swap(first.data, second.data);
		}

		//=================================================================
		// Rectangle
		//=================================================================

		template <typename T>
		class Rectangle
		{
		public:

			//=================================================================
			// Constructors
			//=================================================================

			Rectangle() = default;
			Rectangle(T width, T height);
			Rectangle(T width, T height, T posX, T posY);
			Rectangle(const Rectangle<T>& other);
			Rectangle(Rectangle<T>&& other) noexcept;

			//=================================================================
			// Destructor
			//=================================================================

			virtual ~Rectangle();

			//=================================================================
			// Assignment Operators
			//=================================================================

			Rectangle<T>& operator=(const Rectangle<T>& other) &;
			Rectangle<T>& operator=(Rectangle<T>&& other) & noexcept;

			//=================================================================
			// Swap
			//=================================================================

			void swap(Rectangle<T>& other) noexcept;
			friend void swap(Rectangle<T>& first, Rectangle<T>& second) noexcept;

			//=================================================================
			// Member Access
			//=================================================================

			T getWidth() const;
			T getHeight() const;
			T getPosX() const;
			T getPosY() const;

			void setWidth(T width);
			void setHeight(T height);
			void setPosX(T posX);
			void setPosY(T posY);

			Vector<T, 2> getSize() const;
			Vector<T, 2> getPosition() const;

			//=================================================================
			// Other
			//=================================================================

			bool contains(const Vector<T, 2>& point) const;

			bool intersects(const Rectangle<T>& other) const;
			bool intersects(const Rectangle<T>& other, Rectangle<T>& intersection) const;

		protected:
			T _width{};
			T _height{};

			T _posX{};
			T _posY{};
		};

		//=================================================================
		// Rectangle type specializations.
		//=================================================================

		typedef Rectangle<int> IntRect;
		typedef Rectangle<float> FloatRect;

		//=================================================================
		// rectangle Implementation.
		//=================================================================

		template <typename T>
		Rectangle<T>::Rectangle(T width, T height)
			:_width{ width }, _height{ height } {}

		template <typename T>
		Rectangle<T>::Rectangle(T width, T height, T posX, T posY)
			: _width{ width }, _height{ height }, _posX{ posX }, _posY{ posY } {}

		template <typename T>
		Rectangle<T>::Rectangle(const Rectangle<T>& other)
			: _width{ other._width }, _height{ other._height }, _posX{ other._posX }, _posY{ other._posY } {}

		template <typename T>
		Rectangle<T>::Rectangle(Rectangle<T>&& other) noexcept
		{
			swap(*this, other);
		}

		template <typename T>
		Rectangle<T>::~Rectangle() {}

		template <typename T>
		Rectangle<T>& Rectangle<T>::operator=(const Rectangle<T>& other) &
		{
			_width = other._width;
			_height = other._height;
			_posX = other._posX;
			_posY = other._posY;

			return *this;
		}

		template <typename T>
		Rectangle<T>& Rectangle<T>::operator=(Rectangle<T>&& other) & noexcept
		{
			swap(*this, other);

			return *this;
		}

		template <typename T>
		void Rectangle<T>::swap(Rectangle<T>& other) noexcept
		{
			swap(*this, other);
		}

		template <typename T>
		T Rectangle<T>::getWidth() const
		{
			return _width;
		}

		template <typename T>
		T Rectangle<T>::getHeight() const
		{
			return _height;
		}

		template <typename T>
		T Rectangle<T>::getPosX() const
		{
			return _posX;
		}

		template <typename T>
		T Rectangle<T>::getPosY() const
		{
			return _posY;
		}

		template <typename T>
		void Rectangle<T>::setWidth(T width)
		{
			_width = width;
		}

		template <typename T>
		void Rectangle<T>::setHeight(T height)
		{
			_height = height;
		}

		template <typename T>
		void Rectangle<T>::setPosX(T posX)
		{
			_posX = posX;
		}

		template <typename T>
		void Rectangle<T>::setPosY(T posY)
		{
			_posY = posY;
		}

		template <typename T>
		Vector<T, 2> Rectangle<T>::getSize() const
		{
			return Vector<T, 2>(_width, _height);
		}

		template <typename T>
		Vector<T, 2> Rectangle<T>::getPosition() const
		{
			return Vector<T, 2>(_posX, _posY);
		}

		template <typename T>
		bool Rectangle<T>::contains(const Vector<T, 2>& point) const
		{
			// Right of right side
			if (point.X() >= (_posX + _width))
				return false;

			// Left of left side
			if (point.X() < _posX)
				return false;

			// Above top
			if (point.Y() >= (_posY + _height))
				return false;

			// Under Bottom
			if (point.Y() < _posY)
				return false;

			// Else, inside
			return true;

		}

		template <typename T>
		bool Rectangle<T>::intersects(const Rectangle<T>& other) const
		{
			T left = std::max(_posX, other._posX);
			T right = std::min(_posX + _width, other._posX + other._width);

			T bottom = std::max(_posY, other._posY);
			T top = std::min(_posY + _height, other._posY + other._height);

			return (left < right) && (bottom < top);
		}

		template <typename T>
		bool Rectangle<T>::intersects(const Rectangle<T>& other, Rectangle<T>& intersection) const
		{
			T left = std::max(_posX, other._posX);
			T right = std::min(_posX + _width, other._posX + other._width);

			T bottom = std::max(_posY, other._posY);
			T top = std::min(_posY + _height, other._posY + other._height);

			if ((left < right) && (bottom < top))
			{
				intersection._posX = left;
				intersection._posY = bottom;
				intersection._width = (right - left);
				intersection._height = (top - bottom);

				return true;
			}

			return false;
		}

		template <typename T>
		void swap(Rectangle<T>& first, Rectangle<T>& second) noexcept
		{
			std::swap(first._width, second._width);
			std::swap(first._height, second._height);
			std::swap(first._posX, second._posX);
			std::swap(first._posY, second._posY);
		}
	}
}
