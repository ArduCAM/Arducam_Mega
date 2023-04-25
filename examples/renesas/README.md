# EK-RA6M4

## Deploy development environment

Refer to the link to deploy your environment

[Quick start](https://www.renesas.com/us/en/document/qsg/ek-ra6m4-quick-start-guide?r=1333976)

---

### Hardware Requirements

- EK-RA6M4 board
- Micro USB device cable
- USB to serial port module

---

### Hardware Connection

<img src="static\hardware_connect.jpg" width="900xp" height="500xp">

---

### Software Requirements

- Windows ® 10 operating system
- USB Serial Drivers (included in Windows 10)
- [Arducam Mega GUI](https://github.com/ArduCAM/Arducam_Mega/releases)
- [e2 studio IDE](https://github.com/renesas/fsp/releases/download/v4.3.0/setup_fsp_v4_3_0_e2s_v2023-01.exe)
- SEGGER J-Link ® USB drivers
- FSP

> The FSP, J-Link USB drivers, and e 2 studio are bundled in a downloadable platform installer available on the
> FSP webpage at renesas.com/ra/fsp. New users are recommended to use the Quick Install option provided
> in the installation wizard, to minimize the amount of manual configuration needed

---

#### 1. **Pmod 1 Connector**

<table>
    <tr>
        <th colspan="2">Pmod 1 Connector</th><th>EK-RA6M4</th><th>Mega Camera</th>
    </tr>
    <tr>    <!--rowspan="3"-->
        <td>Pin</td><td>Description</td><td>Signal/Bus</td><td>Pin</td> 
    </tr>
    <tr>
        <td>J26-1</td><td>CS</td><td>P301 (SSLA2/CTS_RTS9)</td><td>CS</td>
    </tr>
    <tr>
        <td>J26-2</td><td>MOSI</td><td>P203 (MOSIA/TXD9)</td><td>MOSI</td>
    </tr>
    <tr>
        <td>J26-3</td><td>MOSO</td><td>P202 (MISOA/RXD9)</td><td>MOSI</td>
    </tr>
    <tr>
        <td>J26-4</td><td>SCK</td><td>P204 (RSPCKA/SCK9)</td><td>MOSI</td>
    </tr>
    <tr>
        <td>J26-5</td><td>GND</td><td>GND</td><td>GND</td>
    </tr>
    <tr>
        <td>J26-6</td><td>VCC</td><td>+3.3V</td><td>VCC</td>
    </tr>
    <tr>
        <td>J26-7</td><td>GPIO / INT (slave to master)</td><td>P008 (IRQ12-DS)</td><td>x</td>
    </tr>
    <tr>
        <td>J26-8</td><td>GPIO / RESET (master to slave) </td><td>P311</td><td>x</td>
    </tr>
    <tr>
        <td>J26-9</td><td>GPIO / CS2</td><td>P312</td><td>x</td>
    </tr>
    <tr>
        <td>J26-10</td><td>GPIO / CS3</td><td>P313</td><td>x</td>
    </tr>
    <tr>
        <td>J26-11</td><td>GND</td><td>GND</td><td>x</td>
    </tr>
    <tr>
        <td>J26-12</td><td>VCC</td><td>+3.3V/+5V</td><td>x</td>
    </tr>
</table>

---

#### 2. **Serial Connector**

| EK-RA6M4 | USB to Serial |
| :------: | :-----------: |
|   P410   |      TX      |
|   P411   |      RX      |
|   GND   |      GND      |

---

### Arducam mega full features preview

<img src="static\renesas_mega_demo.gif" width="900xp" height="500xp">
