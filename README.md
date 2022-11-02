# Traffic-Light
STM32 동호회] Traffic Light

- **개발 필요성 :** 신호등이 폭설로 인해 사고의 발생을 방지하고자 신호등 발열 시스템을 제작
하게되었습니다.

- **개발 목표 :**

       ① CDS센서를 이용해 폭설을 감지하여 SMPS의 히팅시스템으로 발열시켜 눈을 녹임.
       ② TEMP센서를 이용해 일정온도 이하로 내려갔을 시 SMPS의 히팅시스템으로 발열 시켜 눈을

           녹임.

- **개발 기간 :** 2016. 10. 01 ~ 2016. 12. 30 (약 3개월)
- **개발 언어 및 환경 :** C/C++ / Windows 7 (64bit) / Keil_v5 / MCU(STM32F103CBT6)
- **개발 내용 :**

        1) **Button(모드선택) :** CDS센서,Temp센서 조건 모드를 선택.

        2) **CDS Sensor :** 폭설의 량을 실시간 감지 하는 센서

        3) **TEMP Sensor :** 온도를 실시간 감지 하는 센서

        4) **FND Sensor :** 현재 모드를 나타내는 센서

        5) **Buzzer Sensor :** 현재 모드를 소리로 나타내는 센서

- **구현 기술 :**

       1) **Button Sensor : Button, EXTI interrupt**

            □ 용도
                 ▶ 조건 모드 선택
            □ 기술내용
                 ▶ Falling Edge의 인터럽트를 활용

        2) **CDS Sensor : 조도 센서, DMA, ADC**

            □ 용도
                 ▶ DMA의 ADC를 이용하여 CDS의 아날로그값을 디지털 값으로 환산
            □ 기술내용
                 ▶ 환산된 CDS센서의 값을 LUX 값으로 환산하여 조도의 값을 계산
                 ▶ CDS 센서와 MCU간 인터페이스

        3) **TEMP Sensor : 온도 센서, DMA, ADC**

            □  용도
                ▶ DMA의 ADC를 이용하여 TEMP(Thermistor)의 아날로그 값을 디지털 값으로 환산
            □  기술내용
                ▶ 환산된 TEMP(Thermistor)의 값을 테이블의 값으로 환산하여 온도의 값을 계산
                ▶ TEMP 센서와 MCU간 인터페이스

        4) **FND Sensor : FND 세그먼트, PWM**

            □  용도
                ▶ 조건 모드를 나타냄
            □  기술내용
                ▶ GPIO을 제어하여 FND 표현

        5) **Buzzer Sensor : 소리 센서, Timer IRQ**

            □ 용도
                ▶ 조건 모드를 소리로 나타냄
            □ 기술내용
                ▶ PWM을 이용한 Buzzer의 Frequency를 변경하여 dB(데시벨)를 제어
