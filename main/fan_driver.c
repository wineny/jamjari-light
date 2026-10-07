/* 잠자리 — 시연용 가짜 선풍기: 28BYJ-48 스텝 모터를 half-step 으로 돌린다.
 * FreeRTOS tick 이 10ms 라 vTaskDelay 로는 너무 느려서 esp_timer(µs) 로 한 걸음씩 민다. */
#include "fan_driver.h"

#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_timer.h>

static const char *TAG = "fan_driver";

/* 28BYJ-48 half-step 순서 (IN1..IN4) */
static const uint8_t s_seq[8][4] = {
    {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
    {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1},
};
static const gpio_num_t s_pins[4] = {FAN_GPIO_IN1, FAN_GPIO_IN2, FAN_GPIO_IN3, FAN_GPIO_IN4};

/* 한 걸음 간격(µs). 5V 에서 1000µs 아래로 내리면 걸음을 놓친다. */
#define FAN_STEP_US_FAST 1100
#define FAN_STEP_US_SLOW 4000

static esp_timer_handle_t s_timer;
static int s_phase;
static uint8_t s_percent;

static void write_coils(const uint8_t *levels)
{
    for (int i = 0; i < 4; i++) {
        gpio_set_level(s_pins[i], levels[i]);
    }
}

static void step_cb(void *arg)
{
    s_phase = (s_phase + 1) & 7;
    write_coils(s_seq[s_phase]);
}

esp_err_t fan_driver_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = (1ULL << FAN_GPIO_IN1) | (1ULL << FAN_GPIO_IN2) | (1ULL << FAN_GPIO_IN3) |
                        (1ULL << FAN_GPIO_IN4),
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&io));
    static const uint8_t off[4] = {0, 0, 0, 0};
    write_coils(off);

    const esp_timer_create_args_t args = {
        .callback = step_cb,
        .name = "fan_step",
    };
    return esp_timer_create(&args, &s_timer);
}

esp_err_t fan_driver_set_percent(uint8_t percent)
{
    if (percent > 100) {
        percent = 100;
    }
    if (percent == s_percent) {
        return ESP_OK;
    }
    s_percent = percent;
    esp_timer_stop(s_timer); /* 안 돌고 있으면 ESP_ERR_INVALID_STATE, 무시 */

    if (percent == 0) {
        /* 멈출 때 코일 전원을 꺼야 모터·기판이 안 뜨거워진다 */
        static const uint8_t off[4] = {0, 0, 0, 0};
        write_coils(off);
        ESP_LOGI(TAG, "fan off");
        return ESP_OK;
    }
    uint64_t period = FAN_STEP_US_SLOW - (uint64_t)(FAN_STEP_US_SLOW - FAN_STEP_US_FAST) * (percent - 1) / 99;
    ESP_LOGI(TAG, "fan %u%% -> step %lluus", percent, period);
    return esp_timer_start_periodic(s_timer, period);
}
