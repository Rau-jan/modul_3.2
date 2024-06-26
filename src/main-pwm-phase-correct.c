/**
 * Пример для Arduino Nano.
 * 
 * ШИМ (Широтно-импульсная модуляция)
 * PWM (Pulse-width modulation)
 * 
 * Phase Correct PWM (ШИМ с коррекцией фазы):
 * В режиме Phase Correct PWM, счетчик сравнения (counter) увеличивается от нуля до значения, заданного в регистре верхнего сравнения (TOP), затем уменьшается до нуля и начинает увеличиваться снова.
 * Этот режим используется в приложениях, где важна точная симметрия импульсов вокруг нуля, так как позволяет добиться более гладкого изменения скорости или яркости при управлении моторами или светодиодами.
 * Недостатком режима "Phase Correct PWM" можно считать в два раза меньшую частоту выходного сигнала.
 * 
 * Период делителя = 1 / (Частота счетчика / Делитель)
 * T = 1 / (16 000 000 / 8) = 0.5 us; T(255) = 127,5 us;
 * T = 1 / (16 000 000 / 64) = 4 us; T(255) = 1 020 us = 1 ms;
 * T = 1 / (16 000 000 / 256) = 16 us; T(255) = 4 080 us = 4 ms;
 * T = 1 / (16 000 000 / 1024) = 64 us; T(255) = 16 320 us = 16 ms;
 * 
 * Плавно меняем яркость светодиода.
 */

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PD6 // PD6/OC0A (D6)

int main(void) {
  DDRD |= (1<<LED_PIN); // Настройка LED на выход

  // Настройка делителя (TCCR0B - Timer/Counter Control Register B)
  // Задаем prescaler = 256 (CS02=1, CS01=0, CS00=0)
  TCCR0B |= (1<<CS02);

  TCCR0A |= (1<<COM0A1); // Подает HIGH на порт OC0A при совпадении OCR0A и TCNT0 (COM0A1=1, COM0A0=0)
  // TCCR0A |= (1<<COM0A1) | (1<<COM0A0); // Подает LOW на порт OC0A при совпадении OCR0A и TCNT0 (COM0A1=1, COM0A0=1)

  // Задаем режим работы Phase Correct PWM (WGM02=0, WGM01=0, WGM00=1)
  TCCR0A |= (1<<WGM00);

  // TCNT0 // Хранит текущее значение счетчика (доступен для чтения и записи)

  // Чем меньше значение в OCR0A, тем выше скважность (светодиод горит тусклее).
  // Чем больше значение в OCR0A, тем ниже скважность (светодиод горит ярче).

  while (1) {
    if (OCR0A < 255) {
      OCR0A++;
      _delay_ms(10);
    } else {
      OCR0A = 0;
      _delay_ms(500);
    }
  }
}