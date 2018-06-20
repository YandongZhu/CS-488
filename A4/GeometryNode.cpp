#include "GeometryNode.hpp"

using namespace glm;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

Intersection GeometryNode::hit(Ray ray) {
	dvec4 new_origin = invtrans * ray.origin;
	dvec4 new_direction = invtrans * ray.direction;
	Ray new_ray = Ray(new_origin, new_direction);
	Intersection result = Intersection();

  // adding material info and hit for primitive
	result = m_primitive->hit(new_ray);
	result.phong_material = *((PhongMaterial *)m_material);

	for (SceneNode * child : children) {
		Intersection intersection = child->hit(new_ray);
		if (!result.is_intersect() || 
				(intersection.is_intersect() && intersection.t < result.t)) {
			result = intersection;
		}
	}
	result.N = normalize(invtrans * result.N);
	return result;
}