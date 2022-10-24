#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-arducam_mega.mk)" "nbproject/Makefile-local-arducam_mega.mk"
include nbproject/Makefile-local-arducam_mega.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=arducam_mega
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/delay.c ../src/config/arducam_mega/peripheral/clock/plib_clock.c ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/arducam_mega/peripheral/port/plib_port.c ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c ../src/config/arducam_mega/peripheral/tc/plib_tc0.c ../src/config/arducam_mega/stdio/xc32_monitor.c ../src/uart.c ../src/config/arducam_mega/initialization.c ../src/config/arducam_mega/interrupts.c ../src/config/arducam_mega/exceptions.c ../src/config/arducam_mega/startup_xc32.c ../src/config/arducam_mega/libc_syscalls.c ../src/spi.c ../src/main.c ../../../../../src/Arducam/ArducamCamera.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/delay.o ${OBJECTDIR}/_ext/1841970611/plib_clock.o ${OBJECTDIR}/_ext/752155653/plib_cmcc.o ${OBJECTDIR}/_ext/1844120097/plib_evsys.o ${OBJECTDIR}/_ext/752492189/plib_nvic.o ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o ${OBJECTDIR}/_ext/752545340/plib_port.o ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o ${OBJECTDIR}/_ext/1675192598/plib_tc0.o ${OBJECTDIR}/_ext/695432593/xc32_monitor.o ${OBJECTDIR}/_ext/1360937237/uart.o ${OBJECTDIR}/_ext/756862743/initialization.o ${OBJECTDIR}/_ext/756862743/interrupts.o ${OBJECTDIR}/_ext/756862743/exceptions.o ${OBJECTDIR}/_ext/756862743/startup_xc32.o ${OBJECTDIR}/_ext/756862743/libc_syscalls.o ${OBJECTDIR}/_ext/1360937237/spi.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/delay.o.d ${OBJECTDIR}/_ext/1841970611/plib_clock.o.d ${OBJECTDIR}/_ext/752155653/plib_cmcc.o.d ${OBJECTDIR}/_ext/1844120097/plib_evsys.o.d ${OBJECTDIR}/_ext/752492189/plib_nvic.o.d ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/752545340/plib_port.o.d ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o.d ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o.d ${OBJECTDIR}/_ext/1675192598/plib_tc0.o.d ${OBJECTDIR}/_ext/695432593/xc32_monitor.o.d ${OBJECTDIR}/_ext/1360937237/uart.o.d ${OBJECTDIR}/_ext/756862743/initialization.o.d ${OBJECTDIR}/_ext/756862743/interrupts.o.d ${OBJECTDIR}/_ext/756862743/exceptions.o.d ${OBJECTDIR}/_ext/756862743/startup_xc32.o.d ${OBJECTDIR}/_ext/756862743/libc_syscalls.o.d ${OBJECTDIR}/_ext/1360937237/spi.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/delay.o ${OBJECTDIR}/_ext/1841970611/plib_clock.o ${OBJECTDIR}/_ext/752155653/plib_cmcc.o ${OBJECTDIR}/_ext/1844120097/plib_evsys.o ${OBJECTDIR}/_ext/752492189/plib_nvic.o ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o ${OBJECTDIR}/_ext/752545340/plib_port.o ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o ${OBJECTDIR}/_ext/1675192598/plib_tc0.o ${OBJECTDIR}/_ext/695432593/xc32_monitor.o ${OBJECTDIR}/_ext/1360937237/uart.o ${OBJECTDIR}/_ext/756862743/initialization.o ${OBJECTDIR}/_ext/756862743/interrupts.o ${OBJECTDIR}/_ext/756862743/exceptions.o ${OBJECTDIR}/_ext/756862743/startup_xc32.o ${OBJECTDIR}/_ext/756862743/libc_syscalls.o ${OBJECTDIR}/_ext/1360937237/spi.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o

