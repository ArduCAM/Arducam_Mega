

function UpdateLinkFile {
    param(
        [string]$folder
    )
    rm examples\$folder\full_featured\ArducamLink.*  
    cp examples\ArduinoUNO\full_featured\ArducamLink.* examples\$folder\full_featured
    # echo $folder 123
}

UpdateLinkFile ArduinoDUE
UpdateLinkFile ArduinoESP32
UpdateLinkFile ArduinoESP32-C3
UpdateLinkFile ArduinoESP32-S2
UpdateLinkFile ArduinoESP8266
UpdateLinkFile ArduinoMicrobit
UpdateLinkFile ArduinoNano33Ble
UpdateLinkFile ArduinoPico
UpdateLinkFile ArdunioMega2560
UpdateLinkFile Pico
