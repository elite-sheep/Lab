// Copyright 2020 Yuchen Wong

import { mat3, mat4 } from "gl-matrix"

import { gl, degToRad } from "./main.js"

function Shape(_modelSource) {
  this.modelSource = _modelSource;
  this.mvMatrix = mat4.create();
  this.pMatrix = mat4.create();

  this.scale = 1.0;
  this.rotationX = 0.0;
  this.rotationY = 0.0;
  this.rotationZ = 0.0;

  this.translationX = 0.0;
  this.translationY = 0.0;

  this.shearX = 0.0;
  this.shearY = 0.0;
}

Shape.prototype.setShaderProgram = function(newProgram) {
  this.shaderProgram = newProgram
  this.initShader()
  this.setupModel()
}

Shape.prototype.setupModel = function() {
  this.vertexPositionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexPositionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.modelSource.vertexPositions), gl.STATIC_DRAW);
  this.vertexPositionBuffer.itemSize = 3;
  this.vertexPositionBuffer.numItems = this.modelSource.vertexPositions.length / 3;

  this.vertexNormalBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexNormalBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.modelSource.vertexNormals), gl.STATIC_DRAW);
  this.vertexNormalBuffer.itemSize = 3;
  this.vertexNormalBuffer.numItems = this.modelSource.vertexNormals.length / 3;

  this.vertexFrontColorBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexFrontColorBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.modelSource.vertexFrontcolors), gl.STATIC_DRAW);
  this.vertexFrontColorBuffer.itemSize = 3;
  this.vertexFrontColorBuffer.numItems = this.modelSource.vertexFrontcolors.length / 3;
}

Shape.prototype.initShader = function() {
  gl.useProgram(this.shaderProgram);

  this.shaderProgram.vertexPositionAttribute = gl.getAttribLocation(this.shaderProgram, "aVertexPosition");
  gl.enableVertexAttribArray(this.shaderProgram.vertexPositionAttribute);

  this.shaderProgram.vertexNormalAttribute = gl.getAttribLocation(this.shaderProgram, "aVertexNormal");
  gl.enableVertexAttribArray(this.shaderProgram.vertexNormalAttribute);

  this.shaderProgram.pMatrixUniform  = gl.getUniformLocation(this.shaderProgram, "uPMatrix");
  this.shaderProgram.mvMatrixUniform = gl.getUniformLocation(this.shaderProgram, "uMVMatrix");
  this.shaderProgram.nMatrixUniform = gl.getUniformLocation(this.shaderProgram, "uNMatrix");

  this.shaderProgram.pointLightPos1 = gl.getUniformLocation(this.shaderProgram, "u_pointLightPos1");
  this.shaderProgram.lightColor1 = gl.getUniformLocation(this.shaderProgram, "u_lightColor1");
  this.shaderProgram.pointLightPos2 = gl.getUniformLocation(this.shaderProgram, "u_pointLightPos2");
  this.shaderProgram.directionLight = gl.getUniformLocation(this.shaderProgram, "u_directionLight");
  this.shaderProgram.lightColor2 = gl.getUniformLocation(this.shaderProgram, "u_lightColor2");
  this.shaderProgram.ambientColor = gl.getUniformLocation(this.shaderProgram, "u_ambient");
  this.shaderProgram.diffuseColor = gl.getUniformLocation(this.shaderProgram, "u_diffuse");
  this.shaderProgram.specularColor = gl.getUniformLocation(this.shaderProgram, "u_specular");
  this.shaderProgram.shiningness = gl.getUniformLocation(this.shaderProgram, "u_shiningness");
}

