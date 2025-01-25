'****************************************************************
'*  Name    : verici.BAS                                       *
'*  Author  : [Mustafa CAN ]                                    *
'*  Notice  : Copyright (c) 06,06,2006                          *
'*          : All Rights Reserved                               *
'*  Date    : 28.03.2007                                        *
'*  Version : 1.0                                               *
'*  Notes   : PICLE VERICI bas dosyasý                            *
'*          :                                                   *
'****************************************************************
DEFINE OSC 4
TRISA=%00011110
TRISB=%11111111 
OPTION_REG.7=0 
BILGI VAR BYTE


SYMBOL VERIOUT=PORTA.0 

MAIN:      BILGI=255
           GOSUB PORTA_BAK
           IF PORTB<255 THEN 
             BILGI=PORTB
               GOSUB VER
               while portb<255:wend
             ENDIF

            pause 10
            GOTO MAIN            
       

VER:
      serout2 VERIOUT,813,[REP$AA\5,REP$00\5,REP$FF\5] 
      serout2 VERIOUT,813,["E","T","E",BILGI]
      serout2 VERIOUT,813,["E","T","E",BILGI]
      serout2 VERIOUT,813,["E","T","E",BILGI]
      serout2 VERIOUT,813,["E","T","E",BILGI]
      RETURN
      
PORTA_BAK:
       BILGI=PORTA & %00011110
       IF BILGI<30 THEN
         GOSUB VER
'         PAUSE 100
       ENDIF
       WHILE PORTA<30 : WEND
       RETURN  
END
               
                         
