////////////////////////////////////////////////////////////////////////////////
/// @file     mm32_reg_define_v1.H
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE define for version compatibility define
///           FIRMWARE LIBRARY.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#ifndef __MM32_REG_DEFINE_V1_H
#define __MM32_REG_DEFINE_V1_H

// Files includes

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined(__GNUC__)
// anonymous unions are enabled by default
#else
#warning Not supported compiler type
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief redefine for register
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief redefine for ADC of MCU register
////////////////////////////////////////////////////////////////////////////////

#define ADC_COMP_IRQHandler             ADC1_COMP_IRQHandler

#define ADDATA_DATA                     ADC_DR_DATA

#define ADDATA_CHANNELSEL               ADC_DR_CH
#define ADDATA_CHANNELSEL_0             ADC_DR_CH0
#define ADDATA_CHANNELSEL_1             ADC_DR_CH1
#define ADDATA_CHANNELSEL_2             ADC_DR_CH2
#define ADDATA_CHANNELSEL_3             ADC_DR_CH3
#define ADDATA_CHANNELSEL_4             ADC_DR_CH4
#define ADDATA_CHANNELSEL_5             ADC_DR_CH5
#define ADDATA_CHANNELSEL_6             ADC_DR_CH6
#define ADDATA_CHANNELSEL_7             ADC_DR_CH7




#define ADDATA_OVERRUN                  ADC_DR_OVERRUN
#define ADDATA_VALID                    ADC_DR_VALID

#define ADCFG_ADEN                      ADC_CFGR_ADEN
#define ADCFG_ADWEN                     ADC_CFGR_ADWEN

#define ADCFG_RSLTCTL                   ADC_CFGR_RSLTCTL
#define ADCFG_RSLTCTL_12                ADC_CFGR_RSLTCTL_12
#define ADCFG_RSLTCTL_11                ADC_CFGR_RSLTCTL_11
#define ADCFG_RSLTCTL_10                ADC_CFGR_RSLTCTL_10
#define ADCFG_RSLTCTL_9                 ADC_CFGR_RSLTCTL_9
#define ADCFG_RSLTCTL_8                 ADC_CFGR_RSLTCTL_8



#define ADCFG_ADCPRE                    ADC_CFGR_PRE
#define ADCFG_ADCPRE_2                  ADC_CFGR_PRE_2
#define ADCFG_ADCPRE_4                  ADC_CFGR_PRE_4
#define ADCFG_ADCPRE_6                  ADC_CFGR_PRE_6
#define ADCFG_ADCPRE_8                  ADC_CFGR_PRE_8
#define ADCFG_ADCPRE_10                 ADC_CFGR_PRE_10
#define ADCFG_ADCPRE_12                 ADC_CFGR_PRE_12
#define ADCFG_ADCPRE_14                 ADC_CFGR_PRE_14
#define ADCFG_ADCPRE_16                 ADC_CFGR_PRE_16
#define ADCFG_SAMCTL                    ADC_CFGR_SAMCTL
#define ADCFG_SAMCTL_1_5                ADC_CFGR_SAMCTL_1_5
#define ADCFG_SAMCTL_7_5                ADC_CFGR_SAMCTL_7_5
#define ADCFG_SAMCTL_13_5               ADC_CFGR_SAMCTL_13_5
#define ADCFG_SAMCTL_28_5               ADC_CFGR_SAMCTL_28_5
#define ADCFG_SAMCTL_41_5               ADC_CFGR_SAMCTL_41_5
#define ADCFG_SAMCTL_55_5               ADC_CFGR_SAMCTL_55_5
#define ADCFG_SAMCTL_71_5               ADC_CFGR_SAMCTL_71_5
#define ADCFG_SAMCTL_239_5              ADC_CFGR_SAMCTL_239_5


#define ADCR_ADIE                       ADC_CR_ADIE
#define ADCR_ADWIE                      ADC_CR_ADWIE
#define ADCR_TRGEN                      ADC_CR_TRGEN
#define ADCR_ADST                       ADC_CR_ADST
#define ADCR_ADMD                       ADC_CR_MODE
#define ADCR_ADMD_SINGLE                ADC_CR_IMM
#define ADCR_ADMD_PERIOD                ADC_CR_SCAN
#define ADCR_ADMD_CONTINUE              ADC_CR_CONTINUE
#define ADC_Mode_Single                 ADC_CR_IMM
#define ADC_Mode_Single_Period          ADC_CR_SCAN
#define ADC_Mode_Continuous_Scan        ADC_CR_CONTINUE

#define ADCR_ALIGN                      ADC_CR_ALIGN
#define ADCR_ALIGN_LEFT                 ADC_CR_LEFT
#define ADCR_ALIGN_RIGHT                ADC_CR_RIGHT
#define ADCR_CMPCH_Pos                  ADC_CR_CMPCH_Pos
#define ADCR_CMPCH                      ADC_CR_CMPCH
#define ADCR_CMPCH_0                    ADC_CR_CMPCH_0
#define ADCR_CMPCH_1                    ADC_CR_CMPCH_1
#define ADCR_CMPCH_2                    ADC_CR_CMPCH_2
#define ADCR_CMPCH_4                    ADC_CR_CMPCH_4
#define ADCR_CMPCH_5                    ADC_CR_CMPCH_5
#define ADCR_CMPCH_6                    ADC_CR_CMPCH_6
#define ADCR_CMPCH_7                    ADC_CR_CMPCH_7
#define ADCR_CMPCH_8                    ADC_CR_CMPCH_8
#define ADCR_CMPCH_9                    ADC_CR_CMPCH_9
#define ADCR_CMPCH_10                   ADC_CR_CMPCH_10
#define ADCR_CMPCH_11                   ADC_CR_CMPCH_11
#define ADCR_CMPCH_13                   ADC_CR_CMPCH_13
#define ADCR_CMPCH_14                   ADC_CR_CMPCH_14
#define ADCR_CMPCH_ALL                  ADC_CR_CMPCH_ALL


