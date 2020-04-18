precision mediump float;

uniform vec3 u_pointLightPos1;
uniform vec3 u_lightColor1;
uniform vec3 u_pointLightPos2;
uniform vec3 u_lightColor2;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shiningness;

varying vec4 v_position;
varying vec3 v_normal;

void main(void) {
  vec3 n = normalize(v_normal);
 
  vec3 lightDir1 = normalize(u_pointLightPos1 - v_position.xyz);
  vec3 lightDir2 = normalize(u_pointLightPos2 - v_position.xyz);
  vec3 eyeDir = normalize(-v_position.xyz);       
  vec3 halfVector1 = normalize(lightDir1 + eyeDir);
  vec3 halfVector2 = normalize(lightDir2 + eyeDir);
 
  //calculate Ambient  
  vec4 ambient = vec4(u_ambient, 1.0);   
 
  //calculate Diffuse. 
  vec4 diffuseColor1 = vec4(u_diffuse, 1.0) * max(dot(n,lightDir1), 0.0);    
  vec4 diffuseColor2 = vec4(u_diffuse, 1.0) * max(dot(n,lightDir2), 0.0);    
 
  // calculate Specular.
  vec4 specularColor1 = vec4(u_specular, 1.0) *
    pow(max(dot(halfVector1, n), 0.0), u_shiningness);
  vec4 specularColor2 = vec4(u_specular, 1.0) *
    pow(max(dot(halfVector2, n), 0.0), u_shiningness);
 
  vec4 color1 = vec4(u_ambient, 1.0) + (diffuseColor1 + specularColor1) * 0.5;
  vec4 color2 = vec4(u_ambient, 1.0) + (diffuseColor2 + specularColor2) * 0.5;

  gl_FragColor = vec4((color1 + color2).xyz, 1.0);
}
