#include <gtest/gtest.h>
#include <libmath/geometry/node.h>
#include <libmath/geometry/edge.h>
#include <libmath/geometry/polygone.h>


TEST(Geometry, CreateNode)
{
	math::Node A(
		math::Matrix<double>(
				{
					{0.},
					{0.}
				}
			)
	);

	math::Node B(
		math::Matrix<double>(
			{
				{0.},
				{1.}
			}
			)
	);

	math::Node C(
		math::Matrix<double>(
			{
				{1.},
				{1.}
			}
			)
	);

	math::Edge<double> AB({ &A,&B });
	math::Edge<double> AC({ &A,&C });
	math::Edge<double> BC({ &B,&C });

	math::Polygone<double> ABC({ &AB,&AC,&BC });
}

TEST(Geometry, CreateEdge)
{
	math::Node A(
		math::Matrix<double>(
			{
				{0.},
				{0.}
			}
			)
	);

	math::Node B(
		math::Matrix<double>(
			{
				{0.},
				{1.}
			}
			)
	);

	math::Edge<double> AB({ &A,&B });
}

TEST(Geometry, CreatePolygone)
{
	math::Node A(
		math::Matrix<double>(
			{
				{0.},
				{0.}
			}
			)
	);

	math::Node B(
		math::Matrix<double>(
			{
				{0.},
				{1.}
			}
			)
	);

	math::Node C(
		math::Matrix<double>(
			{
				{1.},
				{1.}
			}
			)
	);

	math::Edge<double> AB({ &A,&B });
	math::Edge<double> AC({ &A,&C });
	math::Edge<double> BC({ &B,&C });

	math::Polygone<double> ABC({ &AB,&AC,&BC });
}