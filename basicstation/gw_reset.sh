#!/bin/bash
gpioset gpiochip0 ${GPIO_ENABLE_PIN}=1

gpioset gpiochip0 ${GPIO_RESET_PIN}=1
sleep 0.1
gpioset gpiochip0 ${GPIO_RESET_PIN}=0
sleep 0.1

gpioset gpiochip0 ${GPIO_TEMP_RESET_PIN}=1
sleep 0.1
gpioset gpiochip0 ${GPIO_TEMP_RESET_PIN}=0
sleep 0.1