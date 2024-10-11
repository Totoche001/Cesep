// Code principal
#include "classLed.h"

ClassLed::ClassLed(byte pin) {
  _pin = pin;
  _effect = 0;
  _timer = 0;
  _intensity = 0;
  _fadeDuration = 1000; // Réduire la durée à 1000ms pour que le fading soit plus rapide
  _fadeTarget = 0;
  _blinkDuration = 1000;
  pinMode(_pin, OUTPUT);
}

void ClassLed::on() {
  _intensity = 255;
  digitalWrite(_pin, HIGH);
}

//ClassLed led(13);  // Créer une instance de ClassLed sur la broche 13
void ClassLed::on(byte intensity) {
 _intensity = intensity;
  if (_intensity == 0) {
    digitalWrite(_pin, LOW);  // Éteint complètement la LED si l'intensité est 0
  } else {
    analogWrite(_pin, _intensity);  // Applique le PWM si l'intensité est supérieure à 0
  }
  Serial.println(_intensity);
}

void ClassLed::off() {
  _intensity = 0;
  digitalWrite(_pin, LOW);// Éteint complètement la LED
}
void ClassLed::animation() {
  if (_effect == 0) return;
  else if (_effect == 1) blink();
  else if (_effect == 2) fadeTo(_fadeTarget);
}
void ClassLed::blink(bool state) {
  if (state) _effect = 1;
  else _effect = 0;
}
void ClassLed::blink() {
  //_effect = 1;
  if (_timer + _blinkDuration <= millis()) {
    _timer = millis();
    bool etat = digitalRead(_pin);
    digitalWrite(_pin, !etat);
  }
}
void ClassLed::setBlinkDuration(uint16_t duration) {
  _blinkDuration = duration;
}

void ClassLed::fadeTo(uint8_t target) {
  _fadeTarget = target;
  _effect = 2; // Active l'effet de fading

  // Si l'intensité est déjà égale à la cible
  if (_intensity == _fadeTarget) {
    _effect = 0;  // Arrête l'effet

    // Si l'intensité cible est 0, on éteint la LED complètement
    if (_intensity == 0) {
      off();  // Éteint complètement la LED si l'intensité est 0
    }
  } 
  else if (_intensity < _fadeTarget) {
    _fadeIn(_fadeTarget - _intensity);
  } 
  else if (_intensity > _fadeTarget) {
    _fadeOut(_intensity - _fadeTarget);
  }
}

void ClassLed::_fadeIn(uint8_t step) {
  _effect = 2;
  uint16_t duration = _fadeDuration / step;

  if (_intensity < _fadeTarget) {
    if (_timer + duration <= millis()) {
      _timer = millis();
      _intensity++;
      on(_intensity);
    }
  } 
}
void ClassLed::_fadeOut(uint8_t step) {
  _effect = 2;
  uint16_t duration = _fadeDuration / step;

 if (_intensity > _fadeTarget) {
    if (_timer + duration <= millis()) {
      _timer = millis();
      _intensity--;
      if (_intensity == 0) {
        off();  // Éteint complètement la LED lorsque l'intensité est 0
      } else {
        on(_intensity);  // Sinon, applique la nouvelle intensité
      }
    }
  }
}