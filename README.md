# Telit_xE910

This Arduino based GSM Modem library is compatable with Telit xE910 modem family. 

## Sample Output

```C++
Power On Modem................................[OK]
--------------------------------------------------
AT............................................[OK]
ATE=0.........................................[OK]
AT+CMEE=1.....................................[OK]
AT+FCLASS=0...................................[OK]
AT&K0.........................................[OK]
AT+CPIN?......................................[OK]
AT+CGSN.......................................[OK]
AT+GSN........................................[OK]
AT+CCID.......................................[OK]
AT+GMI........................................[OK]
AT+GMM........................................[OK]
AT+GMR........................................[OK]
AT+SLED=2.....................................[OK]
--------------------------------------------------
Manufacturer      : Telit
Model             : GE910-QUAD
Firmware          : 13.00.007
IMEI Number       : 353613080341905
Serial Number     : 0001767746
ICCID             : 8990011916180288191
--------------------------------------------------
AT#TXMONMODE=1................................[OK]
AT+CREG=0.....................................[OK]
AT+CGREG=0....................................[OK]
GSM Connection Time...........................[12]
AT#SCFG=1,1,300,90,600,50.....................[OK]
AT+CGDCONT=1,"IP","mgbs","0.0.0.0",0,0........[OK]
AT#SERVIFO....................................[OK]
AT#SGACTCFG=1,5,180,0.........................[OK]
AT+CGACT=1,1..................................[OK]
AT+CGPADDR=1..................................[OK]
AT#HTTPCFG=1,"*****",80,0,"","",0,20,1........[OK]
AT#E2SLRI=50..................................[OK]
--------------------------------------------------
Operator          : Turkcell
IP Address        : 178.242.8.147
--------------------------------------------------
AT+CSQ........................................[OK]
--------------------------------------------------
GSM RSS Level     : 12
Signal Strength   : 2
--------------------------------------------------
AT#NITZ=1.....................................[OK]
AT+CTZU=1.....................................[OK]
AT#NTP="85.199.214.98",123,1,3................[OK]
--------------------------------------------------
Time              : 10/1/2022  14:28:41
--------------------------------------------------
Data Send Test    : OK
```


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