#define ADCR_TRGSEL                     ADC_CR_TRGSEL
#define ADCR_TRGSEL_T1_CC1              ADC_CR_T1_CC1
#define ADCR_TRGSEL_T1_CC2              ADC_CR_T1_CC2
#define ADCR_TRGSEL_T1_CC3              ADC_CR_T1_CC3
#define ADCR_TRGSEL_T2_CC2              ADC_CR_T2_CC2
#define ADCR_TRGSEL_T3_TRGO             ADC_CR_T3_TRIG
#define ADCR_TRGSEL_EXTI_11             ADC_CR_EXTI_11




#define ADCHS_CHEN0                     ADC_CHSR_CH0
#define ADCHS_CHEN1                     ADC_CHSR_CH1
#define ADCHS_CHEN2                     ADC_CHSR_CH2
#define ADCHS_CHEN3                     ADC_CHSR_CH3
#define ADCHS_CHEN4                     ADC_CHSR_CH4
#define ADCHS_CHEN5                     ADC_CHSR_CH5
#define ADCHS_CHEN6                     ADC_CHSR_CH6
#define ADCHS_CHEN7                     ADC_CHSR_CH7
#define ADCHS_ALL                       ADC_CHSR_CHALL




#define ADCMPR_CMPLDATA                 ADC_CMPR_CMPLDATA
#define ADCMPR_CMPHDATA                 ADC_CMPR_CMPHDATA

#define ADSTA_ADIF                      ADC_SR_ADIF
#define ADSTA_ADWIF                     ADC_SR_ADWIF
#define ADSTA_BUSY                      ADC_SR_BUSY
#define ADSTA_CHANNEL                   ADC_SR_CH
#define ADSTA_CHANNEL_CH0               ADC_SR_CH0
#define ADSTA_CHANNEL_CH1               ADC_SR_CH1
#define ADSTA_CHANNEL_CH2               ADC_SR_CH2
#define ADSTA_CHANNEL_CH3               ADC_SR_CH3
#define ADSTA_CHANNEL_CH4               ADC_SR_CH4
#define ADSTA_CHANNEL_CH5               ADC_SR_CH5
#define ADSTA_CHANNEL_CH6               ADC_SR_CH6
#define ADSTA_CHANNEL_CH7               ADC_SR_CH7
#define ADSTA_CHANNEL_CH8               ADC_SR_CH8
#define ADSTA_CHANNEL_CH9               ADC_SR_CH9
#define ADSTA_CHANNEL_CH10              ADC_SR_CH10
#define ADSTA_CHANNEL_CH11              ADC_SR_CH11
#define ADSTA_CHANNEL_CH13              ADC_SR_CH13
#define ADSTA_CHANNEL_CH14              ADC_SR_CH14
#define ADSTA_CHANNEL_CH15              ADC_SR_CH15
#define ADSTA_VALID                     ADC_SR_VALID
#define ADSTA_OVERRUN                   ADC_SR_OVERRUN

#define ADDR_OVERRUN                    ADC_CHDR_OVERRUN
#define ADDR_VALID                      ADC_CHDR_VALID
#define ADDR_DATA                       ADC_CHDR_DATA

////////////////////////////////////////////////////////////////////////////////
/// @brief redefine for HAL library
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief redefine for ADC of HAL library
////////////////////////////////////////////////////////////////////////////////

#define ADC_ExternalTrigConv_T1_CC1     ADC1_ExternalTrigConv_T1_CC1
#define ADC_ExternalTrigConv_T1_CC2     ADC1_ExternalTrigConv_T1_CC2
#define ADC_ExternalTrigConv_T1_CC3     ADC1_ExternalTrigConv_T1_CC3
#define ADC_ExternalTrigConv_T2_CC2     ADC1_ExternalTrigConv_T2_CC2
#define ADC_ExternalTrigConv_T3_TRIG    ADC1_ExternalTrigConv_T3_TRIG
#define ADC_ExternalTrigConv_T3_CC1     ADC1_ExternalTrigConv_T3_CC1
#define ADC_ExternalTrigConv_EXTI_11    ADC1_ExternalTrigConv_EXTI_11

#define ADC_SampleTime_1_5Cycles         ADC_Samctl_1_5
#define ADC_SampleTime_7_5Cycles         ADC_Samctl_7_5
#define ADC_SampleTime_13_5Cycles        ADC_Samctl_13_5
#define ADC_SampleTime_28_5Cycles        ADC_Samctl_28_5
#define ADC_SampleTime_41_5Cycles        ADC_Samctl_41_5
#define ADC_SampleTime_55_5Cycles        ADC_Samctl_55_5
#define ADC_SampleTime_71_5Cycles        ADC_Samctl_71_5
#define ADC_SampleTime_239_5Cycles       ADC_Samctl_239_5

////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////
#define AES_KEYR0                       AES_KEYRn                               ///< AES Key Register 0
#define AES_KEYR1                       AES_KEYRn                               ///< AES Key Register 1
#define AES_KEYR2                       AES_KEYRn                               ///< AES Key Register 2
#define AES_KEYR3                       AES_KEYRn                               ///< AES Key Register 3
#define AES_KEYR4                       AES_KEYRn                               ///< AES Key Register 4
#define AES_KEYR5                       AES_KEYRn                               ///< AES Key Register 5
#define AES_KEYR6                       AES_KEYRn                               ///< AES Key Register 6
#define AES_KEYR7                       AES_KEYRn                               ///< AES Key Register 7

