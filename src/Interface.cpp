#include "Interface.h"
#include "Arwing.h"
#include <algorithm>

std::shared_ptr<vao_t> Interface::createColoredQuad(float x1, float y1, float x2, float y2,
                                                     float r, float g, float b) {
    std::vector<float> positions = {
        x1, y1, 0.0f,
        x2, y1, 0.0f,
        x2, y2, 0.0f,
        x1, y2, 0.0f,
    };
    std::vector<float> colors = {
        r, g, b,
        r, g, b,
        r, g, b,
        r, g, b,
    };
    std::vector<unsigned> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    auto shape = std::make_shared<vao_t>();
    CHECKED_GL_CALL(glGenVertexArrays(1, &shape->vaoId));
    CHECKED_GL_CALL(glBindVertexArray(shape->vaoId));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->vboId[0]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->vboId[0]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->vboId[1]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->vboId[1]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), &colors[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->iboId));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->iboId));
    CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned), &indices[0], GL_STATIC_DRAW));
    shape->numElements = indices.size();
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindVertexArray(0));
    return shape;
}

std::shared_ptr<vao_t> Interface::createTriangle(float r, float g, float b) {
    std::vector<float> positions = {
         0.0f,  0.02f, 0.0f,
        -0.015f, -0.01f, 0.0f,
         0.015f, -0.01f, 0.0f,
    };
    std::vector<float> colors = {
        r, g, b,
        r, g, b,
        r, g, b,
    };
    std::vector<unsigned> indices = { 0, 1, 2 };

    auto shape = std::make_shared<vao_t>();
    CHECKED_GL_CALL(glGenVertexArrays(1, &shape->vaoId));
    CHECKED_GL_CALL(glBindVertexArray(shape->vaoId));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->vboId[0]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->vboId[0]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->vboId[1]));
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->vboId[1]));
    CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), &colors[0], GL_STATIC_DRAW));
    CHECKED_GL_CALL(glGenBuffers(1, &shape->iboId));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->iboId));
    CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned), &indices[0], GL_STATIC_DRAW));
    shape->numElements = indices.size();
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindVertexArray(0));
    return shape;
}

Interface::Interface(unsigned maxHealth)
    : maxHealth(maxHealth)
{
    // Health bar background (dark red) - bottom left
    hbarBgShape = createColoredQuad(-0.95f, -0.95f, -0.45f, -0.88f, 0.3f, 0.0f, 0.0f);
    // Health bar foreground (green)
    hbarFgShape = createColoredQuad(-0.95f, -0.95f, -0.45f, -0.88f, 0.0f, 0.8f, 0.0f);
    // Score tick (small white square)
    scoreTickShape = createColoredQuad(0.0f, 0.0f, 0.015f, 0.04f, 1.0f, 1.0f, 1.0f);
    // Game over overlay (red, full screen)
    overlayShape = createColoredQuad(-2.0f, -2.0f, 2.0f, 2.0f, 0.8f, 0.0f, 0.0f);
    // Game over X shape - two crossing bars
    gameOverXShape = createColoredQuad(-0.02f, -0.3f, 0.02f, 0.3f, 1.0f, 1.0f, 1.0f);
    // Damage flash (red, full screen)
    damageFlashShape = createColoredQuad(-2.0f, -2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f);
    // Life triangle (green)
    lifeShape = createTriangle(0.0f, 0.9f, 0.2f);
}

void Interface::renderQuad(const std::shared_ptr<Program> colorProg, std::shared_ptr<vao_t>& vao) {
    CHECKED_GL_CALL(glBindVertexArray(vao->vaoId));
    int h_pos = colorProg->getAttribute("vertPos");
    GLSL::enableVertexAttribArray(h_pos);
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao->vboId[0]));
    CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
    int h_col = colorProg->getAttribute("vertCol");
    GLSL::enableVertexAttribArray(h_col);
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao->vboId[1]));
    CHECKED_GL_CALL(glVertexAttribPointer(h_col, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->iboId));
    CHECKED_GL_CALL(glDrawElements(GL_TRIANGLES, vao->numElements, GL_UNSIGNED_INT, (const void *)0));
    GLSL::disableVertexAttribArray(h_pos);
    GLSL::disableVertexAttribArray(h_col);
    CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    CHECKED_GL_CALL(glBindVertexArray(0));
}

