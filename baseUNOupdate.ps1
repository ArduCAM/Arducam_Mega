# rm .\examples\ArduinoUNO\full_featured\ArducamLink.h  
# rm .\examples\ArduinoUNO\full_featured\ArducamLink.cpp
rm examples\ArdunioMega2560\full_featured\ArducamLink.*
cp examples\ArduinoUNO\full_featured\ArducamLink.* examples\ArdunioMega2560\full_featured

rm examples\Pico\full_featured\ArducamLink.*  
cp examples\ArduinoUNO\full_featured\ArducamLink.* examples\Pico\full_featured

# rm .\examples\ArduinoMega2560\full_featured\ArducamLink.h  
# rm .\examples\ArduinoMega2560\full_featured\ArducamLink.cpp
# cp .\examples\ArduinoUNO\full_featured\ArducamLink.cpp .\examples\Pico\full_featured
# cp .\examples\ArduinoUNO\full_featured\ArducamLink.h .\examples\Pico\full_featured 