#define AES_IVR0                        AES_IVRn                                ///< AES Initialization Vector Register 0
#define AES_IVR1                        AES_IVRn                                ///< AES Initialization Vector Register 1
#define AES_IVR2                        AES_IVRn                                ///< AES Initialization Vector Register 2
#define AES_IVR3                        AES_IVRn                                ///< AES Initialization Vector Register 3

////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////
#define CRS_CR_SYNCOKIE                 CRS_CR_OKIE
#define CRS_CR_SYNCWARNIE               CRS_CR_WARNIE
#define CRS_CR_ESYNCIE                  CRS_CR_EXPTIE
#define CRS_CR_CEN                      CRS_CR_CNTEN
#define CRS_CFGR_SYNCDIV                CRS_CFGR_DIV
#define CRS_CFGR_SYNCSRC                CRS_CFGR_SRC
#define CRS_CFGR_SYNCPOL                CRS_CFGR_POL
#define CRS_ISR_SYNCOKF                 CRS_ISR_OKIF
#define CRS_ISR_SYNCWARNF               CRS_ISR_WARNIF
#define CRS_ISR_ERRF                    CRS_ISR_ERRIF
#define CRS_ISR_ESYNCF                  CRS_ISR_EXPTIF
#define CRS_ISR_SYNCERR                 CRS_ISR_ERR
#define CRS_ISR_SYNCMISS                CRS_ISR_MISS
#define CRS_ISR_TRIMOVF                 CRS_ISR_OVERFLOW
#define CRS_ICR_SYNCOKC                 CRS_ICR_OK
#define CRS_ICR_SYNCWARNC               CRS_ICR_WARN
#define CRS_ICR_ERRC                    CRS_ICR_ERR
#define CRS_ICR_ESYNCC                  CRS_ICR_EXPT

#define CRS_IT_SYNCOK                   CRS_ISR_SYNCOKF                         ///< SYNC event OK
#define CRS_IT_SYNCWARN                 CRS_ISR_SYNCWARNF                       ///< SYNC warning
#define CRS_IT_ERR                      CRS_CR_ERRIE                            ///< error
#define CRS_IT_ESYNC                    CRS_ISR_ESYNCF                          ///< Expected SYNC
#define CRS_IT_TRIMOVF                  CRS_ISR_TRIMOVF                         ///< Trimming overflow or underflow
#define CRS_IT_SYNCERR                  CRS_ISR_SYNCERR                         ///< SYNC error
#define CRS_IT_SYNCMISS                 CRS_ISR_SYNCMISS                        ///< SYNC missed

#define CRS_FLAG_SYNCOK                 CRS_ISR_SYNCOKF                         ///< SYNC event OK
#define CRS_FLAG_SYNCWARN               CRS_ISR_SYNCWARNF                       ///< SYNC warning
#define CRS_FLAG_ERR                    CRS_ISR_ERRF                            ///< error
#define CRS_FLAG_ESYNC                  CRS_ISR_ESYNCF                          ///< Expected SYNC
#define CRS_FLAG_TRIMOVF                CRS_ISR_TRIMOVF                         ///< Trimming overflow or underflow
#define CRS_FLAG_SYNCERR                CRS_ISR_SYNCERR                         ///< SYNC error
#define CRS_FLAG_SYNCMISS               CRS_ISR_SYNCMISS                        ///< SYNC missed
////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#define EXTI_IMR_MR0                    EXTI_IMR_0
#define EXTI_IMR_MR1                    EXTI_IMR_1
#define EXTI_IMR_MR2                    EXTI_IMR_2
#define EXTI_IMR_MR3                    EXTI_IMR_3
#define EXTI_IMR_MR4                    EXTI_IMR_4
#define EXTI_IMR_MR5                    EXTI_IMR_5
#define EXTI_IMR_MR6                    EXTI_IMR_6
#define EXTI_IMR_MR7                    EXTI_IMR_7
#define EXTI_IMR_MR8                    EXTI_IMR_8
#define EXTI_IMR_MR9                    EXTI_IMR_9
#define EXTI_IMR_MR10                   EXTI_IMR_10
#define EXTI_IMR_MR11                   EXTI_IMR_11
#define EXTI_IMR_MR12                   EXTI_IMR_12
#define EXTI_IMR_MR13                   EXTI_IMR_13
#define EXTI_IMR_MR14                   EXTI_IMR_14
#define EXTI_IMR_MR15                   EXTI_IMR_15
#define EXTI_IMR_MR16                   EXTI_IMR_16

#define EXTI_EMR_MR0                    EXTI_EMR_0
#define EXTI_EMR_MR1                    EXTI_EMR_1
#define EXTI_EMR_MR2                    EXTI_EMR_2
#define EXTI_EMR_MR3                    EXTI_EMR_3
#define EXTI_EMR_MR4                    EXTI_EMR_4
#define EXTI_EMR_MR5                    EXTI_EMR_5
#define EXTI_EMR_MR6                    EXTI_EMR_6
#define EXTI_EMR_MR7                    EXTI_EMR_7
#define EXTI_EMR_MR8                    EXTI_EMR_8
#define EXTI_EMR_MR9                    EXTI_EMR_9
#define EXTI_EMR_MR10                   EXTI_EMR_10
#define EXTI_EMR_MR11                   EXTI_EMR_11
#define EXTI_EMR_MR12                   EXTI_EMR_12
#define EXTI_EMR_MR13                   EXTI_EMR_13
#define EXTI_EMR_MR14                   EXTI_EMR_14
#define EXTI_EMR_MR15                   EXTI_EMR_15
#define EXTI_EMR_MR16                   EXTI_EMR_16

