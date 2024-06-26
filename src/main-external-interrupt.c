/**
 * Пример для Arduino Nano.
 * 
 * Внешние прерывания.
 * Внешние прерывания запускаются выводами INT0 и INT1 или любым из выводов PCINT23...0.
 * Раньше срабатывают прерывания с меньшим значением по вектору направления (например RESET = 0x000)
 * 
 * INT0/PD2(D2)
 * INT1/PD3(D3)
 * 
 * Программа. Будем включать и выключать светодиод при нажатии на кнопку.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define LED_PIN PB5 // D13
#define BUTTON_PIN PD2 // INT0/PD2(D2)

volatile bool is_interrupt_button = false;

ISR(INT0_vect) {
  cli(); // Выключить глобальные прерывания
  is_interrupt_button = true;
}

int main(void) {
  DDRB |= (1<<LED_PIN); // Настройка PB5 на выход

  // Настройка PD2 на вход не требуется (значение по умолчанию), регистр DDRD
  PORTD |= (1<<BUTTON_PIN); // Подтягиваем пин INT0/PD2(D2) к HIGH через встроенный резистор

  // Настройка режима срабатывания INT0:
  // 00 - генерировать прерывание при LOW значении
  // 01 - генерировать прерывание при изменении значения (с LOW на HIGH и наоборот)
  // 10 - генерировать прерывание при изменении значения с HIGH на LOW
  // 11 - генерировать прерывание при изменении значения с LOW на HIGH
  EICRA |= (1<<ISC01); // (ISC01=1, ISC00=0)

  EIMSK |= (1<<INT0); // Разрешить прерывания на INT0

  sei(); // Включить глобальные прерывания

  while (1) {
    if (is_interrupt_button) {
      PORTB ^= (1<<LED_PIN); // Меняем значение на противоположное
      _delay_ms(1000); // Для борьбы с дребезгом кнопки
      sei(); // Включить глобальные прерывания
      is_interrupt_button = false;
    }
  }
}