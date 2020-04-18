attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec3 aFrontColor;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat3 uNMatrix;

varying vec4 v_position;
varying vec3 v_normal;

void main() {
  v_position = uMVMatrix * vec4(aVertexPosition, 1.0);
  gl_Position = uPMatrix * v_position;
  v_normal = (uMVMatrix * vec4(aVertexNormal, 1.0)).xyz;
}
