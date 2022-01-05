

| Command | Estimated maximum time to get response (Seconds) |
|---------|:-------------------------------------------------|
| +COPS   | 30 (test command)                                |
| +CLCK   | 25 (SS operation) 5 (FDN enabling/disabling)     |
| +CLAC   | 5                                                |
| +CPWD   | 15 (SS operation) 5 (PIN modification)           |
| +CLIP   | 15 (read command)                                |
| +CLIR   | 15 (read command)                                |
| +CCFC   | 15                                               |
| +CCWA   | 15                                               |
| +CHLD   | 30                                               |
| **+CPIN**   | 5                                                |
| +CPBS   | 5 (FDN enabling/disabling)                       |
| +CPBR   | 5 (single reading)<br> 15 (complete reading of a 250 records full phonebook)|
| +CPBF   | 10 (string present in a 250 records full phonebook)<br> 5 (string not present)|
| +CPBW   | 5                                                |
| +CACM   | 5                                                |
| +CAMM   | 5                                                |
| +CPUC   | 5                                                |
| +VTS    | 20 (transmission of full “1234567890*#ABCD” string with no delay between tones, default duration) | 
| +CSCA   | 5 (read and set commands)                        |
| +CSAS   | 5                                                |
| +CRES   | 5                                                |
| +CMGS   | 60 after CTRL-Z for SMS not concatenated; 1 to get ‘>’ prompt |
| +CMSS   | 60 after CTRL-Z; 1 to get ‘>’ prompt |
| +CMGW   | 5 after CTRL-Z for SMS not concatenated; 1 to get ‘>’ prompt|
| +CMGD   | 5 (single SMS cancellation) 25 (cancellation of 50 SMS)|
| +CMGR   | 5 |
| +CMGL   | 20 (full listing of 50 SMS) |
| +CGACT  | 150 |
| +CGATT  | 10|
| D       | 30 (voice call) Timeout set with ATS7 (data call)|
| A       | 30 (voice call) Timeout set with ATS7 (data call)|
| H       | 30 |
| +CHUP   | 5 |
| +COPN   | 10 |
| +CPOL   | 10 (set command; read command of 84 records) |
| +CRSM   | 5 |
| +FRH    | Timeout set with ATS7 |
| +FTH    | Timeout set with ATS7 |
| +FRM    | Timeout set with ATS7 |
| +FTM    | Timeout set with ATS7 |
| +FRS    | Timeout set with the command itself |
| +FTS    | Timeout set with the command itself |
| #MBN    | 10 |
| #TONE   | 5 (if no duration specified) |
| #ADC    | 5 |
| #EMAILD | 20 |
| #EMAILACT | 150 |
| #SEMAIL   | 170 (context activation + DNS resolution) |
| #MSCLASS  | 15  |
| #SPN      | 5   |
| #STSR     | 10  |
| **#CCID**     | 5   |
| #GPRS     | 150 |
| #SKTD     | 140 (DNS resolution + timeout set with AT#SKTCT) |
| #SKTOP    | 290 (context activation + DNS resolution + timeout set with AT#SKTCT) |
| #QDNS     | 20  |
| #FTPOPEN  | 100 |
| #FTPCLOSE | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPTYPE  | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPDELE  | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPPWD   | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPCWD   | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPLIST  | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) + time to get listing |
| #FTPFSIZE | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPPUT   |500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPAPP   |500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPGET   | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| #FTPGETPKT | 500 (timeout set with AT#FTPTO, <br>in case no response is received from server) |
| **#SGACT**    | 150 |
| #SH       | 3 |
| #SD       | 140 (DNS resolution + connection timeout set with AT#SCFG) |
| #CSURV    | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVC   | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVU   | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVUC  | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVB   | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVBC  | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVP   | 10 to start data output; <br>120 seconds to complete scan |
| #CSURVPC  | 10 to start data output;<br>120 seconds to complete scan |
| #LSCRIPT  | 10 (40 files, 10 Kbyte each) |
| #REBOOT   | 5 |
| #RSCRIPT  | 30 seconds for a 100 Kbyte file 30 seconds timeout and ERROR message if no bytes are received on the serial line |
| #WSCRIPT  | 35 seconds for a 100 Kbyte file 30 seconds timeout and ERROR message if no bytes are sent on the serial line and the file has not been completely sent |
| #DSCRIPT   | 120 |