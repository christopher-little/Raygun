#include "Scene.h"

#include "MDL\mdl.H"
#include "TRACE.h"

#include "Sphere.h"
#include "Mesh.h"
#include "Material.h"

Scene::Scene()
{

}


Scene::~Scene()
{
	
}



Colour mdlRetrieveColour(mdlInput *inp); // Retrieves a colour (ie rgb) from mdl file


Camera* Scene::loadMDL(char *filename)
{
	FILE *inputSceneFile = fopen(filename,"r");
	mdlInput inp(inputSceneFile);

	// Camera object to return
	Camera *cam = NULL;

	// Parse all chunks in the mdl file
	mdlKey k;
	float x,y,z;
	while(true)
	{
		if(inp.NumRemain() == 0)
			break;

		k = inp.BeginChunk();
		// Find the camera chunk
		if(k == mdlKey("cmr"))
		{
			// Skip over the "name"
			inp.ReadString();
			// Camera position/centre of projection (cop)
			x = inp.ReadFloat();
			y = inp.ReadFloat();
			z = inp.ReadFloat();
			Vector e(x,y,z);
			// Gaze direction/view plane normal
			x = inp.ReadFloat();
			y = inp.ReadFloat();
			z = inp.ReadFloat();
			Vector g = Vector(x,y,z).normalized();
			// Up vector
			x = inp.ReadFloat();
			y = inp.ReadFloat();
			z = inp.ReadFloat();
			Vector t = Vector(x,y,z).normalized();
			// Image plane distance/Focal length
			float N = inp.ReadFloat();
			// Image plane dimensions (width x height) in metres
			float w = inp.ReadFloat();
			float h = inp.ReadFloat();
			// Skip over "center of image area"
			inp.ReadFloat();
			inp.ReadFloat();
			// Skip over "time" (for now...)
			inp.ReadFloat();

			// Create the camera object
			cam = new Camera(e, g, t, N, w, h);

			//***Consider when camera is not defined in the mdl file
		}
		// Find the material chunk (For now, lets just support rgb, rather than spectral)
		else if(k == mdlKey("nmdMtrl"))
		{
			// Grab the material "name"
			char *matName = inp.ReadString();
			// Step into material chunk
			mdlKey mat = inp.BeginChunk();
			Material *surfaceMat = new Material(); // Create a default material in case the specified material isn't supported
			if(mat == mdlKey("lmbrtn"))
			{
				// Find a lambertian material (only defines diffuse component)
				surfaceMat->makeLambertian(mdlRetrieveColour(&inp));
			}
			else if(mat == mdlKey("phng"))
			{
				// Find a phong material (diffuse, specular and phong exponent
				Colour diff = mdlRetrieveColour(&inp);
				Colour spec = mdlRetrieveColour(&inp);
				// Retrieve scalar phong exponent
				inp.BeginChunk(); // Step into sclr chunk
				float exp = inp.ReadFloat();
				inp.EndChunk();
				surfaceMat->makePhong(diff, spec, exp);
			}
			else if(mat == mdlKey("pDlctrc"))
			{
				// Find a phong dielecctric material (specular reflection and transmission)
				Colour spec = mdlRetrieveColour(&inp);
				// Retrieve index of refraction
				inp.BeginChunk();
				float refr = inp.ReadFloat();
				inp.EndChunk();
				// Retrieve phong exponent
				inp.BeginChunk();
				float exp = inp.ReadFloat();
				inp.EndChunk();
				surfaceMat->makePhongDielectric(spec, exp, refr);
			}
			else
			{
				TRACE("Currently not supporting material: %s\n", (char *)mat);
			}

			// Map the material name to the material object
			matMap.insert(std::pair<std::string,Material*>(std::string(matName), surfaceMat));

			inp.EndChunk();
		}
		// Find a shared vertex mesh (i.e. ply) chunk
		else if(k == mdlKey("msh"))
		{
			Mesh *msh = new Mesh();
			shapeList.push_back(msh);

			// Skip mesh name
			inp.ReadString();
			// Step through mesh components (material name, vertices, uv coords, point list)
			mdlKey msh_k;
			while(true)
			{
				if(inp.NumRemain() == 0)
					break;

				msh_k = inp.BeginChunk();
				// Grab the material name
				if(msh_k == mdlKey("mtrlNm"))
				{
					// Find the material referenced by the material name
					char *matName = inp.ReadString();
					std::map<std::string, Material*>::iterator it = matMap.find(std::string(matName));
					// Set a default colour when the colour reference can't be found
					if(it == matMap.end())
					{
						msh->setMat(new Material());
					}
					else
					{
						Material *mat = it->second;
						msh->setMat(mat);
					}
				}
				// Grab the vertices
				else if(msh_k == mdlKey("vrtxPstn"))
				{
					while(inp.NumRemain() > 0)
					{
						x = inp.ReadFloat();
						y = inp.ReadFloat();
						z = inp.ReadFloat();
						msh->addVertex(Vector(x,y,z));
					}
				}
				// Grab the point (face) list for a quadrilateral
				else if(msh_k == mdlKey("qdrltrl"))
				{
					// Tell the Mesh it's make of quads
					msh->setVertsPerFace(4);
					
					while(inp.NumRemain() > 0)
					{
						msh->addFacePoint(inp.ReadInt());
					}
				}
				else if(msh_k == mdlKey("trngl"))
				{
					//***Store tri faces inp.NumRemain()/3
				}
				else
					TRACE("For \"msh\", not handling chunk: %s\n", (char *)msh_k);
				inp.EndChunk();
			}
		}
		// Collect a sphere object
		else if(k == mdlKey("sphr"))
		{
			// Skip sphere name
			inp.ReadString();
			
			// Grab the material object
			Material *mat = new Material();
			mdlKey sphr_k = inp.BeginChunk();
			if(sphr_k == mdlKey("mtrlNm"))
			{
				// Find the material referenced by the material name
				char *matName = inp.ReadString();
				std::map<std::string, Material*>::iterator it = matMap.find(std::string(matName));
				if(it != matMap.end())
				{
					// Material is set to stored material if found, otherwise the default material is used
					mat = it->second;
				}
			}
			//***Consider when material is specified within sphr definition in mdl file
			inp.EndChunk();

			// Read the position and radius
			x = inp.ReadFloat();
			y = inp.ReadFloat();
			z = inp.ReadFloat();
			Vector c(x, y, z);
			float r = inp.ReadFloat();

			// Create the sphere
			Sphere *sphere = new Sphere(c, r);
			sphere->setMat(mat);
			shapeList.push_back(sphere);
		}
		else
			TRACE("Not handling chunk: %s\n", (char *)k);
		inp.EndChunk();
	}

	fclose(inputSceneFile);

	return cam;
}




//Colour Scene::mdlRetrieveColour(mdlInput *inp)
Colour mdlRetrieveColour(mdlInput *inp)
{
	Colour returnColour;

	// Step into color chunk
	mdlKey color = inp->BeginChunk();
	// Find an rgb chunk
	if(color == mdlKey("rgb"))
	{
		float x = inp->ReadFloat();
		float y = inp->ReadFloat();
		float z = inp->ReadFloat();
		returnColour = Colour(x,y,z);
	}
	else
		TRACE("Found Non-rgb colour, currently not supporting: %s\n", color);

	inp->EndChunk();

	return returnColour;
}