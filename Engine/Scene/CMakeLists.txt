setup_module(
		NAME scene
		TARGET_DEPS glm assimp core logging json utils
		VARIABLE_DEPS assimp_SOURCE_DIR
		SPECIAL_HEADER_PATHS ${assimp_SOURCE_DIR}/include
		FATAL_ERROR
)
