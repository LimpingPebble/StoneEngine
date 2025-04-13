// Copyright 2024 Stone-Engine

#include "Scene/Shader/ShaderGenerator.hpp"

#include "Scene/Renderable/Shader.hpp"
#include "Utils/FileSystem.hpp"

namespace Stone::Scene {

void ShaderGenerator::generateFragmentShaderTemplate(const ShaderParameters &params, std::ostream &output) {

	output << "// Stone shader template" << std::endl;

	auto to_glsl = [](ShaderParameters::Type type) {
		switch (type) {
		case ShaderParameters::Type::None: return "void";
		case ShaderParameters::Type::Scalar: return "float";
		case ShaderParameters::Type::Vector2: return "vec2";
		case ShaderParameters::Type::Vector3: return "vec3";
		case ShaderParameters::Type::Vector4: return "vec4";
		case ShaderParameters::Type::Texture: return "sampler2D";
		default: return "";
		}
	};

	auto add_uniform_param = [&output, &to_glsl](const char *name, ShaderParameters::Type type) {
		if (type != ShaderParameters::Type::None)
			output << "// " << name << ": " << to_glsl(type) << std::endl;
	};

#define __ADD_UNIFORM_PARAM(PARAM) add_uniform_param(#PARAM, params.PARAM);

	FOR_EACH_SHADER_PARAMETERS(__ADD_UNIFORM_PARAM);
	output << std::endl;

	output << "void customShader(inout Material material) {" << std::endl;
	output << "	material.diffuse = vec3(1, 0, 0);" << std::endl;
	output << "	// TODO: Implement shader generation logic here" << std::endl;
	output << "}" << std::endl;
}

void ShaderGenerator::generateOpenGlForwardFragmentShader(const ShaderParameters &params, FragmentShader *shader,
														  std::ostream &output) {
	std::ostream &source(output);

	source << "#version 400 core" << std::endl;

	source << R"(
in FRAG_DATA {
	vec3 wposition;
	vec2 uv;
	vec3 wnormal;
	vec3 wtangent;
	vec3 wbitangent;
} fs_in;
)";

	source << R"(
struct Material {
	vec3 diffuse;
	vec3 specular;
	float roughness;
	float metalness;
	vec3 occlusion;
	float shininess;
};
)";

	// unused = 0; sunlight = 1; spotlight = 2; pointlight = 3; ambiantlight = 4;
	// caster_index = -1 : no caster;
	source << R"(
struct Light {
	int type;
	vec3 position;
	vec3 color;
	vec3 specular;
	float intensity;
	vec3 attenuation;
	vec3 direction;
	float angle;
	float cone_attenuation;
	int caster_index;
};
)";

	source << R"(
struct LightCaster {
	mat4 vp;
	sampler2D depth_map;
};

)";

	source << "uniform Light u_lights[" << 16 << "];" << std::endl;
	source << "uniform int u_lights_count;" << std::endl;
	source << "uniform LightCaster u_light_casters[" << 4 << "];" << std::endl;

	source << R"(
uniform mat3 u_mat_normal;
uniform mat4 u_mat_projection;
uniform mat4 u_mat_view;
uniform mat4 u_mat_model;
uniform vec3 u_camera_position;

)";

	auto to_glsl = [](ShaderParameters::Type type) {
		switch (type) {
		case ShaderParameters::Type::None: return "void";
		case ShaderParameters::Type::Scalar: return "float";
		case ShaderParameters::Type::Vector2: return "vec2";
		case ShaderParameters::Type::Vector3: return "vec3";
		case ShaderParameters::Type::Vector4: return "vec4";
		case ShaderParameters::Type::Texture: return "sampler2D";
		default: return "";
		}
	};

	auto add_uniform_param = [&source, &to_glsl](const char *name, ShaderParameters::Type type) {
		if (type != ShaderParameters::Type::None)
			source << "uniform " << to_glsl(type) << ' ' << name << ";" << std::endl;
	};

#define __ADD_UNIFORM_PARAM(PARAM) add_uniform_param(#PARAM, params.PARAM);

	FOR_EACH_SHADER_PARAMETERS(__ADD_UNIFORM_PARAM);
	source << std::endl;

	source << "out vec4 FragColor;" << std::endl;

	source << R"(
