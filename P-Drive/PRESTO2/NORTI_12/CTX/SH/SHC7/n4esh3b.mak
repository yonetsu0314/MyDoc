# Create n4esh3b.lib with Renesas C

### PATH ###

SRC = ..\..\..\SRC
INC = ..\..\..\INC

### FILES ###

LIBFILE  = n4esh3b.lib
SRCFILE  = $(SRC)\n4csh3h.asm $(SRC)\noknl4.c
INCFILE  = $(INC)\kernel.h $(INC)\nosys4.h $(INC)\n4rsh.h
MAKEFILE = n4esh3b.mak
CMDFILE  = n4esh3b.sub

### COMMAND ###

CC       = shc
CFLAGS   = -cp=sh3 -op=1 -sp -gb=u -i=$(INC) -nologo
AS       = asmsh
AFLAGS   = -cp=sh3 -def=HOOK=2 -asa=HVER=H'704
LIB      = optlnk
LIBFLAGS = -subcommand=$(CMDFILE)

$(LIBFILE) : $(SRCFILE) $(INCFILE) $(MAKEFILE) $(CMDFILE)
    $(AS) $(SRC)\n4csh3h.asm -o=n4csh3h.obj $(AFLAGS)
    $(CC) $(CFLAGS) -def=HOOK=2,SH3             -ob=n4hook.obj      $(SRC)\n4hook.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,OS_CORE     -ob=os_core.obj     $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_TSK  -ob=V4_CRE_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_TSK  -ob=V3_CRE_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CAN_TSK  -ob=V4_CAN_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_TSK  -ob=V4_DEL_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_ACT_TSK  -ob=V4_ACT_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STA_TSK  -ob=V4_STA_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_EXT_TSK  -ob=V4_EXT_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_EXD_TSK  -ob=V4_EXD_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TER_TSK  -ob=V4_TER_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DIS_DSP  -ob=V4_DIS_DSP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_DIS_DSP  -ob=V3_DIS_DSP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_ENA_DSP  -ob=V4_ENA_DSP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_ENA_DSP  -ob=V3_ENA_DSP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CHG_PRI  -ob=V4_CHG_PRI.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_GET_PRI  -ob=V4_GET_PRI.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_ROT_RDQ  -ob=V4_ROT_RDQ.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REL_WAI  -ob=V4_REL_WAI.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_GET_TID  -ob=V4_GET_TID.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_VGET_TID -ob=V3_VGET_TID.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_TSK  -ob=V4_REF_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_TSK  -ob=V3_REF_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_TST  -ob=V4_REF_TST.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SUS_TSK  -ob=V4_SUS_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_RSM_TSK  -ob=V4_RSM_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TSLP_TSK -ob=V4_TSLP_TSK.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_WUP_TSK  -ob=V4_WUP_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CAN_WUP  -ob=V4_CAN_WUP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CAN_WUP  -ob=V3_CAN_WUP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_VCAN_WUP -ob=V3_VCAN_WUP.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_SEM  -ob=V4_CRE_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_SEM  -ob=V3_CRE_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_SEM  -ob=V4_DEL_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SIG_SEM  -ob=V4_SIG_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TWAI_SEM -ob=V4_TWAI_SEM.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_SEM  -ob=V4_REF_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_SEM  -ob=V3_REF_SEM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_FLG  -ob=V3_CRE_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_FLG  -ob=V4_CRE_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_FLG  -ob=V4_DEL_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SET_FLG  -ob=V4_SET_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CLR_FLG  -ob=V4_CLR_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TWAI_FLG -ob=V4_TWAI_FLG.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_FLG  -ob=V4_REF_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_FLG  -ob=V3_REF_FLG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_MBX  -ob=V4_CRE_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_MBX  -ob=V3_CRE_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_MBX  -ob=V4_DEL_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SND_MBX  -ob=V4_SND_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TRCV_MBX -ob=V4_TRCV_MBX.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_MBX  -ob=V4_REF_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_MBX  -ob=V3_REF_MBX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_MBF  -ob=V4_CRE_MBF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_MBF  -ob=V3_CRE_MBF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_MBF  -ob=V4_DEL_MBF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TSND_MBF -ob=V4_TSND_MBF.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TRCV_MBF -ob=V4_TRCV_MBF.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_TRCV_MBF -ob=V3_TRCV_MBF.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_MBF  -ob=V4_REF_MBF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_MBF  -ob=V3_REF_MBF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_POR  -ob=V4_CRE_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_POR  -ob=V3_CRE_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_POR  -ob=V4_DEL_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TCAL_POR -ob=V4_TCAL_POR.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_TCAL_POR -ob=V3_TCAL_POR.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TACP_POR -ob=V4_TACP_POR.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_TACP_POR -ob=V3_TACP_POR.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_FWD_POR  -ob=V4_FWD_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_RPL_RDV  -ob=V4_RPL_RDV.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_POR  -ob=V4_REF_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_POR  -ob=V3_REF_POR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_RDV  -ob=V4_REF_RDV.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_LOC_CPU  -ob=V4_LOC_CPU.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_UNL_CPU  -ob=V4_UNL_CPU.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_LOC_CPU  -ob=V3_LOC_CPU.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_UNL_CPU  -ob=V3_UNL_CPU.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SNS_CTX  -ob=V4_SNS_CTX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SNS_LOC  -ob=V4_SNS_LOC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SNS_DSP  -ob=V4_SNS_DSP.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SNS_DPN  -ob=V4_SNS_DPN.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CHG_IMS  -ob=V3_CHG_IMS.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_IMS  -ob=V3_REF_IMS.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_MPL  -ob=V4_CRE_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_MPL  -ob=V3_CRE_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_MPL  -ob=V4_DEL_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TGET_MPL -ob=V4_TGET_MPL.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REL_MPL  -ob=V4_REL_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_MPL  -ob=V4_REF_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_MPL  -ob=V3_REF_MPL.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_MPF  -ob=V4_CRE_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_MPF  -ob=V4_DEL_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TGET_MPF -ob=V4_TGET_MPF.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REL_MPF  -ob=V4_REL_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_MPF  -ob=V4_REF_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_CRE_MPF  -ob=V3_CRE_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_MPF  -ob=V3_REF_MPF.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SET_TIM  -ob=V4_SET_TIM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_GET_TIM  -ob=V4_GET_TIM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_SET_TIM  -ob=V3_SET_TIM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_GET_TIM  -ob=V3_GET_TIM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DLY_TSK  -ob=V4_DLY_TSK.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_DEF_CYC  -ob=V3_DEF_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_ACT_CYC  -ob=V3_ACT_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_CYC  -ob=V3_REF_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_CYC  -ob=V4_CRE_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_CYC  -ob=V4_DEL_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STA_CYC  -ob=V4_STA_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STP_CYC  -ob=V4_STP_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_CYC  -ob=V4_REF_CYC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_ALM  -ob=V4_CRE_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_ALM  -ob=V4_DEL_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STA_ALM  -ob=V4_STA_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STP_ALM  -ob=V4_STP_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_ALM  -ob=V4_REF_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_DEF_ALM  -ob=V3_DEF_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_ALM  -ob=V3_REF_ALM.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEF_OVR  -ob=V4_DEF_OVR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STA_OVR  -ob=V4_STA_OVR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_STP_OVR  -ob=V4_STP_OVR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_OVR  -ob=V4_REF_OVR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_ISR  -ob=V4_CRE_ISR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_ISR  -ob=V4_DEL_ISR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_ISR  -ob=V4_REF_ISR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_RET_TMR  -ob=V3_RET_TMR.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_SYS  -ob=V4_REF_SYS.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_CFG  -ob=V4_REF_CFG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_REF_CFG  -ob=V3_REF_CFG.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEF_TEX  -ob=V4_DEF_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_RAS_TEX,ASM_EXT_EXP  -ob=V4_RAS_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DIS_TEX  -ob=V4_DIS_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_ENA_TEX  -ob=V4_ENA_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SNS_TEX  -ob=V4_SNS_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_TEX  -ob=V4_REF_TEX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_DTQ  -ob=V4_CRE_DTQ.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_DTQ  -ob=V4_DEL_DTQ.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_SND_DTQ  -ob=V4_SND_DTQ.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_FSND_DTQ -ob=V4_FSND_DTQ.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TRCV_DTQ -ob=V4_TRCV_DTQ.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_DTQ  -ob=V4_REF_DTQ.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CRE_MTX  -ob=V4_CRE_MTX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_TLOC_MTX -ob=V4_TLOC_MTX.obj $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_UNL_MTX  -ob=V4_UNL_MTX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEL_MTX  -ob=V4_DEL_MTX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_MTX  -ob=V4_REF_MTX.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_MUTEX    -ob=V4_MUTEX.obj    $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_REF_VER  -ob=V4_REF_VER.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V3_GET_VER  -ob=V3_GET_VER.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_DEF_SVC  -ob=V4_DEF_SVC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,V4_CAL_SVC  -ob=V4_CAL_SVC.obj  $(SRC)\noknl4.c
    $(CC) $(CFLAGS) -def=HOOK=2,SH3,SIG_OVR     -ob=SIG_OVR.obj     $(SRC)\noknl4.c
    $(LIB) $(LIBFLAGS)
    del *.obj

# end
