#pragma once

#include <cstdint>


class PID {
public:
    PID(float kp, float ki, float kd, float saida_min = 0.0f, float saida_max = 255.0f);

    float compute(float setpoint, float leitura_atual, float dt);
    void reset();
    void setTunings(float kp, float ki, float kd);
    void setOutputLimits(float min, float max);
    

private:
    float kp_;
    float ki_;
    float kd_;
    float saida_min_;
    float saida_max_;

    float erro_anterior_ = 0.0f;
    float integral_      = 0.0f;
};