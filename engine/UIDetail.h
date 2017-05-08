/**
 * @file	UIDetail.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Helper classes
 */

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

		/**
		 * @brief Compare strategy class
		 * @tparam T Types to compare
		 */
		template <typename T>
		class Compare
		{
		public:
			/**
			 * @brief Compares two objects
			 * @param first First object
			 * @param second Second object
			 * @return True if equal
			 */
			bool operator()(const T& first, const T& second);
		};

		/**
		 * @brief Arbitary sized vector
		 * @tparam T Data type
		 * @tparam SIZE Size of vector
		 * @tparam CMP Compare strategy object
		 */
		template <typename T, size_t SIZE, class CMP = Compare<T>>
		class Vector
		{
			/**
			 * @brief Type definition of compare object.
			 */
			typedef CMP compare_type;

		public:
			//=================================================================
			// Constructors
			//=================================================================

			/**
			 * @brief Default constructor.
			 */
			Vector();

			/**
			 * @brief Initializer list constructor
			 * @param args Arguments to initialize object
			 */
			Vector(std::initializer_list<T> args);

			/**
			 * @brief Copy constructor
			 * @tparam OTHER_SIZE Size of source object
			 * @param other Source object
			 */
			template <size_t OTHER_SIZE, typename = typename std::enable_if<SIZE >= OTHER_SIZE>::type>
			explicit Vector(const Vector<T, OTHER_SIZE, CMP>& other);

			/**
			 * @brief Move constructor
			 * @param other Source object
			 */
			explicit Vector(Vector<T, SIZE, CMP>&& other) noexcept;

			//=================================================================
			// Destructor
			//=================================================================

			/**
			 * @brief Destructor
			 */
			virtual ~Vector();

			//=================================================================
			// Assignment Operators
			//=================================================================

			/**
			 * @brief Copy assignment operator
			 * @tparam OTHER_SIZE Size of other vector
			 * @param other Source object
			 * @return Reference to self.
			 */
			template <size_t OTHER_SIZE, typename = typename std::enable_if<SIZE >= OTHER_SIZE>::type>
			Vector <T, SIZE, CMP>& operator=(const Vector<T, OTHER_SIZE, CMP>& other) &;

			/**
			 * @brief Move assignment operator
			 * @param other Source object
			 * @return Reference to self.
			 */
			Vector<T, SIZE, CMP>& operator=(Vector<T, SIZE, CMP>&& other) & noexcept;

			//=================================================================
			// Swap
			//=================================================================

			/**
			 * @brief Member swap function
			 * @param other Source object
			 */
			void swap(Vector<T, SIZE, CMP>& other) noexcept;

			/**
			 * @brief Non-member swap function
			 * @param first First object
			 * @param second Second object
			 */
			friend void swap(Vector<T, SIZE, CMP>& first, Vector<T, SIZE, CMP>& second) noexcept;

			//=================================================================
			// Member Access
			//=================================================================

			/**
			 * @brief Getter for element at a specific location in vector
			 * @param i 
			 * @return 
			 */
			T at(size_t i) const;
			
			/**
			 * @brief Shortcut for accessing the first element
			 * @return First element
			 */
			T X() const;

			/**
			* @brief Shortcut for accessing the second element
			* @return Second element
			*/
			T Y() const;

			/**
			* @brief Shortcut for accessing the Third element
			* @return Third element
			*/
			T Z() const;				

			//=================================================================
			// Relational Operators
			//=================================================================

			/**
			 * @brief Equality operator
			 * @param rhs Other object
			 * @return True if equal
			 */
			bool operator==(const Vector<T, SIZE, CMP>& rhs) const;

			/**
			 * @brief Inequality operator
			 * @param rhs Other object
			 * @return True if not equal.
			 */
			bool operator!=(const Vector<T, SIZE, CMP>& rhs) const;

		protected:

			/**
			 * @brief Array containing the data
			 */
			T data[SIZE];
		};

		//=================================================================
		// Vector type specializations
		//=================================================================

		/**
		 * @brief Type specification of a vector of 2 ints.
		 */
		typedef Vector<int, 2> Vector2i;

		/**
		* @brief Type specification of a vector of 3 ints.
		*/
		typedef Vector<int, 3> Vector3i;

		/**
		* @brief Type specification of a point of 2 ints.
		*/
		typedef Vector<int, 2> Point2i;

		/**
		* @brief Type specification of a point of 3 ints.
		*/
		typedef Vector<int, 3> Point3i;

		/**
		* @brief Type specification of a vector of 2 unsigned ints.
		*/
		typedef Vector<unsigned int, 2> Vector2u;

		/**
		* @brief Type specification of a vector of 3 unsigned ints.
		*/
		typedef Vector<unsigned int, 3> Vector3u;

		/**
		* @brief Type specification of a point of 2 unsigned ints.
		*/
		typedef Vector<unsigned int, 2> Point2u;

		/**
		* @brief Type specification of a point of 3 unsigned ints.
		*/
		typedef Vector<unsigned int, 3> Point3u;

		/**
		* @brief Type specification of a vector of 2 floats.
		*/
		typedef Vector<float, 2> Vector2f;

		/**
		* @brief Type specification of a vector of 3 floats.
		*/
		typedef Vector<float, 3> Vector3f;

		/**
		* @brief Type specification of a point of 2 floats.
		*/
		typedef Vector<float, 2> Point2f;

		/**
		* @brief Type specification of a point of 3 floats.
		*/
		typedef Vector<float, 3> Point3f;

		/**
		* @brief Type specification of a vector of 2 doubles.
		*/
		typedef Vector<double, 2> Vector2d;

		/**
		* @brief Type specification of a vector of 3 doubles.
		*/
		typedef Vector<double, 3> Vector3d;

		/**
		* @brief Type specification of a point of 2 doubles.
		*/
		typedef Vector<double, 2> Point2d;

		/**
		* @brief Type specification of a point of 3 doubles.
		*/
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
		bool Vector<T, SIZE, CMP>::operator==(const Vector<T, SIZE, CMP>& rhs) const
		{
			for (size_t i{ 0 }; i < SIZE; ++i)
			{
				if (!CMP(data[i], rhs.data[i]))
					return false;
			}

			return true;
		}

		template <typename T, size_t SIZE, class CMP>
		bool Vector<T, SIZE, CMP>::operator!=(const Vector<T, SIZE, CMP>& rhs) const
		{
			return !(*this == rhs);
		}

		template<typename T, size_t SIZE, class CMP>
		inline void swap(Vector<T, SIZE, CMP>& first, Vector<T, SIZE, CMP>& second) noexcept
		{
			std::swap(first.data, second.data);
		}

		//=================================================================
		// Rectangle
		//=================================================================

		/**
		 * @brief Class Representing a rectangle
		 * @tparam T Data type
		 */
		template <typename T>
		class Rectangle
		{
		public:

			//=================================================================
			// Constructors
			//=================================================================

			/**
			 * @brief Default constructor.
			 */
			Rectangle() = default;

			/**
			 * @brief Constructor
			 * @param width Width of the rectangle
			 * @param height Height of the rectangle
			 */
			Rectangle(T width, T height);

			/**
			 * @brief Constructor
			 * @param width Width of the rectangle
			 * @param height Height of the rectangle
			 * @param posX X-position of the bottom left corner.
			 * @param posY Y-position of the bottom left corner.
			 */
			Rectangle(T width, T height, T posX, T posY);

			/**
			 * @brief Copy Constructor
			 * @param other Source object
			 */
			explicit Rectangle(const Rectangle<T>& other);

			/**
			 * @brief Move Constructor
			 * @param other Source object
			 */
			explicit Rectangle(Rectangle<T>&& other) noexcept;

			//=================================================================
			// Destructor
			//=================================================================

			/**
			 * @brief Destructor
			 */
			virtual ~Rectangle();

			//=================================================================
			// Assignment Operators
			//=================================================================

			/**
			 * @brief Copy assignment operator.
			 * @param other Source object
			 * @return Reference to self.
			 */
			Rectangle<T>& operator=(const Rectangle<T>& other) &;

			/**
			 * @brief Move assignment operator.
			 * @param other Source object
			 * @return Reference to self.
			 */
			Rectangle<T>& operator=(Rectangle<T>&& other) & noexcept;

			//=================================================================
			// Swap
			//=================================================================

			/**
			 * @brief Member swap function.
			 * @param other Source object.
			 */
			void swap(Rectangle<T>& other) noexcept;

			/**
			 * @brief Non-member swap function
			 * @param first First object
			 * @param second Second object
			 */
			friend void swap(Rectangle<T>& first, Rectangle<T>& second) noexcept;

			//=================================================================
			// Member Access
			//=================================================================

			/**
			 * @brief Getter for rectangle width
			 * @return Width
			 */
			T getWidth() const;

			/**
			 * @brief Getter for rectangle height
			 * @return Height
			 */
			T getHeight() const;
			
			/**
			 * @brief Getter for rectangle x position
			 * @return X position
			 */
			T getPosX() const;
			
			/**
			 * @brief Getter for rectangle y position
			 * @return Y position
			 */
			T getPosY() const;

			/**
			 * @brief Setter for width
			 * @param width New Width
			 */
			void setWidth(T width);
			
			/**
			 * @brief Setter for height
			 * @param height New height.
			 */
			void setHeight(T height);
			
			/**
			 * @brief Setter for x-postion
			 * @param posX New position
			 */
			void setPosX(T posX);
			
			/**
			 * @brief Setter for y-position
			 * @param posY New position
			 */
			void setPosY(T posY);

			/**
			 * @brief Gets the size of the object
			 * @return Vector containing size
			 */
			Vector<T, 2> getSize() const;
			
			/**
			 * @brief Gets the position of the object
			 * @return Vector containing position
			 */
			Vector<T, 2> getPosition() const;

			/**
			 * @brief Sets the size.
			 * @param size New size.
			 */
			void setSize(const Vector<T, 2>& size);
			
			/**
			 * @brief Sets the position
			 * @param position New position
			 */
			void setPosition(const Vector<T, 2>& position);

			//=================================================================
			// Other
			//=================================================================

			/**
			 * @brief Checks whether the rectangle contains the point p
			 * @param point Point to check
			 * @return True if check passed.
			 */
			bool contains(const Vector<T, 2>& point) const;

			/**
			 * @brief Checks if the rectangle intersects another rectangle.
			 * @param other Other rectangle
			 * @return True if intersects
			 */
			bool intersects(const Rectangle<T>& other) const;

			/**
			 * @brief Checks if the rectangle intersects another rectangle and 
			 * returns intersection size.
			 * 
			 * @param other Other rectangle.
			 * @param intersection Rectangle to fill in intersection data in.
			 * @return True if intersects.
			 */
			bool intersects(const Rectangle<T>& other, Rectangle<T>& intersection) const;

		protected:
			/**
			 * @brief Width of rectangle
			 */
			T _width{};

			/**
			 * @brief Height of rectangle
			 */
			T _height{};

			/**
			 * @brief X-position of rectangle.
			 */
			T _posX{};

			/**
			 * @brief Y-position of rectangle.
			 */
			T _posY{};
		};

		//=================================================================
		// Rectangle type specializations.
		//=================================================================

		/**
		 * @brief Type definition of a rectangle of ints.
		 */
		typedef Rectangle<int> IntRect;

		/**
		 * @brief Type definition of a rectangle of floats.
		 */
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
		void Rectangle<T>::setSize(const Vector<T, 2>& size)
		{
			_width = size.X();
			_height = size.Y();
		}

		template <typename T>
		void Rectangle<T>::setPosition(const Vector<T, 2>& position)
		{
			_posX = position.X();
			_posY = position.Y();
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
		inline void swap(Rectangle<T>& first, Rectangle<T>& second) noexcept
		{
			std::swap(first._width, second._width);
			std::swap(first._height, second._height);
			std::swap(first._posX, second._posX);
			std::swap(first._posY, second._posY);
		}
	}
}
