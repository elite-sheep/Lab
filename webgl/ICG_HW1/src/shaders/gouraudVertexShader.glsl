attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec3 aFrontColor;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat3 uNMatrix;
uniform vec3 u_pointLightPos1;
uniform vec3 u_lightColor1;
uniform vec3 u_pointLightPos2;
uniform vec3 u_lightColor2;
uniform vec3 u_directionLight;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shiningness;

const float lightPower = 160.0;
const float directionLightPower = 0.005;

varying vec4 fragcolor;

void main(){
  vec4 vv = uMVMatrix * vec4(aVertexPosition, 1.0);
  vv = vv / vv.w;
  vec3 v = vv.xyz;

  vec4 nn = uMVMatrix * vec4(aVertexNormal, 1.0);
  nn /= nn.w;
  vec3 n = nn.xyz;

  vec3 lightDir1 = normalize(u_pointLightPos1 - v);
  vec3 lightDir2 = normalize(u_pointLightPos2 - v);
  vec3 lightDir3 = normalize(u_directionLight);
  vec3 eyeDir = normalize(-v);       
  vec3 reflect1 = normalize(-reflect(lightDir1, n));  
  vec3 reflect2 = normalize(-reflect(lightDir2, n));  

  float r1 = length(u_pointLightPos1 - v);
  float r2 = length(u_pointLightPos2 - v);

  //calculate Ambient  
  vec4 ambient = vec4(u_ambient, 1.0);   

  vec4 lightColor1 = vec4(u_lightColor1, 1.0);
  vec4 lightColor2 = vec4(u_lightColor2, 1.0);

  //calculate Diffuse. 
  vec4 diffuseColor1 = lightPower * lightColor1 * vec4(u_diffuse, 1.0) * 
    max(dot(n,lightDir1), 0.0) / pow(r1, 2.0);    
  vec4 diffuseColor2 = lightPower * lightColor2 * vec4(u_diffuse, 1.0) *
    max(dot(n,lightDir2), 0.0) / pow(r2, 2.0);    
  vec4 diffuseColor3 = directionLightPower * vec4(u_diffuse, 1.0) * max(dot(n,lightDir3), 0.0);    

  // calculate Specular.
  vec4 specularColor1 = lightColor1 * lightPower * vec4(u_specular, 1.0) *
    pow(max(dot(reflect1, eyeDir), 0.0), u_shiningness) / pow(r1, 2.0);
  vec4 specularColor2 = lightColor2 * lightPower * vec4(u_specular, 1.0) *
    pow(max(dot(reflect2, eyeDir), 0.0), u_shiningness) / pow(r2, 2.0);
  vec4 specularColor3 = directionLightPower * vec4(u_diffuse, 1.0) * max(dot(n,lightDir3), 0.0);    

  vec4 color1 = diffuseColor1 + specularColor1;
  vec4 color2 = diffuseColor2 + specularColor2;
  vec4 color3 = diffuseColor3 + specularColor3;

  fragcolor = vec4((ambient + color1 + color2 + color3).xyz, 1.0); 

  gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
}
