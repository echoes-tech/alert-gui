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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/EscapeOStream.o \
	${OBJECTDIR}/src/UserEditionWidget.o \
	${OBJECTDIR}/src/EchoesHome.o \
	${OBJECTDIR}/src/UserEditionModel.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/RegistrationWidget.o \
	${OBJECTDIR}/src/AssetManagementModel.o \
	${OBJECTDIR}/src/Login.o \
	${OBJECTDIR}/src/UpdatePasswordWidget.o \
	${OBJECTDIR}/src/AlertEditionModel.o \
	${OBJECTDIR}/src/FormBaseModel.o \
	${OBJECTDIR}/src/RegistrationModel.o \
	${OBJECTDIR}/src/Enums.o \
	${OBJECTDIR}/src/AuthWidget.o \
	${OBJECTDIR}/src/WFormModel.o \
	${OBJECTDIR}/src/DomElement.o \
	${OBJECTDIR}/src/WebUtils.o \
	${OBJECTDIR}/src/WTemplateFormView.o \
	${OBJECTDIR}/src/AssetManagementWidget.o \
	${OBJECTDIR}/src/AlertEditionWidget.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../dbo/dist/Debug_SharedObject/GNU-Linux-x86 -L/var/lib/jenkins/jobs/ea-dbo/workspace/default/dist/Debug_SharedObject/GNU-Linux-x86 -ldbo -lwt -lwthttp -lwtdbopostgres -lwtdbo -lboost_signals -lboost_system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/EscapeOStream.o: src/EscapeOStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EscapeOStream.o src/EscapeOStream.cpp

${OBJECTDIR}/src/UserEditionWidget.o: src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/src/UserEditionModel.o: src/UserEditionModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UserEditionModel.o src/UserEditionModel.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/RegistrationWidget.o: src/RegistrationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RegistrationWidget.o src/RegistrationWidget.cpp

${OBJECTDIR}/src/AssetManagementModel.o: src/AssetManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AssetManagementModel.o src/AssetManagementModel.cpp

${OBJECTDIR}/src/Login.o: src/Login.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Login.o src/Login.cpp

${OBJECTDIR}/src/UpdatePasswordWidget.o: src/UpdatePasswordWidget.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/UpdatePasswordWidget.o src/UpdatePasswordWidget.C

${OBJECTDIR}/src/AlertEditionModel.o: src/AlertEditionModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertEditionModel.o src/AlertEditionModel.cpp

${OBJECTDIR}/src/FormBaseModel.o: src/FormBaseModel.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/FormBaseModel.o src/FormBaseModel.C

${OBJECTDIR}/src/RegistrationModel.o: src/RegistrationModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RegistrationModel.o src/RegistrationModel.cpp

${OBJECTDIR}/src/Enums.o: src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Enums.o src/Enums.cpp

${OBJECTDIR}/src/AuthWidget.o: src/AuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AuthWidget.o src/AuthWidget.cpp

${OBJECTDIR}/src/WFormModel.o: src/WFormModel.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/WFormModel.o src/WFormModel.C

${OBJECTDIR}/src/DomElement.o: src/DomElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DomElement.o src/DomElement.cpp

${OBJECTDIR}/src/WebUtils.o: src/WebUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/WebUtils.o src/WebUtils.cpp

${OBJECTDIR}/src/WTemplateFormView.o: src/WTemplateFormView.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/WTemplateFormView.o src/WTemplateFormView.C

${OBJECTDIR}/src/AssetManagementWidget.o: src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp

${OBJECTDIR}/src/AlertEditionWidget.o: src/AlertEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -I/var/lib/jenkins/jobs/ea-dbo/workspace/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/AlertEditionWidget.o src/AlertEditionWidget.cpp

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
