cd default
avr-objcopy -Iihex EdrumBootloader.hex -Obinary EdrumBootloader.bin
avrdude -pm32 -e -V -U flash:w:EdrumBootloader.hex -c ftisp -P ft0 
REM ftisp -E -fw EdrumBootloader.bin
ftisp -Fr
cd ..