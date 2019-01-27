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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=media/axiom-logo.c media/back-icon-highlight.c media/back-icon.c media/down-icon-highlight.c media/down-icon.c media/help-icon.c media/home-icon-highlight.c media/home-icon.c media/trash-icon.c media/up-icon-highlight.c media/up-icon.c media/wheel-illustration.c main.c glcdfont.c menu.c draw.c page.c page_wb.c page_wb_help.c globals.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/media/axiom-logo.o ${OBJECTDIR}/media/back-icon-highlight.o ${OBJECTDIR}/media/back-icon.o ${OBJECTDIR}/media/down-icon-highlight.o ${OBJECTDIR}/media/down-icon.o ${OBJECTDIR}/media/help-icon.o ${OBJECTDIR}/media/home-icon-highlight.o ${OBJECTDIR}/media/home-icon.o ${OBJECTDIR}/media/trash-icon.o ${OBJECTDIR}/media/up-icon-highlight.o ${OBJECTDIR}/media/up-icon.o ${OBJECTDIR}/media/wheel-illustration.o ${OBJECTDIR}/main.o ${OBJECTDIR}/glcdfont.o ${OBJECTDIR}/menu.o ${OBJECTDIR}/draw.o ${OBJECTDIR}/page.o ${OBJECTDIR}/page_wb.o ${OBJECTDIR}/page_wb_help.o ${OBJECTDIR}/globals.o
POSSIBLE_DEPFILES=${OBJECTDIR}/media/axiom-logo.o.d ${OBJECTDIR}/media/back-icon-highlight.o.d ${OBJECTDIR}/media/back-icon.o.d ${OBJECTDIR}/media/down-icon-highlight.o.d ${OBJECTDIR}/media/down-icon.o.d ${OBJECTDIR}/media/help-icon.o.d ${OBJECTDIR}/media/home-icon-highlight.o.d ${OBJECTDIR}/media/home-icon.o.d ${OBJECTDIR}/media/trash-icon.o.d ${OBJECTDIR}/media/up-icon-highlight.o.d ${OBJECTDIR}/media/up-icon.o.d ${OBJECTDIR}/media/wheel-illustration.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/glcdfont.o.d ${OBJECTDIR}/menu.o.d ${OBJECTDIR}/draw.o.d ${OBJECTDIR}/page.o.d ${OBJECTDIR}/page_wb.o.d ${OBJECTDIR}/page_wb_help.o.d ${OBJECTDIR}/globals.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/media/axiom-logo.o ${OBJECTDIR}/media/back-icon-highlight.o ${OBJECTDIR}/media/back-icon.o ${OBJECTDIR}/media/down-icon-highlight.o ${OBJECTDIR}/media/down-icon.o ${OBJECTDIR}/media/help-icon.o ${OBJECTDIR}/media/home-icon-highlight.o ${OBJECTDIR}/media/home-icon.o ${OBJECTDIR}/media/trash-icon.o ${OBJECTDIR}/media/up-icon-highlight.o ${OBJECTDIR}/media/up-icon.o ${OBJECTDIR}/media/wheel-illustration.o ${OBJECTDIR}/main.o ${OBJECTDIR}/glcdfont.o ${OBJECTDIR}/menu.o ${OBJECTDIR}/draw.o ${OBJECTDIR}/page.o ${OBJECTDIR}/page_wb.o ${OBJECTDIR}/page_wb_help.o ${OBJECTDIR}/globals.o

