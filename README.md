# Telit_xE910

This Arduino based GSM Modem library is compatable with Telit xE910 modem family. 

## Library Functions

### GSM Class

Version[9] 					= "01.00.32";		// Library Version
Debug_Mode					= false;			// Debug Mode Variable

***

#### Power ON

Power ON procedure of GSM modem.

```C++
Power_ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch);
```

***

#### Power OFF

Power OFF procedure of GSM modem.

```C++
Power_OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch, const bool _Clear_Variables);
```

***

#### Begin and Initialize Procedure

GSM Modem initialize function

```C++
Begin(const bool _Debug);
```

***

#### Connection Procedure

GSM Modem connection function

```C++
Connect(void);
```

***

#### Socket Listen Procedure

Socket listen function

```C++
Socket_Listen(void);
```

***

#### RSSI Get Function

RSSI function

```C++
RSSI_Refresh(void);
```

***

#### GSM Modem Manufacturer

Returns GSM modem manufacturer name (string)

```C++
Manufacturer(void);
```

***

#### GSM Modem Model

Returns GSM modem Model name (string)

```C++
Model(void);
```

***

#### Operator Name

Returns GSM operator name (string)

```C++
Operator(void);
```

***

#### GSM Modem Firmware

Returns GSM modem firmware name (string)

```C++
Firmware(void);
```

***

#### GSM Modem IMEI Number

Returns GSM modem IMEI (string)

```C++
IMEI(void);
```

***

#### GSM Modem Serial Number

Returns GSM modem serial id (string)

```C++
Serial_ID(void);
```

***

#### SIM ICCID Number

Returns GSM modem serial id (string)

```C++
ICCID(void);
```

