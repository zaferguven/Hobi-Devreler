/*****************************************************
© Copyright 2008-2017 

Проект  : coder
Версия  : 1.1.2
Дата    : 14.02.2013
Автор   : SFT

Комментарии: Формирование кодовой посылки 
и управление передатчиком RT4.


Микроконтроллер : PIC 16F628A
Тактовая частота: 4,000000 MHz
Компилятор: HI-TECH PICC 9.83
Размер памяти   : 439 Байт.
*****************************************************/

#include <pic.h>
#include <htc.h>


// Конфигурация МК.
__CONFIG (FOSC_INTOSCIO & WDTE_OFF & PWRTE_ON & BOREN_OFF & LVP_OFF 
& MCLRE_OFF & CP_ON & CPD_ON);



#define _XTAL_FREQ 4000000  // 4 МГц



// Определения.
//==================================================================  

#define TX_OUT     RA0
#define TX_VDD     RA1
#define TX_LED     RA2

#define COLUMNS_1  RB0
#define COLUMNS_2  RB1
#define COLUMNS_3  RB2

#define LINE_A     RB3
#define LINE_B     RB4
#define LINE_C     RB5
#define LINE_D     RB6

#define REG_TX     RA5

#define SetBit(reg, bit) reg |= (1<<bit)
#define ClearBit(reg, bit) reg &= (~(1<<bit))
#define InvBit(reg, bit) reg ^= (1<<bit)
#define BitIsSet(reg, bit) ((reg & (1<<bit)) != 0)
#define BitIsClear(reg, bit) ((reg & (1<<bit)) == 0)



//==================================================================
//             Объявление глобальных переменных.
//==================================================================  
volatile unsigned char cnt_kn; 
volatile unsigned char cnt_reg;   
volatile unsigned char kn_st; 
unsigned char crc; 


union
{
    unsigned char data[4];
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
}button;

union
{
    unsigned int total;
    struct
    {
        unsigned f1:1;
        unsigned f2:1;
        unsigned f3:1;
        unsigned f4:1;
        unsigned f5:1;
        unsigned f6:1;
        unsigned f7:1;
        unsigned f8:1;
        unsigned f9:1;
        unsigned f10:1;
        unsigned f11:1;
        unsigned f12:1;
        unsigned f13:1;
        unsigned f14:1;
        unsigned f15:1;
        unsigned f16:1;
    };
}st;




// Прототипы функций.
//==================================================================
unsigned char crc8(const unsigned char *pcBlock, unsigned char len);
void tx_data(unsigned char dt);
void tx_zero(void);
void tx_one(void);
void start(void);
void pause(unsigned int pa);


