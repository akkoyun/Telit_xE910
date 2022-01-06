# Initialize Batch

## 1- At Test

```C++
--> AT // Command
<-- AT // Command Echo
<-- OK // Response
```

## 2- Close Command Echo

    Parameter 0 : Off command echo.

```C++
--> ATE=0 // Command
<-- ATE=0 // Command Echo
<-- OK // Response
```

## 3- Set Numeric Result Format

    Parameter 1 : Numeric error format.

```C++
--> AT+CMEE=1 // Command
<-- OK // Response
```

## 4- Set Connection Mode

    Parameter 0 : Data mode

```C++
--> AT+FCLASS=0 // Command
<-- OK // Response
```

## 5- Set Flow Control Mode

    Parameter 0 : No flow control mode

```C++
--> AT&K0 // Command
<-- OK // Response
```

## 6- Control for PIN 

    Response READY : No pin required

```C++
--> AT+CPIN? // Command
<-- +CPIN: READY // Response
<-- OK // Response
```

## 7- Get the IMEI Number

```C++
--> AT+CGSN // Command
<-- 353613080341905 // Response
<-- OK // Response
```

    Get IMEI number to (IMEI) variable.

## 8- Get the Modem Serial Number

```C++
--> AT+GSN // Command
<-- 0001767746 // Response
<-- OK // Response
```

    Get modem serial number to (Serial_Number) variable.

## 9- Get the SIM ICCID Number

```C++
--> AT#CCID // Command
<-- #CCID: 8990011916180288209 // Response
<-- OK // Response
```

    Get SIM ICCID number to (ICCID) variable.

## 10- Get Modem Manufacturer Name

```C++
--> AT+GMI // Command
<-- Telit // Response
<-- OK // Response
```

    Get modem manufacturer name and convert to (Manufacturer) numeric variable
    1- Telit

## 11- Get Modem Molde Name

```C++
--> AT+GMM // Command
<-- GE910-QUAD// Response
<-- OK // Response
```

    Get modem model name and convert to (Model) numeric variable.
    1- GE910 QUAD

## 12- Get Modem Firmware Release

```C++
--> AT+GMR // Command
<-- 13.00.007// Response
<-- OK // Response
```

    Get modem firmware version to (Modem_Firmware) variable.

## 13- Set Modem LED Connection Status

    Parameter 2 : LED controlled by connection status.

```C++
--> AT#SLED=2 // Command
<-- OK // Response
```
