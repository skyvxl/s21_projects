#version 120
attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

varying vec3 fragColor;
varying vec3 fragNormal;
varying vec3 fragPos;

void main() {
  gl_Position = u_mvp * vec4(position, 1.0);
  fragPos = vec3(u_model * vec4(position, 1.0));
  fragNormal = u_normalMatrix * normal;
  fragColor = color;
}
