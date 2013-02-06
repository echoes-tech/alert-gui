#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=DISTCC_FALLBACK=0 distcc
CXX=DISTCC_FALLBACK=0 distcc
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/UserEditionWidget.o \
	${OBJECTDIR}/src/OptionManagementWidget.o \
	${OBJECTDIR}/src/EchoesHome.o \
	${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o \
	${OBJECTDIR}/src/UserEditionModel.o \
	${OBJECTDIR}/src/MonitoringWidget.o \
	${OBJECTDIR}/src/Auth/RegistrationModelAlert.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/AlertListWidget.o \
	${OBJECTDIR}/src/AssetManagementModel.o \
	${OBJECTDIR}/src/OptionManagementModel.o \
	${OBJECTDIR}/src/AlertEditionModel.o \
	${OBJECTDIR}/src/UserActionManagement.o \
	${OBJECTDIR}/src/Enums.o \
	${OBJECTDIR}/src/AuthWidget.o \
	${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o \
	${OBJECTDIR}/src/AssetManagementWidget.o \
	${OBJECTDIR}/src/AlertEditionWidget.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pedantic
CXXFLAGS=-pedantic

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../dbo/dist/Debug_SharedObject/GNU-Linux-x86 -L/var/lib/jenkins/jobs/ea-dbo/workspace/default/dist/Debug_SharedObject/GNU-Linux-x86 -ldbo -lwt -lwthttp -lwtdbopostgres -lwtdbo -lboost_signals -lboost_system -lboost_random

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/UserEditionWidget.o: src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp

${OBJECTDIR}/src/OptionManagementWidget.o: src/OptionManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/OptionManagementWidget.o src/OptionManagementWidget.cpp

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o: src/Auth/LostPasswordWidgetAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o src/Auth/LostPasswordWidgetAlert.cpp

${OBJECTDIR}/src/UserEditionModel.o: src/UserEditionModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserEditionModel.o src/UserEditionModel.cpp

${OBJECTDIR}/src/MonitoringWidget.o: src/MonitoringWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MonitoringWidget.o src/MonitoringWidget.cpp

${OBJECTDIR}/src/Auth/RegistrationModelAlert.o: src/Auth/RegistrationModelAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/RegistrationModelAlert.o src/Auth/RegistrationModelAlert.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/AlertListWidget.o: src/AlertListWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertListWidget.o src/AlertListWidget.cpp

${OBJECTDIR}/src/AssetManagementModel.o: src/AssetManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AssetManagementModel.o src/AssetManagementModel.cpp

${OBJECTDIR}/src/OptionManagementModel.o: src/OptionManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/OptionManagementModel.o src/OptionManagementModel.cpp

${OBJECTDIR}/src/AlertEditionModel.o: src/AlertEditionModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertEditionModel.o src/AlertEditionModel.cpp

${OBJECTDIR}/src/UserActionManagement.o: src/UserActionManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserActionManagement.o src/UserActionManagement.cpp

${OBJECTDIR}/src/Enums.o: src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Enums.o src/Enums.cpp

${OBJECTDIR}/src/AuthWidget.o: src/AuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AuthWidget.o src/AuthWidget.cpp

${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o: src/Auth/RegistrationWidgetAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o src/Auth/RegistrationWidgetAlert.cpp

${OBJECTDIR}/src/AssetManagementWidget.o: src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp

${OBJECTDIR}/src/AlertEditionWidget.o: src/AlertEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertEditionWidget.o src/AlertEditionWidget.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
