#include "Scene.h"

#include "MDL\mdl.H"
#include "TRACE.h"

#include "ImageIO.h"
#include "Sphere.h"
#include "Light.h"
#include "PointLight.h"
#include "Mesh.h"

Scene::Scene()
{
	
}


Scene::~Scene()
{
	//***Need to safely delete all materials in matMap, and all shapes and lights from shape and light lists
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
			char matName[256];
			char *temp = inp.ReadString();
			strcpy(matName, temp);

			// Step into material chunk
			mdlKey mat = inp.BeginChunk();
			Material *surfaceMat = new Material(); // Create a default material in case the specified material isn't supported
			if(mat == mdlKey("lmbrtn"))
			{
				// Find a lambertian material (only defines diffuse component)
				mdlKey colourKey = inp.BeginChunk();
				if(colourKey == mdlKey("rgb"))
				{
					x = inp.ReadFloat();
					y = inp.ReadFloat();
					z = inp.ReadFloat();
					surfaceMat->makeLambertian(Colour(x,y,z));
				}

				else if(colourKey == mdlKey("txtrMp"))
				{
					surfaceMat->makeLambertian(Colour(1.0f,1.0f,1.0f));
					/*std::map<std::string, ImageBuffer*>::iterator it = texMap.find(std::string("earth tex"));
					if(it != texMap.end())
					{
						if(surfaceMat->setTexture(it->second))
							TRACE("Texture assigned\n");
					}*/
					
					// skip over texture map name
					char *bwah = inp.ReadString();
					
					// Step into texture chunk (can be either txtrNm to ref named texture, or imgFl to directly load texture from image file)
					mdlKey texKey = inp.BeginChunk();
					if(texKey == mdlKey("txtrNm"))
					{
						char texMapName[256];
						temp = inp.ReadString();
						strcpy(texMapName, temp);

						std::map<std::string, ImageBuffer*>::iterator it = texMap.find(std::string(texMapName));
						if(it != texMap.end())
						{
							if(!surfaceMat->setTexture(it->second))
								TRACE("Unable to assign texture: %s\n", texMapName);
						}
						else
							TRACE("Named texture, %s, could not be found in texture map list\n", texMapName);
					}
					else
						TRACE("In txtrMp, currently not supporting chunk: %s\n", (char*)texKey);
					
					inp.EndChunk();
				}
				else
				{
					surfaceMat->makeLambertian(Colour(0.0f,1.0f,1.0f));
				}
				inp.EndChunk();
				//surfaceMat->makeLambertian(mdlRetrieveColour(&inp));
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


		// Find a named texture map
		else if(k == mdlKey("nmdTxtr"))
		{
			// Get the texture name for adding to the texture map list
			char texName[256];
			char *temp = inp.ReadString();
			strcpy(texName, temp);

			mdlKey imageChunk = inp.BeginChunk();
			if(imageChunk == mdlKey("imgFl"))
			{
				// Read the texture file name
				char texFilename[256];
				temp = inp.ReadString();
				strcpy(texFilename, temp);

				std::string texFileType = inp.ReadString();
				if(texFileType.compare("jpg")==0 || texFileType.compare("jpeg")==0)
				{
					ImageBuffer *tex = readJPG(texFilename);
					texMap.insert(std::pair<std::string, ImageBuffer*>(std::string(texName),tex));
					TRACE("Texture added\n");
				}
				else
				{
					TRACE("Texture image file type, %s, currently not supported\n", texFileType.c_str());
				}
			}
			else
			{
				TRACE("Texture data type, %s, currently not supported\n", (char*)imageChunk);
			}
			inp.EndChunk();
		}


		// Find a point light
		else if(k == mdlKey("pntSrc"))
		{
			// Skip light name
			inp.ReadString();

			Colour c = mdlRetrieveColour(&inp);
			x = inp.ReadFloat();
			y = inp.ReadFloat();
			z = inp.ReadFloat();

			PointLight *light = new PointLight(Vector(x,y,z), c);
			lightList.push_back(light);
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
					if(it == matMap.end())
					{
						// Set a default colour when the colour reference can't be found
						msh->setMat(new Material());
					}
					else
					{
						msh->setMat(it->second);
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

				// Grab the point (face) list for quadrilaterals
				else if(msh_k == mdlKey("qdrltrl"))
				{
					// Tell the Mesh it's make of quads
					msh->setVertsPerFace(4);
					
					while(inp.NumRemain() > 0)
					{
						msh->addFacePoint(inp.ReadInt());
					}
				}
				// Or Grab the points (face) list for triangles
				else if(msh_k == mdlKey("trngl"))
				{
					msh->setVertsPerFace(3);
					//***Store triangle vertices
				}
				else
					TRACE("For \"msh\", not handling chunk: %s\n", (char *)msh_k);
				inp.EndChunk();
			}
		}


		// Find a sphere object
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


		// Otherwise forget about loading the piece
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
	mdlKey colour = inp->BeginChunk();
	// Find an rgb chunk
	if(colour == mdlKey("rgb"))
	{
		float x = inp->ReadFloat();
		float y = inp->ReadFloat();
		float z = inp->ReadFloat();
		returnColour = Colour(x,y,z);
	}

	/*
	// Find a texture map chunk
	else if(colour == mdlKey("txtrMp"))
	{
		// Skip over nexture map name
		inp->ReadString();

		// Step into next either a imgFl chunk (where texture image file is located), or txtrNm chunk (where texture has already been loaded and stored using a nmdTxtr chunk)
		mdlKey texType = inp->BeginChunk();
		if(texType == mdlKey("imgFl"))
		{
			// Get texture file name and file type
			std::string texFileName = inp->ReadString();
			std::string texFileType = inp->ReadString();
			if(texFileType.compare("jpeg") == 0 || texFileType.compare("jpg") == 0)
			{
				//***This is where you add texture map EXCEPT IT'S NOT A COLOUR OBJECT, SO THIS FUNCTION NEEDS TO BE RETHOUGHT
			}
			else
			{
				TRACE("Currently not supporting texture image files of type %s\n", texFileType.c_str());
			}
		}
		else if(texType == mdlKey("txtrNm"))
		{
			TRACE("Currently not supporting chunk type txtrNm\n");
		}

		inp->EndChunk();
	}
	*/

	else
		TRACE("Found unsupported colour chunk: %s\n", colour);

	inp->EndChunk();

	return returnColour;
}