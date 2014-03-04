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
CCC=g++-4.4
CXX=g++-4.4
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/AbstractPage.o \
	${OBJECTDIR}/src/AlertsWidget.o \
	${OBJECTDIR}/src/ApiManagement.o \
	${OBJECTDIR}/src/AssetManagementWidget.o \
	${OBJECTDIR}/src/Association.o \
	${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o \
	${OBJECTDIR}/src/Auth/RegistrationModelAlert.o \
	${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o \
	${OBJECTDIR}/src/AuthWidget.o \
	${OBJECTDIR}/src/Conf.o \
	${OBJECTDIR}/src/EchoesHome.o \
	${OBJECTDIR}/src/Enums.o \
	${OBJECTDIR}/src/InformationsWidget.o \
	${OBJECTDIR}/src/MainWidget.o \
	${OBJECTDIR}/src/MonitoringWidget.o \
	${OBJECTDIR}/src/OptionManagementModel.o \
	${OBJECTDIR}/src/OptionManagementWidget.o \
	${OBJECTDIR}/src/PluginEditionModel.o \
	${OBJECTDIR}/src/PluginEditionWidget.o \
	${OBJECTDIR}/src/RecipientsWidget.o \
	${OBJECTDIR}/src/RoleCustomizationWidget.o \
	${OBJECTDIR}/src/SummaryBoard.o \
	${OBJECTDIR}/src/UserActionManagement.o \
	${OBJECTDIR}/src/UserEditionWidget.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DNDEBUG -std=c++0x
CXXFLAGS=-DNDEBUG -std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../dbo/dist/Release_SharedObject/GNU-Linux-x86 -L/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/default/dist/Release_SharedObject/GNU-Linux-x86 -lwt -lwtdbopostgres -ldbo -lwtdbo -lboost_signals -lboost_system -lwtfcgi -lboost_random

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++-4.4 -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui ${OBJECTFILES} ${LDLIBSOPTIONS} -s

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/AbstractPage.o: src/AbstractPage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AbstractPage.o src/AbstractPage.cpp

${OBJECTDIR}/src/AlertsWidget.o: src/AlertsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertsWidget.o src/AlertsWidget.cpp

${OBJECTDIR}/src/ApiManagement.o: src/ApiManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ApiManagement.o src/ApiManagement.cpp

${OBJECTDIR}/src/AssetManagementWidget.o: src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp

${OBJECTDIR}/src/Association.o: src/Association.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Association.o src/Association.cpp

${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o: src/Auth/LostPasswordWidgetAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/LostPasswordWidgetAlert.o src/Auth/LostPasswordWidgetAlert.cpp

${OBJECTDIR}/src/Auth/RegistrationModelAlert.o: src/Auth/RegistrationModelAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/RegistrationModelAlert.o src/Auth/RegistrationModelAlert.cpp

${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o: src/Auth/RegistrationWidgetAlert.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Auth/RegistrationWidgetAlert.o src/Auth/RegistrationWidgetAlert.cpp

${OBJECTDIR}/src/AuthWidget.o: src/AuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AuthWidget.o src/AuthWidget.cpp

${OBJECTDIR}/src/Conf.o: src/Conf.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Conf.o src/Conf.cpp

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/src/Enums.o: src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Enums.o src/Enums.cpp

${OBJECTDIR}/src/InformationsWidget.o: src/InformationsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/InformationsWidget.o src/InformationsWidget.cpp

${OBJECTDIR}/src/MainWidget.o: src/MainWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MainWidget.o src/MainWidget.cpp

${OBJECTDIR}/src/MonitoringWidget.o: src/MonitoringWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/MonitoringWidget.o src/MonitoringWidget.cpp

${OBJECTDIR}/src/OptionManagementModel.o: src/OptionManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/OptionManagementModel.o src/OptionManagementModel.cpp

${OBJECTDIR}/src/OptionManagementWidget.o: src/OptionManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/OptionManagementWidget.o src/OptionManagementWidget.cpp

${OBJECTDIR}/src/PluginEditionModel.o: src/PluginEditionModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/PluginEditionModel.o src/PluginEditionModel.cpp

${OBJECTDIR}/src/PluginEditionWidget.o: src/PluginEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/PluginEditionWidget.o src/PluginEditionWidget.cpp

${OBJECTDIR}/src/RecipientsWidget.o: src/RecipientsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RecipientsWidget.o src/RecipientsWidget.cpp

${OBJECTDIR}/src/RoleCustomizationWidget.o: src/RoleCustomizationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RoleCustomizationWidget.o src/RoleCustomizationWidget.cpp

${OBJECTDIR}/src/SummaryBoard.o: src/SummaryBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SummaryBoard.o src/SummaryBoard.cpp

${OBJECTDIR}/src/UserActionManagement.o: src/UserActionManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserActionManagement.o src/UserActionManagement.cpp

${OBJECTDIR}/src/UserEditionWidget.o: src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -DNDEBUG -std=c++0x -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp

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
