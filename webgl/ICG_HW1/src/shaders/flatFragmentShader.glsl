#extension GL_OES_standard_derivatives : enable
precision highp float;

uniform vec3 u_pointLightPos1;
uniform vec3 u_lightColor1;
uniform vec3 u_pointLightPos2;
uniform vec3 u_lightColor2;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shiningness;

varying vec4 v_position;

const float lightPower = 60.0;

void main() {
  vec4 dx = dFdx(v_position);
  vec4 dy = dFdy(v_position);
  vec3 n = cross(dx.xyz, dy.xyz);
 
  vec3 lightDir1 = normalize(u_pointLightPos1 - v_position.xyz);
  vec3 lightDir2 = normalize(u_pointLightPos2 - v_position.xyz);
  vec3 eyeDir = normalize(-v_position.xyz);       
  vec3 reflect1 = normalize(-reflect(lightDir1, n));  
  vec3 reflect2 = normalize(-reflect(lightDir2, n));  
 
  //calculate Diffuse. 
  vec3 diffuseColor1 = u_diffuse * max(dot(n,lightDir1), 0.0);    
  vec3 diffuseColor2 = u_diffuse * max(dot(n,lightDir2), 0.0);    
 
  // calculate Specular.
  vec3 specularColor1 = u_specular *
    pow(max(dot(reflect1, eyeDir), 0.0), u_shiningness);
  vec3 specularColor2 = u_specular *
    pow(max(dot(reflect2, eyeDir), 0.0), u_shiningness);
 
  vec3 color1 = (diffuseColor1 + specularColor1) * u_lightColor1 * lightPower;
  vec3 color2 = (diffuseColor2 + specularColor2) * u_lightColor2 * lightPower;

  gl_FragColor = vec4(u_ambient + color1.xyz + color2.xyz, 1.0); 
}
