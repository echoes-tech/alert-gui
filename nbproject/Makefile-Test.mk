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
CCC=g++-4.7
CXX=g++-4.7
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Test
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
	${OBJECTDIR}/src/HttpRequest.o \
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

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2

# C Compiler Flags
CFLAGS=`cppunit-config --cflags` 

# CC Compiler Flags
CCFLAGS=`cppunit-config --cflags` 
CXXFLAGS=`cppunit-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`cppunit-config --libs` `cppunit-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${TESTDIR}/TestFiles/f1

${TESTDIR}/TestFiles/f1: ${OBJECTFILES}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/AbstractPage.o: src/AbstractPage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AbstractPage.o src/AbstractPage.cpp

${OBJECTDIR}/src/AlertsWidget.o: src/AlertsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AlertsWidget.o src/AlertsWidget.cpp

${OBJECTDIR}/src/ApiManagement.o: src/ApiManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ApiManagement.o src/ApiManagement.cpp

${OBJECTDIR}/src/AssetManagementWidget.o: src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp

${OBJECTDIR}/src/Auth/SpecializedAuthModel.o: src/Auth/SpecializedAuthModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthModel.o src/Auth/SpecializedAuthModel.cpp

${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o: src/Auth/SpecializedAuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o src/Auth/SpecializedAuthWidget.cpp

${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o: src/Auth/SpecializedRegistrationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o src/Auth/SpecializedRegistrationWidget.cpp

${OBJECTDIR}/src/Conf.o: src/Conf.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Conf.o src/Conf.cpp

${OBJECTDIR}/src/CsvUtil.o: src/CsvUtil.C 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CsvUtil.o src/CsvUtil.C

${OBJECTDIR}/src/DashBoard.o: src/DashBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DashBoard.o src/DashBoard.cpp

${OBJECTDIR}/src/EATableTemplate.o: src/EATableTemplate.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EATableTemplate.o src/EATableTemplate.cpp

${OBJECTDIR}/src/EchoesHome.o: src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp

${OBJECTDIR}/src/Enums.o: src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Enums.o src/Enums.cpp

${OBJECTDIR}/src/HttpRequest.o: src/HttpRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/HttpRequest.o src/HttpRequest.cpp

${OBJECTDIR}/src/InformationsWidget.o: src/InformationsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/InformationsWidget.o src/InformationsWidget.cpp

${OBJECTDIR}/src/MainWidget.o: src/MainWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MainWidget.o src/MainWidget.cpp

${OBJECTDIR}/src/MonitoringWidget.o: src/MonitoringWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MonitoringWidget.o src/MonitoringWidget.cpp

${OBJECTDIR}/src/OptionManagementModel.o: src/OptionManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementModel.o src/OptionManagementModel.cpp

${OBJECTDIR}/src/OptionManagementWidget.o: src/OptionManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementWidget.o src/OptionManagementWidget.cpp

${OBJECTDIR}/src/PluginsTableAssociationWidget.o: src/PluginsTableAssociationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableAssociationWidget.o src/PluginsTableAssociationWidget.cpp

${OBJECTDIR}/src/PluginsTableFilterWidget.o: src/PluginsTableFilterWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableFilterWidget.o src/PluginsTableFilterWidget.cpp

${OBJECTDIR}/src/PluginsTablePluginWidget.o: src/PluginsTablePluginWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTablePluginWidget.o src/PluginsTablePluginWidget.cpp

${OBJECTDIR}/src/PluginsTableSearchWidget.o: src/PluginsTableSearchWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSearchWidget.o src/PluginsTableSearchWidget.cpp

${OBJECTDIR}/src/PluginsTableSourceWidget.o: src/PluginsTableSourceWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSourceWidget.o src/PluginsTableSourceWidget.cpp

${OBJECTDIR}/src/PluginsWidget.o: src/PluginsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsWidget.o src/PluginsWidget.cpp

${OBJECTDIR}/src/ProbesWidget.o: src/ProbesWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ProbesWidget.o src/ProbesWidget.cpp

${OBJECTDIR}/src/RecipientsWidget.o: src/RecipientsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RecipientsWidget.o src/RecipientsWidget.cpp

${OBJECTDIR}/src/RoleCustomizationWidget.o: src/RoleCustomizationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RoleCustomizationWidget.o src/RoleCustomizationWidget.cpp

${OBJECTDIR}/src/SummaryBoard.o: src/SummaryBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SummaryBoard.o src/SummaryBoard.cpp

${OBJECTDIR}/src/TrundleTable.o: src/TrundleTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TrundleTable.o src/TrundleTable.cpp

${OBJECTDIR}/src/UnitsWidget.o: src/UnitsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UnitsWidget.o src/UnitsWidget.cpp

${OBJECTDIR}/src/UserActionManagement.o: src/UserActionManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserActionManagement.o src/UserActionManagement.cpp

${OBJECTDIR}/src/UserEditionWidget.o: src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/HttpRequestTest.o ${TESTDIR}/tests/HttpRequestTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/HttpRequestTest.o: tests/HttpRequestTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/HttpRequestTest.o tests/HttpRequestTest.cpp


${TESTDIR}/tests/HttpRequestTestRunner.o: tests/HttpRequestTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/HttpRequestTestRunner.o tests/HttpRequestTestRunner.cpp


${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/src/AbstractPage_nomain.o: ${OBJECTDIR}/src/AbstractPage.o src/AbstractPage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/AbstractPage.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AbstractPage_nomain.o src/AbstractPage.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/AbstractPage.o ${OBJECTDIR}/src/AbstractPage_nomain.o;\
	fi

${OBJECTDIR}/src/AlertsWidget_nomain.o: ${OBJECTDIR}/src/AlertsWidget.o src/AlertsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/AlertsWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AlertsWidget_nomain.o src/AlertsWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/AlertsWidget.o ${OBJECTDIR}/src/AlertsWidget_nomain.o;\
	fi

${OBJECTDIR}/src/ApiManagement_nomain.o: ${OBJECTDIR}/src/ApiManagement.o src/ApiManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/ApiManagement.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ApiManagement_nomain.o src/ApiManagement.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/ApiManagement.o ${OBJECTDIR}/src/ApiManagement_nomain.o;\
	fi

${OBJECTDIR}/src/AssetManagementWidget_nomain.o: ${OBJECTDIR}/src/AssetManagementWidget.o src/AssetManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/AssetManagementWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AssetManagementWidget_nomain.o src/AssetManagementWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/AssetManagementWidget.o ${OBJECTDIR}/src/AssetManagementWidget_nomain.o;\
	fi

${OBJECTDIR}/src/Auth/SpecializedAuthModel_nomain.o: ${OBJECTDIR}/src/Auth/SpecializedAuthModel.o src/Auth/SpecializedAuthModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Auth/SpecializedAuthModel.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthModel_nomain.o src/Auth/SpecializedAuthModel.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Auth/SpecializedAuthModel.o ${OBJECTDIR}/src/Auth/SpecializedAuthModel_nomain.o;\
	fi

${OBJECTDIR}/src/Auth/SpecializedAuthWidget_nomain.o: ${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o src/Auth/SpecializedAuthWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedAuthWidget_nomain.o src/Auth/SpecializedAuthWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Auth/SpecializedAuthWidget.o ${OBJECTDIR}/src/Auth/SpecializedAuthWidget_nomain.o;\
	fi

${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget_nomain.o: ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o src/Auth/SpecializedRegistrationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Auth
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget_nomain.o src/Auth/SpecializedRegistrationWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget.o ${OBJECTDIR}/src/Auth/SpecializedRegistrationWidget_nomain.o;\
	fi

${OBJECTDIR}/src/Conf_nomain.o: ${OBJECTDIR}/src/Conf.o src/Conf.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Conf.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Conf_nomain.o src/Conf.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Conf.o ${OBJECTDIR}/src/Conf_nomain.o;\
	fi

${OBJECTDIR}/src/CsvUtil_nomain.o: ${OBJECTDIR}/src/CsvUtil.o src/CsvUtil.C 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CsvUtil.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CsvUtil_nomain.o src/CsvUtil.C;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CsvUtil.o ${OBJECTDIR}/src/CsvUtil_nomain.o;\
	fi

${OBJECTDIR}/src/DashBoard_nomain.o: ${OBJECTDIR}/src/DashBoard.o src/DashBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/DashBoard.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DashBoard_nomain.o src/DashBoard.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/DashBoard.o ${OBJECTDIR}/src/DashBoard_nomain.o;\
	fi

${OBJECTDIR}/src/EATableTemplate_nomain.o: ${OBJECTDIR}/src/EATableTemplate.o src/EATableTemplate.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/EATableTemplate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EATableTemplate_nomain.o src/EATableTemplate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/EATableTemplate.o ${OBJECTDIR}/src/EATableTemplate_nomain.o;\
	fi

${OBJECTDIR}/src/EchoesHome_nomain.o: ${OBJECTDIR}/src/EchoesHome.o src/EchoesHome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/EchoesHome.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EchoesHome_nomain.o src/EchoesHome.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/EchoesHome.o ${OBJECTDIR}/src/EchoesHome_nomain.o;\
	fi

${OBJECTDIR}/src/Enums_nomain.o: ${OBJECTDIR}/src/Enums.o src/Enums.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Enums.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Enums_nomain.o src/Enums.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Enums.o ${OBJECTDIR}/src/Enums_nomain.o;\
	fi

${OBJECTDIR}/src/HttpRequest_nomain.o: ${OBJECTDIR}/src/HttpRequest.o src/HttpRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/HttpRequest.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/HttpRequest_nomain.o src/HttpRequest.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/HttpRequest.o ${OBJECTDIR}/src/HttpRequest_nomain.o;\
	fi

${OBJECTDIR}/src/InformationsWidget_nomain.o: ${OBJECTDIR}/src/InformationsWidget.o src/InformationsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/InformationsWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/InformationsWidget_nomain.o src/InformationsWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/InformationsWidget.o ${OBJECTDIR}/src/InformationsWidget_nomain.o;\
	fi

${OBJECTDIR}/src/MainWidget_nomain.o: ${OBJECTDIR}/src/MainWidget.o src/MainWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/MainWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MainWidget_nomain.o src/MainWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/MainWidget.o ${OBJECTDIR}/src/MainWidget_nomain.o;\
	fi

${OBJECTDIR}/src/MonitoringWidget_nomain.o: ${OBJECTDIR}/src/MonitoringWidget.o src/MonitoringWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/MonitoringWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MonitoringWidget_nomain.o src/MonitoringWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/MonitoringWidget.o ${OBJECTDIR}/src/MonitoringWidget_nomain.o;\
	fi

${OBJECTDIR}/src/OptionManagementModel_nomain.o: ${OBJECTDIR}/src/OptionManagementModel.o src/OptionManagementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/OptionManagementModel.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementModel_nomain.o src/OptionManagementModel.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/OptionManagementModel.o ${OBJECTDIR}/src/OptionManagementModel_nomain.o;\
	fi

${OBJECTDIR}/src/OptionManagementWidget_nomain.o: ${OBJECTDIR}/src/OptionManagementWidget.o src/OptionManagementWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/OptionManagementWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/OptionManagementWidget_nomain.o src/OptionManagementWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/OptionManagementWidget.o ${OBJECTDIR}/src/OptionManagementWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsTableAssociationWidget_nomain.o: ${OBJECTDIR}/src/PluginsTableAssociationWidget.o src/PluginsTableAssociationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsTableAssociationWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableAssociationWidget_nomain.o src/PluginsTableAssociationWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsTableAssociationWidget.o ${OBJECTDIR}/src/PluginsTableAssociationWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsTableFilterWidget_nomain.o: ${OBJECTDIR}/src/PluginsTableFilterWidget.o src/PluginsTableFilterWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsTableFilterWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableFilterWidget_nomain.o src/PluginsTableFilterWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsTableFilterWidget.o ${OBJECTDIR}/src/PluginsTableFilterWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsTablePluginWidget_nomain.o: ${OBJECTDIR}/src/PluginsTablePluginWidget.o src/PluginsTablePluginWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsTablePluginWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTablePluginWidget_nomain.o src/PluginsTablePluginWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsTablePluginWidget.o ${OBJECTDIR}/src/PluginsTablePluginWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsTableSearchWidget_nomain.o: ${OBJECTDIR}/src/PluginsTableSearchWidget.o src/PluginsTableSearchWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsTableSearchWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSearchWidget_nomain.o src/PluginsTableSearchWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsTableSearchWidget.o ${OBJECTDIR}/src/PluginsTableSearchWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsTableSourceWidget_nomain.o: ${OBJECTDIR}/src/PluginsTableSourceWidget.o src/PluginsTableSourceWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsTableSourceWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsTableSourceWidget_nomain.o src/PluginsTableSourceWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsTableSourceWidget.o ${OBJECTDIR}/src/PluginsTableSourceWidget_nomain.o;\
	fi

${OBJECTDIR}/src/PluginsWidget_nomain.o: ${OBJECTDIR}/src/PluginsWidget.o src/PluginsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/PluginsWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PluginsWidget_nomain.o src/PluginsWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/PluginsWidget.o ${OBJECTDIR}/src/PluginsWidget_nomain.o;\
	fi

${OBJECTDIR}/src/ProbesWidget_nomain.o: ${OBJECTDIR}/src/ProbesWidget.o src/ProbesWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/ProbesWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ProbesWidget_nomain.o src/ProbesWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/ProbesWidget.o ${OBJECTDIR}/src/ProbesWidget_nomain.o;\
	fi

${OBJECTDIR}/src/RecipientsWidget_nomain.o: ${OBJECTDIR}/src/RecipientsWidget.o src/RecipientsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/RecipientsWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RecipientsWidget_nomain.o src/RecipientsWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/RecipientsWidget.o ${OBJECTDIR}/src/RecipientsWidget_nomain.o;\
	fi

${OBJECTDIR}/src/RoleCustomizationWidget_nomain.o: ${OBJECTDIR}/src/RoleCustomizationWidget.o src/RoleCustomizationWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/RoleCustomizationWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RoleCustomizationWidget_nomain.o src/RoleCustomizationWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/RoleCustomizationWidget.o ${OBJECTDIR}/src/RoleCustomizationWidget_nomain.o;\
	fi

${OBJECTDIR}/src/SummaryBoard_nomain.o: ${OBJECTDIR}/src/SummaryBoard.o src/SummaryBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/SummaryBoard.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SummaryBoard_nomain.o src/SummaryBoard.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/SummaryBoard.o ${OBJECTDIR}/src/SummaryBoard_nomain.o;\
	fi

${OBJECTDIR}/src/TrundleTable_nomain.o: ${OBJECTDIR}/src/TrundleTable.o src/TrundleTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/TrundleTable.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TrundleTable_nomain.o src/TrundleTable.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/TrundleTable.o ${OBJECTDIR}/src/TrundleTable_nomain.o;\
	fi

${OBJECTDIR}/src/UnitsWidget_nomain.o: ${OBJECTDIR}/src/UnitsWidget.o src/UnitsWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/UnitsWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UnitsWidget_nomain.o src/UnitsWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/UnitsWidget.o ${OBJECTDIR}/src/UnitsWidget_nomain.o;\
	fi

${OBJECTDIR}/src/UserActionManagement_nomain.o: ${OBJECTDIR}/src/UserActionManagement.o src/UserActionManagement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/UserActionManagement.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserActionManagement_nomain.o src/UserActionManagement.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/UserActionManagement.o ${OBJECTDIR}/src/UserActionManagement_nomain.o;\
	fi

${OBJECTDIR}/src/UserEditionWidget_nomain.o: ${OBJECTDIR}/src/UserEditionWidget.o src/UserEditionWidget.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/UserEditionWidget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iinclude -I../dbo/include -I. -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserEditionWidget_nomain.o src/UserEditionWidget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/UserEditionWidget.o ${OBJECTDIR}/src/UserEditionWidget_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${TESTDIR}/TestFiles/f1

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
