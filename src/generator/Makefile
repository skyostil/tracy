#---------------------------------------------------------------------------
# Tracy Generator makefile
# (automatically generated)
#---------------------------------------------------------------------------
# Installation prefix
PREFIX=../..
# Project target path
TRACY_TARGET_PATH=$(PREFIX)/lib/tracy
# Generation target path
TRACY_CODEGEN_PATH=output
# Symbian architecture (e.g., winscw, armv5)
ARCH=winscw
# Symbian configuration (e.g., urel, ureb)
CONF=udeb
# Choose compiler
COMP=ABLD
# Verbose output
VERBOSE=0

# Tools
PYTHON=python
RM=/bin/rm
ABLD=cmd /c ..\\..\\..\\abld-wrapper.bat   # ABLD wrapper for Cygwin
NMAKE=cmd /c ..\\..\\nmake-wrapper.bat     # NMake wrapper for Cygwin
SBS=cmd /c ..\\..\\..\\sbs-wrapper.bat         # SBS wrapper for Cygwin
MKDIR=/bin/mkdir
CP=cp
#---------------------------------------------------------------------------

ifeq ($(VERBOSE),1)
TRACY_ARGS=-v
else
TRACY_ARGS?=
endif

ifeq ($(COMP),abld)
BUILD_COMMAND=  $(ABLD) build $(ARCH) $(CONF)
CLEANCOMMAND=   $(ABLD) reallyclean $(ARCH) $(CONF)
else
BUILD_COMMAND= $(SBS) -c $(ARCH)_$(CONF)
CLEANCOMMAND=  $(SBS) -c $(ARCH)_$(CONF) clean
endif

#---------------------------------------------------------------------------
# Global targets
#---------------------------------------------------------------------------
all: projects targets
projects: gles11-vg11-egl13-android vg11-egl13-android vg11-android gles11-vg101-egl13-android gles11-vg11-vgu11-egl13-android vg11-vgu11-egl13-android gles20-egl13-android gles20-vg11-egl13-android gles11-egl13-android vgu11-android gles11-egl11-android gles20-vg11-vgu11-egl13-android egl13-android gles11-android gles20-android gles11-vg11-egl13-unix vg11-egl13-unix vg11-unix gles11-vg101-egl13-unix gles11-vg11-vgu11-egl13-unix vg11-vgu11-egl13-unix gles20-egl13-unix gles20-vg11-egl13-unix gles11-egl13-unix vgu11-unix gles11-egl11-unix gles20-vg11-vgu11-egl13-unix egl13-unix gles11-unix gles20-unix gles11-vg11-egl13-symbian vg11-egl13-symbian vg11-symbian gles11-vg101-egl13-symbian gles11-vg11-vgu11-egl13-symbian vg11-vgu11-egl13-symbian gles20-egl13-symbian gles20-vg11-egl13-symbian gles11-egl13-symbian vgu11-symbian gles11-egl11-symbian gles20-vg11-vgu11-egl13-symbian egl13-symbian gles11-symbian gles20-symbian gles11-vg11-egl13-win32 vg11-egl13-win32 vg11-win32 gles11-vg101-egl13-win32 gles11-vg11-vgu11-egl13-win32 vg11-vgu11-egl13-win32 gles20-egl13-win32 gles20-vg11-egl13-win32 gles11-egl13-win32 vgu11-win32 gles11-egl11-win32 gles20-vg11-vgu11-egl13-win32 egl13-win32 gles11-win32 gles20-win32
targets: gles11-vg11-egl13-tracer-android vg11-egl13-tracer-android vg11-tracer-android gles11-vg101-egl13-tracer-android gles11-vg11-vgu11-egl13-tracer-android vg11-vgu11-egl13-tracer-android gles20-egl13-tracer-android gles20-vg11-egl13-tracer-android gles11-egl13-tracer-android vgu11-tracer-android gles11-egl11-tracer-android gles20-vg11-vgu11-egl13-tracer-android egl13-tracer-android gles11-tracer-android gles20-tracer-android gles11-vg11-egl13-passthrough_tracer-android vg11-egl13-passthrough_tracer-android vg11-passthrough_tracer-android gles11-vg101-egl13-passthrough_tracer-android gles11-vg11-vgu11-egl13-passthrough_tracer-android vg11-vgu11-egl13-passthrough_tracer-android gles20-egl13-passthrough_tracer-android gles20-vg11-egl13-passthrough_tracer-android gles11-egl13-passthrough_tracer-android vgu11-passthrough_tracer-android gles11-egl11-passthrough_tracer-android gles20-vg11-vgu11-egl13-passthrough_tracer-android egl13-passthrough_tracer-android gles11-passthrough_tracer-android gles20-passthrough_tracer-android gles11-vg11-egl13-player-android vg11-egl13-player-android vg11-player-android gles11-vg101-egl13-player-android gles11-vg11-vgu11-egl13-player-android vg11-vgu11-egl13-player-android gles20-egl13-player-android gles20-vg11-egl13-player-android gles11-egl13-player-android vgu11-player-android gles11-egl11-player-android gles20-vg11-vgu11-egl13-player-android egl13-player-android gles11-player-android gles20-player-android gles11-vg11-egl13-tracer-unix vg11-egl13-tracer-unix vg11-tracer-unix gles11-vg101-egl13-tracer-unix gles11-vg11-vgu11-egl13-tracer-unix vg11-vgu11-egl13-tracer-unix gles20-egl13-tracer-unix gles20-vg11-egl13-tracer-unix gles11-egl13-tracer-unix vgu11-tracer-unix gles11-egl11-tracer-unix gles20-vg11-vgu11-egl13-tracer-unix egl13-tracer-unix gles11-tracer-unix gles20-tracer-unix gles11-vg11-egl13-passthrough_tracer-unix vg11-egl13-passthrough_tracer-unix vg11-passthrough_tracer-unix gles11-vg101-egl13-passthrough_tracer-unix gles11-vg11-vgu11-egl13-passthrough_tracer-unix vg11-vgu11-egl13-passthrough_tracer-unix gles20-egl13-passthrough_tracer-unix gles20-vg11-egl13-passthrough_tracer-unix gles11-egl13-passthrough_tracer-unix vgu11-passthrough_tracer-unix gles11-egl11-passthrough_tracer-unix gles20-vg11-vgu11-egl13-passthrough_tracer-unix egl13-passthrough_tracer-unix gles11-passthrough_tracer-unix gles20-passthrough_tracer-unix gles11-vg11-egl13-player-unix vg11-egl13-player-unix vg11-player-unix gles11-vg101-egl13-player-unix gles11-vg11-vgu11-egl13-player-unix vg11-vgu11-egl13-player-unix gles20-egl13-player-unix gles20-vg11-egl13-player-unix gles11-egl13-player-unix vgu11-player-unix gles11-egl11-player-unix gles20-vg11-vgu11-egl13-player-unix egl13-player-unix gles11-player-unix gles20-player-unix gles11-vg11-egl13-tracer-symbian vg11-egl13-tracer-symbian vg11-tracer-symbian gles11-vg101-egl13-tracer-symbian gles11-vg11-vgu11-egl13-tracer-symbian vg11-vgu11-egl13-tracer-symbian gles20-egl13-tracer-symbian gles20-vg11-egl13-tracer-symbian gles11-egl13-tracer-symbian vgu11-tracer-symbian gles11-egl11-tracer-symbian gles20-vg11-vgu11-egl13-tracer-symbian egl13-tracer-symbian gles11-tracer-symbian gles20-tracer-symbian gles11-vg11-egl13-passthrough_tracer-symbian vg11-egl13-passthrough_tracer-symbian vg11-passthrough_tracer-symbian gles11-vg101-egl13-passthrough_tracer-symbian gles11-vg11-vgu11-egl13-passthrough_tracer-symbian vg11-vgu11-egl13-passthrough_tracer-symbian gles20-egl13-passthrough_tracer-symbian gles20-vg11-egl13-passthrough_tracer-symbian gles11-egl13-passthrough_tracer-symbian vgu11-passthrough_tracer-symbian gles11-egl11-passthrough_tracer-symbian gles20-vg11-vgu11-egl13-passthrough_tracer-symbian egl13-passthrough_tracer-symbian gles11-passthrough_tracer-symbian gles20-passthrough_tracer-symbian gles11-vg11-egl13-player-symbian vg11-egl13-player-symbian vg11-player-symbian gles11-vg101-egl13-player-symbian gles11-vg11-vgu11-egl13-player-symbian vg11-vgu11-egl13-player-symbian gles20-egl13-player-symbian gles20-vg11-egl13-player-symbian gles11-egl13-player-symbian vgu11-player-symbian gles11-egl11-player-symbian gles20-vg11-vgu11-egl13-player-symbian egl13-player-symbian gles11-player-symbian gles20-player-symbian gles11-vg11-egl13-tracer-win32 vg11-egl13-tracer-win32 vg11-tracer-win32 gles11-vg101-egl13-tracer-win32 gles11-vg11-vgu11-egl13-tracer-win32 vg11-vgu11-egl13-tracer-win32 gles20-egl13-tracer-win32 gles20-vg11-egl13-tracer-win32 gles11-egl13-tracer-win32 vgu11-tracer-win32 gles11-egl11-tracer-win32 gles20-vg11-vgu11-egl13-tracer-win32 egl13-tracer-win32 gles11-tracer-win32 gles20-tracer-win32 gles11-vg11-egl13-passthrough_tracer-win32 vg11-egl13-passthrough_tracer-win32 vg11-passthrough_tracer-win32 gles11-vg101-egl13-passthrough_tracer-win32 gles11-vg11-vgu11-egl13-passthrough_tracer-win32 vg11-vgu11-egl13-passthrough_tracer-win32 gles20-egl13-passthrough_tracer-win32 gles20-vg11-egl13-passthrough_tracer-win32 gles11-egl13-passthrough_tracer-win32 vgu11-passthrough_tracer-win32 gles11-egl11-passthrough_tracer-win32 gles20-vg11-vgu11-egl13-passthrough_tracer-win32 egl13-passthrough_tracer-win32 gles11-passthrough_tracer-win32 gles20-passthrough_tracer-win32 gles11-vg11-egl13-player-win32 vg11-egl13-player-win32 vg11-player-win32 gles11-vg101-egl13-player-win32 gles11-vg11-vgu11-egl13-player-win32 vg11-vgu11-egl13-player-win32 gles20-egl13-player-win32 gles20-vg11-egl13-player-win32 gles11-egl13-player-win32 vgu11-player-win32 gles11-egl11-player-win32 gles20-vg11-vgu11-egl13-player-win32 egl13-player-win32 gles11-player-win32 gles20-player-win32
.PHONY: all projects targets

projects_clean: gles11-vg11-egl13-android_clean vg11-egl13-android_clean vg11-android_clean gles11-vg101-egl13-android_clean gles11-vg11-vgu11-egl13-android_clean vg11-vgu11-egl13-android_clean gles20-egl13-android_clean gles20-vg11-egl13-android_clean gles11-egl13-android_clean vgu11-android_clean gles11-egl11-android_clean gles20-vg11-vgu11-egl13-android_clean egl13-android_clean gles11-android_clean gles20-android_clean gles11-vg11-egl13-unix_clean vg11-egl13-unix_clean vg11-unix_clean gles11-vg101-egl13-unix_clean gles11-vg11-vgu11-egl13-unix_clean vg11-vgu11-egl13-unix_clean gles20-egl13-unix_clean gles20-vg11-egl13-unix_clean gles11-egl13-unix_clean vgu11-unix_clean gles11-egl11-unix_clean gles20-vg11-vgu11-egl13-unix_clean egl13-unix_clean gles11-unix_clean gles20-unix_clean gles11-vg11-egl13-symbian_clean vg11-egl13-symbian_clean vg11-symbian_clean gles11-vg101-egl13-symbian_clean gles11-vg11-vgu11-egl13-symbian_clean vg11-vgu11-egl13-symbian_clean gles20-egl13-symbian_clean gles20-vg11-egl13-symbian_clean gles11-egl13-symbian_clean vgu11-symbian_clean gles11-egl11-symbian_clean gles20-vg11-vgu11-egl13-symbian_clean egl13-symbian_clean gles11-symbian_clean gles20-symbian_clean gles11-vg11-egl13-win32_clean vg11-egl13-win32_clean vg11-win32_clean gles11-vg101-egl13-win32_clean gles11-vg11-vgu11-egl13-win32_clean vg11-vgu11-egl13-win32_clean gles20-egl13-win32_clean gles20-vg11-egl13-win32_clean gles11-egl13-win32_clean vgu11-win32_clean gles11-egl11-win32_clean gles20-vg11-vgu11-egl13-win32_clean egl13-win32_clean gles11-win32_clean gles20-win32_clean
targets_clean: gles11-vg11-egl13-tracer-android_clean vg11-egl13-tracer-android_clean vg11-tracer-android_clean gles11-vg101-egl13-tracer-android_clean gles11-vg11-vgu11-egl13-tracer-android_clean vg11-vgu11-egl13-tracer-android_clean gles20-egl13-tracer-android_clean gles20-vg11-egl13-tracer-android_clean gles11-egl13-tracer-android_clean vgu11-tracer-android_clean gles11-egl11-tracer-android_clean gles20-vg11-vgu11-egl13-tracer-android_clean egl13-tracer-android_clean gles11-tracer-android_clean gles20-tracer-android_clean gles11-vg11-egl13-passthrough_tracer-android_clean vg11-egl13-passthrough_tracer-android_clean vg11-passthrough_tracer-android_clean gles11-vg101-egl13-passthrough_tracer-android_clean gles11-vg11-vgu11-egl13-passthrough_tracer-android_clean vg11-vgu11-egl13-passthrough_tracer-android_clean gles20-egl13-passthrough_tracer-android_clean gles20-vg11-egl13-passthrough_tracer-android_clean gles11-egl13-passthrough_tracer-android_clean vgu11-passthrough_tracer-android_clean gles11-egl11-passthrough_tracer-android_clean gles20-vg11-vgu11-egl13-passthrough_tracer-android_clean egl13-passthrough_tracer-android_clean gles11-passthrough_tracer-android_clean gles20-passthrough_tracer-android_clean gles11-vg11-egl13-player-android_clean vg11-egl13-player-android_clean vg11-player-android_clean gles11-vg101-egl13-player-android_clean gles11-vg11-vgu11-egl13-player-android_clean vg11-vgu11-egl13-player-android_clean gles20-egl13-player-android_clean gles20-vg11-egl13-player-android_clean gles11-egl13-player-android_clean vgu11-player-android_clean gles11-egl11-player-android_clean gles20-vg11-vgu11-egl13-player-android_clean egl13-player-android_clean gles11-player-android_clean gles20-player-android_clean gles11-vg11-egl13-tracer-unix_clean vg11-egl13-tracer-unix_clean vg11-tracer-unix_clean gles11-vg101-egl13-tracer-unix_clean gles11-vg11-vgu11-egl13-tracer-unix_clean vg11-vgu11-egl13-tracer-unix_clean gles20-egl13-tracer-unix_clean gles20-vg11-egl13-tracer-unix_clean gles11-egl13-tracer-unix_clean vgu11-tracer-unix_clean gles11-egl11-tracer-unix_clean gles20-vg11-vgu11-egl13-tracer-unix_clean egl13-tracer-unix_clean gles11-tracer-unix_clean gles20-tracer-unix_clean gles11-vg11-egl13-passthrough_tracer-unix_clean vg11-egl13-passthrough_tracer-unix_clean vg11-passthrough_tracer-unix_clean gles11-vg101-egl13-passthrough_tracer-unix_clean gles11-vg11-vgu11-egl13-passthrough_tracer-unix_clean vg11-vgu11-egl13-passthrough_tracer-unix_clean gles20-egl13-passthrough_tracer-unix_clean gles20-vg11-egl13-passthrough_tracer-unix_clean gles11-egl13-passthrough_tracer-unix_clean vgu11-passthrough_tracer-unix_clean gles11-egl11-passthrough_tracer-unix_clean gles20-vg11-vgu11-egl13-passthrough_tracer-unix_clean egl13-passthrough_tracer-unix_clean gles11-passthrough_tracer-unix_clean gles20-passthrough_tracer-unix_clean gles11-vg11-egl13-player-unix_clean vg11-egl13-player-unix_clean vg11-player-unix_clean gles11-vg101-egl13-player-unix_clean gles11-vg11-vgu11-egl13-player-unix_clean vg11-vgu11-egl13-player-unix_clean gles20-egl13-player-unix_clean gles20-vg11-egl13-player-unix_clean gles11-egl13-player-unix_clean vgu11-player-unix_clean gles11-egl11-player-unix_clean gles20-vg11-vgu11-egl13-player-unix_clean egl13-player-unix_clean gles11-player-unix_clean gles20-player-unix_clean gles11-vg11-egl13-tracer-symbian_clean vg11-egl13-tracer-symbian_clean vg11-tracer-symbian_clean gles11-vg101-egl13-tracer-symbian_clean gles11-vg11-vgu11-egl13-tracer-symbian_clean vg11-vgu11-egl13-tracer-symbian_clean gles20-egl13-tracer-symbian_clean gles20-vg11-egl13-tracer-symbian_clean gles11-egl13-tracer-symbian_clean vgu11-tracer-symbian_clean gles11-egl11-tracer-symbian_clean gles20-vg11-vgu11-egl13-tracer-symbian_clean egl13-tracer-symbian_clean gles11-tracer-symbian_clean gles20-tracer-symbian_clean gles11-vg11-egl13-passthrough_tracer-symbian_clean vg11-egl13-passthrough_tracer-symbian_clean vg11-passthrough_tracer-symbian_clean gles11-vg101-egl13-passthrough_tracer-symbian_clean gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_clean vg11-vgu11-egl13-passthrough_tracer-symbian_clean gles20-egl13-passthrough_tracer-symbian_clean gles20-vg11-egl13-passthrough_tracer-symbian_clean gles11-egl13-passthrough_tracer-symbian_clean vgu11-passthrough_tracer-symbian_clean gles11-egl11-passthrough_tracer-symbian_clean gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_clean egl13-passthrough_tracer-symbian_clean gles11-passthrough_tracer-symbian_clean gles20-passthrough_tracer-symbian_clean gles11-vg11-egl13-player-symbian_clean vg11-egl13-player-symbian_clean vg11-player-symbian_clean gles11-vg101-egl13-player-symbian_clean gles11-vg11-vgu11-egl13-player-symbian_clean vg11-vgu11-egl13-player-symbian_clean gles20-egl13-player-symbian_clean gles20-vg11-egl13-player-symbian_clean gles11-egl13-player-symbian_clean vgu11-player-symbian_clean gles11-egl11-player-symbian_clean gles20-vg11-vgu11-egl13-player-symbian_clean egl13-player-symbian_clean gles11-player-symbian_clean gles20-player-symbian_clean gles11-vg11-egl13-tracer-win32_clean vg11-egl13-tracer-win32_clean vg11-tracer-win32_clean gles11-vg101-egl13-tracer-win32_clean gles11-vg11-vgu11-egl13-tracer-win32_clean vg11-vgu11-egl13-tracer-win32_clean gles20-egl13-tracer-win32_clean gles20-vg11-egl13-tracer-win32_clean gles11-egl13-tracer-win32_clean vgu11-tracer-win32_clean gles11-egl11-tracer-win32_clean gles20-vg11-vgu11-egl13-tracer-win32_clean egl13-tracer-win32_clean gles11-tracer-win32_clean gles20-tracer-win32_clean gles11-vg11-egl13-passthrough_tracer-win32_clean vg11-egl13-passthrough_tracer-win32_clean vg11-passthrough_tracer-win32_clean gles11-vg101-egl13-passthrough_tracer-win32_clean gles11-vg11-vgu11-egl13-passthrough_tracer-win32_clean vg11-vgu11-egl13-passthrough_tracer-win32_clean gles20-egl13-passthrough_tracer-win32_clean gles20-vg11-egl13-passthrough_tracer-win32_clean gles11-egl13-passthrough_tracer-win32_clean vgu11-passthrough_tracer-win32_clean gles11-egl11-passthrough_tracer-win32_clean gles20-vg11-vgu11-egl13-passthrough_tracer-win32_clean egl13-passthrough_tracer-win32_clean gles11-passthrough_tracer-win32_clean gles20-passthrough_tracer-win32_clean gles11-vg11-egl13-player-win32_clean vg11-egl13-player-win32_clean vg11-player-win32_clean gles11-vg101-egl13-player-win32_clean gles11-vg11-vgu11-egl13-player-win32_clean vg11-vgu11-egl13-player-win32_clean gles20-egl13-player-win32_clean gles20-vg11-egl13-player-win32_clean gles11-egl13-player-win32_clean vgu11-player-win32_clean gles11-egl11-player-win32_clean gles20-vg11-vgu11-egl13-player-win32_clean egl13-player-win32_clean gles11-player-win32_clean gles20-player-win32_clean
clean: targets_clean projects_clean output-dirs_clean
.PHONY: clean projects_clean targets_clean

