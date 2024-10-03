# Motorized-Gas-Knob-With-WIFI-
Design of a prototype for a motorized Knob which can replace existing knobs of gas stoves. This knob rotates and control the flame through Wifi / BLE-based API Calls.

Components 
   1. Arduino Nano RP2040
   2. STEPPERONLINE Nema 17 Stepper Motor:
   3. Motor Driver DRV2288:
   4. 12V Power Supply:
   5. Capacitors, Resistors, Jumper Cables and Breadboard:
Circuit Diagram
  Here are few things to tune before the power supply is connected in the circuit. These will help protect the circuit from excessive current/heat:
      1. Attach heat dissipaters to DRV2288. 
      2. Limit the current on DRV2288 to 1-1.5mA. Less current would result in arbitary movement of the servo. The more current can result in overheating, thereby damaging of              driver. 
      3. Attach capacitor of 100$\mu F$ between power supply and components.
![Circuit Diagram](https://github.com/user-attachments/assets/02c6f18f-1aa7-4a9a-834d-23da7d5288ba)
![Real Life Circuit](https://github.com/user-attachments/assets/9b2f68c5-ff6e-49ce-9272-9fb4554c8641)
     
CAD Model
  ![Cad Front](https://github.com/user-attachments/assets/dfb72a85-01bc-4d7d-a534-e299ca0c3f54)
  ![Cad Side and Back](https://github.com/user-attachments/assets/0ae1c9bb-9706-42a1-974a-25f6ed8b3db5)
  1. J1: This joint serves as the mounting point for the Automated Knob Assembly within the existing gas range mechanism. It allows for rotational adjustment to ensure that the      servo shaft is aligned parallel to the gas knob, accommodating slanted knobs. The installation process is straightforward, secured by tightening a single nut and bolt.
  
  2. J2: This joint represents the connection between the servo shaft and the attached gear, facilitating the transfer of rotational motion from the servo to the rest of the         assembly.
      
  3. J4, J5: These joints are responsible for connecting to the gas knob. J5 includes a slot specifically designed to accommodate the gas knob's dimensions. Additionally, an         extra circular groove is provided to insert a fastener for cases where the gas knob's radius varies.
      
  4. C1, C2: C1 is the housing that secures the servo setup, while C2 serves as the enclosure for the PCB and electrical circuitry, ensuring both are properly protected and          organized.
