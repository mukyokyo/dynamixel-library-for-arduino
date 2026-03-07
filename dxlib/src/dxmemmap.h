#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define ADDRESS_MODEL_NUMBER              0   //W
#define ADDRESS_VERSION_FW                2   //B
#define ADDRESS_ID                        3   //B
#define ADDRESS_BAUDRATE                  4   //B
#define ADDRESS_RETURN_DELAY_TIME         5   //B
#define ADDRESS_CW_ANGLE_LIMIT            6   //W
#define ADDRESS_CCW_ANGLE_LIMIT           8   //W
#define ADDRESS_STATUS_RET_LEVEL          16  //B
#define ADDRESS_TORQUE_ENABLE             24  //B
#define ADDRESS_LED                       25  //B
#define ADDRESS_CW_COMP_MARGIN            26  //B
#define ADDRESS_CCW_COMP_MARGIN           27  //B
#define ADDRESS_CW_COMP_SLOPE             28  //B
#define ADDRESS_CCW_COMP_SLOPE            29  //B
#define ADDRESS_GOAL_POSITION             30  //W
#define ADDRESS_GOAL_SPEED                32  //W
#define ADDRESS_TORQUE_LIMIT              34  //W
#define ADDRESS_PRESENT_POSITION          36  //W
#define ADDRESS_PRESENT_SPEED             38  //W
#define ADDRESS_PRESENT_LOAD              40  //W
#define ADDRESS_PRESENT_VOLTAGE           42  //B
#define ADDRESS_PRESENT_TEMP              43  //B
#define ADDRESS_MOVING                    46  //B
#define ADDRESS_LOCK                      47  //B
#define ADDRESS_PUNCH                     48  //W

#define ADDRESS_EX_SENSED_CURRENT         56  //W

#define ADDRESS_MX_CURRENT                68  //W
#define ADDRESS_MX_TORQUE_MODE            70  //B
#define ADDRESS_MX_GOAL_TORQULE           71  //W
#define ADDRESS_MX_GOAL_ACCELERATION      73  //B


#define ADDRESS_X_MODEL_NUMBER            0   //W
#define ADDRESS_X_MODEL_INFORMATION       4   //L
#define ADDRESS_X_VERSION_FW              6   //B
#define ADDRESS_X_ID                      7   //B
#define ADDRESS_X_BAUDRATE                8   //B
#define ADDRESS_X_RETURN_DELAY_TIME       9   //B
#define ADDRESS_X_DRIVE_MODE              10  //B
#define ADDRESS_X_OPERATING_MODE          11  //B
#define ADDRESS_X_HOMING_OFFSET           20  //L
#define ADDRESS_X_MAX_POSITION_LIMIT      48  //L
#define ADDRESS_X_MIN_POSITION_LIMIT      52  //L
#define ADDRESS_X_TORQUE_ENABLE           64  //B
#define ADDRESS_X_LED_RED                 65  //B
#define ADDRESS_X_STATUS_RETURN_LEVEL     69  //B
#define ADDRESS_X_HARDWARE_ERROR_STATUS   70  //B
#define ADDRESS_X_VELOCITY_IGAIN          76  //W
#define ADDRESS_X_VELOCITY_PGAIN          78  //W
#define ADDRESS_X_POSITION_DGAIN          80  //W
#define ADDRESS_X_POSITION_IGAIN          82  //W
#define ADDRESS_X_POSITION_PGAIN          84  //W
#define ADDRESS_X_GOAL_PWM                100 //W
#define ADDRESS_X_GOAL_CURRENT            102 //W
#define ADDRESS_X_GOAL_VELOCITY           104 //L
#define ADDRESS_X_PROF_ACCELERATION       108 //L
#define ADDRESS_X_PROF_VELOCITY           112 //L
#define ADDRESS_X_GOAL_POSITION           116 //L
#define ADDRESS_X_MOVING                  122 //B
#define ADDRESS_X_MOVING_STATUS           123 //B
#define ADDRESS_X_PRESENT_PWM             124 //W
#define ADDRESS_X_PRESENT_CURRENT         126 //W
#define ADDRESS_X_PRESENT_VELOCITY        128 //L
#define ADDRESS_X_PRESENT_POSITION        132 //L
#define ADDRESS_X_VELOCITY_TRAJECTORY     136 //L
#define ADDRESS_X_POSITION_TRAJECTORY     140 //L
#define ADDRESS_X_PRESENT_VOLTAGE         144 //W
#define ADDRESS_X_PRESENT_TEMP            146 //B

#ifdef __cplusplus
}
#endif
