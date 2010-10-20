cd default
make clean
make
avr-objcopy -Iihex AVRMIDI.hex -Obinary AVRMIDI.bin
REM avrdude -B 9600 -patmega8 -e -U flash:w:AVRMIDI.hex -c ftisp -P ft0
ftisp -E -fw AVRMIDI.bin
cd ..