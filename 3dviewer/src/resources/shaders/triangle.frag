#version 120
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec3 fragPos;

uniform vec3 u_lightPos;
uniform vec3 u_lightPosSecondary;
uniform vec3 u_viewPos;

void main() {
  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(u_lightPos - fragPos);
  vec3 backLightDir = normalize(u_lightPosSecondary - fragPos);
  
  // Ambient
  float ambientStrength = 0.25;
  vec3 ambient = ambientStrength * fragColor;
  
  // Diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * fragColor;
  float backDiff = max(dot(norm, backLightDir), 0.0);
  vec3 backDiffuse = 0.3 * backDiff * fragColor;
  
  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(u_viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
  vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
  vec3 backReflectDir = reflect(-backLightDir, norm);
  float backSpec = pow(max(dot(viewDir, backReflectDir), 0.0), 16.0);
  vec3 backSpecular = 0.2 * specularStrength * backSpec * vec3(1.0, 1.0, 1.0);
  
  vec3 result = ambient + diffuse + backDiffuse + specular + backSpecular;
  result = clamp(result, 0.0, 1.0);
  gl_FragColor = vec4(result, 1.0);
}
