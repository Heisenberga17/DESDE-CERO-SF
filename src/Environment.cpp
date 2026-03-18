#include "Environment.h"
#include "stb_image.h"
#include "common.h"
#include "GLSL.h"
#include <cmath>

Environment::Environment(std::string resourceDir) {
	std::vector<tinyobj::shape_t> TOshapes;
	std::vector<tinyobj::material_t> TOmats;
	std::string err;
	std::string basePath = resourceDir + "/";

	bool rc = tinyobj::LoadObj(TOshapes, TOmats, err, (basePath + "ground.obj").c_str(), basePath.c_str());
	if (!rc) {
		std::cerr << err << std::endl;
	}
	else {
		for (int i = 0; i < TOshapes.size(); ++i) {
			std::shared_ptr<Shape> shape = std::make_shared<Shape>();
			shape->createShape(TOshapes[i]);
			shape->measure();
			shape->init();
            groundShapes.push_back(shape);
			if (TOshapes[i].mesh.material_ids[0] >= 0) {
				shape->loadMaterial(TOmats[TOshapes[i].mesh.material_ids[0]], basePath);
				shape->useMaterial = true;
			}
			else {
                std::cout << "Error loading materials" << std::endl;
			}
		}
    }

	rc = tinyobj::LoadObj(TOshapes, TOmats, err, (basePath + "sky.obj").c_str(), basePath.c_str());
	if (!rc) {
		std::cerr << err << std::endl;
	}
	else {
		for (int i = 0; i < TOshapes.size(); ++i) {
			std::shared_ptr<Shape> shape = std::make_shared<Shape>();
			shape->createShape(TOshapes[i]);
			shape->measure();
			shape->init();
            skyShapes.push_back(shape);
			if (TOshapes[i].mesh.material_ids[0] >= 0) {
				shape->loadMaterial(TOmats[TOshapes[i].mesh.material_ids[0]], basePath);
				shape->useMaterial = true;
			}
			else {
                std::cout << "Error loading materials" << std::endl;
			}
		}
    }

    initRingGeometry();
}

void Environment::initRingGeometry() {
    const int segments = 32;
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<unsigned> indices;

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        positions.push_back(cos(angle));
        positions.push_back(sin(angle));
        positions.push_back(0.0f);
        // Gold color
        colors.push_back(1.0f);
        colors.push_back(0.84f);
        colors.push_back(0.0f);

        indices.push_back(i);
        indices.push_back((i + 1) % segments);
    }

    ringVAO = std::make_shared<vao_t>();
    CHECKED_GL_CALL(glGenVertexArrays(1, &ringVAO->vaoId));
    CHECKED_GL_CALL(glBindVertexArray(ringVAO->vaoId));
    CHECKED_GL_CALL(glGenBuffers(1, &ringVAO->vboId[0]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ringVAO->vboId[0]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &ringVAO->vboId[1]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ringVAO->vboId[1]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), &colors[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &ringVAO->iboId));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ringVAO->iboId));
    CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned), &indices[0], GL_STATIC_DRAW));
    ringVAO->numElements = indices.size();
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindVertexArray(0));
}

