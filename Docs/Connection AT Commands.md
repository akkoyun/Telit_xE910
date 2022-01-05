# Connecion Batch

## 1- TX Monitor LED Command

    Parameter 0 : Blinks when data transfer.

```C++
--> AT#TXMONMODE=0 // Command
<-- OK // Response
```

## 2- GSM Network Registration

    Parameter 1 : Register GSM network.

```C++
--> AT+CREG=1 // Command
<-- OK // Response
<-- +CREG: 2 // Response
<-- +CREG: 1 // Response
```

## 3- GPRS Network Registration

    Parameter 1 : Register GPRS network.

```C++
--> AT+CGREG=1 // Command
<-- OK // Response
<-- +CGREG: 2 // Response
<-- +CGREG: 1 // Response
```

## 4- Define APN 

    Parameter 1 : Context identifier
    Parameter "IP" : Protocol
    Parameter "mgbs" : APN

```C++
--> AT+CGDCONT=1,"IP","mgbs","",1,1 // Command
<-- OK // Response
```

## 5- Socket Configuration

    Parameter 1 : Connection identifier
    Parameter 1 : PDP context identifier
    Parameter 0 : Packet size
    Parameter 0 : Exchange timeout
    Parameter 150 : Connection timeout
    Parameter 0 : TX timeout

```C++
--> AT#SCFG=1,1,0,0,150,0 // Command
<-- OK // Response
```

## 6- Extended Socket Configuration

    Parameter 2 : Connection identifier
    Parameter 1 : SRing mode
    Parameter 0 : Recieve data mode
    Parameter 1 : Keep alieve

```C++
--> AT#SCFGEXT=2,1,0,1 // Command
<-- OK // Response
```

## 7- SGACT Configuration

    Parameter 1 : Connection identifier
    Parameter 5 : numeric parameter which specifies the maximum number of context activation attempts in case of activation failure.
    Parameter 180 : numeric parameter which specifies the delay in seconds between an attempt and the next one.
    Parameter 0 : disable unsolicited result code

```C++
--> AT#SGACTCFG=1,5,180,0 // Command
<-- OK // Response
```

## 8- Release IP

    Parameter 1 : Connection identifier
    Parameter 0 : Stat mode (disable)

```C++
--> AT#SGACT=1,0 // Command
<-- OK // Response
```

## 9- Get IP

    Parameter 1 : Connection identifier
    Parameter 1 : Stat mode (enable)

```C++
--> AT#SGACT=1,1 // Command
<-- OK // Response
```
