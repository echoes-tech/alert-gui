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
CCC=distcc
CXX=distcc
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/include/WebUtils.o \
	${OBJECTDIR}/include/UpdatePasswordWidget.o \
	${OBJECTDIR}/src/EchoesHome.o \
	${OBJECTDIR}/include/Login.o \
	${OBJECTDIR}/include/FormBaseModel.o \
	${OBJECTDIR}/include/WFormModel.o \
	${OBJECTDIR}/include/WTemplateFormView.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/include/EscapeOStream.o \
	${OBJECTDIR}/include/AuthWidget.o \
	${OBJECTDIR}/include/DomElement.o \
	${OBJECTDIR}/include/RegistrationWidget.o \
	${OBJECTDIR}/include/RegistrationModel.o


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
LDLIBSOPTIONS=-L../dbo/dist/SharedObject/GNU-Linux-x86 -lwt -lwthttp -lwtdbopostgres -ldbo -lwtdbo -lboost_signals -lboost_system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gui ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/include/WebUtils.o: include/WebUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/WebUtils.o include/WebUtils.cpp

${OBJECTDIR}/include/UpdatePasswordWidget.o: include/UpdatePasswordWidget.C 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/UpdatePasswordWidget.o include/UpdatePasswordWidget.C

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/include/Login.o: include/Login.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/Login.o include/Login.cpp

${OBJECTDIR}/include/FormBaseModel.o: include/FormBaseModel.C 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/FormBaseModel.o include/FormBaseModel.C

${OBJECTDIR}/include/WFormModel.o: include/WFormModel.C 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/WFormModel.o include/WFormModel.C

${OBJECTDIR}/include/WTemplateFormView.o: include/WTemplateFormView.C 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/WTemplateFormView.o include/WTemplateFormView.C

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/include/EscapeOStream.o: include/EscapeOStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/EscapeOStream.o include/EscapeOStream.cpp

${OBJECTDIR}/include/AuthWidget.o: include/AuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/AuthWidget.o include/AuthWidget.cpp

${OBJECTDIR}/include/DomElement.o: include/DomElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/DomElement.o include/DomElement.cpp

${OBJECTDIR}/include/RegistrationWidget.o: include/RegistrationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/RegistrationWidget.o include/RegistrationWidget.cpp

${OBJECTDIR}/include/RegistrationModel.o: include/RegistrationModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/include
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -Iinclude -I../dbo/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/include/RegistrationModel.o include/RegistrationModel.cpp

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
