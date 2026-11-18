#include <PID.hpp>
#include <algorithm> // Para std::clamp

PID::PID(float kp, float ki, float kd, float saida_min, float saida_max)
    : kp_(kp), ki_(ki), kd_(kd), saida_min_(saida_min), saida_max_(saida_max) {}

float PID::compute(float setpoint, float leitura_atual, float dt) {
    if (dt <= 0.0f) {
        return 0.0f;
    }

    float erro = setpoint - leitura_atual;

    // Termo Proporcional
    float P = kp_ * erro;

    // Termo Integral com proteção anti-windup
    integral_ += erro * dt;
    float I = ki_ * integral_;
    I = std::clamp(I, saida_min_, saida_max_);

    // Termo Derivativo
    float derivativo = (erro - erro_anterior_) / dt;
    float D = kd_ * derivativo;

    erro_anterior_ = erro;

    // Saída final limitada
    float saida = P + I + D;
    return std::clamp(saida, saida_min_, saida_max_);
}

void PID::reset() {
    erro_anterior_ = 0.0f;
    integral_ = 0.0f;
}

void PID::setTunings(float kp, float ki, float kd) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}
