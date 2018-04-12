/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"
#include <stdlib.h>
#include <iostream>

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function. 

	Vector3D normal = ray.intersection.normal;
    Vector3D lightDir = pos - ray.intersection.point;
    lightDir.normalize();

    //2( 𝐿 ∙ 𝑁) 𝑁 − L
    Vector3D reflectDir = 2 * (lightDir.dot(normal)) * normal - lightDir;


    Material *mat = ray.intersection.mat;
   /*

    //Orthonormal basis at intersection point
	Vector3D u = reflectDir.cross(normal);
	u.normalize();
	Vector3D v = reflectDir.cross(u);
	v.normalize();
	//double roughness = 0.8;
	// Choose uniformly sampled random direction to send the ray in
	//double theta = 2 * M_PI * (rand()/((double)RAND_MAX + 1));
	//double phi = 2 * M_PI * (rand()/((double)RAND_MAX + 1));

	double a = rand() / ((double) RAND_MAX + 1 );
    double b = rand() / ((double) RAND_MAX) + 1;
   
    double theta = acos(pow((1 - a), mat->specular_exp));
    double phi = 2 * M_PI * b;
	double x = sin(theta) * cos(phi);
	double y = sin(theta) * sin(phi);
	double z = cos(theta);
	// Convert sample to world coordinates using the orthonormal basis
	Vector3D w = reflectDir;
	w.normalize();
	reflectDir = x * u + y * v + z * w;

	if(normal.dot(reflectDir) < 0.0){
		reflectDir = -x * u - y * v + z * w;
	}*/
	


	reflectDir.normalize();



    Vector3D viewDir = -ray.dir;
    viewDir.normalize();

    double lambertian = fmax(lightDir.dot(normal), 0.0);
    double specular = 0.0;


     // Material at the intersection.
    

    if(lambertian > 0.0) {
       float specAngle = fmax(reflectDir.dot(viewDir), 0.0);
       specular = pow(specAngle, mat->specular_exp);
       
    }
   

   /* gl_FragColor = vec4(Ka * ambientColor +
                      Kd* lambertian * diffuseColor +
                      Ks * specular*specularColor, 1.0); */


    //Strip texture
    Color red(255.0, 0.0, 0.0); 
    Color blue(0.0, 0.0, 255.0);
    Color Ia;
    
    /* Texture Mapping, textbook p 240*/
   /* double w = 0.1;
    if(sin(M_PI * ray.intersection.point[0] / w) > 0.0){
    	Ia = mat->ambient * red;
    }else{
    	Ia = mat->ambient * blue;
    }*/
   /* Texture Mapping End */


    // Ambient Lighting
	Ia = mat->ambient * col_ambient;
	
	// Diffuse Lighting
	Color Id = lambertian * mat->diffuse * col_diffuse;
	
	// Specular Lighting
	//dotprod = fmax(0.0, r.dot(v));
	Color Is = specular * mat->specular * col_specular;
	
	
	// Scene Signature
	//ray.col =  Id;
	

	//2nd Scene
	//ray.col = Ia + Id;
	

	//Third scene
	ray.col = Ia + Id + Is;
	/*if(!ray.intersection.none){
		ray.col = Ia + Id + Is;
	}else{
		ray.col = Ia;
	}
	*/
	// Clamp to 1
	ray.col.clamp();



}

