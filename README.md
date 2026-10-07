# jamjari-light — 잠자리 조명 (ESP32, Matter)

침대 곁 조명 겸 시연용 가짜 선풍기를 Matter 기기로 내는 ESP32 펌웨어. [잠자리](https://github.com/wineny/jamjari-firmware)(nRF54LM20 센서) 시연 세트의 한 조각이다.

*English: ESP32 firmware that exposes a WS2812 LED ring (Extended Color Light) and a stepper-motor "fake fan" as Matter endpoints. Derived from the Espressif esp-matter `light` example.*

## 무엇

| endpoint | 기기 | 하드웨어 |
|---|---|---|
| 1 | Extended Color Light | WS2812 LED 링 (데이터 핀 GPIO5, 버튼 GPIO0) |
| 2 | Fan (Fan Control: FanMode / PercentSetting) | 28BYJ-48 스텝 모터 + ULN2003 (GPIO16~19), 시연용 가짜 선풍기 |

- 대상 칩은 `esp32`(`sdkconfig` 의 `CONFIG_IDF_TARGET`). ESP-IDF 5.5.5 로 만든 `dependencies.lock` 이 들어 있다.
- 콘솔 명령 `matter esp openwindow` 로 다른 fabric 이 남아 있어도 등록 창을 열 수 있다.
- Wi-Fi 이름·비밀번호는 `sdkconfig` 에 비어 있고, 저장소에 비밀 값은 없다.
- `ENABLE_TEST_SETUP_PARAMS=y`, 시험용 DAC·Certification Declaration 을 쓴다(esp-matter 예제 기본). 시연·개발용이며 제품 인증 값이 아니다.

## 빌드

[esp-matter](https://docs.espressif.com/projects/esp-matter/en/latest/esp32/developing.html) 환경(ESP-IDF + `ESP_MATTER_PATH`)을 잡은 뒤:

```bash
idf.py set-target esp32
idf.py build
idf.py -p <포트> flash monitor
```

## 출처·라이선스

- 이 저장소는 Apache-2.0 이다(`LICENSE`, `NOTICE`).
- Espressif esp-matter 의 `examples/light` 에서 시작했다. 원래 예제 README 는 이 파일로 바꿨다. 예제의 `main/` 은 「Public Domain (or CC0)」, `device_hal/` 은 Espressif Apache-2.0 헤더를 그대로 둔다.
- 원본: https://github.com/espressif/esp-matter/tree/main/examples/light
