#pragma once
#include <libmath/boolean.h>
#include <libmath/geometry/node.h>
#include <libmath/geometry/edge.h>
#include <libmath/math_exception.h>
#include <type_traits>
#include <vector>
#include <string>
#include <algorithm>

namespace math
{
	/**
	* @brief Class for n-dimensional polygone
	*/
	template <typename T>
	class Polygone
	{
	protected:
		/// @brief vector of nodes in this polygone
		std::vector<Node<T>*> nodes_;

		/// @brief vector of edges in this polygone
		std::vector<Edge<T>*> edges_;

		/// @brief vector of adjacent polygones
		std::vector<Polygone<T>*> adj_polygones_;

		/// @brief polygone dimension
		size_t dim_ = 0;

		/**
		* @brief Add node
		* @details Method doesnt add node, if node already added
		* @param n: Node
		*/
		void addNode(Node<T>* n)
		{
			if (n == nullptr)
			{
				throw(ExceptionInvalidValue("Polygone<T>.addNode: try to add NULL node"));
			}

			if (std::find(nodes_.begin(), nodes_.end(), n) == nodes_.end())
			{
				nodes_.push_back(n);
			}
		};

		/**
		* @brief Add adjucent polygone
		* @details Method doesnt add polygone, if node already polygone
		* @param p: Polygone
		*/
		void addAdjucentPolygone(Polygone<T>* p)
		{
			if (p == nullptr)
			{
				throw(ExceptionInvalidValue("Polygone<T>.addAdjucentPolygone: try to add NULL node"));
			}

			if ((std::find(adj_polygones_.begin(), adj_polygones_.end(), p) == adj_polygones_.end()) &&
				p != this)
			{
				adj_polygones_.push_back(p);
			}
		};

	public:
		Polygone(const std::vector<Edge<T>*>& edges) : edges_(edges)
		{
			for (const auto& e : edges_)
			{
				for (const auto& n : e->nodes_)
				{
					addNode(n);
				}

				for (const auto& p : e->polygones_)
				{
					addAdjucentPolygone(p);
				}
			}

			for (const auto& n : nodes_)
			{
				if (n->dim_ > dim_)
				{
					dim_ = n->dim_;
				}
			}

			if (!edges_.empty())
			{
				if (edges_.size() != 0.5 * dim_ * (dim_ + 1))
				{
					throw(ExceptionInvalidValue(
						"Polygone<T>: try to add " +
						std::to_string(edges_.size()) +
						" edges, but " +
						std::to_string(0.5 * dim_ * (dim_ + 1)) +
						" edges required for " + std::to_string(dim_) + "-dimensional polygone"));
				}
			}

			for (const auto& n : nodes_)
			{
				n->addPolygone(this);
			}

			for (const auto& e : edges_)
			{
				e->addPolygone(this);
			}
		}

		/**
		* @brief Add edge, which belong to this polygone
		* @details Method doesnt add edge, if edge already added
		* @param e: New edge
		*/
		void addEdge(Edge<T>* e)
		{
			if (e == nullptr)
			{
				throw(ExceptionInvalidValue("Polygone<T>.addEdge: try to add NULL edge"));
			}

			if (std::find(edges_.begin(), edges_.end(), e) == edges_.end())
			{
				edges_.push_back(e);
			}

		};
	};
}