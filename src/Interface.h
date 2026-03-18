#pragma once
#ifndef FINAL_471_INTERFACE_H_INCLUDED
#define FINAL_471_INTERFACE_H_INCLUDED

#include "Shape.h"
#include "common.h"
#include "Program.h"
#include "MatrixStack.h"
#include "GLSL.h"

class Interface {
public:
    Interface(unsigned maxHealth);

    void draw(const std::shared_ptr<Program> colorProg,
              const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
              const glm::mat4& V, int health, unsigned score);

    void drawGameOver(const std::shared_ptr<Program> colorProg,
                      const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                      const glm::mat4& V);

    void drawLives(const std::shared_ptr<Program> colorProg,
                   const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                   const glm::mat4& V, int lives);

    void triggerDamageFlash();
    void drawDamageFlash(const std::shared_ptr<Program> colorProg,
                         const std::shared_ptr<MatrixStack> P, const std::shared_ptr<MatrixStack> M,
                         const glm::mat4& V);

    void advance();

private:
    Interface(Interface const &a) = delete;

    void renderQuad(const std::shared_ptr<Program> colorProg, std::shared_ptr<vao_t>& vao);

    unsigned maxHealth;

    // Health bar background (dark red)
    std::shared_ptr<vao_t> hbarBgShape;
    // Health bar foreground (green)
    std::shared_ptr<vao_t> hbarFgShape;
    // Score tick
    std::shared_ptr<vao_t> scoreTickShape;
    // Game over overlay
    std::shared_ptr<vao_t> overlayShape;
    // Game over X
    std::shared_ptr<vao_t> gameOverXShape;
    // Damage flash overlay
    std::shared_ptr<vao_t> damageFlashShape;
    // Life triangle
    std::shared_ptr<vao_t> lifeShape;

    float damageFlashTimer = 0.0f;

    std::shared_ptr<vao_t> createColoredQuad(float x1, float y1, float x2, float y2,
                                              float r, float g, float b);
    std::shared_ptr<vao_t> createTriangle(float r, float g, float b);
};

#endif // FINAL_471_INTERFACE_H_INCLUDED
