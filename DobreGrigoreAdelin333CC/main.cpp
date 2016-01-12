
#include "lab_blackbox.hpp"
#include "lab_camera.hpp"


//time
#include <ctime>

#define OBJECTS_NO 32
#define PI 3.141592
#define  KEY_ESCAPE	  27
#define SPACEBAR 32

class Laborator5
	: public lab::glut::WindowListener
{
	private:
		// The BlackBox hides some functionality that we'll learn in the next course
		lab::BlackBox BLACKBOX;

		lab::Segments *obiect[OBJECTS_NO], *obiect1, *obiectstart, *obiectfinal;
		glm::mat4 transformation_start, transformation;
		glm::vec4 startPos, currentPos;

		unsigned int width, height;
		int seq = -1, orientation = 0, flag_a = 0, flag_d = 0, flag_space_colision = 0, angle = 0, flag_key;
		float percent, xvalue, zvalue, xdif, ydif, zdif;

		// Buffers used for holding state of keys
		// true - pressed
		// false - not pressed
		bool keyState[256];
		bool specialKeyState[256];

		// helper variables for third person camera drawing
		bool thirdPersonCamera;
		bool alwaysDrawTarget;

		// Projection matrix
		bool isPerspectiveProjection;
		glm::mat4 projectionMatrix;
		float FoV;
		float zNear, zFar;
		float aspectRatio;
		float orthoLeft, orthoRight, orthoTop, orthoBottom;
		int cont = 0;

		// Camera
		lab::Camera camera;
		lab::Camera cameraFPS;

	public:
		Laborator5()
		{
			percent = 0;

			obiectstart = myLoadCube_as_Segments(240, 0, 240, 20);
			startPos = glm::vec4(240, 0, 240, 1);

			obiect[0] = myLoadCube_as_Segments(260, 0, 240, 20);
			obiect[1] = myLoadCube_as_Segments(280, 0, 240, 20);
			obiect[2] = myLoadCube_as_Segments(300, 0, 240, 20);
			obiect[3] = myLoadCube_as_Segments(300, 0, 220, 20);
			obiect[4] = myLoadCube_as_Segments(280, 20, 220, 20); 
			obiect[5] = myLoadCube_as_Segments(280, 20, 200, 20);
			obiect[6] = myLoadCube_as_Segments(280, 20, 180, 20);
			obiect[7] = myLoadCube_as_Segments(260, 20, 180, 20);
			obiect[8] = myLoadCube_as_Segments(300, 20, 180, 20);
			obiect[9] = myLoadCube_as_Segments(240, 20, 180, 20);
			obiect[10] = myLoadCube_as_Segments(220, 20, 180, 20);
			obiect[11] = myLoadCube_as_Segments(200, 20, 180, 20);
			obiect[12] = myLoadCube_as_Segments(180, 20, 180, 20);
			obiect[13] = myLoadCube_as_Segments(240, 40, 180, 20);
			obiect[14] = myLoadCube_as_Segments(220, 40, 180, 20);
			obiect[15] = myLoadCube_as_Segments(220, 60, 180, 20);
			obiect[16] = myLoadCube_as_Segments(160, 0, 180, 20);
			obiect[17] = myLoadCube_as_Segments(160, 0, 200, 20);
			obiect[18] = myLoadCube_as_Segments(160, 0, 220, 20);
			obiect[19] = myLoadCube_as_Segments(160, 0, 240, 20);
			obiect[20] = myLoadCube_as_Segments(140, 20, 220, 20);
			obiect[21] = myLoadCube_as_Segments(120, 20, 240, 20);
			obiect1 = myLoadCube_as_Segments(140, 0, 240, 20);
			obiect[22] = myLoadCube_as_Segments(100, 40, 240, 20);
			obiect[23] = myLoadCube_as_Segments(100, 60, 220, 20);
			obiect[24] = myLoadCube_as_Segments(100, 60, 220, 20);
			obiect[25] = myLoadCube_as_Segments(100, 60, 200, 20);
			obiect[26] = myLoadCube_as_Segments(100, 60, 180, 20);
			obiect[27] = myLoadCube_as_Segments(120, 60, 180, 20);
			obiect[28] = myLoadCube_as_Segments(140, 60, 180, 20);
			obiect[29] = myLoadCube_as_Segments(140, 80, 160, 20);
			obiect[30] = myLoadCube_as_Segments(140, 100, 140, 20);
			obiect[31] = myLoadCube_as_Segments(160, 100, 140, 20);
			obiectfinal = myLoadCube_as_Segments(180, 100, 140, 20);		

			width = lab::glut::getInitialWindowInformation().width;
			height = lab::glut::getInitialWindowInformation().height;
			seq = 0;
			percent = 0;

			transformation_start = glm::mat4(1);
			transformation = glm::mat4(1);
			// init camera
			camera.set(glm::vec3(240, 100, 380), glm::vec3(240, 0, 0), glm::vec3(0, 1, 0));
			cameraFPS.set(glm::vec3(240, 0, 240), glm::vec3(240, 0, 0), glm::vec3(0, 1, 0));
			// initializa all key states to not pressed
			memset(keyState, 0, 256);
			memset(specialKeyState, 0, 256);

			// helper variables for third person camera
			thirdPersonCamera = false;
			alwaysDrawTarget = false;

			// Initialize default projection values
			zNear = 0.1f;
			zFar = 500.0f;
			FoV = 60.0f;
			aspectRatio = 800 / 600.0f;
			orthoLeft = -25;
			orthoRight = 25;
			orthoBottom = -25;
			orthoTop = 25;

			// value may be used for updating the projection when reshaping the window
			isPerspectiveProjection = true;

			projectionMatrix = glm::perspective(FoV, aspectRatio, zNear, zFar);
		}

		~Laborator5()
		{
			delete obiect;
			delete obiect1;
			delete obiectstart;
			delete obiectfinal;
		}

		//format vertex de mana
		struct MyVertexFormat{
			glm::vec3 pozitie;
			MyVertexFormat(){
				pozitie = glm::vec3(0, 0, 0);
			}
			MyVertexFormat(float px, float py, float pz){
				pozitie = glm::vec3(px, py, pz);
			}
			MyVertexFormat operator=(const MyVertexFormat &rhs){
				pozitie = rhs.pozitie;
				return (*this);
			}
		};

		lab::Segments* myLoadCube_as_Segments(float x, float y, float z, float latura){

			std::vector<MyVertexFormat> vertecsi;
			std::vector<unsigned int> indecsi;

			float lat = latura / 2;

			//toti vertecsii cubului
			vertecsi.clear();
			vertecsi.push_back(MyVertexFormat(x - lat, y - lat, z + lat));//0
			vertecsi.push_back(MyVertexFormat(x + lat, y - lat, z + lat));//1
			vertecsi.push_back(MyVertexFormat(x + lat, y + lat, z + lat));//2
			vertecsi.push_back(MyVertexFormat(x - lat, y + lat, z + lat));//3
			vertecsi.push_back(MyVertexFormat(x - lat, y - lat, z - lat));//4
			vertecsi.push_back(MyVertexFormat(x + lat, y - lat, z - lat));//5
			vertecsi.push_back(MyVertexFormat(x + lat, y + lat, z - lat));//6
			vertecsi.push_back(MyVertexFormat(x - lat, y + lat, z - lat));//7

			//cele 12 laturi ale cubului
			indecsi.push_back(0); indecsi.push_back(1);
			indecsi.push_back(1); indecsi.push_back(2);
			indecsi.push_back(2); indecsi.push_back(3);
			indecsi.push_back(3); indecsi.push_back(0);
			indecsi.push_back(0); indecsi.push_back(2);

			indecsi.push_back(4); indecsi.push_back(5);
			indecsi.push_back(5); indecsi.push_back(6);
			indecsi.push_back(6); indecsi.push_back(7);
			indecsi.push_back(7); indecsi.push_back(4);
			indecsi.push_back(4); indecsi.push_back(6);

			indecsi.push_back(3); indecsi.push_back(7);
			indecsi.push_back(2); indecsi.push_back(6);
			indecsi.push_back(1); indecsi.push_back(5);
			indecsi.push_back(0); indecsi.push_back(4);
			
			indecsi.push_back(7); indecsi.push_back(2);
			indecsi.push_back(0); indecsi.push_back(5);
			indecsi.push_back(0); indecsi.push_back(7);

			unsigned int segments_vbo, segments_vao, segments_ibo;
			//creaza vao
			glGenVertexArrays(1, &segments_vao);
			glBindVertexArray(segments_vao);

			//creeaza vbo
			glGenBuffers(1, &segments_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, segments_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertexFormat)*vertecsi.size(), &vertecsi[0], GL_STATIC_DRAW);

			//creeaza ibo
			glGenBuffers(1, &segments_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, segments_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indecsi.size(), &indecsi[0], GL_STATIC_DRAW);

			//creez obiect de tip segments
			lab::Segments* segments = new lab::Segments(segments_vbo, segments_ibo, segments_vao, indecsi.size());
			segments->setStartPos(x, y, z, latura);

			// traducem la OpenGL cum sa foloseasca datele noastre
			BLACKBOX.bindSegments(segments, sizeof(MyVertexFormat));

			//return
			return segments;
		}
		//--------------------------------------------------------------------------------------------
		//functii de transformare --------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------
		glm::mat4 myIdentity(){
			return glm::mat4(1);
		}
		glm::mat4 myTranslate(float tx, float ty, float tz){
			return glm::transpose(glm::mat4(
				1, 0, 0, tx,
				0, 1, 0, ty,
				0, 0, 1, tz,
				0, 0, 0, 1));
		}
		glm::mat4 myRotateX(float u){
			float cosinus = glm::cos(u);
			float sinus = glm::sin(u);

			return glm::transpose(glm::mat4(
				1, 0, 0, 0,
				0, cosinus, -sinus, 0,
				0, sinus, cosinus, 0,
				0, 0, 0, 1));
		}
		glm::mat4 myRotateY(float u){
			float cosinus = glm::cos(u);
			float sinus = glm::sin(u);

			return glm::transpose(glm::mat4(
				cosinus, 0, sinus, 0,
				0, 1, 0, 0,
				-sinus, 0, cosinus, 0,
				0, 0, 0, 1));
		}
		glm::mat4 myRotateZ(float u){
			float cosinus = glm::cos(u);
			float sinus = glm::sin(u);

			return glm::transpose(glm::mat4(
				cosinus, -sinus, 0, 0,
				sinus, cosinus, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1));
		}

		glm::vec4 getCurrentPosition(){
			return transformation_start * startPos;
		}

		void gravityTest(){

			float dif, max = 100000, notify = 0;
			if (obiectstart->y != 0){
				for (int i = 0; i < OBJECTS_NO; i++){
					if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z){
						if (notify == 0 && abs(obiectstart->y - obiect[i]->y) == 20 && obiectstart->y > obiect[i]->y){
							notify = 1;
							break;
						}
						else
						if (abs(obiectstart->y - obiect[i]->y) > 20 && obiectstart->y > obiect[i]->y){
							if (abs(obiectstart->y - obiect[i]->y) < abs(max - obiectstart->y))
								max = obiect[i]->y;
						}
					}
				}
				if (obiectstart->x == obiect1->x && obiectstart->z == obiect1->z){
					if (notify == 0){
						if (abs(obiectstart->y - obiect1->y) == 20 && obiectstart->y > obiect1->y)
							notify = 1;
						else
						if (abs(obiectstart->y - obiect1->y) > 20 && obiectstart->y > obiect1->y){
							if (abs(obiectstart->y - obiect1->y) < abs(max - obiectstart->y))
								max = obiect1->y;
						}
					} 
				}
				
				if (obiectstart->x == obiectfinal->x && obiectstart->z == obiectfinal->z){
					if (notify == 0){
						if (abs(obiectstart->y - obiectfinal->y) == 20 && obiectstart->y > obiectfinal->y)
							notify = 1;
						else
						if (abs(obiectstart->y - obiectfinal->y) > 20 && obiectstart->y > obiectfinal->y){
							if (abs(obiectstart->y - obiectfinal->y) < abs(max - obiectstart->y))
								max = obiectfinal->y;
						}
					}
				}
				if (notify == 0){
					if (max != 100000){
						dif = abs(obiectstart->y - max - 20);
						for (float j = 0; j < dif; j++)
							transformation_start *= myTranslate(0, -1.0f, 0);
						obiectstart->y -= dif;
						cameraFPS.translateUpword(-dif);
					}
					else{
						dif = obiectstart->y;
						for (float j = 0; j < obiectstart->y; j++)
							transformation_start *= myTranslate(0, -1.0f, 0);
						obiectstart->y -= dif;
						cameraFPS.translateUpword(-dif);
					}
				}	
			}
		}

		void spacebarpressed(){

			float moveSpeed = 10;

			switch (seq)
			{
			case 1:
				if (percent < 25){
					transformation_start *= myTranslate(0, 0.16f, 0);
					percent += 0.2;
				}
				else{
					obiectstart->y += 20;
					seq = 2;
					percent = 0;
					cameraFPS.translateUpword(moveSpeed + 10);
				}
				break;
			case 2:
				if (percent < 6.25){
					transformation_start *= myTranslate(xvalue, 0.08f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 3;
					percent = 0;
				}
				break;
			case 3:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.08f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 4;
					percent = 0;
				}
				break;
			case 4:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.04f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 5;
					percent = 0;
				}
				break;
			case 5:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.02f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 6;
					percent = 0;
				}
				break;
			case 6:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.01f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 7;
					percent = 0;
				}
				break;
			case 7:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.005f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 8;
					percent = 0;
				}
				break;
			case 8:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, 0.0025f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 9;
					percent = 0;
				}
				break;
			case 9:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.0025f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 10;
					percent = 0;
				}
				break;
			case 10:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.005f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 11;
					percent = 0;
				}
				break;
			case 11:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.01f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 12;
					percent = 0;
				}
				break;
			case 12:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.02f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 13;
					percent = 0;
				}
				break;
			case 13:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.04f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 14;
					percent = 0;
				}
				break;
			case 14:
				if (percent < 3.125){
					transformation_start *= myTranslate(xvalue, -0.08f, zvalue);
					percent += 0.2;
				}
				else{
					seq = 15;
					percent = 0;
				}
				break;
			case 15:
				if (percent < 6.25){
					transformation_start *= myTranslate(xvalue, -0.08f, zvalue);
					percent += 0.2;
				}
				else{
					currentPos = getCurrentPosition();

					if (orientation == 0 || orientation == 2){
						if (xvalue < 0){
							obiectstart->x -= 20;
							xdif = obiectstart->x - currentPos[0];
							ydif = obiectstart->y - currentPos[1];
							transformation_start *= myTranslate(xdif, ydif, 0);

							if (orientation == 0){
								camera.translateRight(-moveSpeed);
								cameraFPS.translateRight(-moveSpeed - 10);
							}
							else
							if (orientation == 2){
								camera.translateRight(moveSpeed);
								cameraFPS.translateRight(moveSpeed + 10);
							}
						}
						else{
							obiectstart->x += 20;
							xdif = obiectstart->x - currentPos[0];
							ydif = obiectstart->y - currentPos[1];
							transformation_start *= myTranslate(xdif, ydif, 0);

							if (orientation == 0){
								camera.translateRight(moveSpeed);
								cameraFPS.translateRight(moveSpeed + 10);
							}
							else
							if (orientation == 2){
								camera.translateRight( - moveSpeed);
								cameraFPS.translateRight( - moveSpeed - 10);
							}

						}
							
					} else
					if (orientation == 1 || orientation == 3){
						if (zvalue < 0){
							obiectstart->z -= 20;
							zdif = obiectstart->z - currentPos[2];
							ydif = obiectstart->y - currentPos[1];
							transformation_start *= myTranslate(0, ydif, zdif);
						
							if (orientation == 1){
								camera.translateRight(moveSpeed);
								cameraFPS.translateRight(moveSpeed + 10);
							}
							else
							if (orientation == 3){
								camera.translateRight(-moveSpeed);
								cameraFPS.translateRight(-moveSpeed - 10);
							}
						}
						else{
							obiectstart->z += 20;
							zdif = obiectstart->z - currentPos[2];
							ydif = obiectstart->y - currentPos[1];
							transformation_start *= myTranslate(0, ydif, zdif);
					
							if (orientation == 1){
								camera.translateRight(-moveSpeed);
								cameraFPS.translateRight(-moveSpeed - 10);
							}
							else
							if (orientation == 3){
								camera.translateRight(moveSpeed);
								cameraFPS.translateRight(moveSpeed + 10);
							}

						}
					}
					seq = -1;
					percent = 0;
					gravityTest();
				}
				break;
			}
		}

		// Function used to compute perspective projection matrix
		// use glm::perspective and member variables FoV, aspectRation, zNear and zFar
		void computePerspectiveProjection()
		{
			// projectionMatrix = glm::mat4(1);
		}

		void computeOrthograhicProjection()
		{
			//projectionMatrix = glm::mat4(1);
		}

		//---------------------------------------------------------------------
		// Loop Functions - function that are called every single frame

		// Called right before frame update callback (notifyDisplayFrame)
		void notifyBeginFrame() { };

		// Called every frame before we draw
		// Because glut sends only 1 key event every single frame, pressing more than 1 key will have no effect
		// If we treat the input using the 2 special buffers where we hold key states we can treat more than 1
		// key pressed at the same time. Also by not relling on glut to send the continuous pressing event
		// the moving motion will be more smooth because every single frame we apply the changes is contrast to
		// the event based method provided by Glut
		void treatInput()
		{
			// Compute frameRate independent speed for moving the camera
			// We set the speed based on the frameTime between the last 2 frames
			// Speed will remain constant no matter how many frames/second is the GPU cappable to render
			float frameTime = BLACKBOX.getFrameTimeSeconds();
			float moveSpeed = frameTime * 200;

			// if the rotation speed is too high change it
			float rotateSpeedOX = frameTime * 2;
			float rotateSpeedOY = frameTime * 2;
			float rotateSpeedOZ = frameTime * 2;

			// Increase speed when Shift key is pressed
			if (specialKeyState[GLUT_KEY_SHIFT_L])
			{
				moveSpeed *= 2;
			}

			// Camera Translation
			if (keyState['u']) { camera.translateForward(moveSpeed); }
			if (keyState['h']) { camera.translateRight(-moveSpeed); }
			if (keyState['j']) { camera.translateForward(-moveSpeed); }
			if (keyState['k']) { camera.translateRight(moveSpeed); }
			if (keyState['i']) { camera.translateUpword(moveSpeed); }
			if (keyState['y']) { camera.translateUpword(-moveSpeed); }

			// Camera Rotate TPS
			if (keyState['5']) { camera.rotateTPS_OX(rotateSpeedOX); }
			if (keyState['8']) { camera.rotateTPS_OX(-rotateSpeedOX); }
			if (keyState['4']) { camera.rotateTPS_OY(rotateSpeedOY); }
			if (keyState['6']) { camera.rotateTPS_OY(-rotateSpeedOY); }
			if (keyState['7']) { camera.rotateTPS_OZ(rotateSpeedOZ); }
			if (keyState['9']) { camera.rotateTPS_OZ(-rotateSpeedOZ); }

		}
		
		void computeTranslation(char c){
			float moveSpeed = 10;
			flag_a = flag_d = 0;

			if (orientation == 0){
				for (int i = 0; i < OBJECTS_NO; i++){
					if (obiectstart->y == obiect[i]->y && obiectstart->z == obiect[i]->z
						&& abs(obiectstart->x - obiect[i]->x) == 20){
						if (flag_a == 0 && obiectstart->x > obiect[i]->x)
							flag_a = 1;
						else
						if (flag_d == 0 && obiectstart->x < obiect[i]->x)
							flag_d = 1;
					}
				}
				if (obiectstart->y == obiect1->y && obiectstart->z == obiect1->z
					&& abs(obiectstart->x - obiect1->x) == 20){
					if (flag_a == 0 && obiectstart->x > obiect1->x)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->x < obiect1->x)
						flag_d = 1;
				}
				else
				if (obiectstart->y == obiectfinal->y && obiectstart->z == obiectfinal->z
					&& abs(obiectstart->x - obiectfinal->x) == 20){
					if (flag_a == 0 && obiectstart->x > obiectfinal->x)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->x < obiectfinal->x)
						flag_d = 1;
				}
			
				if (c == 'a' && flag_a == 0){
					transformation_start *= myTranslate(-20.0f, 0, 0);
					obiectstart->x -= 20;
					camera.translateRight(-moveSpeed);
					cameraFPS.translateRight(-moveSpeed - 10);
				}
				else
				if (c == 'd' && flag_d == 0){
					transformation_start *= myTranslate(20.0f, 0, 0);
					obiectstart->x += 20;
					camera.translateRight(moveSpeed);
					cameraFPS.translateRight(moveSpeed + 10);
				}
			} 
			else
			if (orientation == 2){
				for (int i = 0; i < OBJECTS_NO; i++){
					if (obiectstart->y == obiect[i]->y && obiectstart->z == obiect[i]->z
						&& abs(obiectstart->x - obiect[i]->x) == 20){
						if (flag_a == 0 && obiectstart->x < obiect[i]->x)
							flag_a = 1;
						else
						if (flag_d == 0 && obiectstart->x > obiect[i]->x)
							flag_d = 1;
					}
				}
				if (obiectstart->y == obiect1->y && obiectstart->z == obiect1->z
					&& abs(obiectstart->x - obiect1->x) == 20){
					if (flag_a == 0 && obiectstart->x < obiect1->x)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->x > obiect1->x)
						flag_d = 1;
				}
				else
				if (obiectstart->y == obiectfinal->y && obiectstart->z == obiectfinal->z
					&& abs(obiectstart->x - obiectfinal->x) == 20){
					if (flag_a == 0 && obiectstart->x < obiectfinal->x)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->x > obiectfinal->x)
						flag_d = 1;
				}
				if (c == 'a' && flag_a == 0){
					transformation_start *= myTranslate(20.0f, 0, 0);
					obiectstart->x += 20;
					camera.translateRight(-moveSpeed);
					cameraFPS.translateRight(-moveSpeed - 10);
				}
				else
				if (c == 'd' && flag_d == 0){
					transformation_start *= myTranslate(-20.0f, 0, 0);
					obiectstart->x -= 20;
					camera.translateRight(moveSpeed);
					cameraFPS.translateRight(moveSpeed + 10);
				}
			}else
			if (orientation == 1){
				for (int i = 0; i < OBJECTS_NO; i++){
					if (obiectstart->y == obiect[i]->y && obiectstart->x == obiect[i]->x
						&& abs(obiectstart->z - obiect[i]->z) == 20){
						if (flag_a == 0 && obiectstart->z < obiect[i]->z)
							flag_a = 1;
						else
						if (flag_d == 0 && obiectstart->z > obiect[i]->z)
							flag_d = 1;
					}
				}
				if (obiectstart->y == obiect1->y && obiectstart->x == obiect1->x
					&& abs(obiectstart->z - obiect1->z) == 20){
					if (flag_a == 0 && obiectstart->z < obiect1->z)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->z > obiect1->z)
						flag_d = 1;
				}
				else
				if (obiectstart->y == obiectfinal->y && obiectstart->x == obiectfinal->x
					&& abs(obiectstart->z - obiectfinal->z) == 20){
					if (flag_a == 0 && obiectstart->z < obiectfinal->z)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->z > obiectfinal->z)
						flag_d = 1;
				}
				if (c == 'a' && flag_a == 0){
					transformation_start *= myTranslate(0, 0, 20.0f);
					obiectstart->z += 20;
					camera.translateRight(-moveSpeed);
					cameraFPS.translateRight(-moveSpeed - 10);
				}
				else
				if (c == 'd' && flag_d == 0){
					transformation_start *= myTranslate(0, 0, -20.0f);
					obiectstart->z -= 20;
					camera.translateRight(moveSpeed);
					cameraFPS.translateRight(moveSpeed + 10);
				}
			}
			else
			if (orientation == 3){
				for (int i = 0; i < OBJECTS_NO; i++){
					if (obiectstart->y == obiect[i]->y && obiectstart->x == obiect[i]->x
						&& abs(obiectstart->z - obiect[i]->z) == 20){
						if (flag_a == 0 && obiectstart->z > obiect[i]->z)
							flag_a = 1;
						else
						if (flag_d == 0 && obiectstart->z < obiect[i]->z)
							flag_d = 1;
					}
				}
				if (obiectstart->y == obiect1->y && obiectstart->x == obiect1->x
					&& abs(obiectstart->z - obiect1->z) == 20){
					if (flag_a == 0 && obiectstart->z > obiect1->z)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->z < obiect1->z)
						flag_d = 1;
				}
				else
				if (obiectstart->y == obiectfinal->y && obiectstart->x == obiectfinal->x
					&& abs(obiectstart->z - obiectfinal->z) == 20){
					if (flag_a == 0 && obiectstart->z > obiectfinal->z)
						flag_a = 1;
					else
					if (flag_d == 0 && obiectstart->z < obiectfinal->z)
						flag_d = 1;
				}
				if (c == 'a' && flag_a == 0){
					transformation_start *= myTranslate(0, 0, -20.0f);
					obiectstart->z -= 20;
					camera.translateRight(-moveSpeed);
					cameraFPS.translateRight(-moveSpeed - 10);
				}
				else
				if (c == 'd' && flag_d == 0){
					transformation_start *= myTranslate(0, 0, 20.0f);
					obiectstart->z += 20;
					camera.translateRight(moveSpeed);
					cameraFPS.translateRight(moveSpeed + 10);
				}		
			}
		}

		// A key was pressed
		void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y)
		{
			float moveSpeed = 10;

			keyState[key_pressed] = 1;

			if (key_pressed == KEY_ESCAPE)
			{
				lab::glut::close();
			}

			if (key_pressed <= '9' && key_pressed >= '0')
				thirdPersonCamera = true;

			if (key_pressed == 't')
			{
				alwaysDrawTarget = !alwaysDrawTarget;
			}

			if (key_pressed == 'a')
			{
				computeTranslation('a');
				gravityTest();
			}
			if (key_pressed == 'd')
			{
				computeTranslation('d');
				gravityTest();
			}
			if (key_pressed == 32){
				flag_space_colision = 0;

				if (orientation == 0){
					if (keyState['a'] == 1){
						xvalue = -0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->x == (obiect[i]->x + 20) && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}		
					}else
					if (keyState['d'] == 1){
						xvalue = 0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->x == (obiect[i]->x - 20) && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}
					zvalue = 0;
				}else
				if (orientation == 2){
					if (keyState['a'] == 1){
						xvalue = 0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->x == (obiect[i]->x - 20) && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}else
					if (keyState['d'] == 1){
						xvalue = -0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->x == (obiect[i]->x + 20) && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}
					zvalue = 0;
				}else
				if (orientation == 1){
					if (keyState['a'] == 1){
						zvalue = 0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->z == (obiect[i]->z - 20) && obiectstart->x == obiect[i]->x &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}else
					if (keyState['d'] == 1){
						zvalue = -0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->z == (obiect[i]->z + 20) && obiectstart->x == obiect[i]->x &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}
					xvalue = 0;
				}else
				if (orientation == 3){
					if (keyState['a'] == 1){
						zvalue = -0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->z == (obiect[i]->z + 20) && obiectstart->x == obiect[i]->x &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}else
					if (keyState['d'] == 1){
						zvalue = 0.08;
						for (int i = 0; i < OBJECTS_NO; i++){
							if (obiectstart->x == obiect[i]->x && obiectstart->z == obiect[i]->z &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
							else
							if (obiectstart->z == (obiect[i]->z - 20) && obiectstart->x == obiect[i]->x &&
								obiect[i]->y == (obiectstart->y + 20)){
								flag_space_colision = 1;
								break;
							}
						}
					}
					xvalue = 0;
				}
				if (flag_space_colision == 0){
					seq = 1;
					percent = 0;
				}
			}
			
			// Reset Camera
			if (keyState['c'])
			{
				camera.set(glm::vec3(0, 0, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			}

		}

		// When a key was released
		void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y)
		{
			if (key_released == 'a'){
				if (keyState['d'] == 1)
					keyState['d'] = 0;
			}else
			if (key_released == 'd'){
				if (keyState['a'] == 1)
					keyState['a'] = 0;
			}else
				keyState[key_released] = 0;

			// Disable drawing of camera target if no longer moving in ThirdPerson
			if (thirdPersonCamera)
			{
				char sum = 0;
				for (unsigned int i = 0; i <= 9; i++) {
					sum += keyState[i + '0'];
				}
				if (!sum) thirdPersonCamera = false;
			}

		}

		// Special key pressed like the navigation arrows or function keys F1, F2, ...
		void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y)
		{
			specialKeyState[key_pressed] = 1;

			if (specialKeyState[GLUT_KEY_LEFT]){
				flag_key = 2;
				if (angle == 0)
					angle = 100;
				if (orientation != 0)
					orientation--;
				else
					orientation = 3;
			}else
			if (specialKeyState[GLUT_KEY_RIGHT]){
				flag_key = 1;
				if (angle == 0)
					angle = 100;
				if (orientation != 3)
					orientation++;
				else
					orientation = 0;
			}

			switch (key_pressed)
			{
			case GLUT_KEY_F1: {
				lab::glut::enterFullscreen();
				break;
			}

			case GLUT_KEY_F2: {
				lab::glut::exitFullscreen();
				break;
			}

			case GLUT_KEY_F5: {
				BLACKBOX.LoadShader();
				break;
			}

			default:
				break;
			}
		}

		// Called when a special key was released
		void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y)
		{
			specialKeyState[key_released] = 0;
		}

		// Called every frame to draw
		void notifyDisplayFrame()
		{
			// Treat continuous input
			treatInput();

			// Clear Color Buffer with the specified color
			glClearColor(1, 1, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			BLACKBOX.notifyDisplay();

			// ----------------------------------
			// Set the viewport and view and projection matrices
			width = lab::glut::getInitialWindowInformation().width;
			height = lab::glut::getInitialWindowInformation().height;

			glViewport(0, 0, width, height);

			// Send view matrix to the GPU
			BLACKBOX.setViewMatrix(camera.getViewMatrix());
	
			 //Send projection matrix to the GPU
			 BLACKBOX.setProjectionMatrix(projectionMatrix);
			
			// ----------------------------------	

			obiectstart->setColor(0.4, 1, 0.1);
			BLACKBOX.setModelMatrix(transformation_start);
			BLACKBOX.drawSegments(obiectstart); //segments

			obiect1->setColor(1, 0.7, 0.2);
			BLACKBOX.setModelMatrix(transformation);
			BLACKBOX.drawSegments(obiect1); //segments

			obiectfinal->setColor(1, 0.2, 0);
			BLACKBOX.setModelMatrix(transformation);
			BLACKBOX.drawSegments(obiectfinal); //segments

			// Draw the object
			for (int i = 0; i < OBJECTS_NO; i++){
				obiect[i]->setColor(0.5, 0.5, 0.5);
				BLACKBOX.setModelMatrix(transformation);
				BLACKBOX.drawSegments(obiect[i]); //segments
			}

			spacebarpressed();

			// Draw world axis (world coordinate system)
			BLACKBOX.setModelMatrix(glm::mat4(1));
			BLACKBOX.drawAxisSystem(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), 500, 2);

			//FPScamera screen
			BLACKBOX.notifyDisplay();

			glViewport(width - 300, height - 300, 300, 300);

			// Send view matrix to the GPU
			BLACKBOX.setViewMatrix(cameraFPS.getViewMatrix());

			// Send projection matrix to the GPU
			BLACKBOX.setProjectionMatrix(projectionMatrix);

			obiectstart->setColor(0.4, 1, 0.1);
			BLACKBOX.setModelMatrix(transformation_start);
			BLACKBOX.drawSegments(obiectstart); //segments

			//	printf("x %f y %f z %f\n", obiectstart->x, obiectstart->y,  obiectstart->z);

			obiect1->setColor(1, 0.7, 0.2);
			BLACKBOX.setModelMatrix(transformation);
			BLACKBOX.drawSegments(obiect1); //segments

			obiectfinal->setColor(1, 0.2, 0);
			BLACKBOX.setModelMatrix(transformation);
			BLACKBOX.drawSegments(obiectfinal); //segments

			// Draw the object
			for (int i = 0; i < OBJECTS_NO; i++){
				obiect[i]->setColor(0.5, 0.5, 0.5);
				BLACKBOX.setModelMatrix(transformation);
				BLACKBOX.drawSegments(obiect[i]); //segments
			}
			spacebarpressed();
		}

		// Called when the frame ended
		void notifyEndFrame() { 
			// Camera Rotate90 TPS
			if (angle != 0){
				if (flag_key == 2){
					camera.rotateTPS_OY(0.0157f);
					cameraFPS.rotateFPS_OY(0.0157f);
					angle--;
				}
				if (flag_key == 1){
					camera.rotateTPS_OY(-0.0157f);
					angle--;
					cameraFPS.rotateFPS_OY(-0.0157f);
				}
			}
		}
		//---------------------------------------------------------------------
		// Function called when the windows was resized
		void notifyReshape(int width, int height, int previos_width, int previous_height)
		{
			//blackbox needs to know
			BLACKBOX.notifyReshape(width, height);
			aspectRatio = (float)width / height;
		}

		//---------------------------------------------------------------------
		// Input function

		// Mouse drag, mouse button pressed 
		void notifyMouseDrag(int mouse_x, int mouse_y) { }

		// Mouse move without pressing any button
		void notifyMouseMove(int mouse_x, int mouse_y) { }

		// Mouse button click
		void notifyMouseClick(int button, int state, int mouse_x, int mouse_y) { }

		// Mouse scrolling
		void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y) { }
};

int main()
{
	// Initialize GLUT: window + input + OpenGL context
	lab::glut::WindowInfo window(std::string("EGC Laborator 5 - Camera and Projections"), 800, 600, 600, 100, true);
	lab::glut::ContextInfo context(3, 3, false);
	lab::glut::FramebufferInfo framebuffer(true, true, false, false);
	lab::glut::init(window, context, framebuffer);

	// Initialize GLEW + load OpenGL extensions 
	glewExperimental = true;
	glewInit();
	std::cout << "[GLEW] : initializare" << std::endl;

	// Create a new instance of Lab and listen for OpenGL callback
	// Must be created after GLEW because we need OpenGL extensions to be loaded

	Laborator5 *lab5 = new Laborator5();
	lab::glut::setListener(lab5);

	// Enter loop
	lab::glut::run();

	return 0;
}