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
varying vec4 v_normal;

void main(void) {
  vec4 n = normalize(v_normal);
 
  vec4 lightDir1 = normalize(vec4(u_pointLightPos1, 1.0) - v_position);
  vec4 lightDir2 = normalize(vec4(u_pointLightPos2, 1.0) - v_position);
  vec4 eyeDir = normalize(-v_position);       
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

  gl_FragColor = vec4(color1.xyz + color2.xyz, 1.0); 
}
