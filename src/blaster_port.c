/*
blaster_port.c - USB-Blaster IO port

MIT License

Copyright (c) 2016 Yongqian Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "base.h"
#include "bitband.h"
#include "blaster_port.h"

#define nCEPortNum GPIO_Pin_13

extern u8 Now_Mode_AS;
void bport_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

    // GPIO Out Configuration: TCK(PB3), TDI(PB5), TMS(PB6)
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO In Configuration: TDO(PB4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#ifdef BLASTER_OE_LED_EN
    // led(OE) pin already init in led.c
#endif

#ifdef BLASTER_AS_MODE_EN
    // init AS mode pins here (nCS/nCE/DATAOUT)
		// DATA0
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

		//nCSO
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
				
		//nCE
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//¶Ë¿Ú¸´ÓÃ 
		GPIO_InitStructure.GPIO_Pin =   nCEPortNum ;//|GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		PWR_BackupAccessCmd(ENABLE);//????RTC ??????		 
		RCC_LSEConfig(RCC_LSE_OFF);//?????????????? ?,PC13 PC14 PC15 ??????IO??		 
		//BKP_TamperPinCmd(DISABLE);//????????,??? PC13,??????IO ??		 
		//BKP_ITConfig(DISABLE);
		
		
//		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
}
//=------------------------------------------------------


// bit-band mode output
void bport_state_set(uint8_t d)
{
    TDI_OUT(0 != (d & BLASTER_STA_OUT_TDI));
    TMS_OUT(0 != (d & BLASTER_STA_OUT_TMS));
    TCK_OUT(0 != (d & BLASTER_STA_OUT_TCK));
#ifdef BLASTER_AS_MODE_EN
	//if(Now_Mode_AS){
		NCE_OUT(0 != (d & BLASTER_STA_OUT_NCE));
		NCS_OUT(0 != (d & BLASTER_STA_OUT_NCS));
	//}

#endif
		
#ifdef BLASTER_OE_LED_EN
    OE_OUT(0 != (d & BLASTER_STA_OUT_OE));
#endif
}

// bit-bang mode input
uint8_t bport_state_get(void)
{
    uint32_t d = 0;

    d |= TDO_IN() << BLASTER_STA_IN_TDO_BIT;
#ifdef BLASTER_AS_MODE_EN
//if(Now_Mode_AS)
			d |= DATAOUT_IN() << BLASTER_STA_IN_DATAOUT_BIT;
#endif

    return (uint8_t)d;
}

// shift mode output
void bport_shift_out(uint8_t d)
{
#define BPORT_SHIFT_BIT()    TDI_OUT(d); TCK_1(); d >>= 1; TCK_0()

    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();
    BPORT_SHIFT_BIT();

#undef BPORT_SHIFT_BIT
}



uint8_t bport_shift_io(uint8_t d)
{
    uint32_t dshift = d;
    uint32_t din;
    
#ifdef BLASTER_AS_MODE_EN
		// as mode
    if (  !NCS_GETOUT()) {
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        TDI_OUT(dshift); din = DATAOUT_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
        return dshift & 0xff;
    }


#endif //BLASTER_AS_MODE_EN
		// Jtag mode
		if (  NCS_GETOUT())
		{
			  TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
				TDI_OUT(dshift); din = TDO_IN(); TCK_1(); dshift = (dshift >> 1) | (din << 7); TCK_0();
		}

    return dshift & 0xff;
}
