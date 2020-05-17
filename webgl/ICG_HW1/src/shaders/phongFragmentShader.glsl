precision mediump float;

uniform vec3 u_pointLightPos1;
uniform vec3 u_lightColor1;
uniform vec3 u_pointLightPos2;
uniform vec3 u_lightColor2;
uniform vec3 u_directionLight;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shiningness;

varying vec4 v_position;
varying vec4 v_normal;

const float directionLightPower = 0.5;
const float lightPower = 10.0;

void main(void) {
  vec3 n = normalize(v_normal.xyz);
 
  vec3 lightDir1 = normalize(u_pointLightPos1 - v_position.xyz);
  vec3 lightDir2 = normalize(u_pointLightPos2 - v_position.xyz);
  vec3 lightDir3 = normalize(u_directionLight);
  vec3 eyeDir = normalize(-v_position.xyz);       
  vec3 reflect1 = normalize(reflect(-lightDir1, n));  
  vec3 reflect2 = normalize(reflect(-lightDir2, n));  
  vec3 reflect3 = normalize(reflect(-lightDir3, n));  
 
  //calculate Ambient  
  vec4 ambient = vec4(u_ambient, 1.0);   
 
  //calculate Diffuse. 
  vec3 diffuseColor1 = lightPower * 
    u_diffuse * max(dot(n, lightDir1), 0.0);    
  vec3 diffuseColor2 = lightPower * 
    u_diffuse * max(dot(n,lightDir2), 0.0);    
  vec3 diffuseColor3 = directionLightPower * 
    u_diffuse * max(dot(n,lightDir3), 0.0);    
 
  // calculate Specular.
  vec3 specularColor1 = lightPower * u_specular *
    pow(max(dot(reflect1, eyeDir), 0.0), u_shiningness);
  vec3 specularColor2 = lightPower * u_specular *
    pow(max(dot(reflect2, eyeDir), 0.0), u_shiningness);
  vec3 specularColor3 = directionLightPower * u_specular *
    pow(max(dot(reflect3, eyeDir), 0.0), u_shiningness);
 
  vec3 color1 = (diffuseColor1 + specularColor1) * u_lightColor1;
  vec3 color2 = (diffuseColor2 + specularColor2) * u_lightColor2;
  vec3 color3 = (diffuseColor3 + specularColor3) * u_lightColor2;

  gl_FragColor = vec4(color1 + color2 + color3 + u_ambient, 1.0); 
}
