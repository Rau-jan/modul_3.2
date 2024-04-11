# AVR

## Сборник примеров программ для AVR:

- [Blink](./src/main-blink.c)
- [Blink Timer](./src/main-blink-timer.c)
- [Timer0 normal mode](./src/main-timer0-normal.c)
- [Timer0 CTC mode](./src/main-timer0-ctc.c)
- [Timer1](./src/main-timer1.c)
- [Sleep modes](./src/main-sleep.c)
- [Watchdog Timer + Sleep](./src/main-wdt-sleep.c)
- [Watchdog Timer](./src/main-wdt.c)
- [EEPROM](./src/main-eeprom.c)
- [External Interrupt](./src/main-external-interrupt.c)
- [External Pin Change Interrupt](./src/main-external-interrupt-pin-change.c)
- [Analog to Digital Converter](./src/main-adc.c)
- [Fast PWM](./src/main-pwm-fast.c)
- [Phase correct PWM](./src/main-pwm-phase-correct.c)
- [Traffic light](./src/main-traffic-light.c)

## Базовая информация (ATmega328P)

### Побитовые операции для управления регистрами

#### Установить бит в 1

```c
int PORT = 0b00000000;
PORT |= (1 << 0); // Установить 0-й бит в значение равно 1
PORT |= (1 << 2); // Установить 2-й бит в значение равно 1
PORT |= (1 << 0) | (1 << 7) // Установить первый и последний бит в значение равно 1
```

#### Установить бит в 0

```c
int PORT = 0b11111111;
PORT &= ~(1 << 0); // Установить 0-й бит в значение равно 0
PORT &= ~((1 << 0) | (1 << 2)); // Установить 0-й и 2-й бит в значение равно 0
```

#### Инвертировать значение бита

```c
int PORT = 0b11111111;
PORT ^= 1 << 0; // Инвертировать 0-й бит
PORT ^= ((1 << 0) | (1 << 2)); // Инвертировать 0-й и 2-й биты
```

#### Проверить бит

```c
int PORT = 0b00000100;

if (0 == (PORT & (1 << 3))) { // Проверить что 3-й бит равен 0
    printf("bit is 0\n");
}

if (~PORT & (1 << 3)) { // Проверить что 3-й бит равен 0
    printf("bit is 0\n");
}

if (PORT & (1 << 2)) { // Проверить что 2-й бит равен 1
    printf("bit is 1\n");
}
```

### Порты ввода-вывода

Порты могут быть настроены на вход или на выход. 
При настройке на вход - порт используется как вольтметр, измеряя поступающее на него напряжение.
При настройке на выход - порт подает напряжение равное LOW(0V) или HIGH(5V), в зависимости от заданного значения регистра `PORTx`.

Регистры управляющие портами:
- `DDRx` задает направление (порт на вход или порт на выход);
- `PORTx` для порта на вход используется для установки внутреннего подтягивающего резистора, для порта на выход задает выходное напряжение;
- `PINx` в основном используется для считывания поступающего значения для порта на вход;

Перед использованием порт необходимо настроить с помощью регистра `DDRx`.

Регистры направления данных DDR (direct data register).

- **порт B** - DDRB (полный - 8 бит, PB0..PB7)
- **порт C** - DDRC (неполный - 7 бит, PC0..PC6)
- **порт D** - DDRD (полный - 8 бит, PD0..PD7)

Для настройки порта записать в регистр:

```c
DDRB |= (1 << PB5); // настройка пина PB5 на выход
```

Управлять подачей напряжения на порт настроенный на выход можно с помощью регистра PORTx:

```c
PORTB |= (1 << PB5); // установить напряжение 5V на порту PB5
```

Сводная таблица:

| Настройка порта | DDRx | PORTx = 0          | PORTx = 1                                | PINx                       |
|-----------------|------|--------------------|------------------------------------------|----------------------------|
| На выход        | 1    | LOW на выходе      | HIGH на выходе                           | значение заданное в PORTx |
| На вход         | 0    | плавающее значение | подтянут к HIGH через внутренний резистр | HIGH или LOW               |

## Приложение

### Время

| Название         | second | millisecond | microsecond | nanosecond    |
|------------------|--------|-------------|-------------|---------------|
| second (s)       | 1      | 1 000       | 1 000 000   | 1 000 000 000 |
| millisecond (ms) | 0.001  | 1           | 1 000       | 1 000 000     |
| microsecond (us) | 1.0E-6 | 0.001       | 1           | 1 000         |
| nanosecond (ns)  | 1.0E-9 | 1.0E-6      | 0.001       | 1             |

### AVR Library

- `avr/sfr_defs.h`: содержит макросы для работы с битовыми операциями (`_BV()`, `bit_is_set()`, `bit_is_clear()` и т.п.);
- `avr/io.h`: содержит константы для работы с пинами микроконтроллера;
- `avr/interrupt.h`: этот модуль предоставляет функции для работы с прерываниями;
- `avr/pgmspace.h`: этот модуль предоставляет функции для работы с программируемой памятью;
- `avr/eeprom.h`: этот модуль предоставляет функции для работы с EEPROM;
- `avr/wdt.h`: этот модуль предоставляет функции для работы с таймером watchdog;
- `avr/sleep.h`: этот модуль предоставляет функции для работы с режимами сна;
- `avr/power.h`: этот модуль предоставляет функции для управления питанием микроконтроллера;
- `util/delay.h`: содержит функции для работы с задержками (`_delay_ms()`, `_delay_us()`);

[Стандартная C библиотека для AVR-GCC](https://www.nongnu.org/avr-libc/user-manual/modules.html)

## Дополнительно

- [Установить драйвер CH340](http://www.wch-ic.com/downloads/CH341SER_MAC_ZIP.html)
- [Установить PlatformIO для VSCode](https://platformio.org)
- [Документация PlatformIO - Arduino](https://docs.platformio.org/en/stable/frameworks/arduino.html)
- [Arduino Nano](https://docs.arduino.cc/hardware/nano)