#define EXTI_RTSR_TR0                   EXTI_RTSR_0
#define EXTI_RTSR_TR1                   EXTI_RTSR_1
#define EXTI_RTSR_TR2                   EXTI_RTSR_2
#define EXTI_RTSR_TR3                   EXTI_RTSR_3
#define EXTI_RTSR_TR4                   EXTI_RTSR_4
#define EXTI_RTSR_TR5                   EXTI_RTSR_5
#define EXTI_RTSR_TR6                   EXTI_RTSR_6
#define EXTI_RTSR_TR7                   EXTI_RTSR_7
#define EXTI_RTSR_TR8                   EXTI_RTSR_8
#define EXTI_RTSR_TR9                   EXTI_RTSR_9
#define EXTI_RTSR_TR10                  EXTI_RTSR_10
#define EXTI_RTSR_TR11                  EXTI_RTSR_11
#define EXTI_RTSR_TR12                  EXTI_RTSR_12
#define EXTI_RTSR_TR13                  EXTI_RTSR_13
#define EXTI_RTSR_TR14                  EXTI_RTSR_14
#define EXTI_RTSR_TR15                  EXTI_RTSR_15
#define EXTI_RTSR_TR16                  EXTI_RTSR_16

#define EXTI_FTSR_TR0                   EXTI_FTSR_0
#define EXTI_FTSR_TR1                   EXTI_FTSR_1
#define EXTI_FTSR_TR2                   EXTI_FTSR_2
#define EXTI_FTSR_TR3                   EXTI_FTSR_3
#define EXTI_FTSR_TR4                   EXTI_FTSR_4
#define EXTI_FTSR_TR5                   EXTI_FTSR_5
#define EXTI_FTSR_TR6                   EXTI_FTSR_6
#define EXTI_FTSR_TR7                   EXTI_FTSR_7
#define EXTI_FTSR_TR8                   EXTI_FTSR_8
#define EXTI_FTSR_TR9                   EXTI_FTSR_9
#define EXTI_FTSR_TR10                  EXTI_FTSR_10
#define EXTI_FTSR_TR11                  EXTI_FTSR_11
#define EXTI_FTSR_TR12                  EXTI_FTSR_12
#define EXTI_FTSR_TR13                  EXTI_FTSR_13
#define EXTI_FTSR_TR14                  EXTI_FTSR_14
#define EXTI_FTSR_TR15                  EXTI_FTSR_15
#define EXTI_FTSR_TR16                  EXTI_FTSR_16

#define EXTI_SWIER_SWIER0               EXTI_SWIER_0
#define EXTI_SWIER_SWIER1               EXTI_SWIER_1
#define EXTI_SWIER_SWIER2               EXTI_SWIER_2
#define EXTI_SWIER_SWIER3               EXTI_SWIER_3
#define EXTI_SWIER_SWIER4               EXTI_SWIER_4
#define EXTI_SWIER_SWIER5               EXTI_SWIER_5
#define EXTI_SWIER_SWIER6               EXTI_SWIER_6
#define EXTI_SWIER_SWIER7               EXTI_SWIER_7
#define EXTI_SWIER_SWIER8               EXTI_SWIER_8
#define EXTI_SWIER_SWIER9               EXTI_SWIER_9
#define EXTI_SWIER_SWIER10              EXTI_SWIER_10
#define EXTI_SWIER_SWIER11              EXTI_SWIER_11
#define EXTI_SWIER_SWIER12              EXTI_SWIER_12
#define EXTI_SWIER_SWIER13              EXTI_SWIER_13
#define EXTI_SWIER_SWIER14              EXTI_SWIER_14
#define EXTI_SWIER_SWIER15              EXTI_SWIER_15
#define EXTI_SWIER_SWIER16              EXTI_SWIER_16

#define EXTI_PR_PR0                     EXTI_PR_0
#define EXTI_PR_PR1                     EXTI_PR_1
#define EXTI_PR_PR2                     EXTI_PR_2
#define EXTI_PR_PR3                     EXTI_PR_3
#define EXTI_PR_PR4                     EXTI_PR_4
#define EXTI_PR_PR5                     EXTI_PR_5
#define EXTI_PR_PR6                     EXTI_PR_6
#define EXTI_PR_PR7                     EXTI_PR_7
#define EXTI_PR_PR8                     EXTI_PR_8
#define EXTI_PR_PR9                     EXTI_PR_9
#define EXTI_PR_PR10                    EXTI_PR_10
#define EXTI_PR_PR11                    EXTI_PR_11
#define EXTI_PR_PR12                    EXTI_PR_12
#define EXTI_PR_PR13                    EXTI_PR_13
#define EXTI_PR_PR14                    EXTI_PR_14
#define EXTI_PR_PR15                    EXTI_PR_15
#define EXTI_PR_PR16                    EXTI_PR_16



#define EXTI_IMR_MR19                   EXTI_IMR_19







#define EXTI_EMR_MR19                   EXTI_EMR_19







#define EXTI_RTSR_MR19                  EXTI_RTSR_19







#define EXTI_FTSR_MR19                  EXTI_FTSR_19







#define EXTI_SWIER_MR19                 EXTI_SWIER_19







#define EXTI_PR_MR19                    EXTI_PR_19





////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////

#define GPIO_Mode_IN_FLOATING           GPIO_Mode_FLOATING