# Source Files
SOURCEFILES=../src/delay.c ../src/config/arducam_mega/peripheral/clock/plib_clock.c ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/arducam_mega/peripheral/port/plib_port.c ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c ../src/config/arducam_mega/peripheral/tc/plib_tc0.c ../src/config/arducam_mega/stdio/xc32_monitor.c ../src/uart.c ../src/config/arducam_mega/initialization.c ../src/config/arducam_mega/interrupts.c ../src/config/arducam_mega/exceptions.c ../src/config/arducam_mega/startup_xc32.c ../src/config/arducam_mega/libc_syscalls.c ../src/spi.c ../src/main.c ../../../../../src/Arducam/ArducamCamera.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-arducam_mega.mk ${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAME54P20A
MP_LINKER_FILE_OPTION=,--script="..\src\config\arducam_mega\ATSAME54P20A.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/delay.o: ../src/delay.c  .generated_files/flags/arducam_mega/aa0f27df59cfe1e4c09fb6ca7ce14f8a19ee5686 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/delay.o.d" -o ${OBJECTDIR}/_ext/1360937237/delay.o ../src/delay.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1841970611/plib_clock.o: ../src/config/arducam_mega/peripheral/clock/plib_clock.c  .generated_files/flags/arducam_mega/d3f7c1055252fc519b6466f6402b6db7ec9ae7c2 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1841970611" 
	@${RM} ${OBJECTDIR}/_ext/1841970611/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841970611/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1841970611/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1841970611/plib_clock.o ../src/config/arducam_mega/peripheral/clock/plib_clock.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752155653/plib_cmcc.o: ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/arducam_mega/a42e3a6ea3b825d143cfaf2ca17be9fe8a924b0 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752155653" 
	@${RM} ${OBJECTDIR}/_ext/752155653/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/752155653/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752155653/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/752155653/plib_cmcc.o ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1844120097/plib_evsys.o: ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c  .generated_files/flags/arducam_mega/1691135ff9a7390b70b1e64f95ce2d4aaf43238e .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1844120097" 
	@${RM} ${OBJECTDIR}/_ext/1844120097/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1844120097/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1844120097/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1844120097/plib_evsys.o ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752492189/plib_nvic.o: ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c  .generated_files/flags/arducam_mega/c900a709e23d9f9f1cb41da3c3b66045a6052df2 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752492189" 
	@${RM} ${OBJECTDIR}/_ext/752492189/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/752492189/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752492189/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/752492189/plib_nvic.o ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o: ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/arducam_mega/f6ecd66cec50254dd526809c2f3b1f8f615c9931 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/2064293877" 
	@${RM} ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752545340/plib_port.o: ../src/config/arducam_mega/peripheral/port/plib_port.c  .generated_files/flags/arducam_mega/497118bab7ed6824c7e8e826932892157d3f94f2 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752545340" 
	@${RM} ${OBJECTDIR}/_ext/752545340/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/752545340/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752545340/plib_port.o.d" -o ${OBJECTDIR}/_ext/752545340/plib_port.o ../src/config/arducam_mega/peripheral/port/plib_port.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o: ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c  .generated_files/flags/arducam_mega/3d61447b6d8307b14a5e8beebd13f8f779096383 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/400909272" 
	@${RM} ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o.d" -o ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o: ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c  .generated_files/flags/arducam_mega/125a91dc209198f5c812b05ff86c8725bfc0d642 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1731558226" 
	@${RM} ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o.d" -o ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1675192598/plib_tc0.o: ../src/config/arducam_mega/peripheral/tc/plib_tc0.c  .generated_files/flags/arducam_mega/636038fc1739da4adf41df05905c2db08fa42b94 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1675192598" 
	@${RM} ${OBJECTDIR}/_ext/1675192598/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1675192598/plib_tc0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1675192598/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/1675192598/plib_tc0.o ../src/config/arducam_mega/peripheral/tc/plib_tc0.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/695432593/xc32_monitor.o: ../src/config/arducam_mega/stdio/xc32_monitor.c  .generated_files/flags/arducam_mega/bea62cfb6525fb3204b35a1cfabfc33a607b69ab .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/695432593" 
	@${RM} ${OBJECTDIR}/_ext/695432593/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/695432593/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/695432593/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/695432593/xc32_monitor.o ../src/config/arducam_mega/stdio/xc32_monitor.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/uart.o: ../src/uart.c  .generated_files/flags/arducam_mega/466df48a00d520ebe75c0168f02897de5e8f4499 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/uart.o.d" -o ${OBJECTDIR}/_ext/1360937237/uart.o ../src/uart.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/initialization.o: ../src/config/arducam_mega/initialization.c  .generated_files/flags/arducam_mega/69370cbe6ae29364eee5772274a19eb07aefacf6 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/initialization.o.d" -o ${OBJECTDIR}/_ext/756862743/initialization.o ../src/config/arducam_mega/initialization.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/interrupts.o: ../src/config/arducam_mega/interrupts.c  .generated_files/flags/arducam_mega/653c97fa840fe4d2e733d1e8bc7f63f956f7862c .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/interrupts.o.d" -o ${OBJECTDIR}/_ext/756862743/interrupts.o ../src/config/arducam_mega/interrupts.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/exceptions.o: ../src/config/arducam_mega/exceptions.c  .generated_files/flags/arducam_mega/87ff3802ae4cff271c0657a606d2b240d91e22ea .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/exceptions.o.d" -o ${OBJECTDIR}/_ext/756862743/exceptions.o ../src/config/arducam_mega/exceptions.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/startup_xc32.o: ../src/config/arducam_mega/startup_xc32.c  .generated_files/flags/arducam_mega/fb70fc722914603ac7d67088af45e8a97897677b .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/756862743/startup_xc32.o ../src/config/arducam_mega/startup_xc32.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/libc_syscalls.o: ../src/config/arducam_mega/libc_syscalls.c  .generated_files/flags/arducam_mega/873b10c1bf1c9f30a572c48e0740e178a887d15f .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/756862743/libc_syscalls.o ../src/config/arducam_mega/libc_syscalls.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/spi.o: ../src/spi.c  .generated_files/flags/arducam_mega/a510928c848c74aca3a97d2b207e6be2561eac59 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi.o ../src/spi.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/arducam_mega/e80d338319e8ff4582f959902ad3d94de3a0fbe8 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/2122318797/ArducamCamera.o: ../../../../../src/Arducam/ArducamCamera.c  .generated_files/flags/arducam_mega/1659ea4c22eb20f7b96a4cd28824a508a9bf984f .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/2122318797" 
	@${RM} ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122318797/ArducamCamera.o.d" -o ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o ../../../../../src/Arducam/ArducamCamera.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1360937237/delay.o: ../src/delay.c  .generated_files/flags/arducam_mega/e9e209886d6f051c852df0fd7270ed4266c885fd .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/delay.o.d" -o ${OBJECTDIR}/_ext/1360937237/delay.o ../src/delay.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1841970611/plib_clock.o: ../src/config/arducam_mega/peripheral/clock/plib_clock.c  .generated_files/flags/arducam_mega/733b52f05d8564ca2a9547bd3a4a96ec23cc2191 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1841970611" 
	@${RM} ${OBJECTDIR}/_ext/1841970611/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841970611/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1841970611/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1841970611/plib_clock.o ../src/config/arducam_mega/peripheral/clock/plib_clock.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752155653/plib_cmcc.o: ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/arducam_mega/a8c94b465185b3b37ba554f042e837b7715be6ff .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752155653" 
	@${RM} ${OBJECTDIR}/_ext/752155653/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/752155653/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752155653/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/752155653/plib_cmcc.o ../src/config/arducam_mega/peripheral/cmcc/plib_cmcc.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1844120097/plib_evsys.o: ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c  .generated_files/flags/arducam_mega/84dcb1e580b2f5d5fb492388b61116d77af05e2a .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1844120097" 
	@${RM} ${OBJECTDIR}/_ext/1844120097/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1844120097/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1844120097/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1844120097/plib_evsys.o ../src/config/arducam_mega/peripheral/evsys/plib_evsys.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752492189/plib_nvic.o: ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c  .generated_files/flags/arducam_mega/7e0cf871f57e4c6f93e62f784ff3ed92fde038be .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752492189" 
	@${RM} ${OBJECTDIR}/_ext/752492189/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/752492189/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752492189/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/752492189/plib_nvic.o ../src/config/arducam_mega/peripheral/nvic/plib_nvic.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o: ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/arducam_mega/ba576039a1c6685d90b26700398eec2562c75b59 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/2064293877" 
	@${RM} ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/2064293877/plib_nvmctrl.o ../src/config/arducam_mega/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/752545340/plib_port.o: ../src/config/arducam_mega/peripheral/port/plib_port.c  .generated_files/flags/arducam_mega/5787fe7fb0664a1ef073c3ef0dd6dc1f05e699f5 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/752545340" 
	@${RM} ${OBJECTDIR}/_ext/752545340/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/752545340/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/752545340/plib_port.o.d" -o ${OBJECTDIR}/_ext/752545340/plib_port.o ../src/config/arducam_mega/peripheral/port/plib_port.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o: ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c  .generated_files/flags/arducam_mega/da5c09a97d89679521a7a15554a67d21fdd01276 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/400909272" 
	@${RM} ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o.d" -o ${OBJECTDIR}/_ext/400909272/plib_sercom6_spi_master.o ../src/config/arducam_mega/peripheral/sercom/spi_master/plib_sercom6_spi_master.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o: ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c  .generated_files/flags/arducam_mega/e2bec429bfaeb609729da32eb52a846be00a14da .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1731558226" 
	@${RM} ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o.d" -o ${OBJECTDIR}/_ext/1731558226/plib_sercom5_usart.o ../src/config/arducam_mega/peripheral/sercom/usart/plib_sercom5_usart.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1675192598/plib_tc0.o: ../src/config/arducam_mega/peripheral/tc/plib_tc0.c  .generated_files/flags/arducam_mega/3f37f57db325314d72c799029e615e7b02de8fcd .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1675192598" 
	@${RM} ${OBJECTDIR}/_ext/1675192598/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1675192598/plib_tc0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1675192598/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/1675192598/plib_tc0.o ../src/config/arducam_mega/peripheral/tc/plib_tc0.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/695432593/xc32_monitor.o: ../src/config/arducam_mega/stdio/xc32_monitor.c  .generated_files/flags/arducam_mega/eaad9f7186a03a451de98589fbe631743784d6d8 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/695432593" 
	@${RM} ${OBJECTDIR}/_ext/695432593/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/695432593/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/695432593/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/695432593/xc32_monitor.o ../src/config/arducam_mega/stdio/xc32_monitor.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/uart.o: ../src/uart.c  .generated_files/flags/arducam_mega/7ec1de017c9c9e45bcdc0896299f753e8fa121e4 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/uart.o.d" -o ${OBJECTDIR}/_ext/1360937237/uart.o ../src/uart.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/initialization.o: ../src/config/arducam_mega/initialization.c  .generated_files/flags/arducam_mega/54f00e0d58fe4991296c2654e0b0a1add0c35341 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/initialization.o.d" -o ${OBJECTDIR}/_ext/756862743/initialization.o ../src/config/arducam_mega/initialization.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/interrupts.o: ../src/config/arducam_mega/interrupts.c  .generated_files/flags/arducam_mega/614f0ea52bf10f3488c632ce795c515ad157d8f0 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/interrupts.o.d" -o ${OBJECTDIR}/_ext/756862743/interrupts.o ../src/config/arducam_mega/interrupts.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/exceptions.o: ../src/config/arducam_mega/exceptions.c  .generated_files/flags/arducam_mega/401b568f9748bc9f3c6fba103acf76cead7bd8f9 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/exceptions.o.d" -o ${OBJECTDIR}/_ext/756862743/exceptions.o ../src/config/arducam_mega/exceptions.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/startup_xc32.o: ../src/config/arducam_mega/startup_xc32.c  .generated_files/flags/arducam_mega/8c1db02704c879c05aa016f7377bedf031223627 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/756862743/startup_xc32.o ../src/config/arducam_mega/startup_xc32.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/756862743/libc_syscalls.o: ../src/config/arducam_mega/libc_syscalls.c  .generated_files/flags/arducam_mega/d89c3b956485688d25500d429c54f3d47b25e801 .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/756862743" 
	@${RM} ${OBJECTDIR}/_ext/756862743/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/756862743/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/756862743/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/756862743/libc_syscalls.o ../src/config/arducam_mega/libc_syscalls.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/spi.o: ../src/spi.c  .generated_files/flags/arducam_mega/6a2ea2dc1d5ac55b179e68ca92d7cf3749b8e4ad .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi.o ../src/spi.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/arducam_mega/b9debe97c4edffa265da86b86b9eac6c7e434b5d .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/2122318797/ArducamCamera.o: ../../../../../src/Arducam/ArducamCamera.c  .generated_files/flags/arducam_mega/15e971a76d30cf94785a8f6e550a29cdfe84657a .generated_files/flags/arducam_mega/2cad12305e87ddf087d890fb067505b8706c0e8a
	@${MKDIR} "${OBJECTDIR}/_ext/2122318797" 
	@${RM} ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/arducam_mega" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122318797/ArducamCamera.o.d" -o ${OBJECTDIR}/_ext/2122318797/ArducamCamera.o ../../../../../src/Arducam/ArducamCamera.c    -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/arducam_mega/ATSAME54P20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o ${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/arducam_mega/ATSAME54P20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o ${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_arducam_mega=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/same54_xpro_arducam_mega.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
