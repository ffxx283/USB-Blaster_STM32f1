/*
blaster_port.h - USB-Blaster IO port header

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
#ifndef __BLASTER_PORT_H__
		#define __BLASTER_PORT_H__

		#include "base.h"

		// mode set
		#define BLASTER_STA_SHIFT           (0x80)
		#define BLASTER_STA_READ            (0x40)
		#define BLASTER_STA_CNT_MASK        (0x3f)

		// bit-bang out
		#define BLASTER_STA_OUT_OE          (0x20)
		#define BLASTER_STA_OUT_TDI         (0x10)
		#define BLASTER_STA_OUT_NCS         (0x08)
		#define BLASTER_STA_OUT_NCE         (0x04)
		#define BLASTER_STA_OUT_TMS         (0x02)
		#define BLASTER_STA_OUT_TCK         (0x01)

		// bit-bang in
		#define BLASTER_STA_IN_TDO          (0x01)
		#define BLASTER_STA_IN_DATAOUT      (0x02)

		#define BLASTER_STA_IN_TDO_BIT      (0)
		#define BLASTER_STA_IN_DATAOUT_BIT  (1)

		void bport_init(void);
		void AS_Mode_Init(void);
		void AS_Mode_Uninit(void);

		void bport_state_set(uint8_t d);
		uint8_t bport_state_get(void);

		void bport_shift_out(uint8_t d);
		uint8_t bport_shift_io(uint8_t d);
		
		/*--------------PORT Define---------------------*/
		// uncomment to enable AS mode (no pins defined)
		#define BLASTER_AS_MODE_EN
		// uncomment to enable OE/LED
		#define BLASTER_OE_LED_EN

		// TCK: PA0
		#define TCK_OUT(d)      PAO(0) = (d)
		#define TCK_0()         TCK_OUT(0)
		#define TCK_1()         TCK_OUT(1)

		// TDO: PB4
		#define TDO_IN()        PAI(6)

		// TDI: PB5
		#define TDI_OUT(d)      PAO(4) = (d)

		// TMS: PB6
		#define TMS_OUT(d)      PAO(1) = (d)

		#ifdef BLASTER_OE_LED_EN
		// OE/LED: PA8
		#define OE_OUT(d)       PAO(5) = (d)
		#endif

		#ifdef BLASTER_AS_MODE_EN
		// nCE: PB0 -- PC13
				#define NCE_OUT(d)      PCO(13) = (d)
				#define NCE_IN()    		PCI(13)
				// nCSO: PB1 -- PA3
				#define NCS_OUT(d)      PAO(3) = (d)
				#define NCS_GETOUT()    PAI(3)

				// DATAOUT: PA7
				#define DATAOUT_IN()    PAI(7)
		#endif
		/*--------------PORT Define End--------------------*/


#endif //__BLASTER_PORT_H__
