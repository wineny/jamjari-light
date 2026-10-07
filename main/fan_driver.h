/* 잠자리 — 시연용 가짜 선풍기: 28BYJ-48 스텝 모터(ULN2003 기판)에 종이 날개 */
#pragma once

#include <esp_err.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ULN2003 IN1~IN4 */
#define FAN_GPIO_IN1 16
#define FAN_GPIO_IN2 17
#define FAN_GPIO_IN3 18
#define FAN_GPIO_IN4 19

esp_err_t fan_driver_init(void);

/* 0 = 멈춤(코일 전원 끔), 1~100 = 도는 속도 */
esp_err_t fan_driver_set_percent(uint8_t percent);

#ifdef __cplusplus
}
#endif
