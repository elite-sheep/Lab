import { mat4 } from "gl-matrix"
import WebGLUtils from "./webgl-utils.js"
import Shape from "./shape.js"

import startVertexShader from "../shaders/gouraudVertexShader.glsl"
import startFragmentShader from "../shaders/startFragmentShader.glsl"
import teapotModel from "../model/Teapot.json"
import carModel from "../model/Car_road.json"
import plantModel from "../model/Plant.json"

// common variables
export var gl;
var shaderProgram;
var shape1;
var shape2;
var shape3;

var lastTime    = 0;

var rotateTeapot = false;

function initGL(canvas) {
    try {
        gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
        gl.viewportWidth  = canvas.width;
        gl.viewportHeight = canvas.height;
    } 
    catch (e) {
    }

    if (!gl) {
      alert("Could not initialise WebGL, sorry :-(");
    }
    if (!gl.getExtension('OES_standard_derivatives')) {
      alert("Could not load OES_standard_derivatives.");
    }
}

function getShader(shaderSource, type) {
    var shader;
    if (type == "fragment") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } 
    else if (type == "vertex") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } 
    else {
        return null;
    }

    gl.shaderSource(shader, shaderSource);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

function initShaders() {
  var fragmentShader = getShader(startFragmentShader, "fragment");
  var vertexShader   = getShader(startVertexShader, "vertex");

  var shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
      alert("Could not initialise shaders");
  }

  shape1 = new Shape(teapotModel, shaderProgram);
  shape1.setRotationX(20);
  shape1.setRotationY(0);
  shape1.setRotationZ(-5);
  shape1.setAmbientColor([0.17, 0.01, 0.01]);
  shape1.setDiffuseColor([0.61, 0.04, 0.04]);
  shape1.setSpecularColor([0.72, 0.62, 0.62]);
  shape1.setTranslation([-25, 2.0, -45.0]);
  shape1.setScale(0.8);
  shape1.setShiningness(0.6);

  shape2 = new Shape(carModel, shaderProgram);
  shape2.setRotationX(-75);
  shape2.setRotationY(0);
  shape2.setRotationZ(60);
  shape2.setAmbientColor([0.19, 0.19, 0.19]);
  shape2.setDiffuseColor([0.50, 0.50, 0.50]);
  shape2.setSpecularColor([0.50, 0.50, 0.50]);
  shape2.setTranslation([10, 0, -45.0]);
  shape2.setScale(20.0);
  shape2.setShiningness(0.6);

  shape3 = new Shape(plantModel, shaderProgram);
  shape3.setRotationX(-45);
  shape3.setRotationY(-10);
  shape3.setRotationZ(55);
  shape3.setAmbientColor([0.0, 0.19, 0.0]);
  shape3.setDiffuseColor([0.4, 0.5, 0.4]);
  shape3.setSpecularColor([0.2, 0.7, 0.2]);
  shape3.setTranslation([22, 0, -45.0]);
  shape3.setShiningness(0.4);
  shape3.setScale(8.0);
}

export function degToRad(degrees) {
    return degrees * Math.PI / 180;
}

/*
    TODO HERE:
    add two or more objects showing on the canvas
    (it needs at least three objects showing at the same time)
*/
function drawScene() {
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
}

function animate() {
    var timeNow = new Date().getTime();
    if (lastTime != 0) {
        var elapsed = timeNow - lastTime;
    }
    
    lastTime = timeNow;
}

var scale = 1.0;

var rotateTeapotDegree = 30;
var carScale = 20.0;
var carTranslationX = 10.0;
var plantShearX = 0.0;
function tick() {
    requestAnimFrame(tick);
    drawScene();
    shape1.drawFrame();
    shape2.drawFrame();
    shape3.drawFrame();

  if (rotateTeapot) {
    rotateTeapotDegree += 1.0;
    if (rotateTeapot >= 180)
      rotateTeapotDegree = 0.0;
    shape1.setRotationX(rotateTeapotDegree);
    shape1.setRotationY(rotateTeapotDegree);
  }

  shape2.setScale(carScale);
  shape2.setTranslation([carTranslationX, 0.0, -45.0]);

  shape3.setShearX(plantShearX);
}

function webGLStart() {
    var canvas = document.getElementById("ICG-canvas");
    initGL(canvas);
    initShaders();

    gl.clearColor(0.25, 0.25, 0.7, 1.0);
    gl.enable(gl.DEPTH_TEST);

    tick();
}

window.onload = function() {
  webGLStart();

  document.onkeydown = function(e) {
    if (e.which == 'R'.charCodeAt(0)) rotateTeapot = !rotateTeapot;
    if (e.which == 'B'.charCodeAt(0)) carScale += 1;
    if (e.which == 'V'.charCodeAt(0)) carScale -= 1;
    if (e.which == 'L'.charCodeAt(0)) carTranslationX -= 1;
    if (e.which == 'T'.charCodeAt(0)) carTranslationX += 1;
    if (e.which == 'S'.charCodeAt(0)) plantShearX += 0.1;
  }
}
