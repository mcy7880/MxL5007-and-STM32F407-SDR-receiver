# MxL5007-and-STM32F407-SDR-receiver
My previous project was SDR receiver based on R820T and STM32F407: https://github.com/mcy7880/R820T2-and-STM32F407-SDR-receiver I decided to take attempt to use MxL5007T instead of R820T. The repository contains source code for STM32F407 (complete project from STM32IDE) and hardware schematics. To be honest it's obviously more fun and educational project rather than reliable SDR receiver. The motivation for this project was built SDR based on STM32F4DISCOVERY (it has CS43L22 audio DAC) board and MxL5007T. So it's basically similar project like previous one but with MxL5007T. The MxL5007T driver comes from Chinese website: https://www.dssz.com/2033550.html It was ported and slightly improved and functionally expended.

The UART console code is still based on Eric's Brombaugh's code: https://github.com/emeb/r820t2/tree/master

Porting, modification, improvements and SDR code was done by @mcy7880 Maciej Fajfer.

# Doc
Documentation, hardware schematic, original MxL5007T driver (previously mentioned) provided by MaxLinear and DVB-T set-top-box schematic (MSD7818 and MxL5007T) from Chinese website: https://www.dssz.com/2404336.html.

# Matlab
Matlab's script and *.FDA files for Filter Designer (fdatool).

# stm32f407_mxl5007t
STM32F407 - the whole project from STM32IDE

Lots of detials (in Polish): https://www.elektroda.pl/rtvforum/topic4135063.html
