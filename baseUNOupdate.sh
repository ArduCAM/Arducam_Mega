
#!/bin/bash
rm examples/ArdunioMega2560/full_featured/ArducamLink.*
cp examples/ArduinoUNO/full_featured/ArducamLink.* examples/ArdunioMega2560/full_featured

rm examples/Pico/full_featured/ArducamLink.*  
cp examples/ArduinoUNO/full_featured/ArducamLink.* examples/Pico/full_featured

rm ./examples/ArduinoESP32/full_featured/ArducamLink.* 
cp ./examples/ArduinoUNO/full_featured/ArducamLink.* ./examples/ArduinoESP32/full_featured

rm ./examples/ArduinoESP8266/full_featured/ArducamLink.* 
cp ./examples/ArduinoUNO/full_featured/ArducamLink.* ./examples/ArduinoESP8266/full_featured