void Environment::draw(const std::shared_ptr<Program> prog, const std::shared_ptr<MatrixStack> P,
                       const std::shared_ptr<MatrixStack> M, const glm::mat4& V, const glm::vec3& lightPos) {
    prog->bind();
    // Draw Ground
	M->pushMatrix();
		M->translate(glm::vec3(0, -AIRSPACE_HEIGHT-2, GROUND_DEPTH));
		M->scale(glm::vec3(GROUND_WIDTH, 1, GROUND_DEPTH));
        M->rotate(glm::radians(90.0), glm::vec3(1, 0, 0));
        for (auto shape = groundShapes.begin(); shape != groundShapes.end(); ++shape) {
	        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
	        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		    glUniform3f(prog->getUniform("lightPosition"), lightPos.x, lightPos.y, lightPos.z);
		    glUniform3f(prog->getUniform("La"), 1, 1, 1);
		    glUniform3f(prog->getUniform("Ld"), 1, 1, 1);
		    glUniform3f(prog->getUniform("Ls"), 1, 1, 1);
		    glUniform2f(prog->getUniform("texOffset"), 0.0, texOffset.y);
		    (*shape)->draw(prog);
        }
	M->popMatrix();

	// Draw Sky
	M->pushMatrix();
		M->translate(glm::vec3(0, SKY_HEIGHT/2, 2*GROUND_DEPTH));
		M->scale(glm::vec3(SKY_WIDTH, SKY_HEIGHT, 1));
        for (auto shape = skyShapes.begin(); shape != skyShapes.end(); ++shape) {
	        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
	        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		    glUniform3f(prog->getUniform("lightPosition"), lightPos.x, lightPos.y, lightPos.z);
		    glUniform3f(prog->getUniform("La"), 1, 1, 1);
		    glUniform3f(prog->getUniform("Ld"), 1, 1, 1);
		    glUniform3f(prog->getUniform("Ls"), 1, 1, 1);
		    glUniform2f(prog->getUniform("texOffset"), 0.0, 0.0);
		    (*shape)->draw(prog);
        }
	M->popMatrix();

	prog->unbind();
}

void Environment::advance(float speedMultiplier) {
	texOffset.y += TEX_SCROLL_RATE * speedMultiplier;

    // Advance ring rotations
    for (auto& ring : rings) {
        ring.rotation += 2.0f;
    }
    // Remove rings that have gone behind the player
    for (auto it = rings.begin(); it != rings.end();) {
        if (it->collected) {
            it = rings.erase(it);
        } else {
            ++it;
        }
    }
}

bool Environment::checkGroundCollision(glm::vec3 position, double radius) {
	return position.y - radius <= groundHeight;
}

void Environment::spawnRing() {
    float x = -AIRSPACE_WIDTH + 2*AIRSPACE_WIDTH * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    float y = -AIRSPACE_HEIGHT/2 + AIRSPACE_HEIGHT * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    rings.push_back({glm::vec3(x, y, 60.0f), 0.0f, false});
}

bool Environment::checkRingCollision(glm::vec3 playerPos, float radius) {
    for (auto& ring : rings) {
        if (!ring.collected && glm::distance(playerPos, ring.position) <= radius + 4.0f) {
            ring.collected = true;
            return true;
        }
    }
    return false;
}

void Environment::drawRings(const std::shared_ptr<Program> colorProg, const std::shared_ptr<MatrixStack> P,
                            const std::shared_ptr<MatrixStack> M, const glm::mat4& V) {
    colorProg->bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0f);

    for (auto& ring : rings) {
        if (ring.collected) continue;
        M->pushMatrix();
            M->translate(ring.position);
            M->rotate(glm::radians(ring.rotation), glm::vec3(0, 1, 0));
            M->scale(glm::vec3(4.0f, 4.0f, 4.0f));
            glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
            glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(V));
            glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
            glUniform1f(colorProg->getUniform("alpha"), 1.0f);

            CHECKED_GL_CALL(glBindVertexArray(ringVAO->vaoId));
            int h_pos = colorProg->getAttribute("vertPos");
            GLSL::enableVertexAttribArray(h_pos);
            CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ringVAO->vboId[0]));
            CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0));
            int h_col = colorProg->getAttribute("vertCol");
            GLSL::enableVertexAttribArray(h_col);
            CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ringVAO->vboId[1]));
            CHECKED_GL_CALL(glVertexAttribPointer(h_col, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0));
            CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ringVAO->iboId));
            CHECKED_GL_CALL(glDrawElements(GL_LINES, ringVAO->numElements, GL_UNSIGNED_INT, (const void*)0));
            GLSL::disableVertexAttribArray(h_pos);
            GLSL::disableVertexAttribArray(h_col);
            CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
            CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            CHECKED_GL_CALL(glBindVertexArray(0));
        M->popMatrix();
    }

    glDisable(GL_BLEND);
    colorProg->unbind();
}
