/*
 * _io.h
 *
 *  Created on: Oct 22, 2014
 *      Author: Nick
 */

#ifndef IO_H_
#define IO_H_

/**
 * Macros to initialize a pin in IO mode.
 */
#define enable_Px(PxSEL, PxSEL2, PIN) \
	PxSEL &= ~(PIN); \
	PxSEL2 &= ~(PIN);

#define enable_P1_0() enable_Px(P1SEL, P1SEL2, PIN0)
#define enable_P1_1() enable_Px(P1SEL, P1SEL2, PIN1)
#define enable_P1_2() enable_Px(P1SEL, P1SEL2, PIN2)
#define enable_P1_3() enable_Px(P1SEL, P1SEL2, PIN3)
#define enable_P1_4() enable_Px(P1SEL, P1SEL2, PIN4)
#define enable_P1_5() enable_Px(P1SEL, P1SEL2, PIN5)
#define enable_P1_6() enable_Px(P1SEL, P1SEL2, PIN6)
#define enable_P1_7() enable_Px(P1SEL, P1SEL2, PIN7)

#define enable_P2_0() enable_Px(P2SEL, P2SEL2, PIN0)
#define enable_P2_1() enable_Px(P2SEL, P2SEL2, PIN1)
#define enable_P2_2() enable_Px(P2SEL, P2SEL2, PIN2)
#define enable_P2_3() enable_Px(P2SEL, P2SEL2, PIN3)
#define enable_P2_4() enable_Px(P2SEL, P2SEL2, PIN4)
#define enable_P2_5() enable_Px(P2SEL, P2SEL2, PIN5)
#define enable_P2_6() enable_Px(P2SEL, P2SEL2, PIN6)
#define enable_P2_7() enable_Px(P2SEL, P2SEL2, PIN7)

#define enable_P3_0() enable_Px(P3SEL, P3SEL2, PIN0)
#define enable_P3_1() enable_Px(P3SEL, P3SEL2, PIN1)
#define enable_P3_2() enable_Px(P3SEL, P3SEL2, PIN2)
#define enable_P3_3() enable_Px(P3SEL, P3SEL2, PIN3)
#define enable_P3_4() enable_Px(P3SEL, P3SEL2, PIN4)
#define enable_P3_5() enable_Px(P3SEL, P3SEL2, PIN5)
#define enable_P3_6() enable_Px(P3SEL, P3SEL2, PIN6)
#define enable_P3_7() enable_Px(P3SEL, P3SEL2, PIN7)

/**
 * Macros to set the direction of a pin in a given IO port.
 */
#define dir_Px_pin(PxDIR, PIN, DIR) \
		PxDIR = (DIR==0) ? PxDIR & ~(PIN) : PxDIR | (PIN);
#define dir_Px_0(PxDIR, DIR) dir_Px_pin(PxDIR, PIN0, DIR)
#define dir_Px_1(PxDIR, DIR) dir_Px_pin(PxDIR, PIN1, DIR)
#define dir_Px_2(PxDIR, DIR) dir_Px_pin(PxDIR, PIN2, DIR)
#define dir_Px_3(PxDIR, DIR) dir_Px_pin(PxDIR, PIN3, DIR)
#define dir_Px_4(PxDIR, DIR) dir_Px_pin(PxDIR, PIN4, DIR)
#define dir_Px_5(PxDIR, DIR) dir_Px_pin(PxDIR, PIN5, DIR)
#define dir_Px_6(PxDIR, DIR) dir_Px_pin(PxDIR, PIN6, DIR)
#define dir_Px_7(PxDIR, DIR) dir_Px_pin(PxDIR, PIN7, DIR)

#define dir_P1_0(DIR) dir_Px_0(P1DIR, DIR)
#define dir_P1_1(DIR) dir_Px_1(P1DIR, DIR)
#define dir_P1_2(DIR) dir_Px_2(P1DIR, DIR)
#define dir_P1_3(DIR) dir_Px_3(P1DIR, DIR)
#define dir_P1_4(DIR) dir_Px_4(P1DIR, DIR)
#define dir_P1_5(DIR) dir_Px_5(P1DIR, DIR)
#define dir_P1_6(DIR) dir_Px_6(P1DIR, DIR)
#define dir_P1_7(DIR) dir_Px_7(P1DIR, DIR)

