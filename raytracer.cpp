/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#define MAXRECURSION 2


void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {


	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list, Scene& scene) {
	/*soft shadow, following line*/
	Color col(0.0, 0.0, 0.0);
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.

		/********* Hard shadow **********/
		/*
		//• Use a ray!

		 //Compute the ray from intersection to light source 
		Point3D light_pos = light->get_position();
		Ray3D ray_to_lightpos;
  		ray_to_lightpos.intersection.none = true;
	    ray_to_lightpos.dir =  light_pos - ray.intersection.point;
	    ray_to_lightpos.dir.normalize();
		ray_to_lightpos.origin = ray.intersection.point + 0.001 * ray_to_lightpos.dir;
		ray_to_lightpos.onSquare = true;

        double distance_to_lightpos = ray_to_lightpos.dir.length();

  
        //If a ray from our intersection point reaches the light before
        //intersecting any other objects, then add the contribution of 
        //the light to the color • Otherwise, the object is shadowed by an object
        traverseScene(scene, ray_to_lightpos);

        //Intersecting with an object.
        
        if (!ray_to_lightpos.intersection.none) {
        	//printf("hi\n");
        	ray.col = ray.intersection.mat->ambient;
        	ray.col.clamp();
        	//Making sure the object is not the light source, can I remove it..
        	//if(ray_to_lightpos.intersection.t_value <= distance_to_lightpos) {
        		
        	//}

        

        //Reached the light source
        } else {
        	light->shade(ray); 
        }*/
        /********* Hard shadow end **********/


		/**Soft shadow code */
		/*
        Point3D light_pos = light->get_position();
		Ray3D ray_to_lightpos;
  		ray_to_lightpos.intersection.none = true;
	    ray_to_lightpos.dir =  light_pos - ray.intersection.point;
	    ray_to_lightpos.dir.normalize();
		ray_to_lightpos.origin = ray.intersection.point + 0.001 * ray_to_lightpos.dir;
		ray_to_lightpos.onSquare = true;

        double distance_to_lightpos = ray_to_lightpos.dir.length();

  
        //If a ray from our intersection point reaches the light before
        //intersecting any other objects, then add the contribution of 
        //the light to the color • Otherwise, the object is shadowed by an object
        traverseScene(scene, ray_to_lightpos);

        //Intersecting with an object.
        
        if (!ray_to_lightpos.intersection.none) {
        	//printf("hi\n");
        	col = col + ray.intersection.mat->ambient;
        	col.clamp();
        	//Making sure the object is not the light source, can I remove it..
        	//if(ray_to_lightpos.intersection.t_value <= distance_to_lightpos) {
        		
        	//}

        

        //Reached the light source
        } else {
        	light->shade(ray); 
        	//soft shadow, following line
       		 col = col + ray.col;
        }

        
		 
		   
	}
	//soft shadow, following line
	ray.col = (1.0 / 6.0) * col;*/
	/**Soft shadow code end https://bb-2018-01.teach.cs.toronto.edu/uploads/default/original/1X/fb3f9d81bc27c36e676c5eb693f4c384b76529f2.pdf*/
		
		//comment the following line to use softshadow or hardshadow.
		light->shade(ray); 
	}

}

