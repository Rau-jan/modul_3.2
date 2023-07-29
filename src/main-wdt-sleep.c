/**
 * Пример для Arduino Nano.
 *
 * При запуске программы быстро помигаем светодиодом и уйдем в спящий режим.
 * После чего через каждые 4 секунды Watchdog Timer будет генерировать прерывание которое будет пробуждать микроконтроллер.
 * После пробуждения будем мигать светодиодом с интервалом в 1 секунду.
 *
 * Watchdog Timer (WDT) - это таймер, который используется для защиты микроконтроллера от зависания.
 * Если программа зависнет, WDT сработает и перезагрузит микроконтроллер.
 *
 * WDT работает следующим образом:
 * - Микроконтроллер запускает WDT.
 * - WDT начинает отсчитывать время.
 * - Если программа не сбросит WDT до того, как он закончит отсчитывать время, WDT сработает и перезагрузит микроконтроллер.
 *
 * Сторожевой таймер Arduino UNO или ATmega 328P управляется не системными часами,
 * как другие таймеры, рассмотренные ранее, а отдельным генератором с частотой 128 кГц.
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "bitwise.h"

#define LED_PIN 5 // PB5(D13)

void blink_led(int count, int timeout_ms)
{
    for (int i = 0; i < count; i++)
    {
        set_bit(PORTB, LED_PIN); // Включить светодиод на PB5
        _delay_ms(timeout_ms);
        clear_bit(PORTB, LED_PIN); // Выключить светодиод на PB5
        if (i < count - 1)
        {
            _delay_ms(timeout_ms);
        }
    }
}

ISR(WDT_vect)
{ // Не будет прерывания от Watchdog Timer если убрать этот обработчик прерывания (будет Reset)
}

int main(void)
{
    set_bit(DDRB, LED_PIN); // Настройка PB5 на выход

    blink_led(3, 100);

    cli(); // Запретить глобальные прерывания

    // Для Arduino Nano, Arduino Pro Mini не получиться использовать функцию wdt_enable(WDTO_4S) из-за проблем с bootloader
    // https://github.com/arduino/ArduinoCore-avr/issues/150

    // WDTCSR - Watchdog Timer Control Register
    // Инициируем изменение параметров сторожевого таймера.
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // Биты нужно проставить одной командой в течение следующих 4 тактов (тут немного заморочено, делать нужно именно так).
    WDTCSR = (1 << WDIE) | (0 << WDE) | (1 << WDP3); // Прерывание от Watchdog Timer через каждые 4 секунды (без Reset)

    while (1)
    {
        sei();       // Разрешить глобальные прерывания
        wdt_reset(); // Сброс Watchdog Timer.  Это заставляет Watchdog Timer начать отсчет с нуля
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();

        // <-- Попадем в это место при выходе из спящего режима

        cli(); // Запретить глобальные прерывания (Чтобы не сработали прерывания пока будет мигать светодиод)

        blink_led(3, 1000);
    }
}