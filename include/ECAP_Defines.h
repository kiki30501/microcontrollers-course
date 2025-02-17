
#ifndef MY_ECAP_DEFINES_H
#define MY_ECAP_DEFINES_H

// ECCTL1 ( ECAP Control Reg 1)
//==========================
// CAPxPOL bits
#define EC_RISING 		0x0
#define EC_FALLING 		0x1

// CTRRSTx bits
#define EC_ABS_MODE 	0x0
#define EC_DELTA_MODE 	0x1

// PRESCALE bits
#define EC_BYPASS 		0x0
#define EC_DIV1 		0x0
#define EC_DIV2 		0x1
#define EC_DIV4 		0x2
#define EC_DIV6 		0x3
#define EC_DIV8 		0x4
#define EC_DIV10 		0x5

// ECCTL2 ( ECAP Control Reg 2)
//==========================
// CONT/ONESHOT bit
#define EC_CONTINUOUS 	0x0
#define EC_ONESHOT 		0x1

// STOPVALUE bit
#define EC_EVENT1 		0x0
#define EC_EVENT2 		0x1
#define EC_EVENT3 		0x2
#define EC_EVENT4 		0x3

// RE-ARM bit
#define EC_ARM 			0x1

// TSCTRSTOP bit
#define EC_FREEZE 		0x0
#define EC_RUN 			0x1

// SYNCO_SEL bit
#define EC_SYNCIN 		0x0
#define EC_CTR_PRD 		0x1
#define EC_SYNCO_DIS 	0x2

// CAP/APWM mode bit
#define EC_CAP_MODE 	0x0
#define EC_APWM_MODE 	0x1

// APWMPOL bit
#define EC_ACTV_HI 		0x0
#define EC_ACTV_LO 		0x1

// Generic
#define EC_DISABLE 		0x0
#define EC_ENABLE 		0x1
#define EC_FORCE 		0x1

#endif