Shape.prototype.drawFrame = function() {
      // Setup Projection Matrix
  mat4.perspective(this.pMatrix, 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0);

    // Setup Model-View Matrix
  var shearMatrix = getShearMatrix(this.shearX, this.shearY);
  mat4.identity(this.mvMatrix);
  mat4.translate(this.mvMatrix, this.mvMatrix, this.translation);
  mat4.rotate(this.mvMatrix, this.mvMatrix, degToRad(this.rotationX), [1, 0, 0]);
  mat4.rotate(this.mvMatrix, this.mvMatrix, degToRad(this.rotationY), [1, 1, 0]);
  mat4.rotate(this.mvMatrix, this.mvMatrix, degToRad(this.rotationZ), [0, 0, 1]);
  mat4.scale(this.mvMatrix, this.mvMatrix, [this.scale, this.scale, this.scale]);
  mat4.multiply(this.mvMatrix, shearMatrix, this.mvMatrix);

  gl.uniformMatrix4fv(this.shaderProgram.pMatrixUniform, false, this.pMatrix);
  gl.uniformMatrix4fv(this.shaderProgram.mvMatrixUniform, false, this.mvMatrix);
  var nMatrix = mat4.create();
  mat4.invert(nMatrix, this.mvMatrix);
  mat4.transpose(nMatrix, nMatrix);
  gl.uniformMatrix4fv(this.shaderProgram.nMatrixUniform, false, nMatrix);

  gl.uniform3f(this.shaderProgram.pointLightPos1, 45.0, 30.0, -35.0);
  gl.uniform3f(this.shaderProgram.lightColor1, 0.47, 0.24, 0.16);
  gl.uniform3f(this.shaderProgram.pointLightPos2, -45.0, 30.0, -35.0);
  gl.uniform3f(this.shaderProgram.lightColor2, 0.47, 0.24, 0.16);
  gl.uniform3f(this.shaderProgram.directionLight, 0.0, -1.0, -1.0);
  gl.uniform3fv(this.shaderProgram.ambientColor, this.ambientColor);
  gl.uniform3fv(this.shaderProgram.diffuseColor, this.diffuseColor);
  gl.uniform3fv(this.shaderProgram.specularColor, this.specularColor);
  gl.uniform1f(this.shaderProgram.shiningness, 128.0 * this.shiningness);

  // Setup teapot position data
  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexPositionBuffer);
  gl.vertexAttribPointer(this.shaderProgram.vertexPositionAttribute, 
                           this.vertexPositionBuffer.itemSize, 
                           gl.FLOAT, 
                           false, 
                           0, 
                           0);

  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexNormalBuffer);
  gl.vertexAttribPointer(this.shaderProgram.vertexNormalAttribute, 
                           this.vertexNormalBuffer.itemSize, 
                           gl.FLOAT, 
                           false, 
                           0, 
                           0);

  // Setup teapot front color data
  gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexFrontColorBuffer);
  gl.vertexAttribPointer(this.shaderProgram.vertexFrontColorAttribute, 
                           this.vertexFrontColorBuffer.itemSize, 
                           gl.FLOAT, 
                           false, 
                           0, 
                           0);

  gl.drawArrays(gl.TRIANGLES, 0, this.vertexPositionBuffer.numItems);
}

Shape.prototype.setScale = function(_scale) {
  this.scale = _scale;
}

Shape.prototype.setTranslation = function(_value) {
  this.translation = _value;
} 

Shape.prototype.setAmbientColor = function(_value) {
  this.ambientColor = _value;
}

Shape.prototype.setDiffuseColor = function(_value) {
  this.diffuseColor = _value;
}

Shape.prototype.setSpecularColor = function(_value) {
  this.specularColor = _value;
}

Shape.prototype.setShiningness = function(_value) {
  this.shiningness = _value;
}

Shape.prototype.setRotationX = function(_value) {
  this.rotationX = _value;
}

Shape.prototype.setRotationY = function(_value) {
  this.rotationY = _value;
}

Shape.prototype.setRotationZ = function(_value) {
  this.rotationZ = _value;
}

Shape.prototype.setShearX = function(_value) {
  this.shearX = _value;
}

function getShearMatrix(shearX, shearY) {
  return mat4.fromValues(
    1.0, 0.0, shearX, 0.0,
    0.0, 1.0, shearY, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0);
}

export default Shape;
