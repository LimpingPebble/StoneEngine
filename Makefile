BUILD_DIR					:=	./build
CMAKE						:=	cmake

GENERATOR					:= # Uses default generator, which may be "Unix makefiles"

# If found, will use ninja, since it's more performant
ifneq (, $(shell which ninja))
GENERATOR					:=	-G"Ninja"
endif

define build_target
	$(eval arg = $1)
endef

LIBS						=	graphics logging physics scene sound

PRESET						=	debug
LIST_PRESETS_TYPE			=	configure

ALL_EXAMPLES 				=	$(subst examples/,,$(shell find examples/ -type d))

all:			$(LIBS)

$(LIBS):		| init_configure
	@${CMAKE} --preset ${PRESET}

clean:
	@rm -rf ${BUILD_DIR}

init_configure:
	@${CMAKE} --preset ${PRESET}

list-presets:
	@${CMAKE} --list-presets=${LIST_PRESETS_TYPE}

test:
	@${CMAKE} --build --preset=debug-tests

examples:
	@${CMAKE} --build --preset=debug-examples

${ALL_EXAMPLES}: examples
	cd ${BUILD_DIR}/${PRESET}/ && ./examples/$@/$@

.PHONY:	clean all test examples $(LIBS)
