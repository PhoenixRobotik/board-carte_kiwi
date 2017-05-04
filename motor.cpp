#include "motor.h"

#include <cmath>


void WheelHubMotor::init() {
    // useless but explicit
    pwm->setMicrosec(0);
}

void WheelHubMotor::enable() {
	pwm->enable();
	// hall_sensor.enable();
	pwm->setMicrosec(static_cast<uint32_t>(std::round(PULSE_MIDPOINT)));
}

bool WheelHubMotor::set_percent_speed(double percent) {
	percent *= multiplier;

	double pwm_pulse;
	if (percent > 0 and percent<=100)
	{
		pwm_pulse  = percent/100*(PULSE_FORWARD_H-PULSE_FORWARD_L);
		pwm_pulse += PULSE_FORWARD_L;
	} else if (percent < 0 and percent>=-100) {
		pwm_pulse = -percent/100*(PULSE_BACKWARD_H-PULSE_BACKWARD_L);
		pwm_pulse += PULSE_BACKWARD_L;
	} else if (percent == 0) {
		pwm_pulse = PULSE_MIDPOINT;
	} else {
		return false;
	}
	pwm->setMicrosec(static_cast<uint32_t>(std::round(pwm_pulse)));
	return true;

}

// bool WheelHubMotor::set_rot_per_min_speed(double target_speed) {
//  target_speed *= multiplier;
// 	// computation for no load speed
// 	double pwm_pulse = target_speed*RPM2PULSE_COEFF;
// 	pwm_pulse += (pwm_pulse > 0 ? RPM2PULSE_OFFSET : -RPM2PULSE_OFFSET);
// 	pwm_pulse += PULSE_MIDPOINT;
// 	if (pwm_pulse < 0) {
// 		return false;
// 	}
// 	if (pwm_pulse < PULSE_MIN) {
// 		pwm_pulse = PULSE_MIN;
// 	}
// 	if (pwm_pulse > PULSE_MAX) {
// 		pwm_pulse = PULSE_MAX;
// 	}
// 	pwm.setMicrosec(std::round(pwm_pulse));
// 	return true;

// }

// bool WheelHubMotor::set_rot_per_sec_speed(double target_speed) {
// 	return set_rot_per_min_speed(target_speed*60);
// }

double WheelHubMotor::get_rot_per_sec_speed() {
	return 1000.0 / (HALL_TICK_PER_ROT * hall_sensor->get_pulse_period_ms());
}

double WheelHubMotor::get_rot_per_min_speed() {
	return 60 * get_rot_per_sec_speed();
}

double WheelHubMotor::get_rot_count() {
	return hall_sensor->get_pulse_count() / HALL_TICK_PER_ROT;
}

extern "C" {

}