# Source Files
SOURCEFILES=media/axiom-logo.c media/back-icon-highlight.c media/back-icon.c media/down-icon-highlight.c media/down-icon.c media/help-icon.c media/home-icon-highlight.c media/home-icon.c media/trash-icon.c media/up-icon-highlight.c media/up-icon.c media/wheel-illustration.c main.c glcdfont.c menu.c draw.c page.c page_wb.c page_wb_help.c globals.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048ECG100
MP_LINKER_FILE_OPTION=
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
${OBJECTDIR}/media/axiom-logo.o: media/axiom-logo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/axiom-logo.o.d 
	@${RM} ${OBJECTDIR}/media/axiom-logo.o 
	@${FIXDEPS} "${OBJECTDIR}/media/axiom-logo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/axiom-logo.o.d" -o ${OBJECTDIR}/media/axiom-logo.o media/axiom-logo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/back-icon-highlight.o: media/back-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/back-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/back-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/back-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/back-icon-highlight.o.d" -o ${OBJECTDIR}/media/back-icon-highlight.o media/back-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/back-icon.o: media/back-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/back-icon.o.d 
	@${RM} ${OBJECTDIR}/media/back-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/back-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/back-icon.o.d" -o ${OBJECTDIR}/media/back-icon.o media/back-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/down-icon-highlight.o: media/down-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/down-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/down-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/down-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/down-icon-highlight.o.d" -o ${OBJECTDIR}/media/down-icon-highlight.o media/down-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/down-icon.o: media/down-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/down-icon.o.d 
	@${RM} ${OBJECTDIR}/media/down-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/down-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/down-icon.o.d" -o ${OBJECTDIR}/media/down-icon.o media/down-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/help-icon.o: media/help-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/help-icon.o.d 
	@${RM} ${OBJECTDIR}/media/help-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/help-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/help-icon.o.d" -o ${OBJECTDIR}/media/help-icon.o media/help-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/home-icon-highlight.o: media/home-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/home-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/home-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/home-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/home-icon-highlight.o.d" -o ${OBJECTDIR}/media/home-icon-highlight.o media/home-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/home-icon.o: media/home-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/home-icon.o.d 
	@${RM} ${OBJECTDIR}/media/home-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/home-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/home-icon.o.d" -o ${OBJECTDIR}/media/home-icon.o media/home-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/trash-icon.o: media/trash-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/trash-icon.o.d 
	@${RM} ${OBJECTDIR}/media/trash-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/trash-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/trash-icon.o.d" -o ${OBJECTDIR}/media/trash-icon.o media/trash-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/up-icon-highlight.o: media/up-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/up-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/up-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/up-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/up-icon-highlight.o.d" -o ${OBJECTDIR}/media/up-icon-highlight.o media/up-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/up-icon.o: media/up-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/up-icon.o.d 
	@${RM} ${OBJECTDIR}/media/up-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/up-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/up-icon.o.d" -o ${OBJECTDIR}/media/up-icon.o media/up-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/wheel-illustration.o: media/wheel-illustration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/wheel-illustration.o.d 
	@${RM} ${OBJECTDIR}/media/wheel-illustration.o 
	@${FIXDEPS} "${OBJECTDIR}/media/wheel-illustration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/wheel-illustration.o.d" -o ${OBJECTDIR}/media/wheel-illustration.o media/wheel-illustration.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/glcdfont.o: glcdfont.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/glcdfont.o.d 
	@${RM} ${OBJECTDIR}/glcdfont.o 
	@${FIXDEPS} "${OBJECTDIR}/glcdfont.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/glcdfont.o.d" -o ${OBJECTDIR}/glcdfont.o glcdfont.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/menu.o: menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/menu.o.d 
	@${RM} ${OBJECTDIR}/menu.o 
	@${FIXDEPS} "${OBJECTDIR}/menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/menu.o.d" -o ${OBJECTDIR}/menu.o menu.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/draw.o: draw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/draw.o.d 
	@${RM} ${OBJECTDIR}/draw.o 
	@${FIXDEPS} "${OBJECTDIR}/draw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/draw.o.d" -o ${OBJECTDIR}/draw.o draw.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page.o: page.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page.o.d 
	@${RM} ${OBJECTDIR}/page.o 
	@${FIXDEPS} "${OBJECTDIR}/page.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page.o.d" -o ${OBJECTDIR}/page.o page.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page_wb.o: page_wb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page_wb.o.d 
	@${RM} ${OBJECTDIR}/page_wb.o 
	@${FIXDEPS} "${OBJECTDIR}/page_wb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page_wb.o.d" -o ${OBJECTDIR}/page_wb.o page_wb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page_wb_help.o: page_wb_help.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page_wb_help.o.d 
	@${RM} ${OBJECTDIR}/page_wb_help.o 
	@${FIXDEPS} "${OBJECTDIR}/page_wb_help.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page_wb_help.o.d" -o ${OBJECTDIR}/page_wb_help.o page_wb_help.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/globals.o: globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/globals.o.d 
	@${RM} ${OBJECTDIR}/globals.o 
	@${FIXDEPS} "${OBJECTDIR}/globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPICkit3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/globals.o.d" -o ${OBJECTDIR}/globals.o globals.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
