#include "mode.h"
#include "Plane.h"

void ModeFBWA::update()
{
    // set nav_roll and nav_pitch using sticks
    plane.nav_roll_cd  = plane.channel_roll->norm_input() * plane.roll_limit_cd;
    plane.update_load_factor();
    float pitch_input = plane.channel_pitch->norm_input();
    if (pitch_input > 0) {
        plane.nav_pitch_cd = pitch_input * plane.aparm.pitch_limit_max_cd;
    } else {
        plane.nav_pitch_cd = -(pitch_input * plane.pitch_limit_min_cd);
    }
    #if HAL_GROUND_EFFECT_ENABLED
    //leitura do canal do rádio configurado para aux_func efeito solo (175)
    RC_Channel *chan_gndef = rc().find_channel_for_option(RC_Channel::AUX_FUNC::GROUND_EFFECT);
    //verifica se o canal está configurado para efeito solo
    bool gndef_mode = chan_gndef->get_aux_switch_pos() == RC_Channel::AuxSwitchPos::HIGH;
    printf("gndef: %d\n", (int)gndef_mode);
    //se chave estiver para baixo, realiza o voo em efeito solo, do contrario, voa com fbwa normalmente
    if(gndef_mode){
        plane.g2.ground_effect_controller.update();
        // If the rc throttle input is zero (within dead zone), supress throttle
        // This allows the user to stop flight by reflexively cutting the throttle
        if(plane.channel_throttle->in_trim_dz()){
            SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, 0);
        } else {
            SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, plane.g2.ground_effect_controller.get_throttle());
        }
        plane.nav_pitch_cd += plane.g2.ground_effect_controller.get_pitch(); // Note that this stacks
        printf("###elevator: %f, elevator_gndef: %f###\n",(float)(plane.nav_pitch_cd),(float)(plane.g2.ground_effect_controller.get_pitch()));
    } else {
    #endif
        plane.adjust_nav_pitch_throttle();
    #if HAL_GROUND_EFFECT_ENABLED 
    }
    #endif
    plane.nav_pitch_cd = constrain_int32(plane.nav_pitch_cd, plane.pitch_limit_min_cd, plane.aparm.pitch_limit_max_cd.get());
    if (plane.fly_inverted()) {
        plane.nav_pitch_cd = -plane.nav_pitch_cd;
    }
    if (plane.failsafe.rc_failsafe && plane.g.fs_action_short == FS_ACTION_SHORT_FBWA) {
        // FBWA failsafe glide
        plane.nav_roll_cd = 0;
        plane.nav_pitch_cd = 0;
        SRV_Channels::set_output_limit(SRV_Channel::k_throttle, SRV_Channel::Limit::MIN);
    }
    RC_Channel *chan = rc().find_channel_for_option(RC_Channel::AUX_FUNC::FBWA_TAILDRAGGER);
    if (chan != nullptr) {
        // check for the user enabling FBWA taildrag takeoff mode
        bool tdrag_mode = chan->get_aux_switch_pos() == RC_Channel::AuxSwitchPos::HIGH;
        if (tdrag_mode && !plane.auto_state.fbwa_tdrag_takeoff_mode) {
            if (plane.auto_state.highest_airspeed < plane.g.takeoff_tdrag_speed1) {
                plane.auto_state.fbwa_tdrag_takeoff_mode = true;
                plane.gcs().send_text(MAV_SEVERITY_WARNING, "FBWA tdrag mode");
            }
        }
    }
}