help:
	@echo
	@echo This makefile is used to generate Tracy projects and targets such
	@echo as tracers. The following generation targets are available:
	@echo
	@echo "    projects  - Generate all project files for all platforms"
	@echo "    targets   - Compile all targets for all platforms"
	@echo "    tracers   - Compile all tracers for all platforms"
	@echo "    clean     - Remove all generated files"
	@echo
	@echo To reduce build time, you can generate or compile only a subset of
	@echo the available targets. For example:
	@echo
	@echo "    projects-unix       - Generate all project files for unix"
	@echo "    targets-win32       - Compile all targets for win32"
	@echo "    targets-symbian_gen - Generate all targets for symbian without building"
	@echo "    vg11-tracer-unix    - Build the OpenVG 1.1 tracer for unix"
	@echo "    symbian_clean       - Remove all generated files for symbian"
	@echo
	@echo The following platforms are available:
	@echo
	@echo "    android"
	@echo "    unix"
	@echo "    symbian"
	@echo "    win32"
	@echo
	@echo The following API configurations are available:
	@echo
	@echo "    gles11-vg11-egl13"
	@echo "    vg11-egl13"
	@echo "    vg11"
	@echo "    gles11-vg101-egl13"
	@echo "    gles11-vg11-vgu11-egl13"
	@echo "    vg11-vgu11-egl13"
	@echo "    gles20-egl13"
	@echo "    gles20-vg11-egl13"
	@echo "    gles11-egl13"
	@echo "    vgu11"
	@echo "    gles11-egl11"
	@echo "    gles20-vg11-vgu11-egl13"
	@echo "    egl13"
	@echo "    gles11"
	@echo "    gles20"
	@echo
	@echo The following configuration targets are available:
	@echo
	@echo "    tracer"
	@echo "    passthrough_tracer"
	@echo "    player"
	@echo
	@echo Append _clean to a project or target name to remove the compilation
	@echo output. Append _dump to a project name to display debug information
	@echo about that project.
	@echo
.PHONY: help

test:
	@$(PYTHON) TestAll.py
.PHONY: test

sync:
	@$(PYTHON) TracyGenerator.py sync $(TRACY_ARGS)
.PHONY: sync

$(TRACY_TARGET_PATH):
	@$(MKDIR) -p $@

$(TRACY_CODEGEN_PATH):
	@$(MKDIR) -p $@

output-dirs_clean:
	@$(RM) -rf $(TRACY_CODEGEN_PATH)
.PHONY: output-dirs_clean



#---------------------------------------------------------------------------
# Project gles11-vg11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles11-vg11-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-android: $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy
gles11-vg11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy
gles11-vg11-egl13-android_dump: gles11-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy
.PHONY: gles11-vg11-egl13-android gles11-vg11-egl13-android_clean gles11-vg11-egl13-android_dump

#---------------------------------------------------------------------------
# Project vg11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-egl/vg11-egl13.conf -p android -o $@ $(TRACY_ARGS)
vg11-egl13-android: $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy
vg11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy
vg11-egl13-android_dump: vg11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy
.PHONY: vg11-egl13-android vg11-egl13-android_clean vg11-egl13-android_dump

#---------------------------------------------------------------------------
# Project vg11-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-android.tcy: $(TRACY_TARGET_PATH) config/vg/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vg/v1.1/vg.conf -p android -o $@ $(TRACY_ARGS)
vg11-android: $(TRACY_TARGET_PATH)/vg11-android.tcy
vg11-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-android.tcy
vg11-android_dump: vg11-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-android.tcy
.PHONY: vg11-android vg11-android_clean vg11-android_dump

#---------------------------------------------------------------------------
# Project gles11-vg101-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles-vg-egl.conf -p android -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-android: $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy
gles11-vg101-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy
gles11-vg101-egl13-android_dump: gles11-vg101-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy
.PHONY: gles11-vg101-egl13-android gles11-vg101-egl13-android_clean gles11-vg101-egl13-android_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-vgu11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles11-vg11-vgu11-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-android: $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy
gles11-vg11-vgu11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy
gles11-vg11-vgu11-egl13-android_dump: gles11-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy
.PHONY: gles11-vg11-vgu11-egl13-android gles11-vg11-vgu11-egl13-android_clean gles11-vg11-vgu11-egl13-android_dump

#---------------------------------------------------------------------------
# Project vg11-vgu11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-vgu-egl/vg11-vgu11-egl13.conf -p android -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-android: $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy
vg11-vgu11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy
vg11-vgu11-egl13-android_dump: vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy
.PHONY: vg11-vgu11-egl13-android vg11-vgu11-egl13-android_clean vg11-vgu11-egl13-android_dump

#---------------------------------------------------------------------------
# Project gles20-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles20-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles20-egl13-android: $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy
gles20-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy
gles20-egl13-android_dump: gles20-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy
.PHONY: gles20-egl13-android gles20-egl13-android_clean gles20-egl13-android_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles20-vg11-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-android: $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy
gles20-vg11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy
gles20-vg11-egl13-android_dump: gles20-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy
.PHONY: gles20-vg11-egl13-android gles20-vg11-egl13-android_clean gles20-vg11-egl13-android_dump

#---------------------------------------------------------------------------
# Project gles11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles11-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles11-egl13-android: $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy
gles11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy
gles11-egl13-android_dump: gles11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy
.PHONY: gles11-egl13-android gles11-egl13-android_clean gles11-egl13-android_dump

#---------------------------------------------------------------------------
# Project vgu11-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vgu11-android.tcy: $(TRACY_TARGET_PATH) config/vgu/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vgu/v1.1/vgu.conf -p android -o $@ $(TRACY_ARGS)
vgu11-android: $(TRACY_TARGET_PATH)/vgu11-android.tcy
vgu11-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vgu11-android.tcy
vgu11-android_dump: vgu11-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vgu11-android.tcy
.PHONY: vgu11-android vgu11-android_clean vgu11-android_dump

#---------------------------------------------------------------------------
# Project gles11-egl11-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl11-android.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles-egl.conf -p android -o $@ $(TRACY_ARGS)
gles11-egl11-android: $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy
gles11-egl11-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy
gles11-egl11-android_dump: gles11-egl11-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy
.PHONY: gles11-egl11-android gles11-egl11-android_clean gles11-egl11-android_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-vgu11-egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles20-vg11-vgu11-egl13.conf -p android -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-android: $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy
gles20-vg11-vgu11-egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy
gles20-vg11-vgu11-egl13-android_dump: gles20-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy
.PHONY: gles20-vg11-vgu11-egl13-android gles20-vg11-vgu11-egl13-android_clean gles20-vg11-vgu11-egl13-android_dump

#---------------------------------------------------------------------------
# Project egl13-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/egl13-android.tcy: $(TRACY_TARGET_PATH) config/egl/v1.3/*
	@$(PYTHON) TracyGenerator.py create config/egl/v1.3/egl.conf -p android -o $@ $(TRACY_ARGS)
egl13-android: $(TRACY_TARGET_PATH)/egl13-android.tcy
egl13-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/egl13-android.tcy
egl13-android_dump: egl13-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/egl13-android.tcy
.PHONY: egl13-android egl13-android_clean egl13-android_dump

#---------------------------------------------------------------------------
# Project gles11-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-android.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles.conf -p android -o $@ $(TRACY_ARGS)
gles11-android: $(TRACY_TARGET_PATH)/gles11-android.tcy
gles11-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-android.tcy
gles11-android_dump: gles11-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-android.tcy
.PHONY: gles11-android gles11-android_clean gles11-android_dump

