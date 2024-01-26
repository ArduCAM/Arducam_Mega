# take piture wite exif
## How to use
Send command to obtain an image via serial port with appended date and time.such as :

```

0x20 0x24 = 2024 year
0x01 = January
0x25 = 25th

The time data ’2024/01/25 01:23:00‘, with each component stored in 4 bytes and represented in hexadecimal format, is: 0x20 0x24 0x01 0x25 0x00 0x01 0x23 0x00.

0x55 0x10 0x20 0x24 0x01 0x25 0x00 0x01 0x23 0x00 0xaa
  |    |    |    |    |    |    |    |    |    |    |
  |    |    |    |    |    |    |    |    |    |    --- Command tail
  |    |    |    |    |    |    |    |    |    --- Seconds
  |    |    |    |    |    |    |    |    --- Minutes 
  |    |    |    |    |    |    |    --- Hours
  |    |    |    |    |    |    --- Reserved
  |    |    |    |    |    --- Day
  |    |    |    |    --- Month
  |    |    |    --- Last two digits of year
  |    |    --- First two digits of year
  |    --- Capture command
  ---Command header

```

Then you receive a JPEG image stream with EXIF data sent over the serial port.The data format to be sent is as follows:

| 2 bytes | 1 byte | 4 bytes | 1 byte | Length (obtained from the header) | 2 bytes |
|---------|--------|---------|--------|---------------------------------|---------|
| 0x55 0xAA | 0x01 | Data Length | Image Format | Payload | 0x55 0xBB |

The payload contains a complete frame of JPEG image data.