<!-- pandoc -V geometry:margin=1cm -V fontsize=12pt -V fontfamily=utopia README.md -o README.pdf -->

# RIIM_TRANSPARENT_MODBUS_Example


## Description
This is an example on how to send modbus messages through a RIIM network. To run this exmaple modbus devices and RS485 driver boards are needed.
But the example can also be used as starting point for your own modbus project. The scenario is as follows:

---

   You want to send Modbus packets from a PLC through RIIM to an modbus device at a differnet location. Since the Modbus message does not contain the RIIM(IPv6) address there has to be a mapping done at the border router. In order to fill up this address mapping table the system initially uses broadcast and then fill entries in mapping table when responses arrives. The next time the same modbus device is addressed, the address mapping table can be used.
   
---

## Requirements
|                       |                                      |
| --------------------- | ------------------------------------ |
| RIIM SDK              | 3.1.0 or later                       |
| RS485 driver          | RS485 breakout board from Sparkfun   |
| Modbus device         | Tibbo BP#01 or similar               |
| External power supply | For RS485 driver and modbus device   |


## Downloading
Download the source, either by cloning the repository like this:

`
git clone https://github.com/radiocrafts/RIIM_TRANSPARENT_MODBUS_Example
`

Copy the catalog Example_TRANSPARENT_MODBUS\ to you SDK under ICI_Applications\


## Full Documentation
Please see the documentation located here:
[https://radiocrafts.github.io/TRANSPARENT_MODBUS_Example]

## Contact
Contact Radiocrafts AS at [https://radiocrafts.com/contact/]