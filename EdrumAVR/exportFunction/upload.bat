cd default
avr-objcopy -Iihex exportFunction.hex -Obinary exportFunction.bin
REM avrdude -B 9600 -patmega8 -e -U flash:w:AVRMIDI.hex -c ftisp -P ft0
REM ftisp -E -fw EdrumAVR.bin
cd ..