'****************************************************************
'*  Name    : alýcý.BAS                                       *
'*  Author  : [Mustafa CAN ]                                    *
'*  Notice  : Copyright (c) 06,06,2006                          *
'*          : All Rights Reserved                               *
'*  Date    : 28.03.2007                                        *
'*  Version : 1.0                                               *
'*  Notes   : PICLE ALICI bas dosyasý                            *
'*          :                                                   *
'****************************************************************
DEFINE OSC 4
PORTA=0
TRISA=%00000001
TRISB=0 

BILGI var BYTE
SYMBOL VERIIN=PORTA.0
 PORTB=0
 porta=0
MAIN:  
   BILGI=0             
   serIN2 VERIIN,813,[WAIT ("ETE"),BILGI] 
   SELECT CASE BILGI
   CASE 255
   GOTO MAIN
   CASE 254
   TOGGLE PORTB.0
   CASE 253
   TOGGLE PORTB.1
   CASE 251
   TOGGLE PORTB.2
   CASE 247
   TOGGLE PORTB.3
   CASE 239
   TOGGLE PORTB.4
   CASE 223
   TOGGLE PORTB.5
   CASE 191
   TOGGLE PORTB.6
   CASE 127
   toggle portb.7
   case 28
   toggle porta.1
   case 26
   toggle porta.2
   case 22
   toggle porta.3
   case 14
   PORTA.4=1-PORTA.4
   
   END SELECT

   PAUSE 200
GOTO MAIN        
            
           
             
END
               
                         
