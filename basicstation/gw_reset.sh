#!/bin/bash

gpioset -l gpiochip0 ${GPIO_RESET_PIN}=0
sleep 0.1
gpioset -l gpiochip0 ${GPIO_RESET_PIN}=1