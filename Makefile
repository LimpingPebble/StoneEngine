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

all: libs				## make libs

help:					## Show this help.
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "  %-10s %s\n", $$1, $$2}' $(MAKEFILE_LIST)

libs: | init_configure	## Build all libraries
	@${CMAKE} --preset ${PRESET}

clean:					## Clean build directory
	@rm -rf ${BUILD_DIR}

re: clean all			## Clean and build all

init_configure:
	@${CMAKE} --preset ${PRESET}

list-presets:
	@${CMAKE} --list-presets=${LIST_PRESETS_TYPE}

test:					## Run tests
	@${CMAKE} --build --preset=${PRESET}-tests

examples:				## Build all examples (use make `example_name` to run a specific example)
	@${CMAKE} --build --preset=${PRESET}-examples

${ALL_EXAMPLES}: examples
	./${BUILD_DIR}/${PRESET}/examples/$@/$@

setup-tidy:
	@${CMAKE} --preset=setup-tidy

.PHONY:	clean all test examples libs setup-tidy
