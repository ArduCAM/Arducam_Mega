# EK-RA6M4

## Arducam Mega Get Start

### Hardware Requirements

- EK-RA6M4 board
- Micro USB device cable
- USB to serial port module

### Software Requirements

- Windows ® 10 operating system
- USB Serial Drivers (included in Windows 10)
- [Arducam Mega GUI]()
- [e2 studio IDE]()
- SEGGER J-Link ® USB drivers
- FSP
- Quick Start Arducam Mega example project
> The FSP, J-Link USB drivers, and e 2 studio are bundled in a downloadable platform installer available on the
FSP webpage at renesas.com/ra/fsp. New users are recommended to use the Quick Install option provided
in the installation wizard, to minimize the amount of manual configuration needed

### Hardware Connection

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

#### 2. **Serial Connector**

EK-RA6M4 | USB to Serial
:-------:|:-------------:
P410     | TX
P411     | RX
GND      | GND

### Flash and Start example

1. Launch e2 studio.
1. Browse to the Workspace where the project file is to be imported. Enter the name in the Workspace dialog box to create a new workspace.  
![Creating a New Workspace](static\Snipaste_2023-04-23_15-24-20.png)  
Figure 1. Creating a New Workspace  
1. Click **Launch**.  
![Launching the Workspace](static\Snipaste_2023-04-23_15-24-21.png)  
Figure 2. Launching the Workspace
1. Click **Import** from the **File** drop-down menu.  
![Importing the Project](static\Snipaste_2023-04-23_15-24-41.png)  
Figure 3. Importing the Project  
1. In the **Import** dialog box, select **General**, and then select **Existing Projects into Workspace**.  
![Importing Existing Projects into the Workspace](static\Snipaste_2023-04-23_15-24-54.png)  
Figure 4. Importing Existing Projects into the Workspace
1. Click **Next**.  
![Clicking Next to Import Existing Projects into the Workspace](static\Snipaste_2023-04-23_15-25-03.png)  
Figure 5. Clicking Next to Import Existing Projects into the Workspace
1. Click Select root directory and click Browse to go to the location of the Quick Start example project
folder.  
![](static\Snipaste_2023-04-23_15-25-14.png)
Figure 6. Selecting the Root Directory
1. Select the Quick Start example project and click Finish.  
![Finishing Importing the Quick Start Example Project](static\Snipaste_2023-04-23_15-26-27.png)  
Figure 7. Finishing Importing the Quick Start Example Project
1. Build the project by clicking the build icon  
![Building the Project](static\Snipaste_2023-04-23_15-27-05.png)  
Figure 8. Building the Project
1. Downloading and Running the Mega Camera Example Project  
![](static\Snipaste_2023-04-23_15-27-17.png)  