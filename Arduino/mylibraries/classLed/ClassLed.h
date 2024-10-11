// Contenu du fichier "classLed.h"
#ifndef CLASSLED_H
#define CLASSLED_H

#include <Arduino.h>

class ClassLed {
private:
  uint8_t _pin;
  uint8_t _effect;
  uint8_t _intensity;
  uint16_t _blinkDuration;
  uint16_t _fadeDuration;
  uint16_t _fadeStep;
  uint16_t _fadeTo;
  uint8_t _fadeTarget;
  uint32_t _timer;
  void _fadeIn(uint8_t step);
  void _fadeOut(uint8_t step);
  //uint8_t target;
  //uint16_t step;
  //int pin;
  //bool etat;

public:
  ClassLed(byte _pin);
  void on();
  void on(byte intensity);
  void off();
  void blink();
  void blink(bool state);
  void animation();
  void setBlinkDuration(uint16_t duration);
  void fadeTo(uint8_t target);
  //void fadeIn(uint8_t step);
  //void basculer();
  //bool getEtat();
};

#endif