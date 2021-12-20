Acronyms
========

=========== ===========================================
UART        Universal Asynchronous Receiver Transmitter
SLIP        Serial Line IP
RTU         Remote Terminal Unit
PLC         Programmable logic controller
=========== ===========================================

Description
===========
MODBUS is a serial line communication protocol used in many industrial use cases. Traditionally at has been run over RS232 and RS485 (called Modbus RTU), but is a also available over IP with Modbus/TCP.
There are several options for integrating RIIM with Modbus.
In this example the central unit is locked to being a modbus master supporting modbus RTU.
This is typically a PLC where the posibilities for the system integrator to choose a different interface like SLIP in limited.
On a single board computer running Linux, a slip interface is recommended as is it more versatile and allow other traffic to the nodes than modbus traffic.
There are other examples available showing the use of SLIP with single board computer.

The main challenges and tasks are therefore these:
   - Addressing: The modbus messages include modbus address. But in a border router there are no inherent understand of how to use this information. Due to this broadcast is initially used.
   - Device registartion: Based on incoming modbus 
   - Address lookup

This example provides a possible solution to these challenges and tasks

System Overview
---------------
This image shows the system. The dashed grey parts are optional, and not part of this example. They probably would, however, be part of a real deployment.

.. mermaid::

   graph LR
      idRPI(PLC/edge computer) -- MODBUS/RTU --- idBR(BorderRouter)
      idBR(BorderRouter) -- RF --- idMR1(MeshRouter)
      idBR(BorderRouter) -- RF --- idMR2(MeshRouter)
      idMR2(MeshRouter) -- MODBUS/RTU --- idModbus1(Modbus Device)
      idMR1(MeshRouter) -- MODBUS/RTU --- idModbus2(Modbus Device)
          
The PLC communicates with the border router through a modbus/RTU. An RS485 driver is required. RIIM mesh router nodes has the same connection to modbus slave. 
The modbus slave need separate power.

Topics not covered in this example
----------------------------------
These topics are not covered in this example, but could be relevant in a real world deployment

- Removing nodes from address mapping table
- Assign modbus address and registerers to the border router, to enable PLC to send commands to Border Router.
- Commisioning (See separate application note for this on www.radiocrafts.com.)

RIIM network nodes
------------------

There are two ICI applications provided:

- Border Router application
   - Setup RIIM TSCH network
   - Manage mapping table between modbus and IPv6
   - Send and receive transparent the modbus packets
   
- Mesh Router application
   - Join RIIM TSCH network
   - Send and receive transparent the modbus packets
