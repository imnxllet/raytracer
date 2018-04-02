/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

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
    Vector3D viewDir = -ray.dir;
    viewDir.normalize();

    double lambertian = fmax(lightDir.dot(normal), 0.0);
    double specular = 0.0;


     // Material at the intersection.
    Material *mat = ray.intersection.mat;

    if(lambertian > 0.0) {
       float specAngle = fmax(reflectDir.dot(viewDir), 0.0);
       specular = pow(specAngle, mat->specular_exp);
       
    }
   

   /* gl_FragColor = vec4(Ka * ambientColor +
                      Kd* lambertian * diffuseColor +
                      Ks * specular*specularColor, 1.0); */


   
    // Ambient Lighting
	Color Ia = mat->ambient * col_ambient;
	
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

