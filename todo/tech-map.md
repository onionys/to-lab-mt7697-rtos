# MicroPython on LinkIt MT7697 MCU

### Standard Library

- System Module
  - `gc`
  - file system (FAT): flash (SPI)
  - `sys`
  - `uerrno`
  - `uio`
  - `uselect`
  - `usocket` (depending on MT767 WiFi )
  - `utime` (hardware clock about)
  - `uos`
  - file system : sd card (SPI)
  - `_thread` (depending on freeRTOS )

- Hardware
  - `pyb.Pin` (I/O)
  - `pyb.PinAF`
  - `pyb.UART`
  - `pyb.I2C` I2C Master / Slave 
  - `pyb.SPI` SPI Master / Slave
  - `pyb.ADC` : Pin Input
  - `pyb.DAC` (depending on MT7697, maybe not)

  - `pyb.ExtInt`
  - `pyb.Timer`
  - `pyb.RTC`

  - `pyb.CAN`
  - `pyb.LED`
  - `pyb.Servo`
  - `pyb.Switch`
  - `pyb.TimerChannel`

- Library (not system about)
  - `array`
  - `cmath` (Maybe about MPU)
  - `ubinascii`
  - `ucollections`
  - `uhashlib` (Maybe about MT7697 AES module)
  - `uheapq`
  - `ujson`
  - `ure`
  - `ustruct`
  - `zlib`

### 7697 feature 

	BLE 4.0 : used for local config system (wifi connection and platform config)


### Specific Library (Optional)

- Hardware & System
  - `micropython`
  - `framebuf`
  - `machine`
  - `network`
  - `pyb` (depending on board)

- Software
  - `btree`
  - `uctypes`

