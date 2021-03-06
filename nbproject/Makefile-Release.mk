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
CCC=g++
CXX=g++
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
	${OBJECTDIR}/src/Auth/SpecializedAuthModel.o \
	${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o \
	${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o \
	${OBJECTDIR}/src/Conf.o \
	${OBJECTDIR}/src/CsvUtil.o \
	${OBJECTDIR}/src/DashBoard.o \
	${OBJECTDIR}/src/EATableTemplate.o \
	${OBJECTDIR}/src/EchoesHome.o \
	${OBJECTDIR}/src/Enums.o \
	${OBJECTDIR}/src/InformationsWidget.o \
	${OBJECTDIR}/src/MainWidget.o \
	${OBJECTDIR}/src/MonitoringWidget.o \
	${OBJECTDIR}/src/OptionManagementModel.o \
	${OBJECTDIR}/src/OptionManagementWidget.o \
	${OBJECTDIR}/src/PluginsTableAssociationWidget.o \
	${OBJECTDIR}/src/PluginsTableFilterWidget.o \
	${OBJECTDIR}/src/PluginsTablePluginWidget.o \
	${OBJECTDIR}/src/PluginsTableSearchWidget.o \
	${OBJECTDIR}/src/PluginsTableSourceWidget.o \
	${OBJECTDIR}/src/PluginsWidget.o \
	${OBJECTDIR}/src/ProbesWidget.o \
	${OBJECTDIR}/src/RecipientsWidget.o \
	${OBJECTDIR}/src/RoleCustomizationWidget.o \
	${OBJECTDIR}/src/SummaryBoard.o \
	${OBJECTDIR}/src/TrundleTable.o \
	${OBJECTDIR}/src/UnitsWidget.o \
	${OBJECTDIR}/src/UserActionManagement.o \
	${OBJECTDIR}/src/UserEditionWidget.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DNDEBUG
CXXFLAGS=-DNDEBUG

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../dbo/dist/Release_SharedObject/GNU-Linux-x86 -L/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/default/dist/Release_SharedObject/GNU-Linux-x86 -lwt -lwtdbopostgres -ldbo -lwtdbo -lboost_signals -lboost_system -lwthttp -lboost_random

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui ${OBJECTFILES} ${LDLIBSOPTIONS} -s

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/AbstractPage.o: src/AbstractPage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AbstractPage.o src/AbstractPage.cpp

${OBJECTDIR}/src/AlertsWidget.o: src/AlertsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AlertsWidget.o src/AlertsWidget.cpp

${OBJECTDIR}/src/ApiManagement.o: src/ApiManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ApiManagement.o src/ApiManagement.cpp

${OBJECTDIR}/src/AssetManagementWidget.o: src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp

${OBJECTDIR}/src/Auth/SpecializedAuthModel.o: src/Auth/SpecializedAuthModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthModel.o src/Auth/SpecializedAuthModel.cpp

${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o: src/Auth/SpecializedAuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o src/Auth/SpecializedAuthWidget.cpp

${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o: src/Auth/SpecializedRegistrationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o src/Auth/SpecializedRegistrationWidget.cpp

${OBJECTDIR}/src/Conf.o: src/Conf.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Conf.o src/Conf.cpp

${OBJECTDIR}/src/CsvUtil.o: src/CsvUtil.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CsvUtil.o src/CsvUtil.C

${OBJECTDIR}/src/DashBoard.o: src/DashBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DashBoard.o src/DashBoard.cpp

${OBJECTDIR}/src/EATableTemplate.o: src/EATableTemplate.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EATableTemplate.o src/EATableTemplate.cpp

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/src/Enums.o: src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Enums.o src/Enums.cpp

${OBJECTDIR}/src/InformationsWidget.o: src/InformationsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/InformationsWidget.o src/InformationsWidget.cpp

${OBJECTDIR}/src/MainWidget.o: src/MainWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MainWidget.o src/MainWidget.cpp

${OBJECTDIR}/src/MonitoringWidget.o: src/MonitoringWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MonitoringWidget.o src/MonitoringWidget.cpp

${OBJECTDIR}/src/OptionManagementModel.o: src/OptionManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementModel.o src/OptionManagementModel.cpp

${OBJECTDIR}/src/OptionManagementWidget.o: src/OptionManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementWidget.o src/OptionManagementWidget.cpp

${OBJECTDIR}/src/PluginsTableAssociationWidget.o: src/PluginsTableAssociationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableAssociationWidget.o src/PluginsTableAssociationWidget.cpp

${OBJECTDIR}/src/PluginsTableFilterWidget.o: src/PluginsTableFilterWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableFilterWidget.o src/PluginsTableFilterWidget.cpp

${OBJECTDIR}/src/PluginsTablePluginWidget.o: src/PluginsTablePluginWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTablePluginWidget.o src/PluginsTablePluginWidget.cpp

${OBJECTDIR}/src/PluginsTableSearchWidget.o: src/PluginsTableSearchWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSearchWidget.o src/PluginsTableSearchWidget.cpp

${OBJECTDIR}/src/PluginsTableSourceWidget.o: src/PluginsTableSourceWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSourceWidget.o src/PluginsTableSourceWidget.cpp

${OBJECTDIR}/src/PluginsWidget.o: src/PluginsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsWidget.o src/PluginsWidget.cpp

${OBJECTDIR}/src/ProbesWidget.o: src/ProbesWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ProbesWidget.o src/ProbesWidget.cpp

${OBJECTDIR}/src/RecipientsWidget.o: src/RecipientsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RecipientsWidget.o src/RecipientsWidget.cpp

${OBJECTDIR}/src/RoleCustomizationWidget.o: src/RoleCustomizationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RoleCustomizationWidget.o src/RoleCustomizationWidget.cpp

${OBJECTDIR}/src/SummaryBoard.o: src/SummaryBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SummaryBoard.o src/SummaryBoard.cpp

${OBJECTDIR}/src/TrundleTable.o: src/TrundleTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TrundleTable.o src/TrundleTable.cpp

${OBJECTDIR}/src/UnitsWidget.o: src/UnitsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UnitsWidget.o src/UnitsWidget.cpp

${OBJECTDIR}/src/UserActionManagement.o: src/UserActionManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserActionManagement.o src/UserActionManagement.cpp

${OBJECTDIR}/src/UserEditionWidget.o: src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo-${target}/workspace/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp

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
