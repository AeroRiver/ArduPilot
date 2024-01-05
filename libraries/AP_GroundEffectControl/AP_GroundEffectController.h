#pragma once

#include <AP_AHRS/AP_AHRS.h>
#include <PID/PID.h>
#include <AP_RangeFinder/AP_RangeFinder.h>
#include <Filter/ComplementaryFilter.h>
#include "AC_PID/AP_PIDInfo.h"

#ifndef HAL_GROUND_EFFECT_ENABLED
 #define HAL_GROUND_EFFECT_ENABLED !HAL_MINIMIZE_FEATURES
#endif

#if HAL_GROUND_EFFECT_ENABLED

class GroundEffectController {
public:
    GroundEffectController()
        : _enabled{false}
        {
            AP_Param::setup_object_defaults(this, var_info);
            _rangefinder = RangeFinder::get_singleton();
            _ahrs = AP_AHRS::get_singleton();
            _singleton = this;
        };

    /* Do not allow copies */
    GroundEffectController(const GroundEffectController &other) = delete;
    GroundEffectController &operator=(const GroundEffectController&) = delete;

    bool user_request_enable(bool enable);

    bool enabled_by_user() { return _enabled; }

    void update(bool land);

    void altitude_adjustment(float ref);

	void reset();

    float turn_correction();

    float get_max_roll();

    int turn_limit_on();

    void speed_adjustment(float ref);
    
    bool throttle_ctrl_enabled();

    void cruise(float alt_error, float airspeed_error, float reading);

    void land_seq(float alt_error, float airspeed_error, float reading);

    bool obstacle_detect();

    void obstacle_avoid();

    float alt_adjust=0;

    float spd_aimed=0;

    float sr = 0;

    float alt_error_aux = 0;

    float spd_error_aux = 0;

    const       AP_PIDInfo& get_pid_info(void) const { return _pid_info; }

	static const struct AP_Param::GroupInfo var_info[];

    int32_t get_auto_lim_roll_cd();

    int32_t get_pitch() { return _pitch; }

    int16_t get_throttle() { return _throttle; }

    static GroundEffectController *get_singleton(void) { return _singleton; }

private:
    PID _pitch_pid{120.0, 0.0, 0.0, 1000};
    PID _throttle_pid{18.0, 4.0, 0, 300};

    AP_Int8 _ACTIVE;
	AP_Float _THR_REF;
    AP_Float _THR_MIN;
    AP_Float _THR_MAX;
    AP_Float _ALT_REF;
    AP_Float _CUTOFF_FREQ;
    AP_Float _LIM_ROLL;
    AP_Float _AIMED_AIRSPEED;
    AP_Int8 _ENABLE_TURN;
    AP_Float _WING_SPAN;
    AP_Float _SPD_PARAM;
    AP_Int8 _ENABLE_THR;
    AP_Float _VERT_SPD;
    AP_Float _FLARE_ANG;
    AP_Float _ALT_AUX;
    AP_Float _SPD_AUX;
    AP_Float _MIN_LAND;
    AP_Float _MAX_LAND;


    AP_PIDInfo _pid_info;

    static GroundEffectController *_singleton;

    uint32_t _last_time_called;

    AP_AHRS* _ahrs;
    RangeFinder* _rangefinder;
    ComplementaryFilter _altFilter;

    float _last_good_rangefinder_reading;

    bool _enabled;
    int32_t _pitch;
    int16_t _throttle;
    int16_t _throttle_ant;

};

#endif // HAL_GROUND_EFFECT_ENABLED