////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////
//Bit
#define I2C_CON_MASTER_MODE             I2C_CR_MASTER
#define I2C_CON_SPEED                   I2C_CR_SPEED
#define I2C_CON_SPEED_STANDARD          I2C_CR_STD
#define I2C_CON_SPEED_FAST              I2C_CR_FAST
#define I2C_CON_10BITADDR_SLAVE         I2C_CR_SLAVE10
#define I2C_CON_10BITADDR_MASTER        I2C_CR_MASTER10
#define I2C_CON_RESTART_EN              I2C_CR_REPEN
#define I2C_CON_SLAVE_DISABLE           I2C_CR_SLAVEDIS
#define I2C_CON_STOP_DET_IFADDRESSED    I2C_CR_STOPINT
#define I2C_CON_EMPTY_CTRL              I2C_CR_EMPINT
#define I2C_CON_STOP                    I2C_CR_STOP
#define I2C_CON_RESTART                 I2C_CR_RESTART

#define I2C_TAR_TAR                     I2C_TAR_ADDR
#define I2C_TAR_GC_OR_START             I2C_TAR_GC
#define I2C_SAR_SAR                     I2C_SAR_ADDR
#define I2C_DATA_CMD_DAT                I2C_DR_DAT
#define I2C_DATA_CMD_CMD                I2C_DR_CMD
#define I2C_DATA_CMD_STOP               I2C_DR_STOP
#define I2C_DATA_CMD_RESTART            I2C_DR_RESTART

#define I2C_SS_SCL_HCNT                 I2C_SSHR_CNT
#define I2C_SS_SCL_LCNT                 I2C_SSLR_CNT
#define I2C_FS_SCL_HCNT                 I2C_FSHR_CNT
#define I2C_FS_SCL_LCNT                 I2C_FSLR_CNT

#define I2C_INTR_STAT_RX_UNDER          I2C_ISR_RX_UNDER
#define I2C_INTR_STAT_RX_OVER           I2C_ISR_RX_OVER
#define I2C_INTR_STAT_RX_FULL           I2C_ISR_RX_FULL
#define I2C_INTR_STAT_TX_OVER           I2C_ISR_TX_OVER
#define I2C_INTR_STAT_TX_EMPTY          I2C_ISR_TX_EMPTY
#define I2C_INTR_STAT_RX_REQ            I2C_ISR_RX_REQ
#define I2C_INTR_STAT_TX_ABRT           I2C_ISR_TX_ABRT
#define I2C_INTR_STAT_RX_DONE           I2C_ISR_RX_DONE
#define I2C_INTR_STAT_ACTIVITY          I2C_ISR_ACTIV
#define I2C_INTR_STAT_STOP_DET          I2C_ISR_STOP
#define I2C_INTR_STAT_START_DET         I2C_ISR_START
#define I2C_INTR_STAT_GEN_CALL          I2C_ISR_GC
#define I2C_INTR_STAT_RESTART_DET       I2C_ISR_RESTART
#define I2C_INTR_STAT_MST_ON_HOLD       I2C_ISR_HOLD

#define I2C_INTR_MASK_RX_UNDER          I2C_IMR_RX_UNDER
#define I2C_INTR_MASK_RX_OVER           I2C_IMR_RX_OVER
#define I2C_INTR_MASK_RX_FULL           I2C_IMR_RX_FULL
#define I2C_INTR_MASK_TX_OVER           I2C_IMR_TX_OVER
#define I2C_INTR_MASK_TX_EMPTY          I2C_IMR_TX_EMPTY
#define I2C_INTR_MASK_RX_REQ            I2C_IMR_RX_REQ
#define I2C_INTR_MASK_TX_ABRT           I2C_IMR_TX_ABRT
#define I2C_INTR_MASK_RX_DONE           I2C_IMR_RX_DONE
#define I2C_INTR_MASK_ACTIVITY          I2C_IMR_ACTIV
#define I2C_INTR_MASK_STOP_DET          I2C_IMR_STOP
#define I2C_INTR_MASK_START_DET         I2C_IMR_START
#define I2C_INTR_MASK_GEN_CALL          I2C_IMR_GC
#define I2C_INTR_MASK_RESTART_DET       I2C_IMR_RESTART
#define I2C_INTR_MASK_MST_ON_HOLD       I2C_IMR_HOLD

#define I2C_RAW_INTR_MASK_RX_UNDER      I2C_RAWISR_RX_UNDER
#define I2C_RAW_INTR_MASK_RX_OVER       I2C_RAWISR_RX_OVER
#define I2C_RAW_INTR_MASK_RX_FULL       I2C_RAWISR_RX_FULL
#define I2C_RAW_INTR_MASK_TX_OVER       I2C_RAWISR_TX_OVER
#define I2C_RAW_INTR_MASK_TX_EMPTY      I2C_RAWISR_TX_EMPTY
#define I2C_RAW_INTR_MASK_RX_REQ        I2C_RAWISR_RX_REQ
#define I2C_RAW_INTR_MASK_TX_ABRT       I2C_RAWISR_TX_ABRT
#define I2C_RAW_INTR_MASK_RX_DONE       I2C_RAWISR_RX_DONE
#define I2C_RAW_INTR_MASK_ACTIVITY      I2C_RAWISR_ACTIV
#define I2C_RAW_INTR_MASK_STOP_DET      I2C_RAWISR_STOP
#define I2C_RAW_INTR_MASK_START_DET     I2C_RAWISR_START
#define I2C_RAW_INTR_MASK_GEN_CALL      I2C_RAWISR_GC
#define I2C_RAW_INTR_MASK_RESTART_DET   I2C_RAWISR_RESTART
#define I2C_RAW_INTR_MASK_MST_ON_HOLD   I2C_RAWISR_HOLD

#define I2C_RX_TL                       I2C_RXTLR_TL
#define I2C_TX_TL                       I2C_TXTLR_TL