float calculShadow(Light light) {
	int caster_index = light.caster_index;
	if (caster_index >= 0) {
		vec4 fpos_light = u_light_casters[caster_index].vp * vec4(fs_in.wposition, 1.0);
		vec3 proj_coords = fpos_light.xyz / fpos_light.w;
		proj_coords = proj_coords * 0.5 + 0.5;
		bool outOfLight = (proj_coords.x < 0 || proj_coords.x > 1 || proj_coords.y < 0 || proj_coords.y > 1);
		if (outOfLight)
			return 1.0;
		float current_depth = proj_coords.z;
		float bias = 0.00001; // max(0.05 * (1.0 - dot(normal_direction, -light.direction)), 0.005);
		vec2 texel_size = 1.0 / textureSize(u_light_casters[caster_index].depth_map, 0);
		float shadow = 0.0;
		int total = 0;
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				float pcf_depth = texture(u_light_casters[caster_index].depth_map, proj_coords.xy + vec2(x, y) * texel_size).r;
				shadow += current_depth - bias > pcf_depth ? 0.0 : 1.0;
				total++;
			}
		}
		return shadow / (total);
	}
	return 1.0;
}

)";

	source << R"(
vec3 calculDirectionalLight(Light light, Material fragMat, vec3 normal_direction, vec3 fcamera_position) {
	vec3 color = vec3(0);
	float shadow = calculShadow(light);
	if (shadow <= 0.0) return color;
	float d = distance(light.position, fs_in.wposition);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * d + light.attenuation.z * d * d);
	vec3 light_direction = normalize(-light.direction);
	float diffuse_intensity = clamp(dot(light_direction, normal_direction), 0.0, 1.0);
	color += light.color * fragMat.diffuse * diffuse_intensity * attenuation * shadow * fragMat.occlusion;
	if (diffuse_intensity > 0) {
		vec3 reflection = reflect(-light_direction, normal_direction);
		vec3 halfway_direction = normalize(fcamera_position + light_direction);
		float specular_intensity = pow(clamp(dot(normal_direction, halfway_direction), 0.0, 1.0), fragMat.shininess);
		color += diffuse_intensity * light.specular * fragMat.specular * specular_intensity * shadow;
	}
	return color;
}

)";

	source << R"(
vec3 calculSpotLight(Light light, Material fragMat, vec3 normal_direction, vec3 fcamera_position) {
	vec3 color = vec3(0);
	float shadow = calculShadow(light);
	if (shadow <= 0.0) return color;
	float d = distance(light.position, fs_in.wposition);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * d + light.attenuation.z * d * d);
	vec3 light_direction = normalize(light.position - fs_in.wposition);
	float cosangle = cos(light.angle);
	float spot_result = dot(-light_direction, light.direction);
	if (spot_result > cosangle) {
		float cosat = cos(light.angle - light.cone_attenuation);
		attenuation *= clamp((spot_result - cosangle) / (cosat - cosangle), 0.0, 1.0);
		float diffuse_intensity = clamp(dot(light_direction, normal_direction), 0.0, 1.0);
		color += light.color * fragMat.diffuse * diffuse_intensity * attenuation * shadow * fragMat.occlusion;
		if (diffuse_intensity > 0) {
			vec3 reflection = reflect(-light_direction, normal_direction);
			vec3 halfway_direction = normalize(fcamera_position + light_direction);
			float specular_intensity = pow(clamp(dot(normal_direction, halfway_direction), 0.0, 1.0), fragMat.shininess);
			color += diffuse_intensity * light.specular * fragMat.specular * specular_intensity * shadow;
		}
	}
	return color;
}

)";

	source << R"(
vec3 calculPointLight(Light light, Material fragMat, vec3 normal_direction, vec3 fcamera_position) {
	vec3 color = vec3(0);
	float shadow = calculShadow(light);
	if (shadow <= 0.0) return color;
	float d = distance(light.position, fs_in.wposition);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * d + light.attenuation.z * d * d);
	vec3 light_direction = normalize(light.position - fs_in.wposition);
	float diffuse_intensity = clamp(dot(light_direction, normal_direction), 0.0, 1.0);
	color += light.color * fragMat.diffuse * diffuse_intensity * attenuation * shadow * fragMat.occlusion;
	if (diffuse_intensity > 0) {
		vec3 reflection = reflect(-light_direction, normal_direction);
		vec3 halfway_direction = normalize(fcamera_position + light_direction);
		float specular_intensity = pow(clamp(dot(normal_direction, halfway_direction), 0.0, 1.0), fragMat.shininess);
		color += diffuse_intensity * light.specular * fragMat.specular * specular_intensity * shadow;
	}
	return color;
}

)";

	source << R"(