//==================================================================
//                     ИНИЦИАЛИЗАЦИЯ МК.
//==================================================================  
void init_mk(void)
{
    // Очистка защелок
    PORTA = 0;
    PORTB = 0;

    // port directions: 1-input, 0-output
    TRISA = 0b00100000;
    TRISB = 0b00000111;
    
    // Откл. компараторов
    CMCON = 0b00000111;
    
    //T1CON = 0b00000001;

    OPTION_REG = 0b00001001;                                        
    
    OSCF = 1;                                                       //Частота внутреннего RC генератора равна 4 МГц.

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
    volatile unsigned char tmp_isr;

    if(TMR0IE&&TMR0IF)                                              /* Прерывания от Таймер0 - опрос матрици клавиатуры. */
    {
        TMR0IF = 0;
        tmp_isr = (PORTB&0x07);
        if(tmp_isr != 0x07)
        {
            cnt_kn++;
            if(cnt_kn > 70)
            {
                PORTB = 0xFF;
                LINE_A = 0;
                if(!COLUMNS_1)
                {
                    if(!st.f1)
                    {
                        button.b1 = 1;
                        button.t1 ^= 1;                             /* Бит toggle - используется для фиксации повторного нажатия кнопки приеником. */
                        st.f1 = 1;
                    }
                }
                else
                {
                    button.b1 = 0;
                    st.f1 = 0;
                }
                
                
                if(!COLUMNS_2)
                {
                    if(!st.f2)
                    {
                        button.b2 = 1;
                        button.t2 ^= 1;
                        st.f2 = 1;
                    }
                }
                else
                {
                    button.b2 = 0;
                    st.f2 = 0;
                }
                
                
                if(!COLUMNS_3)
                {
                    if(!st.f3)
                    {
                        button.b3 = 1;
                        button.t3 ^= 1;
                        st.f3 = 1;
                    }
                }
                else
                {
                    button.b3 = 0;
                    st.f3 = 0;
                }
                
                //LINE_A = 1;
                
                
                PORTB = 0xFF;
                LINE_B = 0;
                if(!COLUMNS_1)
                {
                    if(!st.f4)
                    {
                        button.b4 = 1;
                        button.t4 ^= 1;
                        st.f4 = 1;
                    }
                }
                else
                {
                    button.b4 = 0;
                    st.f4 = 0;
                }
                
                
                if(!COLUMNS_2)
                {
                    if(!st.f5)
                    {
                        button.b5 = 1;
                        button.t5 ^= 1;
                        st.f5 = 1;
                    }
                }
                else
                {
                    button.b5 = 0;
                    st.f5 = 0;
                }
                
                
                if(!COLUMNS_3)
                {
                    if(!st.f6)
                    {
                        button.b6 = 1;
                        button.t6 ^= 1;
                        st.f6 = 1;
                    } 
                }
                else
                {
                    button.b6 = 0;
                    st.f6 = 0;
                }
                
                //LINE_B = 1;
                
                
                PORTB = 0xFF;
                LINE_C = 0;
                if(!COLUMNS_1)
                {
                    if(!st.f7)
                    {
                        button.b7 = 1;
                        button.t7 ^= 1;
                        st.f7 = 1;
                    }
                }
                else
                {
                    button.b7 = 0;
                    st.f7 = 0;
                }
                
                
                if(!COLUMNS_2)
                {
                    if(!st.f8)
                    {
                        button.b8 = 1;
                        button.t8 ^= 1;
                        st.f8 = 1;
                    }
                }
                else
                {
                    button.b8 = 0;
                    st.f8 = 0;
                }
                
                
                if(!COLUMNS_3)
                {
                    if(!st.f9)
                    {
                        button.b9 = 1;
                        button.t9 ^= 1;
                        st.f9 = 1;
                    } 
                }
                else
                {
                    button.b9 = 0;
                    st.f9 = 0;
                }
                
                //LINE_C = 1;
                
                
                PORTB = 0xFF;
                LINE_D = 0;
                if(!COLUMNS_1)
                {
                    if(!st.f10)
                    {
                        button.b10 = 1;
                        button.t10 ^= 1;
                        st.f10 = 1;
                    }
                }
                else
                {
                    button.b10 = 0;
                    st.f10 = 0;
                }
                
                
                if(!COLUMNS_2)
                {
                    if(!st.f11)
                    {
                        button.b11 = 1;
                        button.t11 ^= 1;
                        st.f11 = 1;
                    }
                }
                else
                {
                    button.b11 = 0;
                    st.f11 = 0;
                }
                
                
                if(!COLUMNS_3)
                {
                    if(!st.f12)
                    {
                        button.b12 = 1;
                        button.t12 ^= 1;
                        st.f12 = 1;
                    }
                }
                else
                {
                    button.b12 = 0;
                    st.f12 = 0;
                }
                
                //LINE_D = 1;
                PORTB = 0;
                
                cnt_kn = 35;
                kn_st = 1;
            } // end if(cnt_kn > 70)
            
        } // end if(tmp_isr != 0x07)
        else
        {
            if(cnt_kn)                                              
            {
                cnt_kn--;
            }
            else
            {
                PORTB = 0;
                kn_st = 0;
                st.total = 0;
            }
        }
        
        if(!REG_TX)                                                 /* Опрос преключателя, задающего режим работы передатчика(с фиксацией или без). */
        {
            cnt_reg++;
            if(cnt_reg >= 70)
            {
                button.b14 = 1;
            }
        }
        else
        {
            if(cnt_reg)
            {
                cnt_reg--;
            }
            else
            {
                button.b14 = 0;
            }
        } // end if(!REG_TX)
        
    } // end if(TMR0IE&&TMR0IF)
    
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
    
    TMR0IF = 0;
    TMR0IE = 1;

    button.data[0] = 0;
    button.data[1] = 0;
    button.data[2] = 0;
    button.data[3] = 0;
    st.total = 0;
    
    ei();
    
    do
    {   
        while(kn_st)
        {
            crc = crc8(button.data, sizeof(button.data));
            TX_VDD = 1;                                             /* Вкл. питание передатчика. */
            TX_LED = 1;                                             /* Вкл. индикатор передачи. */
            pause(1);
            start();
            tx_data(button.data[0]);
            tx_data(button.data[1]);
            tx_data(button.data[2]);
            tx_data(button.data[3]);
            tx_data(crc);
            pause(50);
        }
        TX_VDD = 0;
        TX_LED = 0;

    }while(1);
} // end void main(void)




/* 
====================================================================
                  ФУНКЦИЯ ПЕРЕДАЧИ ДАННЫХ. 
====================================================================
*/
void tx_data(unsigned char dt)
{
    volatile unsigned char data;
    unsigned char cnt;

    data = dt;

    cnt = 8;
    while(cnt)
    {
        if(data&0x80)
        tx_one();
        else
        tx_zero();
        data <<= 1;
        cnt--;
    }
}




/* 
====================================================================
               ФУНКЦИЯ ФОРМИРОВАНИЯ УСЛОВИЯ СТАРТ. 
====================================================================
*/
void start(void)
{
    unsigned char count;
    count = 32;
    while(count)
    {
        TX_OUT = 1;
        __delay_us(250);
        TX_OUT = 0;
        __delay_us(250);
        count--;
    }
    TX_OUT = 0;
    pause(5);
}




/* 
====================================================================
                      ФУНКЦИЯ ПЕРЕДАЧИ 1 
====================================================================
*/
void tx_one(void)
{
    unsigned char count;
    count = 18;
    while(count)
    {
        TX_OUT = 1;
        __delay_us(250);
        TX_OUT = 0;
        __delay_us(250);
        count--;
    }
    TX_OUT = 0;
    pause(5);
}




/* 
====================================================================
                      ФУНКЦИЯ ПЕРЕДАЧИ 0 
====================================================================
*/
void tx_zero(void)
{
    unsigned char count;
    count = 9;
    while(count)
    {
        TX_OUT = 1;
        __delay_us(250);
        TX_OUT = 0;
        __delay_us(250);
        count--;
    }
    TX_OUT = 0;
    pause(5);
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