#define I2C_CLR_INTR                    I2C_ICR
#define I2C_CLR_RX_UNDER                I2C_RX_UNDER
#define I2C_CLR_RX_OVER                 I2C_RX_OVER
#define I2C_CLR_TX_OVER                 I2C_TX_OVER
#define I2C_RX_REQ                      I2C_RD_REQ
#define I2C_CLR_RX_REQ                  I2C_RD_REQ
#define I2C_CLR_TX_ABRT                 I2C_TX_ABRT
#define I2C_CLR_RX_DONE                 I2C_RX_DONE
#define I2C_CLR_ACTIVITY                I2C_ACTIV
#define I2C_CLR_STOP_DET                I2C_STOP
#define I2C_CLR_START_DET               I2C_START
#define I2C_CLR_GEN_CALL                I2C_GC

#define I2C_ENABLE_ENABLE               I2C_ENR_ENABLE
#define I2C_ENABLE_ABORT                I2C_ENR_ABORT

#define I2C_STATUS_ACTIVITY             I2C_SR_ACTIV
#define I2C_STATUS_TFNF                 I2C_SR_TFNF
#define I2C_STATUS_TFE                  I2C_SR_TFE
#define I2C_STATUS_RFNE                 I2C_SR_RFNE
#define I2C_STATUS_RFF                  I2C_SR_RFF
#define I2C_STATUS_MST_ACTIVITY         I2C_SR_MST_ACTIV
#define I2C_STATUS_SLV_ACTIVITY         I2C_SR_SLV_ACTIV

#define I2C_TXFLR                       I2C_TXFLR_CNT
#define I2C_RXFLR                       I2C_RXFLR_CNT

#define I2C_SDA_TX_HOLD                 I2C_HOLD_TXCNT
#define I2C_SDA_RX_HOLD                 I2C_HOLD_RXCNT
#define I2C_SDA_SET_UP                  I2C_SETUP_CNT
#define I2C_ACK_GENERAL_CALL            I2C_GCR_GC

#define IWDG_PR_PR                      IWDG_PR_PRE
#define IWDG_PR_PR_DIV4                 IWDG_PR_PRE_DIV4
#define IWDG_PR_PR_DIV8                 IWDG_PR_PRE_DIV8
#define IWDG_PR_PR_DIV16                IWDG_PR_PRE_DIV16
#define IWDG_PR_PR_DIV32                IWDG_PR_PRE_DIV32
#define IWDG_PR_PR_DIV64                IWDG_PR_PRE_DIV64
#define IWDG_PR_PR_DIV128               IWDG_PR_PRE_DIV128
#define IWDG_PR_PR_DIV256               IWDG_PR_PRE_DIV256
////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @defgroup AHB_peripheral
/// @{
#define RCC_AHBPeriph_SRAM              RCC_AHBENR_SRAM
#define RCC_AHBPeriph_FLITF             RCC_AHBENR_FLITF
#define RCC_AHBPeriph_CRC               RCC_AHBENR_CRC



/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup APB2_peripheral
/// @{

#define RCC_APB2ENR_ADC1EN              RCC_APB2ENR_ADC1
#define RCC_APB2ENR_TIM1EN              RCC_APB2ENR_TIM1
#define RCC_APB2RSTR_ADC1RST            RCC_APB2RSTR_ADC1


/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup APB1_peripheral
/// @{
#define RCC_APB1Periph_TIM3             RCC_APB1ENR_TIM3
#define RCC_APB1Periph_UART2            RCC_APB1ENR_UART2
#define RCC_APB1Periph_UART3            RCC_APB1ENR_UART3
#define RCC_APB1Periph_I2C1             RCC_APB1ENR_I2C1
#define RCC_APB1Periph_SPI2             RCC_APB1ENR_SPI2
#define RCC_APB1Periph_WWDG             RCC_APB1ENR_WWDG
#define RCC_APB1Periph_PWR              RCC_APB1ENR_PWR

#define RCC_APB1ENR_PWREN               RCC_APB1ENR_PWR
#define RCC_APB1ENR_TIM2EN              RCC_APB1ENR_TIM2
#define RCC_APB1ENR_TIM3EN              RCC_APB1ENR_TIM3


////////////////////////////////////////////////////////////////////////////////
/// @brief Version compatibility definition
////////////////////////////////////////////////////////////////////////////////
#define SPI_TXREG_TXREG                 SPI_TDR_TXREG
#define SPI_RXREG_RXREG                 SPI_RDR_RXREG

#define SPI_CSTAT_TXEPT                 SPI_SR_TXEPT
#define SPI_CSTAT_RXAVL                 SPI_SR_RXAVL
#define SPI_CSTAT_TXFULL                SPI_SR_TXFULL
#define SPI_CSTAT_RXAVL_4BYTE           SPI_SR_RXAVL_4BYTE
#define SPI_CSTAT_TXFADDR               SPI_SR_TXFADDR
#define SPI_CSTAT_RXFADDR               SPI_SR_RXFADDR

#define SPI_INTSTAT_TX_INTF             SPI_ISR_TX_INTF
#define SPI_INTSTAT_RX_INTF             SPI_ISR_RX_INTF
#define SPI_INTSTAT_UNDERRUN_INTF       SPI_ISR_UNDERRUN_INTF
#define SPI_INTSTAT_RXOERR_INTF         SPI_ISR_RXOERR_INTF
#define SPI_INTSTAT_RXMATCH_INTF        SPI_ISR_RXMATCH_INTF
#define SPI_INTSTAT_RXFULL_INTF         SPI_ISR_RXFULL_INTF
#define SPI_INTSTAT_TXEPT_INTF          SPI_ISR_TXEPT_INTF

