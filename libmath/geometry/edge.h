#pragma once
#include <libmath/boolean.h>
#include <libmath/geometry/node.h>
#include <libmath/geometry/polygone.h>
#include <libmath/math_exception.h>
#include <type_traits>
#include <vector>
#include <string>
#include <algorithm>

namespace math
{
	/**
	* @brief Class for n-dimension edge
	*/
	template<class T>
	class Edge
	{
	protected:
		friend Polygone<T>;

		/// @brief Vector of nodes, contains this edge
		std::vector<Node<T>*> nodes_;

		/// @brief Vector of polygones, contains this edge
		std::vector<Polygone<T>*> polygones_;

		/// @brief Edge dimension (determined as maximum dimension of nodes)
		size_t dim_ = 0;

		/**
		* @brief Add polygone, which contains this edge
		* @details Method doesnt add polygone, if polygone already added
		* @param p: New polygone
		*/
		void addPolygone(Polygone<T>* p)
		{
			if (p == nullptr)
			{
				throw(ExceptionInvalidValue("Edge<T>.addPolygone: try to add NULL polygone"));
			}

			if (std::find(polygones_.begin(), polygones_.end(), p) == polygones_.end())
			{
				polygones_.push_back(p);
			}
		};

	public:
		/**
		* @brief Edge constructor
		* @details If poly is empty, constructor automatically add polygones from nodes
		* @param nodes: Array of nodes for this edge. Number of nodes must be 2.
		*/
		Edge(const std::vector<Node<T>*>& nodes) : nodes_(nodes)
		{
			if (nodes_.size() != 2)
			{
				throw(ExceptionInvalidValue("Edge<T>: number of nodes in the edge must be 2, but try to set " + 
					std::to_string(nodes_.size()) + " nodes"));
			}

			dim_ = std::max(nodes_.at(0)->dim_, nodes_.at(1)->dim_);

			// add this edge to nodes and polygones
			for (const auto& n : nodes_)
			{
				n->addEdge(this);
			}

			nodes_.at(0)->addAdjucentNode(nodes_.at(1));
			nodes_.at(1)->addAdjucentNode(nodes_.at(0));
		}

		/**
		* @brief Get array of polygones, which contains this edge
		* @param[out] poly: Array of polygones
		*/
		void getPolygones(std::vector<Polygone<T>*>& poly) const
		{
			poly = polygones_;
		};

		/**
		* @brief Get array of nodes in this edge
		* @param[out] nodes: Array of nodes
		*/
		void getNodes(std::vector<Node<T>*>& nodes) const
		{
			nodes = nodes_;
		};


	};
}