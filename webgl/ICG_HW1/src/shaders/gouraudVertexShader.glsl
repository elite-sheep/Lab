attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec3 aFrontColor;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform vec3 u_pointLightPos1;
uniform vec3 u_lightColor1;
uniform vec3 u_pointLightPos2;
uniform vec3 u_lightColor2;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shiningness;

varying vec4 fragcolor;

void main(){
  vec4 v = uMVMatrix * vec4(aVertexPosition, 1.0);

  vec4 n = normalize(uMVMatrix * vec4(aVertexNormal, 1.0));

  vec4 lightDir1 = normalize(vec4(u_pointLightPos1, 1.0) - v);
  vec4 lightDir2 = normalize(vec4(u_pointLightPos2, 1.0) - v);
  vec4 eyeDir = normalize(-v);       
  vec4 reflect1 = normalize(-reflect(lightDir1, n));  
  vec4 reflect2 = normalize(-reflect(lightDir2, n));  

  //calculate Ambient  
  vec4 ambient = vec4(u_ambient, 1.0);   

  //calculate Diffuse. 
  vec4 diffuseColor1 = vec4(u_diffuse, 1.0) * max(dot(n,lightDir1), 0.0);    
  vec4 diffuseColor2 = vec4(u_diffuse, 1.0) * max(dot(n,lightDir2), 0.0);    

  // calculate Specular.
  vec4 specularColor1 = vec4(u_specular, 1.0) *
    pow(max(dot(reflect1, eyeDir), 0.0), u_shiningness);
  vec4 specularColor2 = vec4(u_specular, 1.0) *
    pow(max(dot(reflect2, eyeDir), 0.0), u_shiningness);

  vec4 color1 = (ambient + diffuseColor1 + specularColor1) * vec4(u_lightColor1, 1.0);
  vec4 color2 = (ambient + diffuseColor2 + specularColor2) * vec4(u_lightColor2, 1.0);

  fragcolor = color1 + color2; 

  gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
}
