# Predictable shell behavior
.SHELLFLAGS=-cuef -o pipefail
SHELL=/bin/bash

INCS=-I include

# What are we building...
LIB_TARGETS=libcodie

# What modules goes in libcodie
INCS+=-I src/include
libcodie_HDR:=$(wildcard ${libcodie_INC}/codie/*.hpp)
libcodie_SRC:=$(wildcard src/*.cpp)
libcodie_OBJS:=${libcodie_SRC:%.cpp=%.o}

BIN_TARGETS=speed

# What modules goes in speed
INCS+=-I speed
speed_HDR:=$(wildcard speed/*.hpp)
speed_SRC:=$(wildcard speed/*.cpp)
speed_OBJS:=${speed_SRC:%.cpp=%.o}

# Where are we putting the output (for pack later)
LIBOUT:=lib
BINOUT:=bin

# CXX=clang++
CXX=g++

# CXXFLAGS:=${CXXFLAGS} -fconcepts-diagnostics-depth=2 -std=c++20 -g  --coverage -Wall
# CXXFLAGS:=${CXXFLAGS} -std=c++20 -g -fno-inline --coverage -Wall
CXXFLAGS:=${CXXFLAGS} -std=c++20 -g -fno-inline -Wall
# CXXFLAGS:=${CXXFLAGS} -std=c++20 -g -O1 -Wall
# CXXFLAGS:=${CXXFLAGS} -std=c++20 -O3 -Wall

#LIBS=${LIB_TARGETS:lib%=-l%} -lsndfile -lpthread
LIBS=${LIB_TARGETS:lib%=-l%} -lpthread
# LDFLAGS=--coverage -g -L${LIBOUT}
# LDFLAGS=-g -L${LIBOUT}
LDFLAGS=-L${LIBOUT}

ARFLAGS:=rcs

DOX_FILES:=$(wildcard dox/*.dox)
DOX_TARGETS:=${DOX_FILES:dox/%.dox=%}

# Simplify some of the rules
VPATH:=${LIBOUT}:${BINOUT}

what:
	@echo "  libs: ${LIB_TARGETS}"
	@echo "  bins: ${BIN_TARGETS}"
	@echo "  dox: ${DOX_TARGETS}"
	@echo "  test: Create tests"
	@echo "  pack: Create ${PACK_NAME}-${PACK_VERSION}-${PACK_RT}.tgz"
	@echo "deploy: Deploy ${PACK_PROVIDER}/${PACK_GROUP}/${PACK_NAME}-${PACK_VERSION}-${PACK_RT}.tgz"
	@echo " clean: Clean out intermediates"


${LIBOUT}:
	mkdir -p $@

libs: ${LIB_TARGETS:%=${LIBOUT}/%.a}

${BINOUT}:
	mkdir -p $@

bins: ${BINOUT} ${BIN_TARGETS:%=${BINOUT}/%}

#==== Tests ====
# TEST_SRC:=$(wildcard test/*.cpp)
TEST_SRC = test/utest.cpp
TEST_SRC += test/test_islist.cpp
TEST_SRC += test/test_slist.cpp
# TEST_SRC += $(wildcard test/test_cxx*.cpp)

.PHONY: test
test: test/test
	@echo "Running tests ..."
	@test/test

test/test: Makefile ${TEST_SRC:%.cpp=%.o} ${LIB_TARGETS:%=${LIBOUT}/%.a} ${libcodie_HDR}
	@echo -n "Linking tests ..."
	@${CXX} ${LDFLAGS} -o $@ ${TEST_SRC:%.cpp=%.o} ${LIBS}
	@echo "DONE!"
	@echo

.PHONY dox: ${DOX_TARGETS:%=dox/%/index.html}

dox/%/index.html: dox/%.dox ${%_HDR} dox/pzen.css
	echo "doxygen $<"

#==== We need to do this for every dox-target ====
define DOX_templ =
dox/$(1)/index.html: dox/$(1).dox $${$(1)_HDR} dox/pzen.css
	@doxygen dox/$(1).dox
endef

# Apply template above to all dox-targets
$(foreach dox,$(DOX_TARGETS),$(eval $(call DOX_templ,$(dox))))

#==== We need to do this for every bin-target ====
define BIN_templ =
$(1): $${BINOUT}/$(1)

$${BINOUT}/$(1): $${$(1)_OBJS} $${$(1)_HDR}
	@$$(shell mkdir -p $${BINOUT})
	@echo "linking $$@"
	@$${CXX} $${LDFLAGS}  -o $$@ $${$(1)_OBJS} $${LIBS} $${$(1)_LIBS}
endef

# Apply template above to all bin-targets
$(foreach bin,$(BIN_TARGETS),$(eval $(call BIN_templ,$(bin))))

#==== We need to do this for every lib-target ====
define LIB_templ =
$${LIBOUT}/$(1).a: $${$(1)_OBJS}
	$$(shell mkdir -p $${LIBOUT})
	@echo "Packing $$@"
	@$${AR} $${ARFLAGS}  $$@ $${$(1)_OBJS}

#  include $${$(1)_OBJS:%.o=%.d}
endef

# Apply template above to all lib-targets
$(foreach bin,$(LIB_TARGETS),$(eval $(call LIB_templ,$(bin))))

#==== Generic compile rules ====
%.o: %.cpp
	@echo "Building $@..."
	@${CXX} ${CXXFLAGS} ${INCS} -o $@ -c $<

# %.d: %.cpp
# 	@${CXX} -MM ${CXXFLAGS} ${INCS} $< -MF $@

%_dot.png: %.dot
	dot -T png $< > $@

cov: test/test
	test/test
	@mkdir -p cov
	gcovr -k -e test -e include --html --html-details --exclude-throw-branches --exclude-unreachable-branches --html-title "Coverage"  --output cov/index.html

clean:
	@echo "Cleaning up C++ build ..."
	@rm -f ${LIB_TARGETS:%=lib/%.a}
	@rm -f ${BIN_TARGETS:%=bin/%}
	@find . -name '*.o' -delete
	@find . -name '*.gc??' -delete
	@find . -name '*.d' -delete

purge: clean
	@echo "Cleaning up Doxygen and Coverage ..."
	@rm -rf cov ./dox/libpzen/
