#pragma once
#include <libmath/matrix.h>
#include <libmath/boolean.h>
#include <libmath/math_exception.h>
#include <type_traits>
#include <vector>
#include <algorithm>

namespace math
{
	template<typename T>
	class Polygone;

	template<typename T>
	class Edge;

	/**
	* @brief Class for n-dimension node (point)
	*/
	template<typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class Node
	{
	protected:
		friend Polygone<T>;
		friend Edge<T>;

		/// @brief Node dimension
		size_t dim_ = 0;

		/// @brief Vector of Polygones, contains this node
		std::vector<Polygone<T>*> polygones_;

		/// @brief Vector of Edges, contains this node
		std::vector<Edge<T>*> edges_;

		/// @brief vector of adjacent nodes
		std::vector<Node<T>*> adj_nodes_;

		/// @brief Point coordinates (row-vector)
		Matrix<T> coordinates_;

		/**
		* @brief Add polygone, which contains this node
		* @details Method doesnt add polygone, if polygone already added
		* @param p: New polygone
		*/
		void addPolygone(Polygone<T>* p)
		{
			if (p == nullptr)
			{
				throw(ExceptionInvalidValue("Node<T>.addPolygone: try to add NULL polygone"));
			}

			if (std::find(polygones_.begin(), polygones_.end(), p) == polygones_.end())
			{
				polygones_.push_back(p);
			}
		};

		/**
		* @brief Add edge, which contains this node
		* @details Method doesnt add edge, if edge already added
		* @param e: New edge
		*/
		void addEdge(Edge<T>* e)
		{
			if (e == nullptr)
			{
				throw(ExceptionInvalidValue("Node<T>.addEdge: try to add NULL edge"));
			}

			if (std::find(edges_.begin(), edges_.end(), e) == edges_.end())
			{
				edges_.push_back(e);
			}
		};

		/**
		* @brief Add adjucent node
		* @details Method doesnt add node, if node already added
		* @param n: Node
		*/
		void addAdjucentNode(Node<T>* n)
		{
			if (n == nullptr)
			{
				throw(ExceptionInvalidValue("Node<T>.addAdjucentNode: try to add NULL node"));
			}

			if (std::find(adj_nodes_.begin(), adj_nodes_.end(), n) == adj_nodes_.end())
			{
				adj_nodes_.push_back(n);
			}
		};
	public:
		/**
		* @brief Node constructor
		* @param coord: Column vector of node coordinates
		*/
		Node(const Matrix<T>& coord) :
			coordinates_(coord), 
			dim_(coord.rows())
		{
			if (coordinates_.cols() != 1)
			{
				throw(ExceptionNonColumnVector(
					"Node<T>: try to set (" + 
					std::to_string(coordinates_.rows()) + " by " +
					std::to_string(coordinates_.cols()) +
					") matrix as coordinates, but column matrix n by 1 required"));
			}
		};


		/**
		* @brief Set new coordinates to existing node
		* @param coord: Column vector. Number of rows in coord is equal to the new node dimensionality
		*/
		void setCoordinates(const Matrix<T>& coord)
		{
			if (coord.cols() != 1)
			{
				throw(ExceptionNonColumnVector(
					"Node<T>.setCoordinates: try to set (" +
					std::to_string(coord.rows()) + " by " +
					std::to_string(coord.cols()) +
					") matrix as coordinates, but column matrix (n by 1) required"));
			}

			coordinates_ = coord;

			dim_ = coord.rows();
		};

		/**
		* @brief Get array of polygones, which contains this node
		* @param[out] poly: Array of polygones
		*/
		void getPolygones(std::vector<Polygone<T>*>& poly) const
		{
			poly = polygones_;
		};

		/**
		* @brief Get array of edges, which contains this node
		* @param[out] edges: Array of edges
		*/
		void getEdges(std::vector<Edge<T>*>& edges) const
		{
			edges = edges_;
		};

		/**
		* @brief Get node coordinates
		* @param[out] M: Column vector of node coordinates
		*/
		void getCoordinates(Matrix<T>& M) const
		{
			M = coordinates_;
		};

	};
}