else
${OBJECTDIR}/media/axiom-logo.o: media/axiom-logo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/axiom-logo.o.d 
	@${RM} ${OBJECTDIR}/media/axiom-logo.o 
	@${FIXDEPS} "${OBJECTDIR}/media/axiom-logo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/axiom-logo.o.d" -o ${OBJECTDIR}/media/axiom-logo.o media/axiom-logo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/back-icon-highlight.o: media/back-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/back-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/back-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/back-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/back-icon-highlight.o.d" -o ${OBJECTDIR}/media/back-icon-highlight.o media/back-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/back-icon.o: media/back-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/back-icon.o.d 
	@${RM} ${OBJECTDIR}/media/back-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/back-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/back-icon.o.d" -o ${OBJECTDIR}/media/back-icon.o media/back-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/down-icon-highlight.o: media/down-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/down-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/down-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/down-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/down-icon-highlight.o.d" -o ${OBJECTDIR}/media/down-icon-highlight.o media/down-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/down-icon.o: media/down-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/down-icon.o.d 
	@${RM} ${OBJECTDIR}/media/down-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/down-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/down-icon.o.d" -o ${OBJECTDIR}/media/down-icon.o media/down-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/help-icon.o: media/help-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/help-icon.o.d 
	@${RM} ${OBJECTDIR}/media/help-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/help-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/help-icon.o.d" -o ${OBJECTDIR}/media/help-icon.o media/help-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/home-icon-highlight.o: media/home-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/home-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/home-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/home-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/home-icon-highlight.o.d" -o ${OBJECTDIR}/media/home-icon-highlight.o media/home-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/home-icon.o: media/home-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/home-icon.o.d 
	@${RM} ${OBJECTDIR}/media/home-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/home-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/home-icon.o.d" -o ${OBJECTDIR}/media/home-icon.o media/home-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/trash-icon.o: media/trash-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/trash-icon.o.d 
	@${RM} ${OBJECTDIR}/media/trash-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/trash-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/trash-icon.o.d" -o ${OBJECTDIR}/media/trash-icon.o media/trash-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/up-icon-highlight.o: media/up-icon-highlight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/up-icon-highlight.o.d 
	@${RM} ${OBJECTDIR}/media/up-icon-highlight.o 
	@${FIXDEPS} "${OBJECTDIR}/media/up-icon-highlight.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/up-icon-highlight.o.d" -o ${OBJECTDIR}/media/up-icon-highlight.o media/up-icon-highlight.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/up-icon.o: media/up-icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/up-icon.o.d 
	@${RM} ${OBJECTDIR}/media/up-icon.o 
	@${FIXDEPS} "${OBJECTDIR}/media/up-icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/up-icon.o.d" -o ${OBJECTDIR}/media/up-icon.o media/up-icon.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/media/wheel-illustration.o: media/wheel-illustration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/media" 
	@${RM} ${OBJECTDIR}/media/wheel-illustration.o.d 
	@${RM} ${OBJECTDIR}/media/wheel-illustration.o 
	@${FIXDEPS} "${OBJECTDIR}/media/wheel-illustration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/media/wheel-illustration.o.d" -o ${OBJECTDIR}/media/wheel-illustration.o media/wheel-illustration.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/glcdfont.o: glcdfont.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/glcdfont.o.d 
	@${RM} ${OBJECTDIR}/glcdfont.o 
	@${FIXDEPS} "${OBJECTDIR}/glcdfont.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/glcdfont.o.d" -o ${OBJECTDIR}/glcdfont.o glcdfont.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/menu.o: menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/menu.o.d 
	@${RM} ${OBJECTDIR}/menu.o 
	@${FIXDEPS} "${OBJECTDIR}/menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/menu.o.d" -o ${OBJECTDIR}/menu.o menu.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/draw.o: draw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/draw.o.d 
	@${RM} ${OBJECTDIR}/draw.o 
	@${FIXDEPS} "${OBJECTDIR}/draw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/draw.o.d" -o ${OBJECTDIR}/draw.o draw.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page.o: page.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page.o.d 
	@${RM} ${OBJECTDIR}/page.o 
	@${FIXDEPS} "${OBJECTDIR}/page.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page.o.d" -o ${OBJECTDIR}/page.o page.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page_wb.o: page_wb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page_wb.o.d 
	@${RM} ${OBJECTDIR}/page_wb.o 
	@${FIXDEPS} "${OBJECTDIR}/page_wb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page_wb.o.d" -o ${OBJECTDIR}/page_wb.o page_wb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/page_wb_help.o: page_wb_help.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/page_wb_help.o.d 
	@${RM} ${OBJECTDIR}/page_wb_help.o 
	@${FIXDEPS} "${OBJECTDIR}/page_wb_help.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/page_wb_help.o.d" -o ${OBJECTDIR}/page_wb_help.o page_wb_help.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
${OBJECTDIR}/globals.o: globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/globals.o.d 
	@${RM} ${OBJECTDIR}/globals.o 
	@${FIXDEPS} "${OBJECTDIR}/globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/globals.o.d" -o ${OBJECTDIR}/globals.o globals.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O6
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DPICkit3PlatformTool=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  -O6 $(COMPARISON_BUILD)   -mreserve=data@0x0:0x27F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=PICkit3PlatformTool=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  -O6 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AXIOM_Remote_Prototype_V01.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
