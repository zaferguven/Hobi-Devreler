/*****************************************************
© Copyright 2008-2017 

Проект  : decoder
Версия  : 1.1.2
Дата    : 14.02.2013
Автор   : SFT

Комментарии: Приёмник RR10, прием и декодирование сигнала от "coder",
по радиоканалу. Управление нагрузкой.

 
Микроконтроллер : PIC 16F628A
Тактовая частота: 4,000000 MHz
Компилятор: HI-TECH PICC 9.83
Размер памяти   : 563 Байт.
*****************************************************/

#include <pic.h>
#include <htc.h>


// Конфигурация МК.
__CONFIG (FOSC_INTOSCIO & WDTE_OFF & PWRTE_ON & BOREN_OFF & LVP_OFF 
& MCLRE_ON & CP_ON & CPD_ON);

#define _XTAL_FREQ 4000000  // 4 МГц



/* 
 Определения. 
==================================================================== 
*/
#define RX_IN     C2OUT

#define RL1       RB0
#define RL2       RB1
#define RL3       RB2
#define RL4       RB3
#define RL5       RB4
#define RL6       RB5
#define RL7       RB6
#define RL8       RB7
#define RL9       RA0
#define RL10      RA3
#define RL11      RA6
#define RL12      RA7


#define SetBit(reg, bit) reg |= (1<<bit)
#define ClearBit(reg, bit) reg &= (~(1<<bit))
#define InvBit(reg, bit) reg ^= (1<<bit)
#define BitIsSet(reg, bit) ((reg & (1<<bit)) != 0)
#define BitIsClear(reg, bit) ((reg & (1<<bit)) == 0)




//==================================================================
//             Объявление глобальных переменных.
//==================================================================
unsigned char regim;   
unsigned char rx_state;
unsigned char crc; 
unsigned char tmp_crc;
unsigned int tmp_imp;
unsigned int bit_cnt;
unsigned int tmp_rx;
unsigned char rx_in[5];


union
{
    unsigned char arr[4];
    struct
    {
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
        unsigned b8:1;
        unsigned b9:1;
        unsigned b10:1;
        unsigned b11:1;
        unsigned b12:1;
        unsigned b13:1;
        unsigned b14:1;
        unsigned b15:1;
        unsigned b16:1;
        
        unsigned t1:1;
        unsigned t2:1;
        unsigned t3:1;
        unsigned t4:1;
        unsigned t5:1;
        unsigned t6:1;
        unsigned t7:1;
        unsigned t8:1;
        unsigned t9:1;
        unsigned t10:1;
        unsigned t11:1;
        unsigned t12:1;
        unsigned t13:1;
        unsigned t14:1;
        unsigned t15:1;
        unsigned t16:1;
    };
}rx;




// Прототипы функций. 
//==================================================================
unsigned char crc8(const unsigned char *pcBlock, unsigned char len);
unsigned int sinhro(void);
void obrabotka(void);
unsigned int filter(void);
void pause(unsigned int pa);




//==================================================================
//                     ИНИЦИАЛИЗАЦИЯ МК
//==================================================================
void init_mk(void)
{
    // Очистка защелок
    PORTA = 0;
    PORTB = 0;

    // port directions: 1-input, 0-output
    TRISA = 0b00000110;
    TRISB = 0b00000000;
    
    CMCON = 0b00100101;
    
    T1CON = 0b00110000;                                             /* Предделитель 1:8 */

    OPTION_REG = 0b10001001;                                        /* Подтягивающие резисторы выключены. */ 
    
    OSCF = 1;                                                       /* Частота внутреннего RC генератора равна 4 МГц. */

    // Разрешение прерываний от перефирийных модулей
    INTCON = 0b01000000;
}




/* 
====================================================================
                  ФУНКЦИЯ ОБРАБОТКИ ПРЕРЫВАНИЙ.
==================================================================== 
*/

void interrupt isr(void)
{ 
    if(TMR1IE&&TMR1IF)
    {
        if(regim)
        {
            PORTA = 0;
            PORTB = 0;
        }
        TMR1ON = 0;
        TMR1IF = 0;
    }
} // end void interrupt isr(void)




/* 
====================================================================
                        ГЛАВНАЯ ФУНКЦИЯ.
                    ОСНОВНОЙ ЦИКЛ ПРОГРАММЫ.
==================================================================== 
*/