//reflective : http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf
Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int recursionDepth = 0) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {
		computeShading(ray, light_list, scene); 
		col = col + ray.col;

		bool reflection = false;
		bool glossy = false;

		// You'll want to call shadeRay recursively (with a different ray, 
		// of course) here to implement reflection/refraction effects.  

		//!!!recursion   
		if(reflection){
			if (recursionDepth < MAXRECURSION) {
				if(!glossy){
					recursionDepth++;
					//printf("recrsion depth is %d\n",recursionDepth);
					Ray3D reflection_ray;        
			        reflection_ray.dir =  ray.dir - 2 * (ray.dir.dot(ray.intersection.normal) * ray.intersection.normal);
			        reflection_ray.dir.normalize();
				    reflection_ray.origin = ray.intersection.point + 0.01 * reflection_ray.dir;
			        Color reflection_ray_col = shadeRay(reflection_ray, scene, light_list, recursionDepth);
				    col = col + ((ray.intersection.mat)->specular)*reflection_ray_col; 
				    col.clamp();
				
				//glossy reflection
				}else{
					recursionDepth++;
					Vector3D normal = ray.intersection.normal;
					Material *mat = ray.intersection.mat;
					//printf("recrsion depth is %d\n",recursionDepth);
					Ray3D reflection_ray;        
			        reflection_ray.dir =  ray.dir - 2 * (ray.dir.dot(ray.intersection.normal) * ray.intersection.normal);
			        
			        //Orthonormal basis at intersection point
					Vector3D u = reflection_ray.dir.cross(normal);
					u.normalize();
					Vector3D v = reflection_ray.dir.cross(u);
					v.normalize();
					//double roughness = 0.8;
					// Choose uniformly sampled random direction to send the ray in
					//double theta = 2 * M_PI * (rand()/((double)RAND_MAX + 1));
					//double phi = 2 * M_PI * (rand()/((double)RAND_MAX + 1));

					double a = rand() / ((double) RAND_MAX + 1);
				    double b = rand() / ((double) RAND_MAX + 1);
				   
				    double theta = acos(pow((1 - a), mat->specular_exp));
				    double phi = 2 * M_PI * b;
					double x = sin(theta) * cos(phi);
					double y = sin(theta) * sin(phi);
					double z = cos(theta);
					// Convert sample to world coordinates using the orthonormal basis
					Vector3D w = reflection_ray.dir;
					//w.normalize();
					reflection_ray.dir = x * u + y * v + z * w;

			        reflection_ray.dir.normalize();
				    reflection_ray.origin = ray.intersection.point + 0.01 * reflection_ray.dir;
			        Color reflection_ray_col = shadeRay(reflection_ray, scene, light_list, recursionDepth);
				    //Reflection coefficient is Ks
			

				    col = col + ((ray.intersection.mat)->specular)*reflection_ray_col; 
				    col.clamp();

				}

			}

		}

	}

	


	col.clamp();
	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();
	
	//bool anti_aliasing = false;
	bool anti_aliasing = true;

	
	// Construct a ray for each pixel.
	#pragma omp parallel for
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
	//for (int i = 0; i < image.height; i++) {
		//for (int j = 0; j < image.width; j++) {
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.
			Color col;
			if(!anti_aliasing){

				Point3D origin(0, 0, 0);
				Point3D imagePlane;
				imagePlane[0] = (-double(image.width)/2 + 0.5 + j)/factor;
				imagePlane[1] = (-double(image.height)/2 + 0.5 + i)/factor;
				imagePlane[2] = -1;

				
				
				Ray3D ray;
				// TODO: Convert ray to world space  
				//Ray Direction
				Vector3D direction = imagePlane - origin;
				
				//Convert to world-space
				direction = viewToWorld * direction;
				origin = viewToWorld * origin;
				ray = Ray3D(origin, direction);
				
				//Generate colour by calling shadeRay
				col = shadeRay(ray, scene, light_list); 

			//Anti-Aliasing, source text-book
			}else{
				double grid_size = 5.0;
				for (int p = 0; p < grid_size; p++) {
					for (int q = 0;q < grid_size; q++) {
				

						Point3D origin(0, 0, 0);
						Point3D imagePlane;
						imagePlane[0] = (-double(image.width)/2 + (p+ 0.5)/grid_size + j)/factor;
						imagePlane[1] = (-double(image.height)/2 + (q+ 0.5)/grid_size + i)/factor;
						imagePlane[2] = -1;

						
						
						Ray3D ray;
						// TODO: Convert ray to world space  
						//Ray Direction
						Vector3D direction = imagePlane - origin;
						
						//Convert to world-space
						direction = viewToWorld * direction;
						origin = viewToWorld * origin;
						ray = Ray3D(origin, direction);
						
						//Generate colour by calling shadeRay
						col = col + shadeRay(ray, scene, light_list);
					}
				}

				col =  (1.0 / (grid_size * grid_size)) * col;


			}
			image.setColorAtPixel(i, j, col);			
		}
	}
}

