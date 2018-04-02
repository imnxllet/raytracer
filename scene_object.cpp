/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, 
	// which is defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	//Transform the ray (origin, direction) to object space

	Vector3D dir = worldToModel * ray.dir;
	Point3D origin = worldToModel * ray.origin;

	//t value
	double t = - 1 * (origin[2] / dir[2]);
	
	//invalid intersection
	if (t < 0 || dir[2] == 0){
		//ray.intersection.none = true;
	    return false;
	}

	Point3D p = origin + t * dir;
	//p[3] = 0.0;
	Vector3D normal = Vector3D(0.0,0.0,1.0);

	if (ray.intersection.none || t < ray.intersection.t_value) {
 
		if (p[0] >= -0.5 && p[0] <= 0.5 && p[1] >= -0.5 && p[1] <= 0.5) {

		    ray.intersection.t_value = t;
		    ray.intersection.point = modelToWorld * p;
		    ray.intersection.normal = transNorm(worldToModel, normal);
		    ray.intersection.normal.normalize();
		    ray.intersection.none = false;
  
		    return true;
		}
	}
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	//Transform the ray (origin, direction) to object space
	Vector3D dir = worldToModel * ray.dir;
	Point3D origin = worldToModel * ray.origin;

	//Center of the unit sphere
	Point3D center = Point3D(0.0, 0.0, 0.0);

	//Radius of the unit sphere
	float radius = 1.0;

	//Implicit formula: x^2+y^2+z^2=R^2 (R is radius)
	//(x,y,z) is the intersection point P (or P & P')
	//so P^2−R^2=0
	//|O+tD|^2−R^2=0, => O^2+(Dt)^2+2ODt−R^2 = D^2t^2 + 2ODt + O^2 − R^2
	//Cast it to a quadratic equation: f(t)=at^2+bt+c
	//so a = D^2, b = 2OD, c = O^2−R^2

	Vector3D L = origin - center; 

	double a = (dir).dot(dir);
	double b = 2 * ((dir).dot(L));
	double c = L.dot(L) - sqrt(radius); 

	//To determine number of intersection points
	double num_intersect = (b * b) - (4 * a * c);

	
	double t1, t2;
	//=0, ray has one or two intersection point with the unit sephere
	if(num_intersect >= 0){
		//Solve for t1 t2
		t1 = (-b + sqrt(num_intersect)) / (2.0 * a); 
		t2 = (-b - sqrt(num_intersect)) / (2.0 * a);

	//<0, ray has no intersection point with the unit sephere
	}else{
		return false;
	}

	//Get the final t
	double t;


	if(t1 < 0 && t2 < 0){
		return false;
	}

	//only need the closest intersection pint to ray's origin
	t = fmin(t1, t2);
	if(t < 0){//one of t1, t2 is negative
		t = fmax(t1, t2);
	}

	//intersection point
	Point3D p = origin + t * dir;
	
	//Normal at intersection point
	Vector3D normal = p - center;
	normal.normalize();

	//fill ray.intersection with correct values
	if (ray.intersection.none || t < ray.intersection.t_value) {

		ray.intersection.t_value = t;
	    ray.intersection.point = modelToWorld * p;
	    ray.intersection.normal = transNorm(worldToModel, normal);
	    ray.intersection.normal.normalize();
	    ray.intersection.none = false;

		
		return true;
	}

	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
}