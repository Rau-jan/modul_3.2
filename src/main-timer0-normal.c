/**
 * Пример для Arduino Nano.
 * 
 * Режимы работы таймера:
 * - Режим Normal ("Нормальный"). Это самый простой режим. 
 * В этом режиме таймер производит подсчет приходящих на его вход импульсов (от тактового генератора или внешнего устройства) и вызывает прерывание по затруднению.
 * 
 * - Режим Capture ("Захват"). 
 * Суть этого режима заключается в сохранении точности счетного счетчика таймера в текущий момент времени. 
 * Запоминание происходит либо по сигналу, поступающему через специальный вход микроконтроллера, либо от сигнала с выхода встроенного компаратора.
 * 
 * - Режим Clear Timer on Compare Match (CTC) ("Сброс при совпадении"). Для работы в режиме СТС используется специальный регистр - регистр совпадения. 
 * Когда количество импульсов совпадает с регистром совпадения генерируется прерывание.
 * 
 * - Режим Fast PWM ("Быстродействующий ШИМ"). 
 * Для формирования сигнала ШИМ используются те же самые регистры совпадения, которые работают и в режиме СТС. 
 * Сигнал с ШИМ формируется на специальном выходе микроконтроллера.
 * В режиме ШИМ переполнение таймера не вызывает прерываний.
 * Недостаток этого режима - при изменении длительности импульсов меняется и их фаза. Центр каждого импульса как бы сдвигается во времени.
 * Достигнув своего максимального значения счетчик сбрасывается в ноль.
 * 
 * - Режим Phase Correct PWM ("ШИМ с точной фазой").
 * Достигнув своего максимального значения счетчик не сбрасывается в ноль, а переключается в режим реверсивного счета.
 * Недостаток этого режима - в раза меньшая частота выходного сигнала.
 * 
 * - Режим Асинхронный.
 * В этом режиме на вход таймера подается либо частота от внутреннего кварцевого генератора, либо от внешнего генератора.
 * Счетчик не вырабатывает никаких прерываний и дополнительных сигналов. В этом режиме он работает в качестве часов реального времени.
 * 
 * 
 * 8-битный Таймер-счетчик с PWM (Timer/Counter0)
 * Используется для прямого или обратного счета времени.
 * Представляет собой ячейку памяти хранящую 256 значений (0 - 255).
 * Через определенный промежуток времени таймер увеличивает значение в ячейке памяти.
 * При переполнении таймер автоматически обнуляется.
 * Частота работы микроконтроллера f = 16 MHz = 16 000 000 Hz
 *
 * T = 1/f; (время работы таймера без делителя)
 * T = 1 / 16 000 000 = 0.0000000625 s = 0.0625 us
 * T(255) = 0.0625 us * 255 = 15.93 us (время работы таймера до переполнения)
 *
 * Период делителя = 1 / (Частота счетчика / Делитель)
 * T = 1 / (16 000 000 / 8) = 0.5 us; T(255) = 127,5 us;
 * T = 1 / (16 000 000 / 64) = 4 us; T(255) = 1 020 us = 1 ms;
 * T = 1 / (16 000 000 / 256) = 16 us; T(255) = 4 080 us = 4 ms;
 * T = 1 / (16 000 000 / 1024) = 64 us; T(255) = 16 320 us = 16 ms;
 * 
 * Мигаем светодиодом через каждую секунду. Используем таймер в обычном (Normal) режиме.
 * Используем переполнение таймера чтобы увеличить счетчик времени.
 * Не следует делать сравнение с TCNT0 в основном цикле программы т.к. практически невозможно точно определить момент времени,
 * когда ваше значение совпадет с TCNT0. Нужно использовать прерывания по переполнению или режим CTC.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define LED_PIN PB5 // PB5(D13)

volatile uint16_t counter = 0; // одно значение это 1 ms

ISR(TIMER0_OVF_vect) {
  counter++;
}

int main(void) {
  DDRB |= (1<<LED_PIN); // Настройка PB5 на выход

  // Настройка делителя (TCCR0B - Timer/Counter Control Register B)
  // Задаем prescaler = 64 (CS02=0, CS01=1, CS00=1)
  TCCR0B |= (1<<CS01) | (1<<CS00);

  // Значение таймера (TCNT0 - Timer/Counter Register)
  TCNT0 = 0; // Обнуляем значение таймера (0-255)

  TIMSK0 |= (1<<TOIE0); // Включить прерывание по переполнению для таймера

  sei(); // Разрешить прерывания

  while (1) {
    // Мигаем светодиодом каждую секунду
    if (counter >= 1000) {
      counter = 0;
      PORTB ^= (1<<LED_PIN); // Инвертировать значение на пине
    }
  }
}