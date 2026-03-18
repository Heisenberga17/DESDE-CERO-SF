/* ZJ Wood CPE 471 Lab 3 base code - I. Dunn class re-write */
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Shape.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

#include "common.h"
#include "Arwing.h"
#include "Enemy.h"
#include "Environment.h"
#include "Interface.h"

#define TURN_RATE 10
#define CAMERA_MOVE_SPEED 0.75
#define CAMERA_ORIGIN_Y 0.0
#define CAMERA_ORIGIN_Z 0.0
#define CAMERA_FOLLOW_X 0.75
#define CAMERA_FOLLOW_Y 1.2

#define PI 3.14159

enum GameState { PLAYING, GAME_OVER };

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;
	std::map<std::string, std::shared_ptr<Program>> programs;
	std::map<std::string, std::vector<std::shared_ptr<Shape>>> shapes;
	std::map<std::string, GLuint> textures;

	// Game
	double spawnTimer = 0.0;
	bool paused = false;
	unsigned hitCount = 0;
	GameState gameState = PLAYING;
	int lives = 3;

	// Wave system
	int waveNumber = 0;
	int enemiesLeftInWave = 0;
	double waveSpawnTimer = 0.0;
	double waveCooldownTimer = 0.0;
	bool waveActive = false;

	// Ring spawn timer
	double ringSpawnTimer = 0.0;

	// Camera
	bool cameraUnlock = false;
	bool mouseDown = false;
	std::vector<double> initialClick = { 0.0, 0.0 };
	glm::vec3 cameraPos = glm::vec3(0.0, CAMERA_ORIGIN_Y, CAMERA_ORIGIN_Z);
	glm::vec2 lookDir = glm::vec2(glm::radians(90.0), 0.0);
	glm::vec2 newLook = lookDir;

	// Arwing
	std::shared_ptr<Arwing> arwing;
	// Environment
	std::shared_ptr<Environment> environment;
	// Enemy
	std::shared_ptr<Enemy> enemy;
	// Interface
	std::shared_ptr<Interface> interface;

	// Light
	glm::vec3 lightPos = {-10.0, 15.0, 10.0};



	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			paused = !paused;
		}

		// Restart
		if (key == GLFW_KEY_R && action == GLFW_PRESS && gameState == GAME_OVER) {
			restartGame();
		}

		if (key == GLFW_KEY_U && action == GLFW_PRESS) {
			if (cameraUnlock) {
				cameraUnlock = false;
				cameraPos = glm::vec3(0.0, CAMERA_ORIGIN_Y, CAMERA_ORIGIN_Z);
				lookDir = newLook = glm::vec2(glm::radians(90.0), 0.0);
			}
			else {
				cameraUnlock = true;
			}
		}

		// Camera controls
		if (cameraUnlock) {
			if(key == GLFW_KEY_W) {
				cameraPos += glm::vec3(
					CAMERA_MOVE_SPEED*cos(lookDir[1])*cos(lookDir[0]),
					CAMERA_MOVE_SPEED*sin(lookDir[1]),
					CAMERA_MOVE_SPEED*cos(lookDir[1])*sin(lookDir[0])
				);
			}
        	else if(key == GLFW_KEY_S) {
				cameraPos -= glm::vec3(
					CAMERA_MOVE_SPEED*cos(lookDir[1])*cos(lookDir[0]),
					CAMERA_MOVE_SPEED*sin(lookDir[1]),
					CAMERA_MOVE_SPEED*cos(lookDir[1])*sin(lookDir[0])
				);
			}
        	else if( key == GLFW_KEY_A) {
				glm::vec3 scaledLook = glm::vec3(
					CAMERA_MOVE_SPEED*cos(lookDir[1])*cos(lookDir[0]),
					CAMERA_MOVE_SPEED*sin(lookDir[1]),
					CAMERA_MOVE_SPEED*cos(lookDir[1])*sin(lookDir[0])
				);
				cameraPos -= glm::cross(scaledLook, glm::vec3(0, 1, 0));
			}
       		else if( key == GLFW_KEY_D) {
				glm::vec3 scaledLook = glm::vec3(
					CAMERA_MOVE_SPEED*cos(lookDir[1])*cos(lookDir[0]),
					CAMERA_MOVE_SPEED*sin(lookDir[1]),
					CAMERA_MOVE_SPEED*cos(lookDir[1])*sin(lookDir[0])
				);
				cameraPos += glm::cross(scaledLook, glm::vec3(0, 1, 0));
			}
		}

		if (gameState == GAME_OVER) return;

		// Arwing Controls
		if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			arwing->yawLeft(KEY_PRESS);
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
			arwing->yawLeft(KEY_RELEASE);
		}
		if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			arwing->yawRight(KEY_PRESS);
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
			arwing->yawRight(KEY_RELEASE);
		}
		if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			arwing->pitchDown(KEY_PRESS);
		}
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
			arwing->pitchDown(KEY_RELEASE);
		}
		if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			arwing->pitchUp(KEY_PRESS);
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
			arwing->pitchUp(KEY_RELEASE);
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			arwing->shoot();
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			arwing->rollLeft(KEY_PRESS);
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			arwing->rollLeft(KEY_RELEASE);
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			arwing->rollRight(KEY_PRESS);
		}
		if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
			arwing->rollRight(KEY_RELEASE);
		}
		if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			arwing->barrelRoll();
		}

		// Boost & Brake
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
			arwing->speedMultiplier = 2.0f;
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
			arwing->speedMultiplier = 1.0f;
		}
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
			arwing->speedMultiplier = 0.3f;
		}
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
			arwing->speedMultiplier = 1.0f;
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &(initialClick[0]), &(initialClick[1]));
			mouseDown = true;
		}
		if (action == GLFW_RELEASE) {
			lookDir = newLook;
			mouseDown = false;
		}
	}

	void scrollCallback(GLFWwindow *window, double dX, double dY)
	{
	}

	void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
	{
		if (cameraUnlock && mouseDown) {
        	int width, height;
        	glfwGetWindowSize(window, &width, &height);
			newLook[0] = lookDir[0] + glm::radians(-90 * (xpos - initialClick[0])/(width/2));
			newLook[1] = lookDir[1] + glm::radians(glm::clamp(80.0 * (ypos - initialClick[1])/(height/2), -80.0, 80.0));
		}
	}

	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	void initGeom(const std::string& resourceDir)
	{
		std::vector<tinyobj::shape_t> TOshapes;
		std::vector<tinyobj::material_t> TOmats;
		std::string err;
		std::string basePath = resourceDir + "/";

		arwing = std::make_shared<Arwing>(basePath);
		arwing->measure();

		enemy = std::make_shared<Enemy>(basePath);
		enemy->measure();

		environment = std::make_shared<Environment>(basePath);

		interface = std::make_shared<Interface>(ARWING_MAX_HEALTH);
	}

	void initTex(const std::string& resourceDir)
	{
	}

	void initGame()
	{
		spawnTimer = glfwGetTime();
		ringSpawnTimer = glfwGetTime();
		startNewWave();
	}

	void restartGame() {
		gameState = PLAYING;
		lives = 3;
		hitCount = 0;
		waveNumber = 0;
		arwing->reset();
		enemy->clearAll();
		spawnTimer = glfwGetTime();
		ringSpawnTimer = glfwGetTime();
		startNewWave();
	}

	void startNewWave() {
		waveNumber++;
		enemiesLeftInWave = 2 + waveNumber;
		waveActive = true;
		waveSpawnTimer = glfwGetTime();
	}

	void initProg(const std::string& resourceDir)
	{
		GLSL::checkVersion();
		glClearColor(0.55f, 0.2f, 0.7f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glBlendColor(1,1,1,1);

		programs["texture"] = std::make_shared<Program>();
		programs["texture"]->setVerbose(true);
		programs["texture"]->setShaderNames(resourceDir + "/texture_vert_shader.glsl",
    										resourceDir + "/texture_frag_shader.glsl");
		programs["texture"]->init();
		programs["texture"]->addUniform("uTexture");
		programs["texture"]->addUniform("P");
		programs["texture"]->addUniform("V");
		programs["texture"]->addUniform("M");
		programs["texture"]->addUniform("lightPosition");
		programs["texture"]->addUniform("La");
		programs["texture"]->addUniform("Ld");
		programs["texture"]->addUniform("Ls");
		programs["texture"]->addUniform("Ka");
		programs["texture"]->addUniform("Kd");
		programs["texture"]->addUniform("Ks");
		programs["texture"]->addUniform("Shine");
		programs["texture"]->addUniform("texOffset");
		programs["texture"]->addAttribute("vertPos");
		programs["texture"]->addAttribute("vertNor");
		programs["texture"]->addAttribute("vertTex");

		programs["crosshair"] = std::make_shared<Program>();
		programs["crosshair"]->setVerbose(true);
		programs["crosshair"]->setShaderNames(resourceDir + "/crosshair_vert_shader.glsl",
    										  resourceDir + "/crosshair_frag_shader.glsl");
		programs["crosshair"]->init();
		programs["crosshair"]->addUniform("P");
		programs["crosshair"]->addUniform("V");
		programs["crosshair"]->addUniform("M");
		programs["crosshair"]->addAttribute("vertPos");
		programs["crosshair"]->addAttribute("vertNor");
		programs["crosshair"]->addAttribute("vertTex");

		programs["exhaust"] = std::make_shared<Program>();
		programs["exhaust"]->setVerbose(true);
		programs["exhaust"]->setShaderNames(resourceDir + "/exhaust_vert_shader.glsl",
    										resourceDir + "/exhaust_frag_shader.glsl");
		programs["exhaust"]->init();
		programs["exhaust"]->addUniform("P");
		programs["exhaust"]->addUniform("V");
		programs["exhaust"]->addUniform("M");
		programs["exhaust"]->addAttribute("vertPos");

		programs["simple_color"] = std::make_shared<Program>();
		programs["simple_color"]->setVerbose(true);
		programs["simple_color"]->setShaderNames(resourceDir + "/simple_color_vert_shader.glsl",
    										  	 resourceDir + "/simple_color_frag_shader.glsl");
		programs["simple_color"]->init();
		programs["simple_color"]->addUniform("P");
		programs["simple_color"]->addUniform("V");
		programs["simple_color"]->addUniform("M");
		programs["simple_color"]->addUniform("alpha");
		programs["simple_color"]->addAttribute("vertPos");
		programs["simple_color"]->addAttribute("vertCol");

		programs["explosion"] = std::make_shared<Program>();
		programs["explosion"]->setVerbose(true);
		programs["explosion"]->setShaderNames(resourceDir + "/explosion_vert_shader.glsl",
    										  resourceDir + "/explosion_frag_shader.glsl");
		programs["explosion"]->init();
		programs["explosion"]->addUniform("P");
		programs["explosion"]->addUniform("V");
		programs["explosion"]->addUniform("M");
		programs["explosion"]->addAttribute("vertPos");
	}

	void render()
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto P = std::make_shared<MatrixStack>();
		auto M = std::make_shared<MatrixStack>();
		glm::mat4 V;
		if (cameraUnlock) {
			glm::vec3 lookV = glm::vec3(
				cos(newLook[1])*cos(newLook[0]),
				sin(newLook[1]),
				cos(newLook[1])*sin(newLook[0])
			);
			V = glm::lookAt(cameraPos, cameraPos + lookV , glm::vec3(0, 1, 0));
		}
		else {
			glm::vec3 newCameraPos = cameraPos + glm::vec3(0.5*arwing->position.x, 0.5*arwing->position.y, 0);
			glm::vec3 newCameraLook = glm::vec3(
				CAMERA_FOLLOW_X*cos(glm::radians(arwing->yaw-90.0)),
				CAMERA_FOLLOW_Y*cos(glm::radians(arwing->pitch)),
				arwing->position.z
			);
			V = glm::lookAt(newCameraPos, newCameraPos + newCameraLook, glm::vec3(0, 1, 0));
		}


		P->pushMatrix();
		P->perspective(45.f, aspect, 1.f, 100.f);
		M->loadIdentity();

		// ENVIRONMENT
		environment->draw(programs["texture"], P, M, V, lightPos);

		// RINGS disabled

		// ENEMIES
		enemy->draw(programs["texture"], programs["explosion"], P, M, V, lightPos);
		enemy->drawProjectiles(programs["texture"], P, M, V, lightPos);

		// ARWING
		arwing->draw(programs["texture"], programs["exhaust"], programs["crosshair"], P, M, V, lightPos);

		P->popMatrix();

		if (!paused && gameState == PLAYING) {
			// ADVANCE ENVIRONMENT
			environment->advance(arwing->speedMultiplier);

			// ENEMY COLLISIONS & ADVANCE
			if (arwing->isAlive && enemy->checkCollisions(arwing->position, ARWING_HIT_RADIUS)) {
				arwing->takeDamage(1);
				interface->triggerDamageFlash();
			}

			// Enemy projectile collision
			if (arwing->isAlive && enemy->checkEnemyProjectileCollision(arwing->position, ARWING_HIT_RADIUS)) {
				arwing->takeDamage(1);
				interface->triggerDamageFlash();
			}

			for (auto p = arwing->projectiles.begin(); p != arwing->projectiles.end(); ++p) {
				std::vector<std::shared_ptr<EnemyUnit>> enemiesHit = enemy->checkProjectile((*p)->position, ARWING_PROJECTILE_HIT_RADIUS);
				for (auto e = enemiesHit.begin(); e != enemiesHit.end(); ++e) {
					++hitCount;
					(*e)->explode();
				}
			}
			enemy->advance(arwing->position);

			// WAVE SPAWNING
			double now = glfwGetTime();
			if (waveActive && enemiesLeftInWave > 0) {
				if (now - waveSpawnTimer >= 0.5) {
					waveSpawnTimer = now;
					float speedScale = 1.0f + 0.1f * waveNumber;
					enemy->spawnEnemy(speedScale);
					enemiesLeftInWave--;
					if (enemiesLeftInWave <= 0) {
						waveActive = false;
						waveCooldownTimer = now;
					}
				}
			} else if (!waveActive) {
				if (now - waveCooldownTimer >= 8.0) {
					startNewWave();
				}
			}

			// RINGS disabled

			// ARWING ADVANCE
			arwing->advance();

			// GROUND COLLISION
			if (arwing->isAlive && environment->checkGroundCollision(arwing->position, ARWING_HIT_RADIUS)) {
				arwing->takeDamage(1);
				interface->triggerDamageFlash();
			}

			// CHECK DEATH
			if (!arwing->isAlive) {
				lives--;
				if (lives > 0) {
					arwing->reset();
				} else {
					gameState = GAME_OVER;
				}
			}

			// INTERFACE ADVANCE
			interface->advance();
		}

		// INTERFACE (HUD) - always render, even when paused
		P->pushMatrix();
		if (width > height) {
			P->ortho(-1*aspect, 1*aspect, -1, 1, -2, 100.0f);
		} else {
			P->ortho(-1, 1, -1*1/aspect, 1*1/aspect, -2, 100.0f);
		}
		M->pushMatrix();
			glDisable(GL_DEPTH_TEST);
			interface->drawDamageFlash(programs["simple_color"], P, M, V);
			if (gameState == GAME_OVER) {
				interface->drawGameOver(programs["simple_color"], P, M, V);
			}
			glEnable(GL_DEPTH_TEST);
		M->popMatrix();
		P->popMatrix();
	}

};


int main(int argc, char **argv)
{

	std::string shaderDir = "../shaders";
	std::string resourceDir = "../resources";

	Application *application = new Application();

	WindowManager * windowManager = new WindowManager();
	windowManager->init(1024, 768);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	application->initProg(shaderDir);
	application->initTex(resourceDir);
	application->initGeom(resourceDir);
	application->initGame();

    srand (static_cast <unsigned> (time(0)));
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		application->render();
		glfwSwapBuffers(windowManager->getHandle());
		glfwPollEvents();
	}

	windowManager->shutdown();
	return 0;
}
