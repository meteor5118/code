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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/_ext/e0233f51/backtrace.o \
	${OBJECTDIR}/_ext/e0233f51/main.o \
	${OBJECTDIR}/_ext/e0233f51/parse_curl.o \
	${OBJECTDIR}/_ext/e0233f51/parse_rsync.o \
	${OBJECTDIR}/_ext/e0233f51/parse_wget.o \
	${OBJECTDIR}/_ext/7bd80be/gzip_file.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/_ext/4a113312/gzip_file_runner.o \
	${TESTDIR}/_ext/4a113312/gzip_file_test.o

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
LDLIBSOPTIONS=-lz

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry

${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}
	${LINK.cc} -o ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/e0233f51/backtrace.o: ../../../src/dry/backtrace.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/backtrace.o ../../../src/dry/backtrace.cpp

${OBJECTDIR}/_ext/e0233f51/main.o: ../../../src/dry/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/main.o ../../../src/dry/main.cpp

${OBJECTDIR}/_ext/e0233f51/parse_curl.o: ../../../src/dry/parse_curl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_curl.o ../../../src/dry/parse_curl.cpp

${OBJECTDIR}/_ext/e0233f51/parse_rsync.o: ../../../src/dry/parse_rsync.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_rsync.o ../../../src/dry/parse_rsync.cpp

${OBJECTDIR}/_ext/e0233f51/parse_wget.o: ../../../src/dry/parse_wget.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_wget.o ../../../src/dry/parse_wget.cpp

${OBJECTDIR}/_ext/7bd80be/gzip_file.o: ../../../src/lib/gzip/gzip_file.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/7bd80be
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/7bd80be/gzip_file.o ../../../src/lib/gzip/gzip_file.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/_ext/4a113312/gzip_file_runner.o ${TESTDIR}/_ext/4a113312/gzip_file_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/_ext/4a113312/gzip_file_runner.o: ../../../test/lib/gzip/gzip_file_runner.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/4a113312
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/4a113312/gzip_file_runner.o ../../../test/lib/gzip/gzip_file_runner.cpp


${TESTDIR}/_ext/4a113312/gzip_file_test.o: ../../../test/lib/gzip/gzip_file_test.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/4a113312
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/4a113312/gzip_file_test.o ../../../test/lib/gzip/gzip_file_test.cpp


${OBJECTDIR}/_ext/e0233f51/backtrace_nomain.o: ${OBJECTDIR}/_ext/e0233f51/backtrace.o ../../../src/dry/backtrace.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e0233f51/backtrace.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/backtrace_nomain.o ../../../src/dry/backtrace.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e0233f51/backtrace.o ${OBJECTDIR}/_ext/e0233f51/backtrace_nomain.o;\
	fi

${OBJECTDIR}/_ext/e0233f51/main_nomain.o: ${OBJECTDIR}/_ext/e0233f51/main.o ../../../src/dry/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e0233f51/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/main_nomain.o ../../../src/dry/main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e0233f51/main.o ${OBJECTDIR}/_ext/e0233f51/main_nomain.o;\
	fi

${OBJECTDIR}/_ext/e0233f51/parse_curl_nomain.o: ${OBJECTDIR}/_ext/e0233f51/parse_curl.o ../../../src/dry/parse_curl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e0233f51/parse_curl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_curl_nomain.o ../../../src/dry/parse_curl.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e0233f51/parse_curl.o ${OBJECTDIR}/_ext/e0233f51/parse_curl_nomain.o;\
	fi

${OBJECTDIR}/_ext/e0233f51/parse_rsync_nomain.o: ${OBJECTDIR}/_ext/e0233f51/parse_rsync.o ../../../src/dry/parse_rsync.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e0233f51/parse_rsync.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_rsync_nomain.o ../../../src/dry/parse_rsync.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e0233f51/parse_rsync.o ${OBJECTDIR}/_ext/e0233f51/parse_rsync_nomain.o;\
	fi

${OBJECTDIR}/_ext/e0233f51/parse_wget_nomain.o: ${OBJECTDIR}/_ext/e0233f51/parse_wget.o ../../../src/dry/parse_wget.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e0233f51/parse_wget.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/parse_wget_nomain.o ../../../src/dry/parse_wget.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e0233f51/parse_wget.o ${OBJECTDIR}/_ext/e0233f51/parse_wget_nomain.o;\
	fi

${OBJECTDIR}/_ext/7bd80be/gzip_file_nomain.o: ${OBJECTDIR}/_ext/7bd80be/gzip_file.o ../../../src/lib/gzip/gzip_file.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/7bd80be
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/7bd80be/gzip_file.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/7bd80be/gzip_file_nomain.o ../../../src/lib/gzip/gzip_file.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/7bd80be/gzip_file.o ${OBJECTDIR}/_ext/7bd80be/gzip_file_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