vec3 calculAmbiantLight(Light light, Material fragMat) {
	return fragMat.diffuse * light.color * light.intensity * fragMat.occlusion;
}
)";

	// directional = 1; spotlight = 2; pointlight = 3; ambiantlight = 4;
	source << R"(
vec3 calculLight(Light light, Material fragMat, vec3 normal_direction, vec3 fcamera_position) {
	switch (light.type) {
		case 1:	// directional
			return calculDirectionalLight(light, fragMat, normal_direction, fcamera_position);
		case 2:	// spotlight
			return calculSpotLight(light, fragMat, normal_direction, fcamera_position);
		case 3:	// pointlight
			return calculPointLight(light, fragMat, normal_direction, fcamera_position);
		case 4:	// ambiant
			return calculAmbiantLight(light, fragMat);
		default:
			return vec3(0);
	}
}

)";

	if (shader != nullptr) {
		auto [contentType, content] = shader->getContent();

		using ContentType = Scene::AShader::ContentType;
		switch (contentType) {
		case ContentType::SourceCode: source << content << std::endl; break;
		case ContentType::SourceFile: source << Utils::readTextFile(content) << std::endl; break;
		default: break;
		}
	}


	source << "void main() {" << std::endl;
	// if ((texturebitmask & 128) != 0) {
	//	if (texture(texture_mask, (mask_transform * vec3(fs_in.uv, 1.0)).xy).a == 0.0)
	//		discard;
	// }
	source << "	Material fragMat;" << std::endl;

	// TODO: Handle default values
	const auto assign_to_vec = [&source](ShaderParameters::Type type, const std::string &name) {
		switch (type) {
		case ShaderParameters::Type::None: break;
		case ShaderParameters::Type::Scalar:
			source << "	fragMat." << name << " = vec3(" << name << ", 0, 0);" << std::endl;
			break;
		case ShaderParameters::Type::Vector2:
			source << "	fragMat." << name << " = vec3(" << name << ", 0);" << std::endl;
			break;
		case ShaderParameters::Type::Vector3:
			source << "	fragMat." << name << " = " << name << ";" << std::endl; //
			break;
		case ShaderParameters::Type::Vector4:
			source << "	fragMat." << name << " = " << name << ".xyz;" << std::endl;
			break;
		case ShaderParameters::Type::Texture:
			source << "	fragMat." << name << " = texture(" << name << ", fs_in.uv).xyz;" << std::endl;
			break;
		}
	};

	const auto assign_to_float = [&source](ShaderParameters::Type type, const std::string &name, char x) {
		switch (type) {
		case ShaderParameters::Type::None: break;
		case ShaderParameters::Type::Scalar:
			source << "	fragMat." << name << " = " << name << ";" << std::endl; //
			break;
		case ShaderParameters::Type::Vector2:
		case ShaderParameters::Type::Vector3:
		case ShaderParameters::Type::Vector4:
			source << "	fragMat." << name << " = " << name << "." << x << ";" << std::endl;
			break;
		case ShaderParameters::Type::Texture:
			source << "	fragMat." << name << " = texture(" << name << ", fs_in.uv)." << x << ";" << std::endl;
			break;
		}
	};

	assign_to_vec(params.diffuse, "diffuse");
	assign_to_vec(params.specular, "specular");
	assign_to_float(params.roughness, "roughness", 'x');
	assign_to_float(params.metallic, "metallic", 'x');
	assign_to_vec(params.occlusion, "occlusion");
	assign_to_float(params.shininess, "shininess", 'x');

	if (shader) {
		source << "	" << shader->getFunction() << "(fragMat);" << std::endl;
	}

	if (params.normal == ShaderParameters::Type::Texture) {
		source << "	vec3 normal_value = normalize(texture(normal, fs_in.uv).xyz * 2 - 1);" << std::endl;
	} else {
		source << "	vec3 normal_value = vec3(0, 0, 1);" << std::endl;
	}

	source << R"(
	vec3 normal_direction = fs_in.wnormal * normal_value.z
		+ fs_in.wtangent * normal_value.x
		+ fs_in.wbitangent * normal_value.y;
	normal_direction = normalize(normal_direction);
	vec3 fcamera_position = normalize(u_camera_position - fs_in.wposition);

	vec3 color = vec3(0);

	// for (int i = 0; i < u_lights_count; i++) {
	// 	color += calculLight(u_lights[i], fragMat, normal_direction, fcamera_position);
	// }
	color = vec3(1.0, 0.0, 0.0);

	FragColor = vec4(color, 1.0);
}
)";
}

} // namespace Stone::Scene
