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

LIBS						= $(shell echo $(subst Engine/,,$(shell find Engine/ -maxdepth 1 -type d)) | tr A-Z a-z)

PRESET						=	debug
LIST_PRESETS_TYPE			=	configure

ALL_EXAMPLES 				=	$(subst examples/,,$(shell find examples/ -maxdepth 1 -type d))

all:		libs

libs:		| init_configure
	@${CMAKE} --preset ${PRESET}

clean:
	@rm -rf ${BUILD_DIR}

re:			clean all

init_configure:
	@${CMAKE} --preset ${PRESET}

list-presets:
	@${CMAKE} --list-presets=${LIST_PRESETS_TYPE}

test:
	@${CMAKE} --build --preset=${PRESET}-tests

examples:
	@${CMAKE} --build --preset=${PRESET}-examples

${ALL_EXAMPLES}: examples
	./${BUILD_DIR}/${PRESET}/examples/$@/$@

setup-tidy:
	@${CMAKE} --preset=setup-tidy

.PHONY:	clean all test examples libs setup-tidy
