#pragma once

#include <stdexcept>
#include <string>

namespace math
{
	/**
	* @brief Basic RSP eception class
	*/
	class Exception :
		public std::runtime_error
	{
	public:
		Exception(const std::string& m)
			: std::runtime_error(m),
			type_{ "Basic" }
		{

		}
		virtual const std::string& type() const
		{
			return type_;
		};

	protected:
		std::string type_;
	};

	/**
	* @brief Exception invalid values
	*/
	class ExceptionInvalidValue :
		public Exception
	{
	public:
		ExceptionInvalidValue(const std::string& m)
			: Exception(m)
		{
			type_ = "InvalidValue";
		}
	};

	/**
	* @brief Exception index out of bound
	*/
	class ExceptionIndexOutOfBounds :
		public Exception
	{
	public:
		ExceptionIndexOutOfBounds(const std::string& m)
			: Exception(m)
		{
			type_ = "IndexOutOfBounds";
		}
	};

	/**
	* @brief Exception non-square matrix
	*/
	class ExceptionNonSquareMatrix :
		public Exception
	{
	public:
		ExceptionNonSquareMatrix(const std::string& m)
			: Exception(m)
		{
			type_ = "NonSquareMatrixDecomposition";
		}
	};

	/**
	* @brief Exception degenerate matrix
	*/
	class ExceptionDegenerateMatrix:
		public Exception
	{
	public:
		ExceptionDegenerateMatrix(const std::string& m)
			: Exception(m)
		{
			type_ = "DegenerateMatrix";
		}
	};

	/**
	* @brief Exception incorrect matrix
	*/
	class ExceptionIncorrectMatrix :
		public Exception
	{
	public:
		ExceptionIncorrectMatrix(const std::string& m)
			: Exception(m)
		{
			type_ = "IncorrectMatrix";
		}
	};

	/**
	* @brief Exception too many iterations
	*/
	class ExceptionTooManyIterations :
		public Exception
	{
	public:
		ExceptionTooManyIterations(const std::string& m)
			: Exception(m)
		{
			type_ = "TooManyIterations";
		}
	};

	/**
	* @brief Exception non-equal columns number
	*/
	class ExceptionNonEqualColumnsNum :
		public ExceptionIncorrectMatrix
	{
	public:
		ExceptionNonEqualColumnsNum(const std::string& m)
			: ExceptionIncorrectMatrix(m)
		{
			type_ = "NonEqualColumnsNum";
		}
	};

	/**
	* @brief Exception non-equal rows number
	*/
	class ExceptionNonEqualRowsNum :
		public ExceptionIncorrectMatrix
	{
	public:
		ExceptionNonEqualRowsNum(const std::string& m)
			: ExceptionIncorrectMatrix(m)
		{
			type_ = "NonEqualRowsNum";
		}
	};

	/**
	* @brief Exception non-row vector
	*/
	class ExceptionNonRowVector :
		public ExceptionIncorrectMatrix
	{
	public:
		ExceptionNonRowVector(const std::string& m)
			: ExceptionIncorrectMatrix(m)
		{
			type_ = "NonRowVector";
		}
	};

	/**
	* @brief Exception non-column vector
	*/
	class ExceptionNonColumnVector :
		public ExceptionIncorrectMatrix
	{
	public:
		ExceptionNonColumnVector(const std::string& m)
			: ExceptionIncorrectMatrix(m)
		{
			type_ = "NonColumnVector";
		}
	};
}