#define SPI_INTEN_TX_IEN                SPI_IER_TX_IEN
#define SPI_INTEN_RX_IEN                SPI_IER_RX_IEN
#define SPI_INTEN_UNDERRUN_IEN          SPI_IER_UNDERRUN_IEN
#define SPI_INTEN_RXOERR_IEN            SPI_IER_RXOERR_IEN
#define SPI_INTEN_RXMATCH_IEN           SPI_IER_RXMATCH_IEN
#define SPI_INTEN_RXFULL_IEN            SPI_IER_RXFULL_IEN
#define SPI_INTEN_TXEPT_IEN             SPI_IER_TXEPT_IEN

#define SPI_INTCLR_TX_ICLR              SPI_ICR_TX_ICLR
#define SPI_INTCLR_RX_ICLR              SPI_ICR_RX_ICLR
#define SPI_INTCLR_UNDERRUN_ICLR        SPI_ICR_UNDERRUN_ICLR
#define SPI_INTCLR_RXOERR_ICLR          SPI_ICR_RXOERR_ICLR
#define SPI_INTCLR_RXMATCH_ICLR         SPI_ICR_RXMATCH_ICLR
#define SPI_INTCLR_RXFULL_ICLR          SPI_ICR_RXFULL_ICLR
#define SPI_INTCLR_TXEPT_ICLR           SPI_ICR_TXEPT_ICLR

#define SPI_GCTL_SPIEN                  SPI_GCR_SPIEN
#define SPI_GCTL_INT_EN                 SPI_GCR_IEN
#define SPI_GCTL_MM                     SPI_GCR_MODE
#define SPI_GCTL_TXEN                   SPI_GCR_TXEN
#define SPI_GCTL_RXEN                   SPI_GCR_RXEN

#define SPI_GCTL_RXTLF                  SPI_GCR_RXTLF
#define SPI_GCTL_RXTLF_One              SPI_GCR_RXTLF_One
#define SPI_GCTL_RXTLF_Half             SPI_GCR_RXTLF_Half

#define SPI_GCTL_TXTLF_Pos              SPI_GCR_TXTLF_Pos
#define SPI_GCTL_TXTLF                  SPI_GCR_TXTLF
#define SPI_GCTL_TXTLF_One              SPI_GCR_TXTLF_One
#define SPI_GCTL_TXTLF_Half             SPI_GCR_TXTLF_Half
#define SPI_GCTL_NSS_SEL                SPI_GCR_NSS
#define SPI_GCTL_DATA_SEL               SPI_GCR_DWSEL


#define SPI_CCTL_CPHA                   SPI_CCR_CPHA
#define SPI_CCTL_CPOL                   SPI_CCR_CPOL
#define SPI_CCTL_LSBFE                  SPI_CCR_LSBFE
#define SPI_CCTL_SPILEN                 SPI_CCR_SPILEN
#define SPI_CCTL_RXEDGE                 SPI_CCR_RXEDGE
#define SPI_CCTL_TXEDGE                 SPI_CCR_TXEDGE



#define SPI_SPBRG_SPBRG                 SPI_BRR_DIVF

#define SPI_RXDNR_RXDNR                 SPI_RDNR_RDN

#define SPI_EXTCTL_EXTLEN               SPI_ECR_EXTLEN
#define TIM_CR1_ARPE                    TIM_CR1_ARPEN

#define TIM_SMCR_ECE                    TIM_SMCR_ECEN
#if !defined(TIM_DIER_UIE)
#define TIM_DIER_UIE                    TIM_DIER_UI
#define TIM_DIER_CC1IE                  TIM_DIER_CC1I
#define TIM_DIER_CC2IE                  TIM_DIER_CC2I
#define TIM_DIER_CC3IE                  TIM_DIER_CC3I
#define TIM_DIER_CC4IE                  TIM_DIER_CC4I
#define TIM_DIER_COMIE                  TIM_DIER_COMI
#define TIM_DIER_TIE                    TIM_DIER_TI
#define TIM_DIER_BIE                    TIM_DIER_BI
#define TIM_DIER_UDE                    TIM_DIER_UD
#define TIM_DIER_CC1DE                  TIM_DIER_CC1D
#define TIM_DIER_CC2DE                  TIM_DIER_CC2D
#define TIM_DIER_CC3DE                  TIM_DIER_CC3D
#define TIM_DIER_CC4DE                  TIM_DIER_CC4D
#define TIM_DIER_COMDE                  TIM_DIER_COMD
#define TIM_DIER_TDE                    TIM_DIER_TD
#endif
#if !defined(TIM_DIER_UIEN)
#define TIM_DIER_UIEN                   TIM_DIER_UI
#define TIM_DIER_CC1IEN                 TIM_DIER_CC1I
#define TIM_DIER_CC2IEN                 TIM_DIER_CC2I
#define TIM_DIER_CC3IEN                 TIM_DIER_CC3I
#define TIM_DIER_CC4IEN                 TIM_DIER_CC4I
#define TIM_DIER_COMIEN                 TIM_DIER_COMI
#define TIM_DIER_TIEN                   TIM_DIER_TI
#define TIM_DIER_BIEN                   TIM_DIER_BI
#define TIM_DIER_UDEN                   TIM_DIER_UD
#define TIM_DIER_CC1DEN                 TIM_DIER_CC1D
#define TIM_DIER_CC2DEN                 TIM_DIER_CC2D
#define TIM_DIER_CC3DEN                 TIM_DIER_CC3D
#define TIM_DIER_CC4DEN                 TIM_DIER_CC4D
#define TIM_DIER_COMDEN                 TIM_DIER_COMD
#define TIM_DIER_TDEN                   TIM_DIER_TD
#endif
#if !defined(TIM_SR_UIF)
#define TIM_SR_UIF                      TIM_SR_UI
#define TIM_SR_CC1IF                    TIM_SR_CC1I
#define TIM_SR_CC2IF                    TIM_SR_CC2I
#define TIM_SR_CC3IF                    TIM_SR_CC3I
#define TIM_SR_CC4IF                    TIM_SR_CC4I
#define TIM_SR_COMIF                    TIM_SR_COMI
#define TIM_SR_TIF                      TIM_SR_TI
#define TIM_SR_BIF                      TIM_SR_BI
#define TIM_SR_CC1OF                    TIM_SR_CC1O
#define TIM_SR_CC2OF                    TIM_SR_CC2O
#define TIM_SR_CC3OF                    TIM_SR_CC3O
#define TIM_SR_CC4OF                    TIM_SR_CC4O
#endif