void Interface::draw(const std::shared_ptr<Program> colorProg,
                     const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                     const glm::mat4& V, int health, unsigned score)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    colorProg->bind();

    // Health bar background
    M->pushMatrix();
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), 0.7f);
        renderQuad(colorProg, hbarBgShape);
    M->popMatrix();

    // Health bar foreground (scaled by health ratio)
    float healthRatio = (float)health / (float)maxHealth;
    M->pushMatrix();
        M->translate(glm::vec3(-0.95f, 0.0f, 0.0f));
        M->scale(glm::vec3(healthRatio, 1.0f, 1.0f));
        M->translate(glm::vec3(0.95f, 0.0f, 0.0f));
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), 0.9f);
        renderQuad(colorProg, hbarFgShape);
    M->popMatrix();

    // Score ticks (one per kill, max 50)
    unsigned displayScore = std::min(score, 50u);
    for (unsigned i = 0; i < displayScore; ++i) {
        M->pushMatrix();
            float xOff = -0.95f + i * 0.02f;
            M->translate(glm::vec3(xOff, -0.80f, 0.0f));
            glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
            glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
            glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
            glUniform1f(colorProg->getUniform("alpha"), 0.8f);
            renderQuad(colorProg, scoreTickShape);
        M->popMatrix();
    }

    colorProg->unbind();
    glDisable(GL_BLEND);
}

void Interface::drawGameOver(const std::shared_ptr<Program> colorProg,
                             const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                             const glm::mat4& V)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    colorProg->bind();

    // Red overlay
    M->pushMatrix();
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), 0.4f);
        renderQuad(colorProg, overlayShape);
    M->popMatrix();

    // X shape - first bar (rotated 45 degrees)
    M->pushMatrix();
        M->rotate(glm::radians(45.0f), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), 0.9f);
        renderQuad(colorProg, gameOverXShape);
    M->popMatrix();

    // X shape - second bar (rotated -45 degrees)
    M->pushMatrix();
        M->rotate(glm::radians(-45.0f), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), 0.9f);
        renderQuad(colorProg, gameOverXShape);
    M->popMatrix();

    colorProg->unbind();
    glDisable(GL_BLEND);
}

void Interface::drawLives(const std::shared_ptr<Program> colorProg,
                          const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                          const glm::mat4& V, int lives) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    colorProg->bind();

    for (int i = 0; i < lives; ++i) {
        M->pushMatrix();
            M->translate(glm::vec3(-0.93f + i * 0.04f, 0.92f, 0.0f));
            glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
            glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
            glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
            glUniform1f(colorProg->getUniform("alpha"), 0.9f);
            renderQuad(colorProg, lifeShape);
        M->popMatrix();
    }

    colorProg->unbind();
    glDisable(GL_BLEND);
}

void Interface::triggerDamageFlash() {
    damageFlashTimer = 15.0f;
}

void Interface::drawDamageFlash(const std::shared_ptr<Program> colorProg,
                                const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                                const glm::mat4& V) {
    if (damageFlashTimer <= 0) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    colorProg->bind();

    M->pushMatrix();
        glUniformMatrix4fv(colorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(colorProg->getUniform("V"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(colorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniform1f(colorProg->getUniform("alpha"), damageFlashTimer / 15.0f * 0.3f);
        renderQuad(colorProg, damageFlashShape);
    M->popMatrix();

    colorProg->unbind();
    glDisable(GL_BLEND);
}

void Interface::advance() {
    if (damageFlashTimer > 0) {
        damageFlashTimer -= 1.0f;
    }
}
