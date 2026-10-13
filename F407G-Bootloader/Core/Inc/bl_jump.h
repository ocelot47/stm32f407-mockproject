#ifndef INC_BL_JUMP_H_
#define INC_BL_JUMP_H_

typedef enum
{
    APP_VALID = 0,
    APP_MSP_INVALID,
    APP_RESET_HANDLER_THUMB_INVALID,
    APP_RESET_HANDLER_RANGE_INVALID
} app_status_t;

app_status_t validate_app(void);
void jump_to_app(void);

#endif /* INC_BL_JUMP_H_ */