#define dir_P2_0(DIR) dir_Px_0(P2DIR, DIR)
#define dir_P2_1(DIR) dir_Px_1(P2DIR, DIR)
#define dir_P2_2(DIR) dir_Px_2(P2DIR, DIR)
#define dir_P2_3(DIR) dir_Px_3(P2DIR, DIR)
#define dir_P2_4(DIR) dir_Px_4(P2DIR, DIR)
#define dir_P2_5(DIR) dir_Px_5(P2DIR, DIR)
#define dir_P2_6(DIR) dir_Px_6(P2DIR, DIR)
#define dir_P2_7(DIR) dir_Px_7(P2DIR, DIR)

#define dir_P3_0(DIR) dir_Px_0(P3DIR, DIR)
#define dir_P3_1(DIR) dir_Px_1(P3DIR, DIR)
#define dir_P3_2(DIR) dir_Px_2(P3DIR, DIR)
#define dir_P3_3(DIR) dir_Px_3(P3DIR, DIR)
#define dir_P3_4(DIR) dir_Px_4(P3DIR, DIR)
#define dir_P3_5(DIR) dir_Px_5(P3DIR, DIR)
#define dir_P3_6(DIR) dir_Px_6(P3DIR, DIR)
#define dir_P3_7(DIR) dir_Px_7(P3DIR, DIR)

/**
 * Macros to set the output of a pin in a given IO port.
 */
#define write_Px_pin(PxOUT, PIN, VAL) \
	PxOUT = (VAL==0) ? PxOUT & ~(PIN) : PxOUT | (PIN);
#define write_Px_0(PxOUT, VAL) write_Px_pin(PxOUT, PIN0, VAL)
#define write_Px_1(PxOUT, VAL) write_Px_pin(PxOUT, PIN1, VAL)
#define write_Px_2(PxOUT, VAL) write_Px_pin(PxOUT, PIN2, VAL)
#define write_Px_3(PxOUT, VAL) write_Px_pin(PxOUT, PIN3, VAL)
#define write_Px_4(PxOUT, VAL) write_Px_pin(PxOUT, PIN4, VAL)
#define write_Px_5(PxOUT, VAL) write_Px_pin(PxOUT, PIN5, VAL)
#define write_Px_6(PxOUT, VAL) write_Px_pin(PxOUT, PIN6, VAL)
#define write_Px_7(PxOUT, VAL) write_Px_pin(PxOUT, PIN7, VAL)

#define write_P1_0(VAL) write_Px_0(P1OUT, VAL)
#define write_P1_1(VAL) write_Px_1(P1OUT, VAL)
#define write_P1_2(VAL) write_Px_2(P1OUT, VAL)
#define write_P1_3(VAL) write_Px_3(P1OUT, VAL)
#define write_P1_4(VAL) write_Px_4(P1OUT, VAL)
#define write_P1_5(VAL) write_Px_5(P1OUT, VAL)
#define write_P1_6(VAL) write_Px_6(P1OUT, VAL)
#define write_P1_7(VAL) write_Px_7(P1OUT, VAL)

#define write_P2_0(VAL) write_Px_0(P2OUT, VAL)
#define write_P2_1(VAL) write_Px_1(P2OUT, VAL)
#define write_P2_2(VAL) write_Px_2(P2OUT, VAL)
#define write_P2_3(VAL) write_Px_3(P2OUT, VAL)
#define write_P2_4(VAL) write_Px_4(P2OUT, VAL)
#define write_P2_5(VAL) write_Px_5(P2OUT, VAL)
#define write_P2_6(VAL) write_Px_6(P2OUT, VAL)
#define write_P2_7(VAL) write_Px_7(P2OUT, VAL)

#define write_P3_0(VAL) write_Px_0(P3OUT, VAL)
#define write_P3_1(VAL) write_Px_1(P3OUT, VAL)
#define write_P3_2(VAL) write_Px_2(P3OUT, VAL)
#define write_P3_3(VAL) write_Px_3(P3OUT, VAL)
#define write_P3_4(VAL) write_Px_4(P3OUT, VAL)
#define write_P3_5(VAL) write_Px_5(P3OUT, VAL)
#define write_P3_6(VAL) write_Px_6(P3OUT, VAL)
#define write_P3_7(VAL) write_Px_7(P3OUT, VAL)

/**
 * Macros to toggle a pin in a given IO port.
 */
