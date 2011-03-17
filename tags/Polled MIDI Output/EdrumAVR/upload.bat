cd default
avr-objcopy -Iihex EdrumAVR.hex -Obinary EdrumAVR.bin
REM avrdude -B 9600 -patmega8 -e -U flash:w:AVRMIDI.hex -c ftisp -P ft0
ftisp -E -fw EdrumAVR.bin
cd ..