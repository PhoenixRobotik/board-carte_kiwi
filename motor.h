#pragma once

#include "pwm.h"

#ifdef __cplusplus

#define RPM2PULSE_COEFF   (-0.34)
#define RPM2PULSE_OFFSET     (15)
#define PULSE_MIDPOINT   (1494.5)
#define PULSE_FORWARD_H    (1099)
#define PULSE_FORWARD_L    (1469)
#define PULSE_BACKWARD_L   (1520)
#define PULSE_BACKWARD_H   (1890)

#define PULSE_MIN (PULSE_FORWARD_H)
#define PULSE_MAX (PULSE_BACKWARD_H)


class WheelHubMotor
{
public:
    WheelHubMotor(PWM _pwm)
    : pwm(_pwm)
    {
        init();
    }

    ~WheelHubMotor() {
        deinit();
    }

    void init();
    void deinit() { }

    void enable();

    // setters
    bool set_percent_speed(double percent);
    // bool set_rot_per_min_speed(double target_speed);
    // bool set_rot_per_sec_speed(double target_speed);
    // bool set_rad_per_sec_speed(double target_speed);

    // getters
    // void get_rot_per_min_speed(double& physical_speed);
    // void get_rot_per_sec_speed(double& physical_speed);
    // void get_rad_per_sec_speed(double& physical_speed);

private:
    PWM pwm;
};


extern "C" {
#endif

struct WheelHubMotor;
void WheelHubMotor_set_duty   (WheelHubMotor* WheelHubMotor, uint32_t duty);
void WheelHubMotor_set_ms     (WheelHubMotor* WheelHubMotor, int _millisec);
void WheelHubMotor_set_percent(WheelHubMotor* WheelHubMotor, int _percent);

#ifdef __cplusplus
}
#endif