#define toggle_Px_pin(PxOUT, PIN) PxOUT ^= PIN;
#define toggle_Px_0(PxOUT) toggle_Px_pin(PxOUT, PIN0)
#define toggle_Px_1(PxOUT) toggle_Px_pin(PxOUT, PIN1)
#define toggle_Px_2(PxOUT) toggle_Px_pin(PxOUT, PIN2)
#define toggle_Px_3(PxOUT) toggle_Px_pin(PxOUT, PIN3)
#define toggle_Px_4(PxOUT) toggle_Px_pin(PxOUT, PIN4)
#define toggle_Px_5(PxOUT) toggle_Px_pin(PxOUT, PIN5)
#define toggle_Px_6(PxOUT) toggle_Px_pin(PxOUT, PIN6)
#define toggle_Px_7(PxOUT) toggle_Px_pin(PxOUT, PIN7)

#define toggle_P1_0() toggle_Px_0(P1OUT)
#define toggle_P1_1() toggle_Px_1(P1OUT)
#define toggle_P1_2() toggle_Px_2(P1OUT)
#define toggle_P1_3() toggle_Px_3(P1OUT)
#define toggle_P1_4() toggle_Px_4(P1OUT)
#define toggle_P1_5() toggle_Px_5(P1OUT)
#define toggle_P1_6() toggle_Px_6(P1OUT)
#define toggle_P1_7() toggle_Px_7(P1OUT)

#define toggle_P2_0() toggle_Px_0(P2OUT)
#define toggle_P2_1() toggle_Px_1(P2OUT)
#define toggle_P2_2() toggle_Px_2(P2OUT)
#define toggle_P2_3() toggle_Px_3(P2OUT)
#define toggle_P2_4() toggle_Px_4(P2OUT)
#define toggle_P2_5() toggle_Px_5(P2OUT)
#define toggle_P2_6() toggle_Px_6(P2OUT)
#define toggle_P2_7() toggle_Px_7(P2OUT)

#define toggle_P3_0() toggle_Px_0(P3OUT)
#define toggle_P3_1() toggle_Px_1(P3OUT)
#define toggle_P3_2() toggle_Px_2(P3OUT)
#define toggle_P3_3() toggle_Px_3(P3OUT)
#define toggle_P3_4() toggle_Px_4(P3OUT)
#define toggle_P3_5() toggle_Px_5(P3OUT)
#define toggle_P3_6() toggle_Px_6(P3OUT)
#define toggle_P3_7() toggle_Px_7(P3OUT)

/**
 * Macros to read from a pin in a given IO port.
 */
#define read_Px_0(PxIN) ((PxIN & PIN0)>>0)
#define read_Px_1(PxIN) ((PxIN & PIN1)>>1)
#define read_Px_2(PxIN) ((PxIN & PIN1)>>2)
#define read_Px_3(PxIN) ((PxIN & PIN1)>>3)
#define read_Px_4(PxIN) ((PxIN & PIN1)>>4)
#define read_Px_5(PxIN) ((PxIN & PIN1)>>5)
#define read_Px_6(PxIN) ((PxIN & PIN1)>>6)
#define read_Px_7(PxIN) ((PxIN & PIN1)>>7)

#define read_P1_0() read_Px_0(P1IN)
#define read_P1_1() read_Px_1(P1IN)
#define read_P1_2() read_Px_2(P1IN)
#define read_P1_3() read_Px_3(P1IN)
#define read_P1_4() read_Px_4(P1IN)
#define read_P1_5() read_Px_5(P1IN)
#define read_P1_6() read_Px_6(P1IN)
#define read_P1_7() read_Px_7(P1IN)

#define read_P2_0() read_Px_0(P2IN)
#define read_P2_1() read_Px_1(P2IN)
#define read_P2_2() read_Px_2(P2IN)
#define read_P2_3() read_Px_3(P2IN)
#define read_P2_4() read_Px_4(P2IN)
#define read_P2_5() read_Px_5(P2IN)
#define read_P2_6() read_Px_6(P2IN)
#define read_P2_7() read_Px_7(P2IN)

#define read_P3_0() read_Px_0(P3IN)
#define read_P3_1() read_Px_1(P3IN)
#define read_P3_2() read_Px_2(P3IN)
#define read_P3_3() read_Px_3(P3IN)
#define read_P3_4() read_Px_4(P3IN)
#define read_P3_5() read_Px_5(P3IN)
#define read_P3_6() read_Px_6(P3IN)
#define read_P3_7() read_Px_7(P3IN)


#endif /* IO_H_ */
