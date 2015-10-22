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
	${OBJECTDIR}/_ext/e0233f51/main.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry

${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}
	${LINK.cc} -o ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/e0233f51/main.o: ../../../src/dry/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e0233f51
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0233f51/main.o ../../../src/dry/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/../../dist/${CND_PLATFORM}/${CND_CONF}/dry

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
