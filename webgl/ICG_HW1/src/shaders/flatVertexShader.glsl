attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec3 aFrontColor;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;

varying vec4 v_position;

void main() {
  v_position = uMVMatrix * vec4(aVertexPosition, 1.0);
  gl_Position = uPMatrix * v_position;
}