#---------------------------------------------------------------------------
# Project gles20-android
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-android.tcy: $(TRACY_TARGET_PATH) config/gles/v2.0/*
	@$(PYTHON) TracyGenerator.py create config/gles/v2.0/gles.conf -p android -o $@ $(TRACY_ARGS)
gles20-android: $(TRACY_TARGET_PATH)/gles20-android.tcy
gles20-android_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-android.tcy
gles20-android_dump: gles20-android
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-android.tcy
.PHONY: gles20-android gles20-android_clean gles20-android_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles11-vg11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-unix: $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy
gles11-vg11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy
gles11-vg11-egl13-unix_dump: gles11-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy
.PHONY: gles11-vg11-egl13-unix gles11-vg11-egl13-unix_clean gles11-vg11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project vg11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-egl/vg11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
vg11-egl13-unix: $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy
vg11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy
vg11-egl13-unix_dump: vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy
.PHONY: vg11-egl13-unix vg11-egl13-unix_clean vg11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project vg11-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-unix.tcy: $(TRACY_TARGET_PATH) config/vg/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vg/v1.1/vg.conf -p unix -o $@ $(TRACY_ARGS)
vg11-unix: $(TRACY_TARGET_PATH)/vg11-unix.tcy
vg11-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-unix.tcy
vg11-unix_dump: vg11-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-unix.tcy
.PHONY: vg11-unix vg11-unix_clean vg11-unix_dump

#---------------------------------------------------------------------------
# Project gles11-vg101-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles-vg-egl.conf -p unix -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-unix: $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy
gles11-vg101-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy
gles11-vg101-egl13-unix_dump: gles11-vg101-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy
.PHONY: gles11-vg101-egl13-unix gles11-vg101-egl13-unix_clean gles11-vg101-egl13-unix_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-vgu11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles11-vg11-vgu11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-unix: $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy
gles11-vg11-vgu11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy
gles11-vg11-vgu11-egl13-unix_dump: gles11-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy
.PHONY: gles11-vg11-vgu11-egl13-unix gles11-vg11-vgu11-egl13-unix_clean gles11-vg11-vgu11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project vg11-vgu11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-vgu-egl/vg11-vgu11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-unix: $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy
vg11-vgu11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy
vg11-vgu11-egl13-unix_dump: vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy
.PHONY: vg11-vgu11-egl13-unix vg11-vgu11-egl13-unix_clean vg11-vgu11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project gles20-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles20-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles20-egl13-unix: $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy
gles20-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy
gles20-egl13-unix_dump: gles20-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy
.PHONY: gles20-egl13-unix gles20-egl13-unix_clean gles20-egl13-unix_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles20-vg11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-unix: $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy
gles20-vg11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy
gles20-vg11-egl13-unix_dump: gles20-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy
.PHONY: gles20-vg11-egl13-unix gles20-vg11-egl13-unix_clean gles20-vg11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project gles11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles11-egl13-unix: $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy
gles11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy
gles11-egl13-unix_dump: gles11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy
.PHONY: gles11-egl13-unix gles11-egl13-unix_clean gles11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project vgu11-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vgu11-unix.tcy: $(TRACY_TARGET_PATH) config/vgu/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vgu/v1.1/vgu.conf -p unix -o $@ $(TRACY_ARGS)
vgu11-unix: $(TRACY_TARGET_PATH)/vgu11-unix.tcy
vgu11-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vgu11-unix.tcy
vgu11-unix_dump: vgu11-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vgu11-unix.tcy
.PHONY: vgu11-unix vgu11-unix_clean vgu11-unix_dump

#---------------------------------------------------------------------------
# Project gles11-egl11-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles-egl.conf -p unix -o $@ $(TRACY_ARGS)
gles11-egl11-unix: $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy
gles11-egl11-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy
gles11-egl11-unix_dump: gles11-egl11-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy
.PHONY: gles11-egl11-unix gles11-egl11-unix_clean gles11-egl11-unix_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-vgu11-egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles20-vg11-vgu11-egl13.conf -p unix -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-unix: $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy
gles20-vg11-vgu11-egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy
gles20-vg11-vgu11-egl13-unix_dump: gles20-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy
.PHONY: gles20-vg11-vgu11-egl13-unix gles20-vg11-vgu11-egl13-unix_clean gles20-vg11-vgu11-egl13-unix_dump

#---------------------------------------------------------------------------
# Project egl13-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/egl13-unix.tcy: $(TRACY_TARGET_PATH) config/egl/v1.3/*
	@$(PYTHON) TracyGenerator.py create config/egl/v1.3/egl.conf -p unix -o $@ $(TRACY_ARGS)
egl13-unix: $(TRACY_TARGET_PATH)/egl13-unix.tcy
egl13-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/egl13-unix.tcy
egl13-unix_dump: egl13-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/egl13-unix.tcy
.PHONY: egl13-unix egl13-unix_clean egl13-unix_dump

#---------------------------------------------------------------------------
# Project gles11-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-unix.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles.conf -p unix -o $@ $(TRACY_ARGS)
gles11-unix: $(TRACY_TARGET_PATH)/gles11-unix.tcy
gles11-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-unix.tcy
gles11-unix_dump: gles11-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-unix.tcy
.PHONY: gles11-unix gles11-unix_clean gles11-unix_dump

#---------------------------------------------------------------------------
# Project gles20-unix
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-unix.tcy: $(TRACY_TARGET_PATH) config/gles/v2.0/*
	@$(PYTHON) TracyGenerator.py create config/gles/v2.0/gles.conf -p unix -o $@ $(TRACY_ARGS)
gles20-unix: $(TRACY_TARGET_PATH)/gles20-unix.tcy
gles20-unix_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-unix.tcy
gles20-unix_dump: gles20-unix
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-unix.tcy
.PHONY: gles20-unix gles20-unix_clean gles20-unix_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles11-vg11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-symbian: $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy
gles11-vg11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy
gles11-vg11-egl13-symbian_dump: gles11-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy
.PHONY: gles11-vg11-egl13-symbian gles11-vg11-egl13-symbian_clean gles11-vg11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project vg11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-egl/vg11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
vg11-egl13-symbian: $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy
vg11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy
vg11-egl13-symbian_dump: vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy
.PHONY: vg11-egl13-symbian vg11-egl13-symbian_clean vg11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project vg11-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-symbian.tcy: $(TRACY_TARGET_PATH) config/vg/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vg/v1.1/vg.conf -p symbian -o $@ $(TRACY_ARGS)
vg11-symbian: $(TRACY_TARGET_PATH)/vg11-symbian.tcy
vg11-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-symbian.tcy
vg11-symbian_dump: vg11-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-symbian.tcy
.PHONY: vg11-symbian vg11-symbian_clean vg11-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-vg101-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles-vg-egl.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-symbian: $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy
gles11-vg101-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy
gles11-vg101-egl13-symbian_dump: gles11-vg101-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy
.PHONY: gles11-vg101-egl13-symbian gles11-vg101-egl13-symbian_clean gles11-vg101-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-vgu11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles11-vg11-vgu11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-symbian: $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy
gles11-vg11-vgu11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy
gles11-vg11-vgu11-egl13-symbian_dump: gles11-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy
.PHONY: gles11-vg11-vgu11-egl13-symbian gles11-vg11-vgu11-egl13-symbian_clean gles11-vg11-vgu11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project vg11-vgu11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-vgu-egl/vg11-vgu11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-symbian: $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy
vg11-vgu11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy
vg11-vgu11-egl13-symbian_dump: vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy
.PHONY: vg11-vgu11-egl13-symbian vg11-vgu11-egl13-symbian_clean vg11-vgu11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project gles20-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles20-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles20-egl13-symbian: $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy
gles20-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy
gles20-egl13-symbian_dump: gles20-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy
.PHONY: gles20-egl13-symbian gles20-egl13-symbian_clean gles20-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles20-vg11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-symbian: $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy
gles20-vg11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy
gles20-vg11-egl13-symbian_dump: gles20-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy
.PHONY: gles20-vg11-egl13-symbian gles20-vg11-egl13-symbian_clean gles20-vg11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-egl13-symbian: $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy
gles11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy
gles11-egl13-symbian_dump: gles11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy
.PHONY: gles11-egl13-symbian gles11-egl13-symbian_clean gles11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project vgu11-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vgu11-symbian.tcy: $(TRACY_TARGET_PATH) config/vgu/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vgu/v1.1/vgu.conf -p symbian -o $@ $(TRACY_ARGS)
vgu11-symbian: $(TRACY_TARGET_PATH)/vgu11-symbian.tcy
vgu11-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vgu11-symbian.tcy
vgu11-symbian_dump: vgu11-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vgu11-symbian.tcy
.PHONY: vgu11-symbian vgu11-symbian_clean vgu11-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-egl11-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles-egl.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-egl11-symbian: $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy
gles11-egl11-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy
gles11-egl11-symbian_dump: gles11-egl11-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy
.PHONY: gles11-egl11-symbian gles11-egl11-symbian_clean gles11-egl11-symbian_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-vgu11-egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles20-vg11-vgu11-egl13.conf -p symbian -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-symbian: $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy
gles20-vg11-vgu11-egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy
gles20-vg11-vgu11-egl13-symbian_dump: gles20-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy
.PHONY: gles20-vg11-vgu11-egl13-symbian gles20-vg11-vgu11-egl13-symbian_clean gles20-vg11-vgu11-egl13-symbian_dump

#---------------------------------------------------------------------------
# Project egl13-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/egl13-symbian.tcy: $(TRACY_TARGET_PATH) config/egl/v1.3/*
	@$(PYTHON) TracyGenerator.py create config/egl/v1.3/egl.conf -p symbian -o $@ $(TRACY_ARGS)
egl13-symbian: $(TRACY_TARGET_PATH)/egl13-symbian.tcy
egl13-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/egl13-symbian.tcy
egl13-symbian_dump: egl13-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/egl13-symbian.tcy
.PHONY: egl13-symbian egl13-symbian_clean egl13-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-symbian.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles.conf -p symbian -o $@ $(TRACY_ARGS)
gles11-symbian: $(TRACY_TARGET_PATH)/gles11-symbian.tcy
gles11-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-symbian.tcy
gles11-symbian_dump: gles11-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-symbian.tcy
.PHONY: gles11-symbian gles11-symbian_clean gles11-symbian_dump

#---------------------------------------------------------------------------
# Project gles20-symbian
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-symbian.tcy: $(TRACY_TARGET_PATH) config/gles/v2.0/*
	@$(PYTHON) TracyGenerator.py create config/gles/v2.0/gles.conf -p symbian -o $@ $(TRACY_ARGS)
gles20-symbian: $(TRACY_TARGET_PATH)/gles20-symbian.tcy
gles20-symbian_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-symbian.tcy
gles20-symbian_dump: gles20-symbian
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-symbian.tcy
.PHONY: gles20-symbian gles20-symbian_clean gles20-symbian_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles11-vg11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-win32: $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy
gles11-vg11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy
gles11-vg11-egl13-win32_dump: gles11-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy
.PHONY: gles11-vg11-egl13-win32 gles11-vg11-egl13-win32_clean gles11-vg11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project vg11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-egl/vg11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
vg11-egl13-win32: $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy
vg11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy
vg11-egl13-win32_dump: vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy
.PHONY: vg11-egl13-win32 vg11-egl13-win32_clean vg11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project vg11-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-win32.tcy: $(TRACY_TARGET_PATH) config/vg/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vg/v1.1/vg.conf -p win32 -o $@ $(TRACY_ARGS)
vg11-win32: $(TRACY_TARGET_PATH)/vg11-win32.tcy
vg11-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-win32.tcy
vg11-win32_dump: vg11-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-win32.tcy
.PHONY: vg11-win32 vg11-win32_clean vg11-win32_dump

#---------------------------------------------------------------------------
# Project gles11-vg101-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles-vg-egl.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-win32: $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy
gles11-vg101-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy
gles11-vg101-egl13-win32_dump: gles11-vg101-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy
.PHONY: gles11-vg101-egl13-win32 gles11-vg101-egl13-win32_clean gles11-vg101-egl13-win32_dump

#---------------------------------------------------------------------------
# Project gles11-vg11-vgu11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles11-vg11-vgu11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-win32: $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy
gles11-vg11-vgu11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy
gles11-vg11-vgu11-egl13-win32_dump: gles11-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy
.PHONY: gles11-vg11-vgu11-egl13-win32 gles11-vg11-vgu11-egl13-win32_clean gles11-vg11-vgu11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project vg11-vgu11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/vg-vgu-egl/vg11-vgu11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-win32: $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy
vg11-vgu11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy
vg11-vgu11-egl13-win32_dump: vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy
.PHONY: vg11-vgu11-egl13-win32 vg11-vgu11-egl13-win32_clean vg11-vgu11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project gles20-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles20-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles20-egl13-win32: $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy
gles20-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy
gles20-egl13-win32_dump: gles20-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy
.PHONY: gles20-egl13-win32 gles20-egl13-win32_clean gles20-egl13-win32_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-egl/gles20-vg11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-win32: $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy
gles20-vg11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy
gles20-vg11-egl13-win32_dump: gles20-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy
.PHONY: gles20-vg11-egl13-win32 gles20-vg11-egl13-win32_clean gles20-vg11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project gles11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-egl/gles11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-egl13-win32: $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy
gles11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy
gles11-egl13-win32_dump: gles11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy
.PHONY: gles11-egl13-win32 gles11-egl13-win32_clean gles11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project vgu11-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/vgu11-win32.tcy: $(TRACY_TARGET_PATH) config/vgu/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/vgu/v1.1/vgu.conf -p win32 -o $@ $(TRACY_ARGS)
vgu11-win32: $(TRACY_TARGET_PATH)/vgu11-win32.tcy
vgu11-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/vgu11-win32.tcy
vgu11-win32_dump: vgu11-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/vgu11-win32.tcy
.PHONY: vgu11-win32 vgu11-win32_clean vgu11-win32_dump

#---------------------------------------------------------------------------
# Project gles11-egl11-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles-egl.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-egl11-win32: $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy
gles11-egl11-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy
gles11-egl11-win32_dump: gles11-egl11-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy
.PHONY: gles11-egl11-win32 gles11-egl11-win32_clean gles11-egl11-win32_dump

#---------------------------------------------------------------------------
# Project gles20-vg11-vgu11-egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy: $(TRACY_TARGET_PATH) config/cross-api/gles-vg-vgu-egl/*
	@$(PYTHON) TracyGenerator.py create config/cross-api/gles-vg-vgu-egl/gles20-vg11-vgu11-egl13.conf -p win32 -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-win32: $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy
gles20-vg11-vgu11-egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy
gles20-vg11-vgu11-egl13-win32_dump: gles20-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy
.PHONY: gles20-vg11-vgu11-egl13-win32 gles20-vg11-vgu11-egl13-win32_clean gles20-vg11-vgu11-egl13-win32_dump

#---------------------------------------------------------------------------
# Project egl13-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/egl13-win32.tcy: $(TRACY_TARGET_PATH) config/egl/v1.3/*
	@$(PYTHON) TracyGenerator.py create config/egl/v1.3/egl.conf -p win32 -o $@ $(TRACY_ARGS)
egl13-win32: $(TRACY_TARGET_PATH)/egl13-win32.tcy
egl13-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/egl13-win32.tcy
egl13-win32_dump: egl13-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/egl13-win32.tcy
.PHONY: egl13-win32 egl13-win32_clean egl13-win32_dump

#---------------------------------------------------------------------------
# Project gles11-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles11-win32.tcy: $(TRACY_TARGET_PATH) config/gles/v1.1/*
	@$(PYTHON) TracyGenerator.py create config/gles/v1.1/gles.conf -p win32 -o $@ $(TRACY_ARGS)
gles11-win32: $(TRACY_TARGET_PATH)/gles11-win32.tcy
gles11-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles11-win32.tcy
gles11-win32_dump: gles11-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles11-win32.tcy
.PHONY: gles11-win32 gles11-win32_clean gles11-win32_dump

#---------------------------------------------------------------------------
# Project gles20-win32
#---------------------------------------------------------------------------
$(TRACY_TARGET_PATH)/gles20-win32.tcy: $(TRACY_TARGET_PATH) config/gles/v2.0/*
	@$(PYTHON) TracyGenerator.py create config/gles/v2.0/gles.conf -p win32 -o $@ $(TRACY_ARGS)
gles20-win32: $(TRACY_TARGET_PATH)/gles20-win32.tcy
gles20-win32_clean:
	@-$(RM) -f $(TRACY_TARGET_PATH)/gles20-win32.tcy
gles20-win32_dump: gles20-win32
	@$(PYTHON) TracyGenerator.py show all -c $(TRACY_TARGET_PATH)/gles20-win32.tcy
.PHONY: gles20-win32 gles20-win32_clean gles20-win32_dump







#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-android
gles11-vg11-egl13-tracer-android: gles11-vg11-egl13-tracer-android_build
gles11-vg11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-android
.PHONY: gles11-vg11-egl13-tracer-android gles11-vg11-egl13-tracer-android_build gles11-vg11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-android
vg11-egl13-tracer-android: vg11-egl13-tracer-android_build
vg11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-android
.PHONY: vg11-egl13-tracer-android vg11-egl13-tracer-android_build vg11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-android.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-tracer-android
vg11-tracer-android: vg11-tracer-android_build
vg11-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-tracer-android
.PHONY: vg11-tracer-android vg11-tracer-android_build vg11-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-android
gles11-vg101-egl13-tracer-android: gles11-vg101-egl13-tracer-android_build
gles11-vg101-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-android
.PHONY: gles11-vg101-egl13-tracer-android gles11-vg101-egl13-tracer-android_build gles11-vg101-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-android
gles11-vg11-vgu11-egl13-tracer-android: gles11-vg11-vgu11-egl13-tracer-android_build
gles11-vg11-vgu11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-android
.PHONY: gles11-vg11-vgu11-egl13-tracer-android gles11-vg11-vgu11-egl13-tracer-android_build gles11-vg11-vgu11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-android
vg11-vgu11-egl13-tracer-android: vg11-vgu11-egl13-tracer-android_build
vg11-vgu11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-android
.PHONY: vg11-vgu11-egl13-tracer-android vg11-vgu11-egl13-tracer-android_build vg11-vgu11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-android
gles20-egl13-tracer-android: gles20-egl13-tracer-android_build
gles20-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-android
.PHONY: gles20-egl13-tracer-android gles20-egl13-tracer-android_build gles20-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-android
gles20-vg11-egl13-tracer-android: gles20-vg11-egl13-tracer-android_build
gles20-vg11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-android
.PHONY: gles20-vg11-egl13-tracer-android gles20-vg11-egl13-tracer-android_build gles20-vg11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-android
gles11-egl13-tracer-android: gles11-egl13-tracer-android_build
gles11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-android
.PHONY: gles11-egl13-tracer-android gles11-egl13-tracer-android_build gles11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target vgu11-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-android.tcy generate tracer -o $@ $(TRACY_ARGS)
vgu11-tracer-android_build: $(TRACY_CODEGEN_PATH)/vgu11-tracer-android
vgu11-tracer-android: vgu11-tracer-android_build
vgu11-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-tracer-android
.PHONY: vgu11-tracer-android vgu11-tracer-android_build vgu11-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl11-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-android
gles11-egl11-tracer-android: gles11-egl11-tracer-android_build
gles11-egl11-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-android
.PHONY: gles11-egl11-tracer-android gles11-egl11-tracer-android_build gles11-egl11-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-android
gles20-vg11-vgu11-egl13-tracer-android: gles20-vg11-vgu11-egl13-tracer-android_build
gles20-vg11-vgu11-egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-android
.PHONY: gles20-vg11-vgu11-egl13-tracer-android gles20-vg11-vgu11-egl13-tracer-android_build gles20-vg11-vgu11-egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target egl13-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-android.tcy generate tracer -o $@ $(TRACY_ARGS)
egl13-tracer-android_build: $(TRACY_CODEGEN_PATH)/egl13-tracer-android
egl13-tracer-android: egl13-tracer-android_build
egl13-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-tracer-android
.PHONY: egl13-tracer-android egl13-tracer-android_build egl13-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-tracer-android
gles11-tracer-android: gles11-tracer-android_build
gles11-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-tracer-android
.PHONY: gles11-tracer-android gles11-tracer-android_build gles11-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-android.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-tracer-android
gles20-tracer-android: gles20-tracer-android_build
gles20-tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-tracer-android
.PHONY: gles20-tracer-android gles20-tracer-android_build gles20-tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-android
gles11-vg11-egl13-passthrough_tracer-android: gles11-vg11-egl13-passthrough_tracer-android_build
gles11-vg11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-android
.PHONY: gles11-vg11-egl13-passthrough_tracer-android gles11-vg11-egl13-passthrough_tracer-android_build gles11-vg11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-android
vg11-egl13-passthrough_tracer-android: vg11-egl13-passthrough_tracer-android_build
vg11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-android
.PHONY: vg11-egl13-passthrough_tracer-android vg11-egl13-passthrough_tracer-android_build vg11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-android
vg11-passthrough_tracer-android: vg11-passthrough_tracer-android_build
vg11-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-android
.PHONY: vg11-passthrough_tracer-android vg11-passthrough_tracer-android_build vg11-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-android
gles11-vg101-egl13-passthrough_tracer-android: gles11-vg101-egl13-passthrough_tracer-android_build
gles11-vg101-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-android
.PHONY: gles11-vg101-egl13-passthrough_tracer-android gles11-vg101-egl13-passthrough_tracer-android_build gles11-vg101-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-android
gles11-vg11-vgu11-egl13-passthrough_tracer-android: gles11-vg11-vgu11-egl13-passthrough_tracer-android_build
gles11-vg11-vgu11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-android
.PHONY: gles11-vg11-vgu11-egl13-passthrough_tracer-android gles11-vg11-vgu11-egl13-passthrough_tracer-android_build gles11-vg11-vgu11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-android
vg11-vgu11-egl13-passthrough_tracer-android: vg11-vgu11-egl13-passthrough_tracer-android_build
vg11-vgu11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-android
.PHONY: vg11-vgu11-egl13-passthrough_tracer-android vg11-vgu11-egl13-passthrough_tracer-android_build vg11-vgu11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-android
gles20-egl13-passthrough_tracer-android: gles20-egl13-passthrough_tracer-android_build
gles20-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-android
.PHONY: gles20-egl13-passthrough_tracer-android gles20-egl13-passthrough_tracer-android_build gles20-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-android
gles20-vg11-egl13-passthrough_tracer-android: gles20-vg11-egl13-passthrough_tracer-android_build
gles20-vg11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-android
.PHONY: gles20-vg11-egl13-passthrough_tracer-android gles20-vg11-egl13-passthrough_tracer-android_build gles20-vg11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-android
gles11-egl13-passthrough_tracer-android: gles11-egl13-passthrough_tracer-android_build
gles11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-android
.PHONY: gles11-egl13-passthrough_tracer-android gles11-egl13-passthrough_tracer-android_build gles11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target vgu11-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vgu11-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-android
vgu11-passthrough_tracer-android: vgu11-passthrough_tracer-android_build
vgu11-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-android
.PHONY: vgu11-passthrough_tracer-android vgu11-passthrough_tracer-android_build vgu11-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl11-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-android
gles11-egl11-passthrough_tracer-android: gles11-egl11-passthrough_tracer-android_build
gles11-egl11-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-android
.PHONY: gles11-egl11-passthrough_tracer-android gles11-egl11-passthrough_tracer-android_build gles11-egl11-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-android
gles20-vg11-vgu11-egl13-passthrough_tracer-android: gles20-vg11-vgu11-egl13-passthrough_tracer-android_build
gles20-vg11-vgu11-egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-android
.PHONY: gles20-vg11-vgu11-egl13-passthrough_tracer-android gles20-vg11-vgu11-egl13-passthrough_tracer-android_build gles20-vg11-vgu11-egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target egl13-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
egl13-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-android
egl13-passthrough_tracer-android: egl13-passthrough_tracer-android_build
egl13-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-android
.PHONY: egl13-passthrough_tracer-android egl13-passthrough_tracer-android_build egl13-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-android
gles11-passthrough_tracer-android: gles11-passthrough_tracer-android_build
gles11-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-android
.PHONY: gles11-passthrough_tracer-android gles11-passthrough_tracer-android_build gles11-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles20-passthrough_tracer-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-android.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-passthrough_tracer-android_build: $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-android
gles20-passthrough_tracer-android: gles20-passthrough_tracer-android_build
gles20-passthrough_tracer-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-android
.PHONY: gles20-passthrough_tracer-android gles20-passthrough_tracer-android_build gles20-passthrough_tracer-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-android
gles11-vg11-egl13-player-android: gles11-vg11-egl13-player-android_build
gles11-vg11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-android
.PHONY: gles11-vg11-egl13-player-android gles11-vg11-egl13-player-android_build gles11-vg11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
vg11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-player-android
vg11-egl13-player-android: vg11-egl13-player-android_build
vg11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-player-android
.PHONY: vg11-egl13-player-android vg11-egl13-player-android_build vg11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target vg11-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-android.tcy generate player -o $@ $(TRACY_ARGS)
vg11-player-android_build: $(TRACY_CODEGEN_PATH)/vg11-player-android
vg11-player-android: vg11-player-android_build
vg11-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-player-android
.PHONY: vg11-player-android vg11-player-android_build vg11-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-android
gles11-vg101-egl13-player-android: gles11-vg101-egl13-player-android_build
gles11-vg101-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-android
.PHONY: gles11-vg101-egl13-player-android gles11-vg101-egl13-player-android_build gles11-vg101-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-android
gles11-vg11-vgu11-egl13-player-android: gles11-vg11-vgu11-egl13-player-android_build
gles11-vg11-vgu11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-android
.PHONY: gles11-vg11-vgu11-egl13-player-android gles11-vg11-vgu11-egl13-player-android_build gles11-vg11-vgu11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-android
vg11-vgu11-egl13-player-android: vg11-vgu11-egl13-player-android_build
vg11-vgu11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-android
.PHONY: vg11-vgu11-egl13-player-android vg11-vgu11-egl13-player-android_build vg11-vgu11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles20-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-player-android
gles20-egl13-player-android: gles20-egl13-player-android_build
gles20-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-player-android
.PHONY: gles20-egl13-player-android gles20-egl13-player-android_build gles20-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-android
gles20-vg11-egl13-player-android: gles20-vg11-egl13-player-android_build
gles20-vg11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-android
.PHONY: gles20-vg11-egl13-player-android gles20-vg11-egl13-player-android_build gles20-vg11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-player-android
gles11-egl13-player-android: gles11-egl13-player-android_build
gles11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-player-android
.PHONY: gles11-egl13-player-android gles11-egl13-player-android_build gles11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target vgu11-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-android.tcy generate player -o $@ $(TRACY_ARGS)
vgu11-player-android_build: $(TRACY_CODEGEN_PATH)/vgu11-player-android
vgu11-player-android: vgu11-player-android_build
vgu11-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-player-android
.PHONY: vgu11-player-android vgu11-player-android_build vgu11-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl11-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-player-android
gles11-egl11-player-android: gles11-egl11-player-android_build
gles11-egl11-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-player-android
.PHONY: gles11-egl11-player-android gles11-egl11-player-android_build gles11-egl11-player-android_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-player-android_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-android
gles20-vg11-vgu11-egl13-player-android: gles20-vg11-vgu11-egl13-player-android_build
gles20-vg11-vgu11-egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-android
.PHONY: gles20-vg11-vgu11-egl13-player-android gles20-vg11-vgu11-egl13-player-android_build gles20-vg11-vgu11-egl13-player-android_clean

#---------------------------------------------------------------------------
# Target egl13-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-android.tcy generate player -o $@ $(TRACY_ARGS)
egl13-player-android_build: $(TRACY_CODEGEN_PATH)/egl13-player-android
egl13-player-android: egl13-player-android_build
egl13-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-player-android
.PHONY: egl13-player-android egl13-player-android_build egl13-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-android.tcy generate player -o $@ $(TRACY_ARGS)
gles11-player-android_build: $(TRACY_CODEGEN_PATH)/gles11-player-android
gles11-player-android: gles11-player-android_build
gles11-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-player-android
.PHONY: gles11-player-android gles11-player-android_build gles11-player-android_clean

#---------------------------------------------------------------------------
# Target gles20-player-android
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-player-android: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-android
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-android.tcy generate player -o $@ $(TRACY_ARGS)
gles20-player-android_build: $(TRACY_CODEGEN_PATH)/gles20-player-android
gles20-player-android: gles20-player-android_build
gles20-player-android_clean:
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-player-android
.PHONY: gles20-player-android gles20-player-android_build gles20-player-android_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix && $(MAKE)
gles11-vg11-egl13-tracer-unix: gles11-vg11-egl13-tracer-unix_build
gles11-vg11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-unix
.PHONY: gles11-vg11-egl13-tracer-unix gles11-vg11-egl13-tracer-unix_build gles11-vg11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix && $(MAKE)
vg11-egl13-tracer-unix: vg11-egl13-tracer-unix_build
vg11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-unix
.PHONY: vg11-egl13-tracer-unix vg11-egl13-tracer-unix_build vg11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-tracer-unix && $(MAKE)
vg11-tracer-unix: vg11-tracer-unix_build
vg11-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-tracer-unix
.PHONY: vg11-tracer-unix vg11-tracer-unix_build vg11-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix && $(MAKE)
gles11-vg101-egl13-tracer-unix: gles11-vg101-egl13-tracer-unix_build
gles11-vg101-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-unix
.PHONY: gles11-vg101-egl13-tracer-unix gles11-vg101-egl13-tracer-unix_build gles11-vg101-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix && $(MAKE)
gles11-vg11-vgu11-egl13-tracer-unix: gles11-vg11-vgu11-egl13-tracer-unix_build
gles11-vg11-vgu11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-unix
.PHONY: gles11-vg11-vgu11-egl13-tracer-unix gles11-vg11-vgu11-egl13-tracer-unix_build gles11-vg11-vgu11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix && $(MAKE)
vg11-vgu11-egl13-tracer-unix: vg11-vgu11-egl13-tracer-unix_build
vg11-vgu11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-unix
.PHONY: vg11-vgu11-egl13-tracer-unix vg11-vgu11-egl13-tracer-unix_build vg11-vgu11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix && $(MAKE)
gles20-egl13-tracer-unix: gles20-egl13-tracer-unix_build
gles20-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-unix
.PHONY: gles20-egl13-tracer-unix gles20-egl13-tracer-unix_build gles20-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix && $(MAKE)
gles20-vg11-egl13-tracer-unix: gles20-vg11-egl13-tracer-unix_build
gles20-vg11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-unix
.PHONY: gles20-vg11-egl13-tracer-unix gles20-vg11-egl13-tracer-unix_build gles20-vg11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix && $(MAKE)
gles11-egl13-tracer-unix: gles11-egl13-tracer-unix_build
gles11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-unix
.PHONY: gles11-egl13-tracer-unix gles11-egl13-tracer-unix_build gles11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target vgu11-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
vgu11-tracer-unix_build: $(TRACY_CODEGEN_PATH)/vgu11-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-unix && $(MAKE)
vgu11-tracer-unix: vgu11-tracer-unix_build
vgu11-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-tracer-unix && cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-tracer-unix
.PHONY: vgu11-tracer-unix vgu11-tracer-unix_build vgu11-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl11-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix && $(MAKE)
gles11-egl11-tracer-unix: gles11-egl11-tracer-unix_build
gles11-egl11-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-unix
.PHONY: gles11-egl11-tracer-unix gles11-egl11-tracer-unix_build gles11-egl11-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix && $(MAKE)
gles20-vg11-vgu11-egl13-tracer-unix: gles20-vg11-vgu11-egl13-tracer-unix_build
gles20-vg11-vgu11-egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-unix
.PHONY: gles20-vg11-vgu11-egl13-tracer-unix gles20-vg11-vgu11-egl13-tracer-unix_build gles20-vg11-vgu11-egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target egl13-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
egl13-tracer-unix_build: $(TRACY_CODEGEN_PATH)/egl13-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/egl13-tracer-unix && $(MAKE)
egl13-tracer-unix: egl13-tracer-unix_build
egl13-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-tracer-unix && cd $(TRACY_CODEGEN_PATH)/egl13-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-tracer-unix
.PHONY: egl13-tracer-unix egl13-tracer-unix_build egl13-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-tracer-unix && $(MAKE)
gles11-tracer-unix: gles11-tracer-unix_build
gles11-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-tracer-unix
.PHONY: gles11-tracer-unix gles11-tracer-unix_build gles11-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-unix.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-tracer-unix && $(MAKE)
gles20-tracer-unix: gles20-tracer-unix_build
gles20-tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-tracer-unix
.PHONY: gles20-tracer-unix gles20-tracer-unix_build gles20-tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix && $(MAKE)
gles11-vg11-egl13-passthrough_tracer-unix: gles11-vg11-egl13-passthrough_tracer-unix_build
gles11-vg11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-unix
.PHONY: gles11-vg11-egl13-passthrough_tracer-unix gles11-vg11-egl13-passthrough_tracer-unix_build gles11-vg11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix && $(MAKE)
vg11-egl13-passthrough_tracer-unix: vg11-egl13-passthrough_tracer-unix_build
vg11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-unix
.PHONY: vg11-egl13-passthrough_tracer-unix vg11-egl13-passthrough_tracer-unix_build vg11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix && $(MAKE)
vg11-passthrough_tracer-unix: vg11-passthrough_tracer-unix_build
vg11-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-unix
.PHONY: vg11-passthrough_tracer-unix vg11-passthrough_tracer-unix_build vg11-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix && $(MAKE)
gles11-vg101-egl13-passthrough_tracer-unix: gles11-vg101-egl13-passthrough_tracer-unix_build
gles11-vg101-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-unix
.PHONY: gles11-vg101-egl13-passthrough_tracer-unix gles11-vg101-egl13-passthrough_tracer-unix_build gles11-vg101-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE)
gles11-vg11-vgu11-egl13-passthrough_tracer-unix: gles11-vg11-vgu11-egl13-passthrough_tracer-unix_build
gles11-vg11-vgu11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-unix
.PHONY: gles11-vg11-vgu11-egl13-passthrough_tracer-unix gles11-vg11-vgu11-egl13-passthrough_tracer-unix_build gles11-vg11-vgu11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE)
vg11-vgu11-egl13-passthrough_tracer-unix: vg11-vgu11-egl13-passthrough_tracer-unix_build
vg11-vgu11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-unix
.PHONY: vg11-vgu11-egl13-passthrough_tracer-unix vg11-vgu11-egl13-passthrough_tracer-unix_build vg11-vgu11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix && $(MAKE)
gles20-egl13-passthrough_tracer-unix: gles20-egl13-passthrough_tracer-unix_build
gles20-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-unix
.PHONY: gles20-egl13-passthrough_tracer-unix gles20-egl13-passthrough_tracer-unix_build gles20-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix && $(MAKE)
gles20-vg11-egl13-passthrough_tracer-unix: gles20-vg11-egl13-passthrough_tracer-unix_build
gles20-vg11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-unix
.PHONY: gles20-vg11-egl13-passthrough_tracer-unix gles20-vg11-egl13-passthrough_tracer-unix_build gles20-vg11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix && $(MAKE)
gles11-egl13-passthrough_tracer-unix: gles11-egl13-passthrough_tracer-unix_build
gles11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-unix
.PHONY: gles11-egl13-passthrough_tracer-unix gles11-egl13-passthrough_tracer-unix_build gles11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target vgu11-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vgu11-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix && $(MAKE)
vgu11-passthrough_tracer-unix: vgu11-passthrough_tracer-unix_build
vgu11-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-unix
.PHONY: vgu11-passthrough_tracer-unix vgu11-passthrough_tracer-unix_build vgu11-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl11-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix && $(MAKE)
gles11-egl11-passthrough_tracer-unix: gles11-egl11-passthrough_tracer-unix_build
gles11-egl11-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-unix
.PHONY: gles11-egl11-passthrough_tracer-unix gles11-egl11-passthrough_tracer-unix_build gles11-egl11-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE)
gles20-vg11-vgu11-egl13-passthrough_tracer-unix: gles20-vg11-vgu11-egl13-passthrough_tracer-unix_build
gles20-vg11-vgu11-egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-unix
.PHONY: gles20-vg11-vgu11-egl13-passthrough_tracer-unix gles20-vg11-vgu11-egl13-passthrough_tracer-unix_build gles20-vg11-vgu11-egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target egl13-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
egl13-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix && $(MAKE)
egl13-passthrough_tracer-unix: egl13-passthrough_tracer-unix_build
egl13-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-unix
.PHONY: egl13-passthrough_tracer-unix egl13-passthrough_tracer-unix_build egl13-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix && $(MAKE)
gles11-passthrough_tracer-unix: gles11-passthrough_tracer-unix_build
gles11-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-unix
.PHONY: gles11-passthrough_tracer-unix gles11-passthrough_tracer-unix_build gles11-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles20-passthrough_tracer-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-unix.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-passthrough_tracer-unix_build: $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix && $(MAKE)
gles20-passthrough_tracer-unix: gles20-passthrough_tracer-unix_build
gles20-passthrough_tracer-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix && cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-unix
.PHONY: gles20-passthrough_tracer-unix gles20-passthrough_tracer-unix_build gles20-passthrough_tracer-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix && $(MAKE)
gles11-vg11-egl13-player-unix: gles11-vg11-egl13-player-unix_build
gles11-vg11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-unix
.PHONY: gles11-vg11-egl13-player-unix gles11-vg11-egl13-player-unix_build gles11-vg11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
vg11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix && $(MAKE)
vg11-egl13-player-unix: vg11-egl13-player-unix_build
vg11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-player-unix
.PHONY: vg11-egl13-player-unix vg11-egl13-player-unix_build vg11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target vg11-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-unix.tcy generate player -o $@ $(TRACY_ARGS)
vg11-player-unix_build: $(TRACY_CODEGEN_PATH)/vg11-player-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-player-unix && $(MAKE)
vg11-player-unix: vg11-player-unix_build
vg11-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-player-unix && cd $(TRACY_CODEGEN_PATH)/vg11-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-player-unix
.PHONY: vg11-player-unix vg11-player-unix_build vg11-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix && $(MAKE)
gles11-vg101-egl13-player-unix: gles11-vg101-egl13-player-unix_build
gles11-vg101-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-unix
.PHONY: gles11-vg101-egl13-player-unix gles11-vg101-egl13-player-unix_build gles11-vg101-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix && $(MAKE)
gles11-vg11-vgu11-egl13-player-unix: gles11-vg11-vgu11-egl13-player-unix_build
gles11-vg11-vgu11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-unix
.PHONY: gles11-vg11-vgu11-egl13-player-unix gles11-vg11-vgu11-egl13-player-unix_build gles11-vg11-vgu11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix && $(MAKE)
vg11-vgu11-egl13-player-unix: vg11-vgu11-egl13-player-unix_build
vg11-vgu11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-unix
.PHONY: vg11-vgu11-egl13-player-unix vg11-vgu11-egl13-player-unix_build vg11-vgu11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles20-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix && $(MAKE)
gles20-egl13-player-unix: gles20-egl13-player-unix_build
gles20-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-player-unix
.PHONY: gles20-egl13-player-unix gles20-egl13-player-unix_build gles20-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix && $(MAKE)
gles20-vg11-egl13-player-unix: gles20-vg11-egl13-player-unix_build
gles20-vg11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-unix
.PHONY: gles20-vg11-egl13-player-unix gles20-vg11-egl13-player-unix_build gles20-vg11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix && $(MAKE)
gles11-egl13-player-unix: gles11-egl13-player-unix_build
gles11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-player-unix
.PHONY: gles11-egl13-player-unix gles11-egl13-player-unix_build gles11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target vgu11-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-unix.tcy generate player -o $@ $(TRACY_ARGS)
vgu11-player-unix_build: $(TRACY_CODEGEN_PATH)/vgu11-player-unix
	@cd $(TRACY_CODEGEN_PATH)/vgu11-player-unix && $(MAKE)
vgu11-player-unix: vgu11-player-unix_build
vgu11-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-player-unix && cd $(TRACY_CODEGEN_PATH)/vgu11-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-player-unix
.PHONY: vgu11-player-unix vgu11-player-unix_build vgu11-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl11-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix && $(MAKE)
gles11-egl11-player-unix: gles11-egl11-player-unix_build
gles11-egl11-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-player-unix
.PHONY: gles11-egl11-player-unix gles11-egl11-player-unix_build gles11-egl11-player-unix_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix && $(MAKE)
gles20-vg11-vgu11-egl13-player-unix: gles20-vg11-vgu11-egl13-player-unix_build
gles20-vg11-vgu11-egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-unix
.PHONY: gles20-vg11-vgu11-egl13-player-unix gles20-vg11-vgu11-egl13-player-unix_build gles20-vg11-vgu11-egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target egl13-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-unix.tcy generate player -o $@ $(TRACY_ARGS)
egl13-player-unix_build: $(TRACY_CODEGEN_PATH)/egl13-player-unix
	@cd $(TRACY_CODEGEN_PATH)/egl13-player-unix && $(MAKE)
egl13-player-unix: egl13-player-unix_build
egl13-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-player-unix && cd $(TRACY_CODEGEN_PATH)/egl13-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-player-unix
.PHONY: egl13-player-unix egl13-player-unix_build egl13-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles11-player-unix_build: $(TRACY_CODEGEN_PATH)/gles11-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles11-player-unix && $(MAKE)
gles11-player-unix: gles11-player-unix_build
gles11-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-player-unix && cd $(TRACY_CODEGEN_PATH)/gles11-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-player-unix
.PHONY: gles11-player-unix gles11-player-unix_build gles11-player-unix_clean

#---------------------------------------------------------------------------
# Target gles20-player-unix
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-player-unix: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-unix
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-unix.tcy generate player -o $@ $(TRACY_ARGS)
gles20-player-unix_build: $(TRACY_CODEGEN_PATH)/gles20-player-unix
	@cd $(TRACY_CODEGEN_PATH)/gles20-player-unix && $(MAKE)
gles20-player-unix: gles20-player-unix_build
gles20-player-unix_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-player-unix && cd $(TRACY_CODEGEN_PATH)/gles20-player-unix && $(MAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-player-unix
.PHONY: gles20-player-unix gles20-player-unix_build gles20-player-unix_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg11-egl13-tracer-symbian: gles11-vg11-egl13-tracer-symbian_build
gles11-vg11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-symbian
.PHONY: gles11-vg11-egl13-tracer-symbian gles11-vg11-egl13-tracer-symbian_build gles11-vg11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
vg11-egl13-tracer-symbian: vg11-egl13-tracer-symbian_build
vg11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-symbian
.PHONY: vg11-egl13-tracer-symbian vg11-egl13-tracer-symbian_build vg11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-tracer-symbian/group && $(BUILD_COMMAND)
vg11-tracer-symbian: vg11-tracer-symbian_build
vg11-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-tracer-symbian
.PHONY: vg11-tracer-symbian vg11-tracer-symbian_build vg11-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg101-egl13-tracer-symbian: gles11-vg101-egl13-tracer-symbian_build
gles11-vg101-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-symbian
.PHONY: gles11-vg101-egl13-tracer-symbian gles11-vg101-egl13-tracer-symbian_build gles11-vg101-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg11-vgu11-egl13-tracer-symbian: gles11-vg11-vgu11-egl13-tracer-symbian_build
gles11-vg11-vgu11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-symbian
.PHONY: gles11-vg11-vgu11-egl13-tracer-symbian gles11-vg11-vgu11-egl13-tracer-symbian_build gles11-vg11-vgu11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
vg11-vgu11-egl13-tracer-symbian: vg11-vgu11-egl13-tracer-symbian_build
vg11-vgu11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-symbian
.PHONY: vg11-vgu11-egl13-tracer-symbian vg11-vgu11-egl13-tracer-symbian_build vg11-vgu11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles20-egl13-tracer-symbian: gles20-egl13-tracer-symbian_build
gles20-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-symbian
.PHONY: gles20-egl13-tracer-symbian gles20-egl13-tracer-symbian_build gles20-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles20-vg11-egl13-tracer-symbian: gles20-vg11-egl13-tracer-symbian_build
gles20-vg11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-symbian
.PHONY: gles20-vg11-egl13-tracer-symbian gles20-vg11-egl13-tracer-symbian_build gles20-vg11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles11-egl13-tracer-symbian: gles11-egl13-tracer-symbian_build
gles11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-symbian
.PHONY: gles11-egl13-tracer-symbian gles11-egl13-tracer-symbian_build gles11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vgu11-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
vgu11-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian/group && $(BUILD_COMMAND)
vgu11-tracer-symbian: vgu11-tracer-symbian_build
vgu11-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-tracer-symbian
.PHONY: vgu11-tracer-symbian vgu11-tracer-symbian_build vgu11-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl11-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian/group && $(BUILD_COMMAND)
gles11-egl11-tracer-symbian: gles11-egl11-tracer-symbian_build
gles11-egl11-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-symbian
.PHONY: gles11-egl11-tracer-symbian gles11-egl11-tracer-symbian_build gles11-egl11-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian/group && $(BUILD_COMMAND)
gles20-vg11-vgu11-egl13-tracer-symbian: gles20-vg11-vgu11-egl13-tracer-symbian_build
gles20-vg11-vgu11-egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-symbian
.PHONY: gles20-vg11-vgu11-egl13-tracer-symbian gles20-vg11-vgu11-egl13-tracer-symbian_build gles20-vg11-vgu11-egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target egl13-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
egl13-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/egl13-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/egl13-tracer-symbian/group && $(BUILD_COMMAND)
egl13-tracer-symbian: egl13-tracer-symbian_build
egl13-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/egl13-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-tracer-symbian
.PHONY: egl13-tracer-symbian egl13-tracer-symbian_build egl13-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-tracer-symbian/group && $(BUILD_COMMAND)
gles11-tracer-symbian: gles11-tracer-symbian_build
gles11-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-tracer-symbian
.PHONY: gles11-tracer-symbian gles11-tracer-symbian_build gles11-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-symbian.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-tracer-symbian/group && $(BUILD_COMMAND)
gles20-tracer-symbian: gles20-tracer-symbian_build
gles20-tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-tracer-symbian
.PHONY: gles20-tracer-symbian gles20-tracer-symbian_build gles20-tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg11-egl13-passthrough_tracer-symbian: gles11-vg11-egl13-passthrough_tracer-symbian_build
gles11-vg11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-symbian
.PHONY: gles11-vg11-egl13-passthrough_tracer-symbian gles11-vg11-egl13-passthrough_tracer-symbian_build gles11-vg11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
vg11-egl13-passthrough_tracer-symbian: vg11-egl13-passthrough_tracer-symbian_build
vg11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-symbian
.PHONY: vg11-egl13-passthrough_tracer-symbian vg11-egl13-passthrough_tracer-symbian_build vg11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
vg11-passthrough_tracer-symbian: vg11-passthrough_tracer-symbian_build
vg11-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-symbian
.PHONY: vg11-passthrough_tracer-symbian vg11-passthrough_tracer-symbian_build vg11-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg101-egl13-passthrough_tracer-symbian: gles11-vg101-egl13-passthrough_tracer-symbian_build
gles11-vg101-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-symbian
.PHONY: gles11-vg101-egl13-passthrough_tracer-symbian gles11-vg101-egl13-passthrough_tracer-symbian_build gles11-vg101-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-vg11-vgu11-egl13-passthrough_tracer-symbian: gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_build
gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-symbian
.PHONY: gles11-vg11-vgu11-egl13-passthrough_tracer-symbian gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_build gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
vg11-vgu11-egl13-passthrough_tracer-symbian: vg11-vgu11-egl13-passthrough_tracer-symbian_build
vg11-vgu11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-symbian
.PHONY: vg11-vgu11-egl13-passthrough_tracer-symbian vg11-vgu11-egl13-passthrough_tracer-symbian_build vg11-vgu11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles20-egl13-passthrough_tracer-symbian: gles20-egl13-passthrough_tracer-symbian_build
gles20-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-symbian
.PHONY: gles20-egl13-passthrough_tracer-symbian gles20-egl13-passthrough_tracer-symbian_build gles20-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles20-vg11-egl13-passthrough_tracer-symbian: gles20-vg11-egl13-passthrough_tracer-symbian_build
gles20-vg11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-symbian
.PHONY: gles20-vg11-egl13-passthrough_tracer-symbian gles20-vg11-egl13-passthrough_tracer-symbian_build gles20-vg11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-egl13-passthrough_tracer-symbian: gles11-egl13-passthrough_tracer-symbian_build
gles11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-symbian
.PHONY: gles11-egl13-passthrough_tracer-symbian gles11-egl13-passthrough_tracer-symbian_build gles11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target vgu11-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vgu11-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
vgu11-passthrough_tracer-symbian: vgu11-passthrough_tracer-symbian_build
vgu11-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-symbian
.PHONY: vgu11-passthrough_tracer-symbian vgu11-passthrough_tracer-symbian_build vgu11-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl11-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-egl11-passthrough_tracer-symbian: gles11-egl11-passthrough_tracer-symbian_build
gles11-egl11-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-symbian
.PHONY: gles11-egl11-passthrough_tracer-symbian gles11-egl11-passthrough_tracer-symbian_build gles11-egl11-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles20-vg11-vgu11-egl13-passthrough_tracer-symbian: gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_build
gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-symbian
.PHONY: gles20-vg11-vgu11-egl13-passthrough_tracer-symbian gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_build gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target egl13-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
egl13-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
egl13-passthrough_tracer-symbian: egl13-passthrough_tracer-symbian_build
egl13-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-symbian
.PHONY: egl13-passthrough_tracer-symbian egl13-passthrough_tracer-symbian_build egl13-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles11-passthrough_tracer-symbian: gles11-passthrough_tracer-symbian_build
gles11-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-symbian
.PHONY: gles11-passthrough_tracer-symbian gles11-passthrough_tracer-symbian_build gles11-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-passthrough_tracer-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-symbian.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-passthrough_tracer-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian/group && $(BUILD_COMMAND)
gles20-passthrough_tracer-symbian: gles20-passthrough_tracer-symbian_build
gles20-passthrough_tracer-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-symbian
.PHONY: gles20-passthrough_tracer-symbian gles20-passthrough_tracer-symbian_build gles20-passthrough_tracer-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian/group && $(BUILD_COMMAND)
gles11-vg11-egl13-player-symbian: gles11-vg11-egl13-player-symbian_build
gles11-vg11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-symbian
.PHONY: gles11-vg11-egl13-player-symbian gles11-vg11-egl13-player-symbian_build gles11-vg11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
vg11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian/group && $(BUILD_COMMAND)
vg11-egl13-player-symbian: vg11-egl13-player-symbian_build
vg11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-player-symbian
.PHONY: vg11-egl13-player-symbian vg11-egl13-player-symbian_build vg11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-symbian.tcy generate player -o $@ $(TRACY_ARGS)
vg11-player-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-player-symbian/group && $(BUILD_COMMAND)
vg11-player-symbian: vg11-player-symbian_build
vg11-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-player-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-player-symbian
.PHONY: vg11-player-symbian vg11-player-symbian_build vg11-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian/group && $(BUILD_COMMAND)
gles11-vg101-egl13-player-symbian: gles11-vg101-egl13-player-symbian_build
gles11-vg101-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-symbian
.PHONY: gles11-vg101-egl13-player-symbian gles11-vg101-egl13-player-symbian_build gles11-vg101-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian/group && $(BUILD_COMMAND)
gles11-vg11-vgu11-egl13-player-symbian: gles11-vg11-vgu11-egl13-player-symbian_build
gles11-vg11-vgu11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-symbian
.PHONY: gles11-vg11-vgu11-egl13-player-symbian gles11-vg11-vgu11-egl13-player-symbian_build gles11-vg11-vgu11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian/group && $(BUILD_COMMAND)
vg11-vgu11-egl13-player-symbian: vg11-vgu11-egl13-player-symbian_build
vg11-vgu11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-symbian
.PHONY: vg11-vgu11-egl13-player-symbian vg11-vgu11-egl13-player-symbian_build vg11-vgu11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles20-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian/group && $(BUILD_COMMAND)
gles20-egl13-player-symbian: gles20-egl13-player-symbian_build
gles20-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-player-symbian
.PHONY: gles20-egl13-player-symbian gles20-egl13-player-symbian_build gles20-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian/group && $(BUILD_COMMAND)
gles20-vg11-egl13-player-symbian: gles20-vg11-egl13-player-symbian_build
gles20-vg11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-symbian
.PHONY: gles20-vg11-egl13-player-symbian gles20-vg11-egl13-player-symbian_build gles20-vg11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian/group && $(BUILD_COMMAND)
gles11-egl13-player-symbian: gles11-egl13-player-symbian_build
gles11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-player-symbian
.PHONY: gles11-egl13-player-symbian gles11-egl13-player-symbian_build gles11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target vgu11-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-symbian.tcy generate player -o $@ $(TRACY_ARGS)
vgu11-player-symbian_build: $(TRACY_CODEGEN_PATH)/vgu11-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/vgu11-player-symbian/group && $(BUILD_COMMAND)
vgu11-player-symbian: vgu11-player-symbian_build
vgu11-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-player-symbian && cd $(TRACY_CODEGEN_PATH)/vgu11-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-player-symbian
.PHONY: vgu11-player-symbian vgu11-player-symbian_build vgu11-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl11-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian/group && $(BUILD_COMMAND)
gles11-egl11-player-symbian: gles11-egl11-player-symbian_build
gles11-egl11-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-player-symbian
.PHONY: gles11-egl11-player-symbian gles11-egl11-player-symbian_build gles11-egl11-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian/group && $(BUILD_COMMAND)
gles20-vg11-vgu11-egl13-player-symbian: gles20-vg11-vgu11-egl13-player-symbian_build
gles20-vg11-vgu11-egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-symbian
.PHONY: gles20-vg11-vgu11-egl13-player-symbian gles20-vg11-vgu11-egl13-player-symbian_build gles20-vg11-vgu11-egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target egl13-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-symbian.tcy generate player -o $@ $(TRACY_ARGS)
egl13-player-symbian_build: $(TRACY_CODEGEN_PATH)/egl13-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/egl13-player-symbian/group && $(BUILD_COMMAND)
egl13-player-symbian: egl13-player-symbian_build
egl13-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-player-symbian && cd $(TRACY_CODEGEN_PATH)/egl13-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-player-symbian
.PHONY: egl13-player-symbian egl13-player-symbian_build egl13-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles11-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles11-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles11-player-symbian/group && $(BUILD_COMMAND)
gles11-player-symbian: gles11-player-symbian_build
gles11-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles11-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-player-symbian
.PHONY: gles11-player-symbian gles11-player-symbian_build gles11-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles20-player-symbian
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-player-symbian: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-symbian
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-symbian.tcy generate player -o $@ $(TRACY_ARGS)
gles20-player-symbian_build: $(TRACY_CODEGEN_PATH)/gles20-player-symbian
	 cd $(TRACY_CODEGEN_PATH)/gles20-player-symbian/group && $(BUILD_COMMAND)
gles20-player-symbian: gles20-player-symbian_build
gles20-player-symbian_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-player-symbian && cd $(TRACY_CODEGEN_PATH)/gles20-player-symbian/group && $(CLEANCOMMAND)) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-player-symbian
.PHONY: gles20-player-symbian gles20-player-symbian_build gles20-player-symbian_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32 && $(NMAKE)
gles11-vg11-egl13-tracer-win32: gles11-vg11-egl13-tracer-win32_build
gles11-vg11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-tracer-win32
.PHONY: gles11-vg11-egl13-tracer-win32 gles11-vg11-egl13-tracer-win32_build gles11-vg11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32 && $(NMAKE)
vg11-egl13-tracer-win32: vg11-egl13-tracer-win32_build
vg11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-tracer-win32
.PHONY: vg11-egl13-tracer-win32 vg11-egl13-tracer-win32_build vg11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-tracer-win32 && $(NMAKE)
vg11-tracer-win32: vg11-tracer-win32_build
vg11-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-tracer-win32
.PHONY: vg11-tracer-win32 vg11-tracer-win32_build vg11-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32 && $(NMAKE)
gles11-vg101-egl13-tracer-win32: gles11-vg101-egl13-tracer-win32_build
gles11-vg101-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-tracer-win32
.PHONY: gles11-vg101-egl13-tracer-win32 gles11-vg101-egl13-tracer-win32_build gles11-vg101-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32 && $(NMAKE)
gles11-vg11-vgu11-egl13-tracer-win32: gles11-vg11-vgu11-egl13-tracer-win32_build
gles11-vg11-vgu11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-tracer-win32
.PHONY: gles11-vg11-vgu11-egl13-tracer-win32 gles11-vg11-vgu11-egl13-tracer-win32_build gles11-vg11-vgu11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32 && $(NMAKE)
vg11-vgu11-egl13-tracer-win32: vg11-vgu11-egl13-tracer-win32_build
vg11-vgu11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-tracer-win32
.PHONY: vg11-vgu11-egl13-tracer-win32 vg11-vgu11-egl13-tracer-win32_build vg11-vgu11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32 && $(NMAKE)
gles20-egl13-tracer-win32: gles20-egl13-tracer-win32_build
gles20-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-tracer-win32
.PHONY: gles20-egl13-tracer-win32 gles20-egl13-tracer-win32_build gles20-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32 && $(NMAKE)
gles20-vg11-egl13-tracer-win32: gles20-vg11-egl13-tracer-win32_build
gles20-vg11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-tracer-win32
.PHONY: gles20-vg11-egl13-tracer-win32 gles20-vg11-egl13-tracer-win32_build gles20-vg11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32 && $(NMAKE)
gles11-egl13-tracer-win32: gles11-egl13-tracer-win32_build
gles11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-tracer-win32
.PHONY: gles11-egl13-tracer-win32 gles11-egl13-tracer-win32_build gles11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target vgu11-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
vgu11-tracer-win32_build: $(TRACY_CODEGEN_PATH)/vgu11-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-win32 && $(NMAKE)
vgu11-tracer-win32: vgu11-tracer-win32_build
vgu11-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vgu11-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-tracer-win32
.PHONY: vgu11-tracer-win32 vgu11-tracer-win32_build vgu11-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-egl11-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32 && $(NMAKE)
gles11-egl11-tracer-win32: gles11-egl11-tracer-win32_build
gles11-egl11-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-tracer-win32
.PHONY: gles11-egl11-tracer-win32 gles11-egl11-tracer-win32_build gles11-egl11-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32 && $(NMAKE)
gles20-vg11-vgu11-egl13-tracer-win32: gles20-vg11-vgu11-egl13-tracer-win32_build
gles20-vg11-vgu11-egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-tracer-win32
.PHONY: gles20-vg11-vgu11-egl13-tracer-win32 gles20-vg11-vgu11-egl13-tracer-win32_build gles20-vg11-vgu11-egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target egl13-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
egl13-tracer-win32_build: $(TRACY_CODEGEN_PATH)/egl13-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/egl13-tracer-win32 && $(NMAKE)
egl13-tracer-win32: egl13-tracer-win32_build
egl13-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/egl13-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-tracer-win32
.PHONY: egl13-tracer-win32 egl13-tracer-win32_build egl13-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles11-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-tracer-win32 && $(NMAKE)
gles11-tracer-win32: gles11-tracer-win32_build
gles11-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-tracer-win32
.PHONY: gles11-tracer-win32 gles11-tracer-win32_build gles11-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-win32.tcy generate tracer -o $@ $(TRACY_ARGS)
gles20-tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-tracer-win32 && $(NMAKE)
gles20-tracer-win32: gles20-tracer-win32_build
gles20-tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-tracer-win32
.PHONY: gles20-tracer-win32 gles20-tracer-win32_build gles20-tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32 && $(NMAKE)
gles11-vg11-egl13-passthrough_tracer-win32: gles11-vg11-egl13-passthrough_tracer-win32_build
gles11-vg11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-passthrough_tracer-win32
.PHONY: gles11-vg11-egl13-passthrough_tracer-win32 gles11-vg11-egl13-passthrough_tracer-win32_build gles11-vg11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32 && $(NMAKE)
vg11-egl13-passthrough_tracer-win32: vg11-egl13-passthrough_tracer-win32_build
vg11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-passthrough_tracer-win32
.PHONY: vg11-egl13-passthrough_tracer-win32 vg11-egl13-passthrough_tracer-win32_build vg11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32 && $(NMAKE)
vg11-passthrough_tracer-win32: vg11-passthrough_tracer-win32_build
vg11-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-passthrough_tracer-win32
.PHONY: vg11-passthrough_tracer-win32 vg11-passthrough_tracer-win32_build vg11-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32 && $(NMAKE)
gles11-vg101-egl13-passthrough_tracer-win32: gles11-vg101-egl13-passthrough_tracer-win32_build
gles11-vg101-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-passthrough_tracer-win32
.PHONY: gles11-vg101-egl13-passthrough_tracer-win32 gles11-vg101-egl13-passthrough_tracer-win32_build gles11-vg101-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE)
gles11-vg11-vgu11-egl13-passthrough_tracer-win32: gles11-vg11-vgu11-egl13-passthrough_tracer-win32_build
gles11-vg11-vgu11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-passthrough_tracer-win32
.PHONY: gles11-vg11-vgu11-egl13-passthrough_tracer-win32 gles11-vg11-vgu11-egl13-passthrough_tracer-win32_build gles11-vg11-vgu11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE)
vg11-vgu11-egl13-passthrough_tracer-win32: vg11-vgu11-egl13-passthrough_tracer-win32_build
vg11-vgu11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-passthrough_tracer-win32
.PHONY: vg11-vgu11-egl13-passthrough_tracer-win32 vg11-vgu11-egl13-passthrough_tracer-win32_build vg11-vgu11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32 && $(NMAKE)
gles20-egl13-passthrough_tracer-win32: gles20-egl13-passthrough_tracer-win32_build
gles20-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-passthrough_tracer-win32
.PHONY: gles20-egl13-passthrough_tracer-win32 gles20-egl13-passthrough_tracer-win32_build gles20-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32 && $(NMAKE)
gles20-vg11-egl13-passthrough_tracer-win32: gles20-vg11-egl13-passthrough_tracer-win32_build
gles20-vg11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-passthrough_tracer-win32
.PHONY: gles20-vg11-egl13-passthrough_tracer-win32 gles20-vg11-egl13-passthrough_tracer-win32_build gles20-vg11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32 && $(NMAKE)
gles11-egl13-passthrough_tracer-win32: gles11-egl13-passthrough_tracer-win32_build
gles11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-passthrough_tracer-win32
.PHONY: gles11-egl13-passthrough_tracer-win32 gles11-egl13-passthrough_tracer-win32_build gles11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target vgu11-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
vgu11-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32 && $(NMAKE)
vgu11-passthrough_tracer-win32: vgu11-passthrough_tracer-win32_build
vgu11-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-passthrough_tracer-win32
.PHONY: vgu11-passthrough_tracer-win32 vgu11-passthrough_tracer-win32_build vgu11-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-egl11-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32 && $(NMAKE)
gles11-egl11-passthrough_tracer-win32: gles11-egl11-passthrough_tracer-win32_build
gles11-egl11-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-passthrough_tracer-win32
.PHONY: gles11-egl11-passthrough_tracer-win32 gles11-egl11-passthrough_tracer-win32_build gles11-egl11-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE)
gles20-vg11-vgu11-egl13-passthrough_tracer-win32: gles20-vg11-vgu11-egl13-passthrough_tracer-win32_build
gles20-vg11-vgu11-egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-passthrough_tracer-win32
.PHONY: gles20-vg11-vgu11-egl13-passthrough_tracer-win32 gles20-vg11-vgu11-egl13-passthrough_tracer-win32_build gles20-vg11-vgu11-egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target egl13-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
egl13-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32 && $(NMAKE)
egl13-passthrough_tracer-win32: egl13-passthrough_tracer-win32_build
egl13-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-passthrough_tracer-win32
.PHONY: egl13-passthrough_tracer-win32 egl13-passthrough_tracer-win32_build egl13-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles11-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32 && $(NMAKE)
gles11-passthrough_tracer-win32: gles11-passthrough_tracer-win32_build
gles11-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-passthrough_tracer-win32
.PHONY: gles11-passthrough_tracer-win32 gles11-passthrough_tracer-win32_build gles11-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles20-passthrough_tracer-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-win32.tcy generate passthrough_tracer -o $@ $(TRACY_ARGS)
gles20-passthrough_tracer-win32_build: $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32 && $(NMAKE)
gles20-passthrough_tracer-win32: gles20-passthrough_tracer-win32_build
gles20-passthrough_tracer-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-passthrough_tracer-win32
.PHONY: gles20-passthrough_tracer-win32 gles20-passthrough_tracer-win32_build gles20-passthrough_tracer-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32 && $(NMAKE)
gles11-vg11-egl13-player-win32: gles11-vg11-egl13-player-win32_build
gles11-vg11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-egl13-player-win32
.PHONY: gles11-vg11-egl13-player-win32 gles11-vg11-egl13-player-win32_build gles11-vg11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target vg11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
vg11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32 && $(NMAKE)
vg11-egl13-player-win32: vg11-egl13-player-win32_build
vg11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-egl13-player-win32
.PHONY: vg11-egl13-player-win32 vg11-egl13-player-win32_build vg11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target vg11-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-win32.tcy generate player -o $@ $(TRACY_ARGS)
vg11-player-win32_build: $(TRACY_CODEGEN_PATH)/vg11-player-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-player-win32 && $(NMAKE)
vg11-player-win32: vg11-player-win32_build
vg11-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-player-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-player-win32
.PHONY: vg11-player-win32 vg11-player-win32_build vg11-player-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg101-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg101-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg101-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg101-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32 && $(NMAKE)
gles11-vg101-egl13-player-win32: gles11-vg101-egl13-player-win32_build
gles11-vg101-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg101-egl13-player-win32
.PHONY: gles11-vg101-egl13-player-win32 gles11-vg101-egl13-player-win32_build gles11-vg101-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target gles11-vg11-vgu11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-vg11-vgu11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-vg11-vgu11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32 && $(NMAKE)
gles11-vg11-vgu11-egl13-player-win32: gles11-vg11-vgu11-egl13-player-win32_build
gles11-vg11-vgu11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-vg11-vgu11-egl13-player-win32
.PHONY: gles11-vg11-vgu11-egl13-player-win32 gles11-vg11-vgu11-egl13-player-win32_build gles11-vg11-vgu11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target vg11-vgu11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vg11-vgu11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
vg11-vgu11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32 && $(NMAKE)
vg11-vgu11-egl13-player-win32: vg11-vgu11-egl13-player-win32_build
vg11-vgu11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vg11-vgu11-egl13-player-win32
.PHONY: vg11-vgu11-egl13-player-win32 vg11-vgu11-egl13-player-win32_build vg11-vgu11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target gles20-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles20-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32 && $(NMAKE)
gles20-egl13-player-win32: gles20-egl13-player-win32_build
gles20-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-egl13-player-win32
.PHONY: gles20-egl13-player-win32 gles20-egl13-player-win32_build gles20-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32 && $(NMAKE)
gles20-vg11-egl13-player-win32: gles20-vg11-egl13-player-win32_build
gles20-vg11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-egl13-player-win32
.PHONY: gles20-vg11-egl13-player-win32 gles20-vg11-egl13-player-win32_build gles20-vg11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32 && $(NMAKE)
gles11-egl13-player-win32: gles11-egl13-player-win32_build
gles11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl13-player-win32
.PHONY: gles11-egl13-player-win32 gles11-egl13-player-win32_build gles11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target vgu11-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/vgu11-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl vgu11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/vgu11-win32.tcy generate player -o $@ $(TRACY_ARGS)
vgu11-player-win32_build: $(TRACY_CODEGEN_PATH)/vgu11-player-win32
	@cd $(TRACY_CODEGEN_PATH)/vgu11-player-win32 && $(NMAKE)
vgu11-player-win32: vgu11-player-win32_build
vgu11-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/vgu11-player-win32 && cd $(TRACY_CODEGEN_PATH)/vgu11-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/vgu11-player-win32
.PHONY: vgu11-player-win32 vgu11-player-win32_build vgu11-player-win32_clean

#---------------------------------------------------------------------------
# Target gles11-egl11-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-egl11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-egl11-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-egl11-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32 && $(NMAKE)
gles11-egl11-player-win32: gles11-egl11-player-win32_build
gles11-egl11-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-egl11-player-win32
.PHONY: gles11-egl11-player-win32 gles11-egl11-player-win32_build gles11-egl11-player-win32_clean

#---------------------------------------------------------------------------
# Target gles20-vg11-vgu11-egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-vg11-vgu11-egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-vg11-vgu11-egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles20-vg11-vgu11-egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32 && $(NMAKE)
gles20-vg11-vgu11-egl13-player-win32: gles20-vg11-vgu11-egl13-player-win32_build
gles20-vg11-vgu11-egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-vg11-vgu11-egl13-player-win32
.PHONY: gles20-vg11-vgu11-egl13-player-win32 gles20-vg11-vgu11-egl13-player-win32_build gles20-vg11-vgu11-egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target egl13-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/egl13-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl egl13-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/egl13-win32.tcy generate player -o $@ $(TRACY_ARGS)
egl13-player-win32_build: $(TRACY_CODEGEN_PATH)/egl13-player-win32
	@cd $(TRACY_CODEGEN_PATH)/egl13-player-win32 && $(NMAKE)
egl13-player-win32: egl13-player-win32_build
egl13-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/egl13-player-win32 && cd $(TRACY_CODEGEN_PATH)/egl13-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/egl13-player-win32
.PHONY: egl13-player-win32 egl13-player-win32_build egl13-player-win32_clean

#---------------------------------------------------------------------------
# Target gles11-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles11-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles11-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles11-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles11-player-win32_build: $(TRACY_CODEGEN_PATH)/gles11-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles11-player-win32 && $(NMAKE)
gles11-player-win32: gles11-player-win32_build
gles11-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles11-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles11-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles11-player-win32
.PHONY: gles11-player-win32 gles11-player-win32_build gles11-player-win32_clean

#---------------------------------------------------------------------------
# Target gles20-player-win32
#---------------------------------------------------------------------------
$(TRACY_CODEGEN_PATH)/gles20-player-win32: $(TRACY_CODEGEN_PATH) ../tracer/*.h ../tracer/*.c ../tracer/*.cpp ../tracer/*.inl gles20-win32
	@$(PYTHON) TracyGenerator.py -c $(TRACY_TARGET_PATH)/gles20-win32.tcy generate player -o $@ $(TRACY_ARGS)
gles20-player-win32_build: $(TRACY_CODEGEN_PATH)/gles20-player-win32
	@cd $(TRACY_CODEGEN_PATH)/gles20-player-win32 && $(NMAKE)
gles20-player-win32: gles20-player-win32_build
gles20-player-win32_clean:
	@-(test -d $(TRACY_CODEGEN_PATH)/gles20-player-win32 && cd $(TRACY_CODEGEN_PATH)/gles20-player-win32 && $(NMAKE) clean) || true
	@-$(RM) -rf $(TRACY_CODEGEN_PATH)/gles20-player-win32
.PHONY: gles20-player-win32 gles20-player-win32_build gles20-player-win32_clean

#---------------------------------------------------------------------------
# android targets
#---------------------------------------------------------------------------
projects-android: gles11-vg11-egl13-android vg11-egl13-android vg11-android gles11-vg101-egl13-android gles11-vg11-vgu11-egl13-android vg11-vgu11-egl13-android gles20-egl13-android gles20-vg11-egl13-android gles11-egl13-android vgu11-android gles11-egl11-android gles20-vg11-vgu11-egl13-android egl13-android gles11-android gles20-android
targets-android: gles11-vg11-egl13-tracer-android vg11-egl13-tracer-android vg11-tracer-android gles11-vg101-egl13-tracer-android gles11-vg11-vgu11-egl13-tracer-android vg11-vgu11-egl13-tracer-android gles20-egl13-tracer-android gles20-vg11-egl13-tracer-android gles11-egl13-tracer-android vgu11-tracer-android gles11-egl11-tracer-android gles20-vg11-vgu11-egl13-tracer-android egl13-tracer-android gles11-tracer-android gles20-tracer-android gles11-vg11-egl13-passthrough_tracer-android vg11-egl13-passthrough_tracer-android vg11-passthrough_tracer-android gles11-vg101-egl13-passthrough_tracer-android gles11-vg11-vgu11-egl13-passthrough_tracer-android vg11-vgu11-egl13-passthrough_tracer-android gles20-egl13-passthrough_tracer-android gles20-vg11-egl13-passthrough_tracer-android gles11-egl13-passthrough_tracer-android vgu11-passthrough_tracer-android gles11-egl11-passthrough_tracer-android gles20-vg11-vgu11-egl13-passthrough_tracer-android egl13-passthrough_tracer-android gles11-passthrough_tracer-android gles20-passthrough_tracer-android gles11-vg11-egl13-player-android vg11-egl13-player-android vg11-player-android gles11-vg101-egl13-player-android gles11-vg11-vgu11-egl13-player-android vg11-vgu11-egl13-player-android gles20-egl13-player-android gles20-vg11-egl13-player-android gles11-egl13-player-android vgu11-player-android gles11-egl11-player-android gles20-vg11-vgu11-egl13-player-android egl13-player-android gles11-player-android gles20-player-android
projects-android_clean: gles11-vg11-egl13-android_clean vg11-egl13-android_clean vg11-android_clean gles11-vg101-egl13-android_clean gles11-vg11-vgu11-egl13-android_clean vg11-vgu11-egl13-android_clean gles20-egl13-android_clean gles20-vg11-egl13-android_clean gles11-egl13-android_clean vgu11-android_clean gles11-egl11-android_clean gles20-vg11-vgu11-egl13-android_clean egl13-android_clean gles11-android_clean gles20-android_clean
targets-android_clean: gles11-vg11-egl13-tracer-android_clean vg11-egl13-tracer-android_clean vg11-tracer-android_clean gles11-vg101-egl13-tracer-android_clean gles11-vg11-vgu11-egl13-tracer-android_clean vg11-vgu11-egl13-tracer-android_clean gles20-egl13-tracer-android_clean gles20-vg11-egl13-tracer-android_clean gles11-egl13-tracer-android_clean vgu11-tracer-android_clean gles11-egl11-tracer-android_clean gles20-vg11-vgu11-egl13-tracer-android_clean egl13-tracer-android_clean gles11-tracer-android_clean gles20-tracer-android_clean gles11-vg11-egl13-passthrough_tracer-android_clean vg11-egl13-passthrough_tracer-android_clean vg11-passthrough_tracer-android_clean gles11-vg101-egl13-passthrough_tracer-android_clean gles11-vg11-vgu11-egl13-passthrough_tracer-android_clean vg11-vgu11-egl13-passthrough_tracer-android_clean gles20-egl13-passthrough_tracer-android_clean gles20-vg11-egl13-passthrough_tracer-android_clean gles11-egl13-passthrough_tracer-android_clean vgu11-passthrough_tracer-android_clean gles11-egl11-passthrough_tracer-android_clean gles20-vg11-vgu11-egl13-passthrough_tracer-android_clean egl13-passthrough_tracer-android_clean gles11-passthrough_tracer-android_clean gles20-passthrough_tracer-android_clean gles11-vg11-egl13-player-android_clean vg11-egl13-player-android_clean vg11-player-android_clean gles11-vg101-egl13-player-android_clean gles11-vg11-vgu11-egl13-player-android_clean vg11-vgu11-egl13-player-android_clean gles20-egl13-player-android_clean gles20-vg11-egl13-player-android_clean gles11-egl13-player-android_clean vgu11-player-android_clean gles11-egl11-player-android_clean gles20-vg11-vgu11-egl13-player-android_clean egl13-player-android_clean gles11-player-android_clean gles20-player-android_clean
android: projects-android targets-android
android_clean: projects-android_clean targets-android_clean
.PHONY: projects-android targets-android projects-android_clean targets-android_clean android android_clean

#---------------------------------------------------------------------------
# unix targets
#---------------------------------------------------------------------------
projects-unix: gles11-vg11-egl13-unix vg11-egl13-unix vg11-unix gles11-vg101-egl13-unix gles11-vg11-vgu11-egl13-unix vg11-vgu11-egl13-unix gles20-egl13-unix gles20-vg11-egl13-unix gles11-egl13-unix vgu11-unix gles11-egl11-unix gles20-vg11-vgu11-egl13-unix egl13-unix gles11-unix gles20-unix
targets-unix: gles11-vg11-egl13-tracer-unix vg11-egl13-tracer-unix vg11-tracer-unix gles11-vg101-egl13-tracer-unix gles11-vg11-vgu11-egl13-tracer-unix vg11-vgu11-egl13-tracer-unix gles20-egl13-tracer-unix gles20-vg11-egl13-tracer-unix gles11-egl13-tracer-unix vgu11-tracer-unix gles11-egl11-tracer-unix gles20-vg11-vgu11-egl13-tracer-unix egl13-tracer-unix gles11-tracer-unix gles20-tracer-unix gles11-vg11-egl13-passthrough_tracer-unix vg11-egl13-passthrough_tracer-unix vg11-passthrough_tracer-unix gles11-vg101-egl13-passthrough_tracer-unix gles11-vg11-vgu11-egl13-passthrough_tracer-unix vg11-vgu11-egl13-passthrough_tracer-unix gles20-egl13-passthrough_tracer-unix gles20-vg11-egl13-passthrough_tracer-unix gles11-egl13-passthrough_tracer-unix vgu11-passthrough_tracer-unix gles11-egl11-passthrough_tracer-unix gles20-vg11-vgu11-egl13-passthrough_tracer-unix egl13-passthrough_tracer-unix gles11-passthrough_tracer-unix gles20-passthrough_tracer-unix gles11-vg11-egl13-player-unix vg11-egl13-player-unix vg11-player-unix gles11-vg101-egl13-player-unix gles11-vg11-vgu11-egl13-player-unix vg11-vgu11-egl13-player-unix gles20-egl13-player-unix gles20-vg11-egl13-player-unix gles11-egl13-player-unix vgu11-player-unix gles11-egl11-player-unix gles20-vg11-vgu11-egl13-player-unix egl13-player-unix gles11-player-unix gles20-player-unix
projects-unix_clean: gles11-vg11-egl13-unix_clean vg11-egl13-unix_clean vg11-unix_clean gles11-vg101-egl13-unix_clean gles11-vg11-vgu11-egl13-unix_clean vg11-vgu11-egl13-unix_clean gles20-egl13-unix_clean gles20-vg11-egl13-unix_clean gles11-egl13-unix_clean vgu11-unix_clean gles11-egl11-unix_clean gles20-vg11-vgu11-egl13-unix_clean egl13-unix_clean gles11-unix_clean gles20-unix_clean
targets-unix_clean: gles11-vg11-egl13-tracer-unix_clean vg11-egl13-tracer-unix_clean vg11-tracer-unix_clean gles11-vg101-egl13-tracer-unix_clean gles11-vg11-vgu11-egl13-tracer-unix_clean vg11-vgu11-egl13-tracer-unix_clean gles20-egl13-tracer-unix_clean gles20-vg11-egl13-tracer-unix_clean gles11-egl13-tracer-unix_clean vgu11-tracer-unix_clean gles11-egl11-tracer-unix_clean gles20-vg11-vgu11-egl13-tracer-unix_clean egl13-tracer-unix_clean gles11-tracer-unix_clean gles20-tracer-unix_clean gles11-vg11-egl13-passthrough_tracer-unix_clean vg11-egl13-passthrough_tracer-unix_clean vg11-passthrough_tracer-unix_clean gles11-vg101-egl13-passthrough_tracer-unix_clean gles11-vg11-vgu11-egl13-passthrough_tracer-unix_clean vg11-vgu11-egl13-passthrough_tracer-unix_clean gles20-egl13-passthrough_tracer-unix_clean gles20-vg11-egl13-passthrough_tracer-unix_clean gles11-egl13-passthrough_tracer-unix_clean vgu11-passthrough_tracer-unix_clean gles11-egl11-passthrough_tracer-unix_clean gles20-vg11-vgu11-egl13-passthrough_tracer-unix_clean egl13-passthrough_tracer-unix_clean gles11-passthrough_tracer-unix_clean gles20-passthrough_tracer-unix_clean gles11-vg11-egl13-player-unix_clean vg11-egl13-player-unix_clean vg11-player-unix_clean gles11-vg101-egl13-player-unix_clean gles11-vg11-vgu11-egl13-player-unix_clean vg11-vgu11-egl13-player-unix_clean gles20-egl13-player-unix_clean gles20-vg11-egl13-player-unix_clean gles11-egl13-player-unix_clean vgu11-player-unix_clean gles11-egl11-player-unix_clean gles20-vg11-vgu11-egl13-player-unix_clean egl13-player-unix_clean gles11-player-unix_clean gles20-player-unix_clean
unix: projects-unix targets-unix
unix_clean: projects-unix_clean targets-unix_clean
.PHONY: projects-unix targets-unix projects-unix_clean targets-unix_clean unix unix_clean

#---------------------------------------------------------------------------
# symbian targets
#---------------------------------------------------------------------------
projects-symbian: gles11-vg11-egl13-symbian vg11-egl13-symbian vg11-symbian gles11-vg101-egl13-symbian gles11-vg11-vgu11-egl13-symbian vg11-vgu11-egl13-symbian gles20-egl13-symbian gles20-vg11-egl13-symbian gles11-egl13-symbian vgu11-symbian gles11-egl11-symbian gles20-vg11-vgu11-egl13-symbian egl13-symbian gles11-symbian gles20-symbian
targets-symbian: gles11-vg11-egl13-tracer-symbian vg11-egl13-tracer-symbian vg11-tracer-symbian gles11-vg101-egl13-tracer-symbian gles11-vg11-vgu11-egl13-tracer-symbian vg11-vgu11-egl13-tracer-symbian gles20-egl13-tracer-symbian gles20-vg11-egl13-tracer-symbian gles11-egl13-tracer-symbian vgu11-tracer-symbian gles11-egl11-tracer-symbian gles20-vg11-vgu11-egl13-tracer-symbian egl13-tracer-symbian gles11-tracer-symbian gles20-tracer-symbian gles11-vg11-egl13-passthrough_tracer-symbian vg11-egl13-passthrough_tracer-symbian vg11-passthrough_tracer-symbian gles11-vg101-egl13-passthrough_tracer-symbian gles11-vg11-vgu11-egl13-passthrough_tracer-symbian vg11-vgu11-egl13-passthrough_tracer-symbian gles20-egl13-passthrough_tracer-symbian gles20-vg11-egl13-passthrough_tracer-symbian gles11-egl13-passthrough_tracer-symbian vgu11-passthrough_tracer-symbian gles11-egl11-passthrough_tracer-symbian gles20-vg11-vgu11-egl13-passthrough_tracer-symbian egl13-passthrough_tracer-symbian gles11-passthrough_tracer-symbian gles20-passthrough_tracer-symbian gles11-vg11-egl13-player-symbian vg11-egl13-player-symbian vg11-player-symbian gles11-vg101-egl13-player-symbian gles11-vg11-vgu11-egl13-player-symbian vg11-vgu11-egl13-player-symbian gles20-egl13-player-symbian gles20-vg11-egl13-player-symbian gles11-egl13-player-symbian vgu11-player-symbian gles11-egl11-player-symbian gles20-vg11-vgu11-egl13-player-symbian egl13-player-symbian gles11-player-symbian gles20-player-symbian
projects-symbian_clean: gles11-vg11-egl13-symbian_clean vg11-egl13-symbian_clean vg11-symbian_clean gles11-vg101-egl13-symbian_clean gles11-vg11-vgu11-egl13-symbian_clean vg11-vgu11-egl13-symbian_clean gles20-egl13-symbian_clean gles20-vg11-egl13-symbian_clean gles11-egl13-symbian_clean vgu11-symbian_clean gles11-egl11-symbian_clean gles20-vg11-vgu11-egl13-symbian_clean egl13-symbian_clean gles11-symbian_clean gles20-symbian_clean
targets-symbian_clean: gles11-vg11-egl13-tracer-symbian_clean vg11-egl13-tracer-symbian_clean vg11-tracer-symbian_clean gles11-vg101-egl13-tracer-symbian_clean gles11-vg11-vgu11-egl13-tracer-symbian_clean vg11-vgu11-egl13-tracer-symbian_clean gles20-egl13-tracer-symbian_clean gles20-vg11-egl13-tracer-symbian_clean gles11-egl13-tracer-symbian_clean vgu11-tracer-symbian_clean gles11-egl11-tracer-symbian_clean gles20-vg11-vgu11-egl13-tracer-symbian_clean egl13-tracer-symbian_clean gles11-tracer-symbian_clean gles20-tracer-symbian_clean gles11-vg11-egl13-passthrough_tracer-symbian_clean vg11-egl13-passthrough_tracer-symbian_clean vg11-passthrough_tracer-symbian_clean gles11-vg101-egl13-passthrough_tracer-symbian_clean gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_clean vg11-vgu11-egl13-passthrough_tracer-symbian_clean gles20-egl13-passthrough_tracer-symbian_clean gles20-vg11-egl13-passthrough_tracer-symbian_clean gles11-egl13-passthrough_tracer-symbian_clean vgu11-passthrough_tracer-symbian_clean gles11-egl11-passthrough_tracer-symbian_clean gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_clean egl13-passthrough_tracer-symbian_clean gles11-passthrough_tracer-symbian_clean gles20-passthrough_tracer-symbian_clean gles11-vg11-egl13-player-symbian_clean vg11-egl13-player-symbian_clean vg11-player-symbian_clean gles11-vg101-egl13-player-symbian_clean gles11-vg11-vgu11-egl13-player-symbian_clean vg11-vgu11-egl13-player-symbian_clean gles20-egl13-player-symbian_clean gles20-vg11-egl13-player-symbian_clean gles11-egl13-player-symbian_clean vgu11-player-symbian_clean gles11-egl11-player-symbian_clean gles20-vg11-vgu11-egl13-player-symbian_clean egl13-player-symbian_clean gles11-player-symbian_clean gles20-player-symbian_clean
symbian: projects-symbian targets-symbian
symbian_clean: projects-symbian_clean targets-symbian_clean
.PHONY: projects-symbian targets-symbian projects-symbian_clean targets-symbian_clean symbian symbian_clean

#---------------------------------------------------------------------------
# win32 targets
#---------------------------------------------------------------------------
projects-win32: gles11-vg11-egl13-win32 vg11-egl13-win32 vg11-win32 gles11-vg101-egl13-win32 gles11-vg11-vgu11-egl13-win32 vg11-vgu11-egl13-win32 gles20-egl13-win32 gles20-vg11-egl13-win32 gles11-egl13-win32 vgu11-win32 gles11-egl11-win32 gles20-vg11-vgu11-egl13-win32 egl13-win32 gles11-win32 gles20-win32
targets-win32: gles11-vg11-egl13-tracer-win32 vg11-egl13-tracer-win32 vg11-tracer-win32 gles11-vg101-egl13-tracer-win32 gles11-vg11-vgu11-egl13-tracer-win32 vg11-vgu11-egl13-tracer-win32 gles20-egl13-tracer-win32 gles20-vg11-egl13-tracer-win32 gles11-egl13-tracer-win32 vgu11-tracer-win32 gles11-egl11-tracer-win32 gles20-vg11-vgu11-egl13-tracer-win32 egl13-tracer-win32 gles11-tracer-win32 gles20-tracer-win32 gles11-vg11-egl13-passthrough_tracer-win32 vg11-egl13-passthrough_tracer-win32 vg11-passthrough_tracer-win32 gles11-vg101-egl13-passthrough_tracer-win32 gles11-vg11-vgu11-egl13-passthrough_tracer-win32 vg11-vgu11-egl13-passthrough_tracer-win32 gles20-egl13-passthrough_tracer-win32 gles20-vg11-egl13-passthrough_tracer-win32 gles11-egl13-passthrough_tracer-win32 vgu11-passthrough_tracer-win32 gles11-egl11-passthrough_tracer-win32 gles20-vg11-vgu11-egl13-passthrough_tracer-win32 egl13-passthrough_tracer-win32 gles11-passthrough_tracer-win32 gles20-passthrough_tracer-win32 gles11-vg11-egl13-player-win32 vg11-egl13-player-win32 vg11-player-win32 gles11-vg101-egl13-player-win32 gles11-vg11-vgu11-egl13-player-win32 vg11-vgu11-egl13-player-win32 gles20-egl13-player-win32 gles20-vg11-egl13-player-win32 gles11-egl13-player-win32 vgu11-player-win32 gles11-egl11-player-win32 gles20-vg11-vgu11-egl13-player-win32 egl13-player-win32 gles11-player-win32 gles20-player-win32
projects-win32_clean: gles11-vg11-egl13-win32_clean vg11-egl13-win32_clean vg11-win32_clean gles11-vg101-egl13-win32_clean gles11-vg11-vgu11-egl13-win32_clean vg11-vgu11-egl13-win32_clean gles20-egl13-win32_clean gles20-vg11-egl13-win32_clean gles11-egl13-win32_clean vgu11-win32_clean gles11-egl11-win32_clean gles20-vg11-vgu11-egl13-win32_clean egl13-win32_clean gles11-win32_clean gles20-win32_clean
targets-win32_clean: gles11-vg11-egl13-tracer-win32_clean vg11-egl13-tracer-win32_clean vg11-tracer-win32_clean gles11-vg101-egl13-tracer-win32_clean gles11-vg11-vgu11-egl13-tracer-win32_clean vg11-vgu11-egl13-tracer-win32_clean gles20-egl13-tracer-win32_clean gles20-vg11-egl13-tracer-win32_clean gles11-egl13-tracer-win32_clean vgu11-tracer-win32_clean gles11-egl11-tracer-win32_clean gles20-vg11-vgu11-egl13-tracer-win32_clean egl13-tracer-win32_clean gles11-tracer-win32_clean gles20-tracer-win32_clean gles11-vg11-egl13-passthrough_tracer-win32_clean vg11-egl13-passthrough_tracer-win32_clean vg11-passthrough_tracer-win32_clean gles11-vg101-egl13-passthrough_tracer-win32_clean gles11-vg11-vgu11-egl13-passthrough_tracer-win32_clean vg11-vgu11-egl13-passthrough_tracer-win32_clean gles20-egl13-passthrough_tracer-win32_clean gles20-vg11-egl13-passthrough_tracer-win32_clean gles11-egl13-passthrough_tracer-win32_clean vgu11-passthrough_tracer-win32_clean gles11-egl11-passthrough_tracer-win32_clean gles20-vg11-vgu11-egl13-passthrough_tracer-win32_clean egl13-passthrough_tracer-win32_clean gles11-passthrough_tracer-win32_clean gles20-passthrough_tracer-win32_clean gles11-vg11-egl13-player-win32_clean vg11-egl13-player-win32_clean vg11-player-win32_clean gles11-vg101-egl13-player-win32_clean gles11-vg11-vgu11-egl13-player-win32_clean vg11-vgu11-egl13-player-win32_clean gles20-egl13-player-win32_clean gles20-vg11-egl13-player-win32_clean gles11-egl13-player-win32_clean vgu11-player-win32_clean gles11-egl11-player-win32_clean gles20-vg11-vgu11-egl13-player-win32_clean egl13-player-win32_clean gles11-player-win32_clean gles20-player-win32_clean
win32: projects-win32 targets-win32
win32_clean: projects-win32_clean targets-win32_clean
.PHONY: projects-win32 targets-win32 projects-win32_clean targets-win32_clean win32 win32_clean

#---------------------------------------------------------------------------
# tracer targets
#---------------------------------------------------------------------------
tracers: gles11-vg11-egl13-tracer-android vg11-egl13-tracer-android vg11-tracer-android gles11-vg101-egl13-tracer-android gles11-vg11-vgu11-egl13-tracer-android vg11-vgu11-egl13-tracer-android gles20-egl13-tracer-android gles20-vg11-egl13-tracer-android gles11-egl13-tracer-android vgu11-tracer-android gles11-egl11-tracer-android gles20-vg11-vgu11-egl13-tracer-android egl13-tracer-android gles11-tracer-android gles20-tracer-android gles11-vg11-egl13-tracer-unix vg11-egl13-tracer-unix vg11-tracer-unix gles11-vg101-egl13-tracer-unix gles11-vg11-vgu11-egl13-tracer-unix vg11-vgu11-egl13-tracer-unix gles20-egl13-tracer-unix gles20-vg11-egl13-tracer-unix gles11-egl13-tracer-unix vgu11-tracer-unix gles11-egl11-tracer-unix gles20-vg11-vgu11-egl13-tracer-unix egl13-tracer-unix gles11-tracer-unix gles20-tracer-unix gles11-vg11-egl13-tracer-symbian vg11-egl13-tracer-symbian vg11-tracer-symbian gles11-vg101-egl13-tracer-symbian gles11-vg11-vgu11-egl13-tracer-symbian vg11-vgu11-egl13-tracer-symbian gles20-egl13-tracer-symbian gles20-vg11-egl13-tracer-symbian gles11-egl13-tracer-symbian vgu11-tracer-symbian gles11-egl11-tracer-symbian gles20-vg11-vgu11-egl13-tracer-symbian egl13-tracer-symbian gles11-tracer-symbian gles20-tracer-symbian gles11-vg11-egl13-tracer-win32 vg11-egl13-tracer-win32 vg11-tracer-win32 gles11-vg101-egl13-tracer-win32 gles11-vg11-vgu11-egl13-tracer-win32 vg11-vgu11-egl13-tracer-win32 gles20-egl13-tracer-win32 gles20-vg11-egl13-tracer-win32 gles11-egl13-tracer-win32 vgu11-tracer-win32 gles11-egl11-tracer-win32 gles20-vg11-vgu11-egl13-tracer-win32 egl13-tracer-win32 gles11-tracer-win32 gles20-tracer-win32
tracers_clean: gles11-vg11-egl13-tracer-android_clean vg11-egl13-tracer-android_clean vg11-tracer-android_clean gles11-vg101-egl13-tracer-android_clean gles11-vg11-vgu11-egl13-tracer-android_clean vg11-vgu11-egl13-tracer-android_clean gles20-egl13-tracer-android_clean gles20-vg11-egl13-tracer-android_clean gles11-egl13-tracer-android_clean vgu11-tracer-android_clean gles11-egl11-tracer-android_clean gles20-vg11-vgu11-egl13-tracer-android_clean egl13-tracer-android_clean gles11-tracer-android_clean gles20-tracer-android_clean gles11-vg11-egl13-tracer-unix_clean vg11-egl13-tracer-unix_clean vg11-tracer-unix_clean gles11-vg101-egl13-tracer-unix_clean gles11-vg11-vgu11-egl13-tracer-unix_clean vg11-vgu11-egl13-tracer-unix_clean gles20-egl13-tracer-unix_clean gles20-vg11-egl13-tracer-unix_clean gles11-egl13-tracer-unix_clean vgu11-tracer-unix_clean gles11-egl11-tracer-unix_clean gles20-vg11-vgu11-egl13-tracer-unix_clean egl13-tracer-unix_clean gles11-tracer-unix_clean gles20-tracer-unix_clean gles11-vg11-egl13-tracer-symbian_clean vg11-egl13-tracer-symbian_clean vg11-tracer-symbian_clean gles11-vg101-egl13-tracer-symbian_clean gles11-vg11-vgu11-egl13-tracer-symbian_clean vg11-vgu11-egl13-tracer-symbian_clean gles20-egl13-tracer-symbian_clean gles20-vg11-egl13-tracer-symbian_clean gles11-egl13-tracer-symbian_clean vgu11-tracer-symbian_clean gles11-egl11-tracer-symbian_clean gles20-vg11-vgu11-egl13-tracer-symbian_clean egl13-tracer-symbian_clean gles11-tracer-symbian_clean gles20-tracer-symbian_clean gles11-vg11-egl13-tracer-win32_clean vg11-egl13-tracer-win32_clean vg11-tracer-win32_clean gles11-vg101-egl13-tracer-win32_clean gles11-vg11-vgu11-egl13-tracer-win32_clean vg11-vgu11-egl13-tracer-win32_clean gles20-egl13-tracer-win32_clean gles20-vg11-egl13-tracer-win32_clean gles11-egl13-tracer-win32_clean vgu11-tracer-win32_clean gles11-egl11-tracer-win32_clean gles20-vg11-vgu11-egl13-tracer-win32_clean egl13-tracer-win32_clean gles11-tracer-win32_clean gles20-tracer-win32_clean
.PHONY: tracers tracers_clean

#---------------------------------------------------------------------------
# passthrough_tracer targets
#---------------------------------------------------------------------------
passthrough_tracers: gles11-vg11-egl13-passthrough_tracer-android vg11-egl13-passthrough_tracer-android vg11-passthrough_tracer-android gles11-vg101-egl13-passthrough_tracer-android gles11-vg11-vgu11-egl13-passthrough_tracer-android vg11-vgu11-egl13-passthrough_tracer-android gles20-egl13-passthrough_tracer-android gles20-vg11-egl13-passthrough_tracer-android gles11-egl13-passthrough_tracer-android vgu11-passthrough_tracer-android gles11-egl11-passthrough_tracer-android gles20-vg11-vgu11-egl13-passthrough_tracer-android egl13-passthrough_tracer-android gles11-passthrough_tracer-android gles20-passthrough_tracer-android gles11-vg11-egl13-passthrough_tracer-unix vg11-egl13-passthrough_tracer-unix vg11-passthrough_tracer-unix gles11-vg101-egl13-passthrough_tracer-unix gles11-vg11-vgu11-egl13-passthrough_tracer-unix vg11-vgu11-egl13-passthrough_tracer-unix gles20-egl13-passthrough_tracer-unix gles20-vg11-egl13-passthrough_tracer-unix gles11-egl13-passthrough_tracer-unix vgu11-passthrough_tracer-unix gles11-egl11-passthrough_tracer-unix gles20-vg11-vgu11-egl13-passthrough_tracer-unix egl13-passthrough_tracer-unix gles11-passthrough_tracer-unix gles20-passthrough_tracer-unix gles11-vg11-egl13-passthrough_tracer-symbian vg11-egl13-passthrough_tracer-symbian vg11-passthrough_tracer-symbian gles11-vg101-egl13-passthrough_tracer-symbian gles11-vg11-vgu11-egl13-passthrough_tracer-symbian vg11-vgu11-egl13-passthrough_tracer-symbian gles20-egl13-passthrough_tracer-symbian gles20-vg11-egl13-passthrough_tracer-symbian gles11-egl13-passthrough_tracer-symbian vgu11-passthrough_tracer-symbian gles11-egl11-passthrough_tracer-symbian gles20-vg11-vgu11-egl13-passthrough_tracer-symbian egl13-passthrough_tracer-symbian gles11-passthrough_tracer-symbian gles20-passthrough_tracer-symbian gles11-vg11-egl13-passthrough_tracer-win32 vg11-egl13-passthrough_tracer-win32 vg11-passthrough_tracer-win32 gles11-vg101-egl13-passthrough_tracer-win32 gles11-vg11-vgu11-egl13-passthrough_tracer-win32 vg11-vgu11-egl13-passthrough_tracer-win32 gles20-egl13-passthrough_tracer-win32 gles20-vg11-egl13-passthrough_tracer-win32 gles11-egl13-passthrough_tracer-win32 vgu11-passthrough_tracer-win32 gles11-egl11-passthrough_tracer-win32 gles20-vg11-vgu11-egl13-passthrough_tracer-win32 egl13-passthrough_tracer-win32 gles11-passthrough_tracer-win32 gles20-passthrough_tracer-win32
passthrough_tracers_clean: gles11-vg11-egl13-passthrough_tracer-android_clean vg11-egl13-passthrough_tracer-android_clean vg11-passthrough_tracer-android_clean gles11-vg101-egl13-passthrough_tracer-android_clean gles11-vg11-vgu11-egl13-passthrough_tracer-android_clean vg11-vgu11-egl13-passthrough_tracer-android_clean gles20-egl13-passthrough_tracer-android_clean gles20-vg11-egl13-passthrough_tracer-android_clean gles11-egl13-passthrough_tracer-android_clean vgu11-passthrough_tracer-android_clean gles11-egl11-passthrough_tracer-android_clean gles20-vg11-vgu11-egl13-passthrough_tracer-android_clean egl13-passthrough_tracer-android_clean gles11-passthrough_tracer-android_clean gles20-passthrough_tracer-android_clean gles11-vg11-egl13-passthrough_tracer-unix_clean vg11-egl13-passthrough_tracer-unix_clean vg11-passthrough_tracer-unix_clean gles11-vg101-egl13-passthrough_tracer-unix_clean gles11-vg11-vgu11-egl13-passthrough_tracer-unix_clean vg11-vgu11-egl13-passthrough_tracer-unix_clean gles20-egl13-passthrough_tracer-unix_clean gles20-vg11-egl13-passthrough_tracer-unix_clean gles11-egl13-passthrough_tracer-unix_clean vgu11-passthrough_tracer-unix_clean gles11-egl11-passthrough_tracer-unix_clean gles20-vg11-vgu11-egl13-passthrough_tracer-unix_clean egl13-passthrough_tracer-unix_clean gles11-passthrough_tracer-unix_clean gles20-passthrough_tracer-unix_clean gles11-vg11-egl13-passthrough_tracer-symbian_clean vg11-egl13-passthrough_tracer-symbian_clean vg11-passthrough_tracer-symbian_clean gles11-vg101-egl13-passthrough_tracer-symbian_clean gles11-vg11-vgu11-egl13-passthrough_tracer-symbian_clean vg11-vgu11-egl13-passthrough_tracer-symbian_clean gles20-egl13-passthrough_tracer-symbian_clean gles20-vg11-egl13-passthrough_tracer-symbian_clean gles11-egl13-passthrough_tracer-symbian_clean vgu11-passthrough_tracer-symbian_clean gles11-egl11-passthrough_tracer-symbian_clean gles20-vg11-vgu11-egl13-passthrough_tracer-symbian_clean egl13-passthrough_tracer-symbian_clean gles11-passthrough_tracer-symbian_clean gles20-passthrough_tracer-symbian_clean gles11-vg11-egl13-passthrough_tracer-win32_clean vg11-egl13-passthrough_tracer-win32_clean vg11-passthrough_tracer-win32_clean gles11-vg101-egl13-passthrough_tracer-win32_clean gles11-vg11-vgu11-egl13-passthrough_tracer-win32_clean vg11-vgu11-egl13-passthrough_tracer-win32_clean gles20-egl13-passthrough_tracer-win32_clean gles20-vg11-egl13-passthrough_tracer-win32_clean gles11-egl13-passthrough_tracer-win32_clean vgu11-passthrough_tracer-win32_clean gles11-egl11-passthrough_tracer-win32_clean gles20-vg11-vgu11-egl13-passthrough_tracer-win32_clean egl13-passthrough_tracer-win32_clean gles11-passthrough_tracer-win32_clean gles20-passthrough_tracer-win32_clean
.PHONY: passthrough_tracers passthrough_tracers_clean

#---------------------------------------------------------------------------
# player targets
#---------------------------------------------------------------------------
players: gles11-vg11-egl13-player-android vg11-egl13-player-android vg11-player-android gles11-vg101-egl13-player-android gles11-vg11-vgu11-egl13-player-android vg11-vgu11-egl13-player-android gles20-egl13-player-android gles20-vg11-egl13-player-android gles11-egl13-player-android vgu11-player-android gles11-egl11-player-android gles20-vg11-vgu11-egl13-player-android egl13-player-android gles11-player-android gles20-player-android gles11-vg11-egl13-player-unix vg11-egl13-player-unix vg11-player-unix gles11-vg101-egl13-player-unix gles11-vg11-vgu11-egl13-player-unix vg11-vgu11-egl13-player-unix gles20-egl13-player-unix gles20-vg11-egl13-player-unix gles11-egl13-player-unix vgu11-player-unix gles11-egl11-player-unix gles20-vg11-vgu11-egl13-player-unix egl13-player-unix gles11-player-unix gles20-player-unix gles11-vg11-egl13-player-symbian vg11-egl13-player-symbian vg11-player-symbian gles11-vg101-egl13-player-symbian gles11-vg11-vgu11-egl13-player-symbian vg11-vgu11-egl13-player-symbian gles20-egl13-player-symbian gles20-vg11-egl13-player-symbian gles11-egl13-player-symbian vgu11-player-symbian gles11-egl11-player-symbian gles20-vg11-vgu11-egl13-player-symbian egl13-player-symbian gles11-player-symbian gles20-player-symbian gles11-vg11-egl13-player-win32 vg11-egl13-player-win32 vg11-player-win32 gles11-vg101-egl13-player-win32 gles11-vg11-vgu11-egl13-player-win32 vg11-vgu11-egl13-player-win32 gles20-egl13-player-win32 gles20-vg11-egl13-player-win32 gles11-egl13-player-win32 vgu11-player-win32 gles11-egl11-player-win32 gles20-vg11-vgu11-egl13-player-win32 egl13-player-win32 gles11-player-win32 gles20-player-win32
players_clean: gles11-vg11-egl13-player-android_clean vg11-egl13-player-android_clean vg11-player-android_clean gles11-vg101-egl13-player-android_clean gles11-vg11-vgu11-egl13-player-android_clean vg11-vgu11-egl13-player-android_clean gles20-egl13-player-android_clean gles20-vg11-egl13-player-android_clean gles11-egl13-player-android_clean vgu11-player-android_clean gles11-egl11-player-android_clean gles20-vg11-vgu11-egl13-player-android_clean egl13-player-android_clean gles11-player-android_clean gles20-player-android_clean gles11-vg11-egl13-player-unix_clean vg11-egl13-player-unix_clean vg11-player-unix_clean gles11-vg101-egl13-player-unix_clean gles11-vg11-vgu11-egl13-player-unix_clean vg11-vgu11-egl13-player-unix_clean gles20-egl13-player-unix_clean gles20-vg11-egl13-player-unix_clean gles11-egl13-player-unix_clean vgu11-player-unix_clean gles11-egl11-player-unix_clean gles20-vg11-vgu11-egl13-player-unix_clean egl13-player-unix_clean gles11-player-unix_clean gles20-player-unix_clean gles11-vg11-egl13-player-symbian_clean vg11-egl13-player-symbian_clean vg11-player-symbian_clean gles11-vg101-egl13-player-symbian_clean gles11-vg11-vgu11-egl13-player-symbian_clean vg11-vgu11-egl13-player-symbian_clean gles20-egl13-player-symbian_clean gles20-vg11-egl13-player-symbian_clean gles11-egl13-player-symbian_clean vgu11-player-symbian_clean gles11-egl11-player-symbian_clean gles20-vg11-vgu11-egl13-player-symbian_clean egl13-player-symbian_clean gles11-player-symbian_clean gles20-player-symbian_clean gles11-vg11-egl13-player-win32_clean vg11-egl13-player-win32_clean vg11-player-win32_clean gles11-vg101-egl13-player-win32_clean gles11-vg11-vgu11-egl13-player-win32_clean vg11-vgu11-egl13-player-win32_clean gles20-egl13-player-win32_clean gles20-vg11-egl13-player-win32_clean gles11-egl13-player-win32_clean vgu11-player-win32_clean gles11-egl11-player-win32_clean gles20-vg11-vgu11-egl13-player-win32_clean egl13-player-win32_clean gles11-player-win32_clean gles20-player-win32_clean
.PHONY: players players_clean

