cd default
avr-objcopy -Iihex EdrumBootloader.hex -Obinary EdrumBootloader.bin
REM avrdude -pm32 -e -V -U flash:w:EdrumBootloader.hex -c ftisp -P ft0 
ftisp -E -fw EdrumBootloader.bin
REM ftisp -Fr
cd ..