void main(void)
{
    init_mk();
    
    TMR1IE = 1;
    
    ei();
    
    do
    {   
        while(!RX_IN)continue;
        obrabotka();
        
        // Если приняли пакет данных.
        if(rx_state)
        {
            crc = rx_in[4];
            rx.arr[3] = rx_in[3];
            rx.arr[2] = rx_in[2];
            rx.arr[1] = rx_in[1];
            rx.arr[0] = rx_in[0];

            tmp_crc = crc8(rx.arr, sizeof(rx.arr));
            
            rx_state = 0;

                                                    
            if(crc == tmp_crc)                                      /* Проверка принятого кода. */
            { 
                regim = rx.b14;
                if(!regim)
                {
                    if(rx.b1)
                    {
                        RL1 = rx.t1;
                    }
                    if(rx.b2)
                    {
                        RL2 = rx.t2;
                    }
                    if(rx.b3)
                    {
                        RL3 = rx.t3;
                    }
                    if(rx.b4)
                    {
                        RL4 = rx.t4;
                    }
                    if(rx.b5)
                    {
                        RL5 = rx.t5;
                    }
                    if(rx.b6)
                    {
                        RL6 = rx.t6;
                    }
                    if(rx.b7)
                    {
                        RL7 = rx.t7;
                    }
                    if(rx.b8)
                    {
                        RL8 = rx.t8;
                    }
                    if(rx.b9)
                    {
                        RL9 = rx.t9;
                    }
                    if(rx.b10)
                    {
                        RL10 = rx.t10;
                    }
                    if(rx.b11)
                    {
                        RL11 = rx.t11;
                    }
                    if(rx.b12)
                    {
                        RL12 = rx.t12;
                    }
                } // end if(!regim)
                else 
                {
                    if(rx.b1)
                    {
                        RL1 = 1;
                    }
                    if(rx.b2)
                    {
                        RL2 = 1;
                    }
                    if(rx.b3)
                    {
                        RL3 = 1;
                    }
                    if(rx.b4)
                    {
                        RL4 = 1;
                    }
                    if(rx.b5)
                    {
                        RL5 = 1;
                    }
                    if(rx.b6)
                    {
                        RL6 = 1;
                    }
                    if(rx.b7)
                    {
                        RL7 = 1;
                    }
                    if(rx.b8)
                    {
                        RL8 = 1;
                    }
                    if(rx.b9)
                    {
                        RL9 = 1;
                    }
                    if(rx.b10)
                    {
                        RL10 = 1;
                    }
                    if(rx.b11)
                    {
                        RL11 = 1;
                    }
                    if(rx.b12)
                    {
                        RL12 = 1;
                    }
                } // end else

            } // end if(rx_crc == tmp_crc)
            
        } // end if(bit_cnt >= 32)
        
    }while(1);
} // end void main(void)




/* 
====================================================================
            ФУНКЦИЯ ОБРАБОТКИ ПРИНЯТОГО СИГНАЛА.
==================================================================== 
*/
void obrabotka(void)
{
    unsigned char n;
    n = 0;
    TMR1 = 0;
    TMR1IF = 0;
    TMR1ON =1;
        
    while(sinhro() != 3)continue;

    //cnt_2 = 1;
    
    while(1)
    {
        TMR1 = 0;
        tmp_rx = filter();

        if(tmp_rx == 1)
        {
            ClearBit(rx_in[n],7 - bit_cnt);
            bit_cnt++;
        }
        else if(tmp_rx == 2)
        {
            SetBit(rx_in[n],7 - bit_cnt);
            bit_cnt++;
        }

        
        
        if(bit_cnt >= 8)
        {
            n++;
            bit_cnt = 0;
            if(n >= 5)
            {
                rx_state = 1;
                //cnt_2 = 0;
                break;
            }
        }
        
    } // end while(1)
    tmp_rx = 0;
    tmp_imp = 0;
    
}// end void obrabotka(void)




/* 
====================================================================
                 ФУНКЦИЯ ПРИЕМА ПОСЫЛОК.
==================================================================== 
*/
unsigned int sinhro(void)
{
unsigned char count;

    tmp_imp = 0;
    count = 40;
    while(count)
    {
        if(RX_IN)
        {
            if(RX_IN)
            {
                goto IMP1;
            }
        }
        
        if(count)
        {
            count--;
        }
        else
        {
            return 0;
        }
    }
    

IMP1:                                                               /* Ожидание окончания импульса */
    tmp_imp++;
    count = 250;
    while(count)
    {
        if(!RX_IN)
        {
            goto IMP0;
        }
        count--;
    }
    goto AN;
    

IMP0:                                                               /* Ожидание начала импульса */
    count = 250;
    while(count)
    {
        if(RX_IN)
        {
            goto IMP1;
        }
        count--;
    }


AN:                                                                 /* Сравнение полученного количества импульсов */
    if(tmp_imp > 6 && tmp_imp < 15)
    {
        return 1;                                                   /* Приняли 0 */
    }
    if(tmp_imp > 15 && tmp_imp < 24)
    {
        return 2;                                                   /* Приняли 1 */
    }
    if(tmp_imp > 24 && tmp_imp < 40)
    {
        return 3;                                                   /* Приняли условие старт */
    }
    if(tmp_imp > 40)
    {
        return 0;                                                   /* Ошибка */
    } 
    return 0;    
}// end unsigned int sinhro(void)




/* 
====================================================================
               ФУНКЦИЯ ФИЛЬТРОВАНИЯ ПРИНЯТЫХ 
              1 И 0 ОТ УСЛОВИЯ СТАРТ И ОШИБОК.
==================================================================== 
*/
unsigned int filter(void)
{
    unsigned int tmp;
    tmp = sinhro();
    if(tmp == 0 || tmp == 3)
    {
        return 0;
    }
    else if(tmp == 2)
    {
        return 2;
    }
    else if(tmp == 1)
    {
        return 1;
    }

    return 0;
}




/*
====================================================================
Функция расчета контрольной суммы.
Name  : CRC-8
Poly  : 0x31    x^8 + x^5 + x^4 + 1
Init  : 0xFF
Revert: false
XorOut: 0x00
Check : 0xF7 ("123456789")
MaxLen: 15 байт(127 бит) - обнаружение
одинарных, двойных, тройных и всех нечетных ошибок
====================================================================
*/
unsigned char crc8(const unsigned char *pcBlock, unsigned char len)
{
    unsigned char crc = 0xFF;
    unsigned char i;

    while(len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
} // end unsigned char crc8(const unsigned char *pcBlock, unsigned char len)




/*
====================================================================
Функция формирования задержки. Функция формирует задержку 1мС.
Входной параметр - количество милисекунд.
====================================================================
*/
void pause(unsigned int pa)
{
    unsigned int i;
    for(i = 0; i < pa; i++)
    {
        __delay_ms(1);
    }
}// end void pause(unsigned int pa)


    