#define TIM_CCMR1_OC1FE                 TIM_CCMR1_OC1FEN
#define TIM_CCMR1_OC1PE                 TIM_CCMR1_OC1PEN
#define TIM_CCMR1_OC1CE                 TIM_CCMR1_OC1CEN
#define TIM_CCMR1_OC2FE                 TIM_CCMR1_OC2FEN
#define TIM_CCMR1_OC2PE                 TIM_CCMR1_OC2PEN
#define TIM_CCMR1_OC2CE                 TIM_CCMR1_OC2CEN

#define TIM_CCMR2_OC3FE                 TIM_CCMR2_OC3FEN
#define TIM_CCMR2_OC3PE                 TIM_CCMR2_OC3PEN
#define TIM_CCMR2_OC3CE                 TIM_CCMR2_OC3CEN
#define TIM_CCMR2_OC4FE                 TIM_CCMR2_OC4FEN
#define TIM_CCMR2_OC4PE                 TIM_CCMR2_OC4PEN
#define TIM_CCMR2_OC4CE                 TIM_CCMR2_OC4CEN

#define TIM_CCER_CC1E                   TIM_CCER_CC1EN
#define TIM_CCER_CC1NE                  TIM_CCER_CC1NEN
#define TIM_CCER_CC2E                   TIM_CCER_CC2EN
#define TIM_CCER_CC2NE                  TIM_CCER_CC2NEN
#define TIM_CCER_CC3E                   TIM_CCER_CC3EN
#define TIM_CCER_CC3NE                  TIM_CCER_CC3NEN
#define TIM_CCER_CC4E                   TIM_CCER_CC4EN


#define TIM_BDTR_BKE                    TIM_BDTR_BKEN
#define TIM_BDTR_AOE                    TIM_BDTR_AOEN
#define TIM_BDTR_MOE                    TIM_BDTR_MOEN


#define UART_TDR_TXREG                  UART_TDR_DATA
#define UART_RDR_RXREG                  UART_RDR_DATA

#define UART_ISR_TX_INTF                UART_ISR_TX
#define UART_ISR_RX_INTF                UART_ISR_RX
#define UART_ISR_RXOERR_INTF            UART_ISR_RXOERR
#define UART_ISR_RXPERR_INTF            UART_ISR_RXPERR
#define UART_ISR_RXFERR_INTF            UART_ISR_RXFERR
#define UART_ISR_RXBRK_INTF             UART_ISR_RXBRK

#define UART_IER_TXIEN                  UART_IER_TX
#define UART_IER_RXIEN                  UART_IER_RXI
#define UART_IER_RXOERREN               UART_IER_RXOERR
#define UART_IER_RXPERREN               UART_IER_RXPERR
#define UART_IER_RXFERREN               UART_IER_RXFERR
#define UART_IER_RXBRKEN                UART_IER_RXBRK

#define UART_ICR_TXICLR                 UART_ICR_TX
#define UART_ICR_RXICLR                 UART_ICR_RX
#define UART_ICR_RXOERRCLR              UART_ICR_RXOERR
#define UART_ICR_RXPERRCLR              UART_ICR_RXPERR
#define UART_ICR_RXFERRCLR              UART_ICR_RXFERR
#define UART_ICR_RXBRKCLR               UART_ICR_RXBRK

#define UART_Mode_Rx                    UART_GCR_RX
#define UART_Mode_Tx                    UART_GCR_TX
#define UART_EN                         UART_GCR_UART
#define UART_IT_RXBRK                   UART_IER_RXBRK
#define UART_IT_ERR                     UART_IER_RXFERR
#define UART_IT_PE                      UART_IER_RXPERR
#define UART_OVER_ERR                   UART_IER_RXOERR
#define UART_IT_RXIEN                   UART_IER_RX
#define UART_IT_TXIEN                   UART_IER_TX
#define UART_IT_RXIDLEIEN               UART_IER_RXIDLE

#define UART_HardwareFlowControl_None   UART_HWFlowControl_None

#define UART_BRR_DIV_MANTISSA           UART_BRR_MANTISSA
#define UART_BRR_DIV_FRACTION           UART_BRR_FRACTION

#define UART_DMAReq_EN                  UART_GCR_DMA
#define UART_FLAG_TXEMPTY               UART_CSR_TXEPT
#define UART_FLAG_TXFULL                UART_CSR_TXFULL
#define UART_FLAG_RXAVL                 UART_CSR_RXAVL
#define UART_FLAG_TXEPT                 UART_CSR_TXC
#define WWDG_CR_T                       WWDG_CR_CNT



#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////
#endif//__MM32_REG_DEFINE_V1_H
////////////////////////////////////////////////////////////////////////////////
