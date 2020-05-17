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

const float lightPower = 5.0;

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
  vec3 diffuseColor1 = u_diffuse * max(dot(n,lightDir1), 0.0);    
  vec3 diffuseColor2 = u_diffuse * max(dot(n,lightDir2), 0.0);    
 
  // calculate Specular.
  vec3 specularColor1 = u_specular *
    pow(max(dot(halfVector1, n), 0.0), u_shiningness);
  vec3 specularColor2 = u_specular *
    pow(max(dot(halfVector2, n), 0.0), u_shiningness);
 
  vec3 color1 = (diffuseColor1 + specularColor1) * lightPower * u_lightColor1;
  vec3 color2 = (diffuseColor2 + specularColor2) * lightPower * u_lightColor2;

  gl_FragColor = vec4(u_ambient + color1 + color2, 1.0);
}
