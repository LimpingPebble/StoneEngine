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

all:			$(LIBS)

$(LIBS):		| init_configure
	@${CMAKE} --build ${BUILD_DIR} -t $@

clean:
	@rm -rf ${BUILD_DIR}

init_configure:
	@${CMAKE} -B ${BUILD_DIR} -S .

.PHONY:	clean all $(LIBS)
