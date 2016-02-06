/*
Jonathan Sanderson
CS 4600 - Chuck Hansen
3 November 2014
*/

/*Declaration of Global Variables-----------------------------------------*/
var canvas;
var gl;
var program;
 
var numOfVert = 0;

var pointsArray = [];
var normalsArray = [];
    
//for lighting
var lightPosition = vec4(1.0, 1.0, 1.0, 0.0 );
var lightAmbient = vec4(0.2, 0.2, 0.2, 1.0 );
var lightDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
var lightSpecular = vec4( 1.0, 1.0, 1.0, 1.0 );

var materialAmbient = vec4( 1.0, 0.0, 1.0, 1.0 );
var materialDiffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
var materialSpecular = vec4( 1.0, 0.8, 0.0, 1.0 );
var materialShininess = 100.0;
var emissive = vec4( 1.0, 0.8, 0.0, 1.0);

var ctm;
var ambientColor, diffuseColor, specularColor;

//for perspective
var eyeX = 0.0;
var eyeY = 0.0;
var eyeZ = 5;
var atX = 0.0;
var atY = 0.0;
var atZ = 0.0;
var eye = vec3(eyeX, eyeY, eyeZ);
var at = vec3(atX, atY, atZ);
var up = vec3(0.0, 1.0, 0.0);
var near = 1;
var far = 300;

//for matricies
var frustrum = perspective(80, 1, near, far);
var mPer;
var modelViewMatrix, projectionMatrix;
var modelViewMatrixLoc, projectionMatrixLoc;
var normalMatrix, normalMatrixLoc;

//used to transform matricies
var stack = [];
var turtle;
var floor;
var lightFollow = true;
var useVertexShading = true;
var vertexShading;
var fragmentShading;

// variables used for animation
var maxFrames = 50;
var frames = maxFrames / 2.0;
var headAnimation = .01;
var headAnimationSpeed = 0.004;
var quadAnimation = .00;
var quadAnimationSpeed = 0.004;
var shinAnimation = 0.0;
var shinAnimationSpeed = 0.012;
var speed = 0.00;
var direction = 0;
var lastDirection = direction;
/*End of Declaration of Global Variables-----------------------------------*/

/*
 * makeSphere
 * makes a Sphere object that holds
 * the transform matrix and 
 * material information
 * 
 * Params:
 * 
 * 
 * Returns:
 *  Sphere
 *
 * Author Jonathan Sanderson
 */
function makeSphere(transform, material)
{
    var shape =
    {
        transform: transform,
        material: material
    }

    return shape;
}

function triangle(a, b, c) {

     var t1 = subtract(a, b);
     var t2 = subtract(a, c);
     var normal = normalize(cross(t1, t2));
     normal = vec4(normal);

     normalsArray.push(normal);
     normalsArray.push(normal);
     normalsArray.push(normal);
     
     pointsArray.push(a);
     pointsArray.push(b);      
     pointsArray.push(c);

     numOfVert += 3;
}

/*
 * divideTriangle
 * recursively divides a triangle into
 * smaller triangles
 * 
 * Params:
 *  a: a positon of vec4
 *  b: a positon of vec4
 *  c: a positon of vec4
 *  count: number of subdivisons
 * 
 * Returns:
 * 
 *
 * Modified from shadedSphere3.js
 */
function divideTriangle(a, b, c, count) {
    if ( count > 0 ) {
                
        var ab = mix( a, b, 0.5);
        var ac = mix( a, c, 0.5);
        var bc = mix( b, c, 0.5);
                
        ab = normalize(ab, true);
        ac = normalize(ac, true);
        bc = normalize(bc, true);
                                
        divideTriangle( a, ab, ac, count - 1 );
        divideTriangle( ab, b, bc, count - 1 );
        divideTriangle( bc, c, ac, count - 1 );
        divideTriangle( ab, bc, ac, count - 1 );
    }
    else { 
        triangle( a, b, c );
    }
}

/*
 * tetrahedron
 * constructs a tetrahedron from 4 points and
 * a number of triangle subdivisons
 * 
 * Params:
 *  a: a positon of vec4
 *  b: a positon of vec4
 *  c: a positon of vec4
 *  d: a positon of vec4
 *  n: number of subdivisons
 * 
 * Returns:
 * 
 *
 * Modified from shadedSphere3.js
 */
function tetrahedron(a, b, c, d, n) 
{
    divideTriangle(a, b, c, n);
    divideTriangle(d, c, b, n);
    divideTriangle(a, d, b, n);
    divideTriangle(a, c, d, n);
}

/*
 * makeTurtle
 * Creates a Turtle object that contains all the 
 * parts required to construct a Turtle out of
 * Sphere objects
 * 
 * Params:
 *  part: Sphere being drawn
 *  firstVertex: index of the first vertex to be used to contruct a triangle
 *  lastVertex: index after the last vertex to be used to contruct a triangle
 * 
 * Returns:
 * 
 *
 * Author Jonathan Sanderson
 */
function drawShape(part, firstVertex, lastVertex)
{
    var shape = part;

    //update materials
    materialAmbient = shape.material.materialAmbient;
    materialDiffuse = shape.material.materialDiffuse;
    materialSpecular = shape.material.materialSpecular;
    materialShininess = shape.material.materialShininess;
    emissive = shape.material.emissive;

    ambientProduct = mult(lightAmbient, materialAmbient);
    diffuseProduct = mult(lightDiffuse, materialDiffuse);
    specularProduct = mult(lightSpecular, materialSpecular);
    var m = stack.pop();
    normalMatrix = transpose(inverse(m));

    //send material in as uniform
    gl.uniform4fv( gl.getUniformLocation(program, 
        "ambientProduct"),flatten(ambientProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "diffuseProduct"),flatten(diffuseProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "specularProduct"),flatten(specularProduct) );   
    gl.uniform4fv( gl.getUniformLocation(program, 
       "lightPosition"),flatten(lightPosition) );
    gl.uniform1f( gl.getUniformLocation(program, 
       "shininess"),materialShininess );
    gl.uniform4fv( gl.getUniformLocation(program, 
        "emissive"),flatten(emissive) );
    gl.uniformMatrix4fv(normalMatrixLoc, false, flatten(normalMatrix) );

    //perform transformation
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(m));

    //draw the triangles
    for( var i=firstVertex; i<lastVertex; i+=3) 
          gl.drawArrays( gl.TRIANGLES, i, 3 );
}

/*
 * makeTurtle
 * Creates a Turtle object that contains all the 
 * parts required to construct a Turtle out of
 * Sphere objects
 * 
 * Params:
 * 
 * 
 * Returns:
 *  Turtle
 *
 * Author Jonathan Sanderson
 */
function makeTurtle()
{
    //makes the shell
    var shell = makeSphere(scale4(1.5, 0.5, 1.5), materialChrome());

    //makes the neck
    m = translate(1.5, 0.3, 0);
    m = mult(m, rotate(40, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.3, 0.3));
    var neck = makeSphere(m, materialTurquoise());

    //makes the head
    var m = translate(1.8, 0.5, 0);
    m = mult(m, scale4(0.5, 0.4, 0.4));
    var head = makeSphere(m, materialJade());

    //makes the right ear
    m = translate(1.6, 0.7, 0.23);
    m = mult(m, scale4(0.1, 0.1, 0.1));
    var rightEar = makeSphere(m, materialGlowingRuby());

    //makes the left ear
    m = translate(1.6, 0.7, -0.23);
    m = mult(m, scale4(0.1, 0.1, 0.1));
    var leftEar = makeSphere(m, materialGlowingRuby());

    //makes the rightEyeWhite
    m = translate(2, 0.7, 0.15);
    m = mult(m, scale4(0.05, 0.3, 0.2));
    var rightEyeWhite = makeSphere(m, materialGlowingPearl());

    //makes the rightEyeLid
    m = translate(1.99, 0.7, 0.15);
    m = mult(m, scale4(0.05, 0.3, 0.2));
    var rightEyeLid = makeSphere(m, materialJade());

    //makes the rightEyeBlack
    m = translate(2.01, 0.75, 0.15);
    m = mult(m, scale4(0.05, 0.2, 0.1));
    var rightEyeBlack = makeSphere(m, materialGlowingObsidian());

    //makes the LeftEyeWhite
    m = translate(2, 0.7, -0.15);
    m = mult(m, scale4(0.05, 0.3, 0.2));
    var leftEyeWhite = makeSphere(m, materialGlowingPearl());

    //makes the LeftEyeLid
    m = translate(1.99, 0.7, -0.15);
    m = mult(m, scale4(0.05, 0.3, 0.2));
    var leftEyeLid = makeSphere(m, materialJade());

    //makes the leftEyeBlack
    m = translate(2.01, 0.75, -0.15);
    m = mult(m, scale4(0.05, 0.2, 0.1));
    var leftEyeBlack = makeSphere(m, materialGlowingObsidian());

    //makes the rightNoseHole
    m = translate(2.25, 0.6, 0.1);
    m = mult(m, scale4(0.05, 0.05, 0.05));
    var rightNoseHole = makeSphere(m, materialGlowingObsidian());

    //makes the leftNoseHole
    m = translate(2.25, 0.6, -0.1);
    m = mult(m, scale4(0.05, 0.05, 0.05));
    var leftNoseHole = makeSphere(m, materialGlowingObsidian());

    //makes the leftBackQuad
    m = translate(-0.9, -0.3, 1.2);
    m = mult(m, rotate(90, 0, 1, 0));
    m = mult(m, rotate(25, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var leftBackQuad = makeSphere(m, materialTurquoise());

    //makes the leftBackShin
    m = translate(-0.9, -0.7, 1.579);
    m = mult(m, rotate(90, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var leftBackShin = makeSphere(m, materialTurquoise());

    //makes the leftFrontQuad
    m = translate(0.9, -0.3, 1.2);
    m = mult(m, rotate(90, 0, 1, 0));
    m = mult(m, rotate(25, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var leftFrontQuad = makeSphere(m, materialTurquoise());

    //makes the leftFrontShin
    m = translate(0.9, -0.7, 1.579);
    m = mult(m, rotate(90, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var leftFrontShin = makeSphere(m, materialTurquoise());

    //makes the rightBackQuad
    m = translate(-0.9, -0.3, -1.2);
    m = mult(m, rotate(90, 0, 1, 0));
    m = mult(m, rotate(-25, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var rightBackQuad = makeSphere(m, materialTurquoise());

    //makes the rightBackShin
    m = translate(-0.9, -0.7, -1.579);
    m = mult(m, rotate(90, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var rightBackShin = makeSphere(m, materialTurquoise());

    //makes the rightFrontQuad
    m = translate(0.9, -0.3, -1.2);
    m = mult(m, rotate(90, 0, 1, 0));
    m = mult(m, rotate(-25, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var rightFrontQuad = makeSphere(m, materialTurquoise());

    //makes the rightFrontShin
    m = translate(0.9, -0.7, -1.579);
    m = mult(m, rotate(90, 0, 0, 1));
    m = mult(m, scale4(0.5, 0.2, 0.2));
    var rightFrontShin = makeSphere(m, materialTurquoise());

    //makes the tail
    m = translate(-1.4, -0.2, 0);
    m = mult(m, rotate(20, 0, 0, 1));
    m = mult(m, scale4(0.4, 0.2, 0.2));
    var tail = makeSphere(m, materialTurquoise());

    var turtle = 
    {
        shell: shell,
        neck: neck, 
        head: head,
        rightEar: rightEar,
        leftEar: leftEar,
        rightEyeWhite: rightEyeWhite,
        rightEyeLid: rightEyeLid,
        rightEyeBlack: rightEyeBlack,
        leftEyeWhite: leftEyeWhite,
        leftEyeLid: leftEyeLid,
        leftEyeBlack: leftEyeBlack,
        rightNoseHole: rightNoseHole,
        leftNoseHole: leftNoseHole,
        rightFrontQuad: rightFrontQuad,
        rightFrontShin: rightFrontShin,
        leftFrontQuad: leftFrontQuad,
        leftFrontShin: leftFrontShin,
        rightBackQuad: rightBackQuad,
        rightBackShin: rightBackShin,
        leftBackQuad: leftBackQuad,
        leftBackShin: leftBackShin,
        tail: tail,
    }

    return turtle;
}

/*
 * drawEverything
 * This function manages the drawing of the
 * Turtle and the floor below it. Uses Hierarchical Transformation
 * The place the Turtle parts in the scene
 * 
 * Params:
 * 
 * 
 * Returns:
 * 
 *
 * Author Jonathan Sanderson
 */
function drawEverything()
{
    //pops the MVM and saves it
    var MVM = stack.pop();

    //pushes it back onto the matrix stack
    stack.push(MVM);

    /*Draws each body part*/

    //draws the shell
    m = stack.pop();
    m = mult(m, turtle.shell.transform);
    stack.push(m);
    drawShape(turtle.shell, 3072, numOfVert);
    stack.push(MVM);

    //draws the neck
    m = stack.pop();
    var headAni = mult(m , translate(headAnimation, 0, 0)); //headAnimation neck and head
    m = mult(headAni, turtle.neck.transform);
    stack.push(m);
    drawShape(turtle.neck, 0, 3072);
    stack.push(headAni);

    //draws the head
    m = stack.pop();
    m = mult(m, turtle.head.transform);
    stack.push(m);
    drawShape(turtle.head, 0, 3072);
    stack.push(headAni);

    //draws the rightEar
    m = stack.pop();
    m = mult(m, turtle.rightEar.transform);
    stack.push(m);
    drawShape(turtle.rightEar, 0, 3072);
    stack.push(headAni);

    //draws the leftEar
    m = stack.pop();
    m = mult(m, turtle.leftEar.transform);
    stack.push(m);
    drawShape(turtle.leftEar, 0, 3072);
    stack.push(headAni);

    //draws the rightEyeWhite
    m = stack.pop();
    m = mult(m, turtle.rightEyeWhite.transform);
    stack.push(m);
    drawShape(turtle.rightEyeWhite, 0, 3072);
    stack.push(headAni);

    //draws the rightEyeLid
    m = stack.pop();
    m = mult(m, turtle.rightEyeLid.transform);
    stack.push(m);
    drawShape(turtle.rightEyeLid, 0, 3072);
    stack.push(headAni);

    //draws the rightEyeBlack
    m = stack.pop();
    m = mult(m, turtle.rightEyeBlack.transform);
    stack.push(m);
    drawShape(turtle.rightEyeBlack, 0, 3072);
    stack.push(headAni);

    //draws the leftEyeWhite
    m = stack.pop();
    m = mult(m, turtle.leftEyeWhite.transform);
    stack.push(m);
    drawShape(turtle.leftEyeWhite, 0, 3072);
    stack.push(headAni);

    //draws the leftEyeLid
    m = stack.pop();
    m = mult(m, turtle.leftEyeLid.transform);
    stack.push(m);
    drawShape(turtle.leftEyeLid, 0, 3072);
    stack.push(headAni);

    //draws the leftEyeBlack
    m = stack.pop();
    m = mult(m, turtle.leftEyeBlack.transform);
    stack.push(m);
    drawShape(turtle.leftEyeBlack, 0, 3072);
    stack.push(headAni);

    //draws the rightNoseHole
    m = stack.pop();
    m = mult(m, turtle.rightNoseHole.transform);
    stack.push(m);
    drawShape(turtle.rightNoseHole, 0, 3072);
    stack.push(headAni);

    //draws the leftNoseHole
    m = stack.pop();
    m = mult(m, turtle.leftNoseHole.transform);
    stack.push(m);
    drawShape(turtle.leftNoseHole, 0, 3072);
    stack.push(MVM);

    //draws the tail
    m = stack.pop();
    m = mult(m, turtle.tail.transform);
    stack.push(m);
    drawShape(turtle.tail, 0, 3072);
    stack.push(MVM);

    //draws the rightFrontQuad
    m = stack.pop();
    var quad = mult(m , translate(quadAnimation, 0, 0)); //moves quad up and down
    m = mult(quad, turtle.rightFrontQuad.transform);
    stack.push(m);
    drawShape(turtle.rightFrontQuad, 0, 3072);
    stack.push(quad);

    //draws the rightFrontShin
    m = stack.pop();
    m = mult(m, turtle.rightFrontShin.transform);
    m = mult(m , translate(shinAnimation, 0, 0)); //moves shin forward and back
    stack.push(m);
    drawShape(turtle.rightFrontShin, 0, 3072);
    stack.push(MVM);

    //draws the leftFrontQuad
    m = stack.pop();
    quad = mult(m , translate(-quadAnimation, 0, 0)); //moves quad up and down
    m = mult(quad, turtle.leftFrontQuad.transform);
    stack.push(m);
    drawShape(turtle.leftFrontQuad, 0, 3072);
    stack.push(quad);

    //draws the leftFrontShin
    m = stack.pop();
    m = mult(m, turtle.leftFrontShin.transform);
    m = mult(m , translate(-shinAnimation, 0, 0)); //moves shin forward and back
    stack.push(m);
    drawShape(turtle.leftFrontShin, 0, 3072);
    stack.push(MVM);

    //draws the rightBackQuad
    m = stack.pop();
    quad = mult(m , translate(-quadAnimation, 0, 0)); //moves quad up and down
    m = mult(quad, turtle.rightBackQuad.transform);
    stack.push(m);
    drawShape(turtle.rightBackQuad, 0, 3072);
    stack.push(quad);

    //draws the rightBackShin
    m = stack.pop();
    m = mult(m, turtle.rightBackShin.transform);
    m = mult(m , translate(-shinAnimation, 0, 0)); //moves shin forward and back
    stack.push(m);
    drawShape(turtle.rightBackShin, 0, 3072);
    stack.push(MVM);

    //draws the leftBackQuad
    m = stack.pop();
    quad = mult(m , translate(quadAnimation, 0, 0)); //moves quad up and down
    m = mult(quad, turtle.leftBackQuad.transform);
    stack.push(m);
    drawShape(turtle.leftBackQuad, 0, 3072);
    stack.push(quad);

    //draws the leftBackShin
    m = stack.pop();
    m = mult(m, turtle.leftBackShin.transform);
    m = mult(m , translate(shinAnimation, 0, 0)); //moves shin forward and back
    stack.push(m);
    drawShape(turtle.leftBackShin, 0, 3072);
    stack.push(MVM);

    //draws the floor
    m = stack.pop();
    m = rotate(0, 1, 0, 0);
    m = mult(m, floor.transform);
    stack.push(m);
    drawShape(floor, 0, 3072);
}

/*
 * keyListener
 * This function reads keyboard input
 * and determines how to handle the input
 * 
 * Params:
 *  evt: keyboard inputs
 * 
 * Returns:
 * 
 *
 * Author Jonathan Sanderson
 */
function keyListener(evt)
{
    var key = String.fromCharCode(evt.keyCode);

    if (key == 'r' || key === 'r' || key == 'R' || key === 'R')
    {
        //resets the turtle
        modelViewMatrix = rotate(direction,0, 1, 0);
        modelViewMatrix =  mult( modelViewMatrix, translate(0, 0, 0));
        frames = maxFrames / 2.0;
        headAnimation = .01;
        quadAnimation = .00;
        shinAnimation = 0.0;
        document.getElementById("radioFollow" ).click();
    }

    //space bar
    if (evt.keyCode == 32 || evt.keyCode === 32)
        ;

    //change speed value
    document.getElementById("speedSliderValue").innerHTML = speed;

    //display speed slider
    document.getElementById("speedSlider").value = speed;
}

/*
 * init
 * Initalizes the WebGL program (the vertex and fragment shaders),
 * Initalizes global variables,
 * Sets up Event handlers for HTML buttons,
 * and sends inital vertices and colors to GPU
 * 
 * Params:
 * 
 * 
 * Returns:
 * 
 *
 * Modified from shadedSphere3.js
 */
window.onload = function init() {

    /* Begin listening for key strokes */
    document.onkeypress = keyListener;

    canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.9, 0.9, 0.9, 1.0 );
    
    gl.enable(gl.DEPTH_TEST);

    //
    //  Load shaders and initialize attribute buffers
    //
    vertexShading = initShaders( gl, "vertex-shader1", "fragment-shader1" );
    fragmentShading = initShaders( gl, "vertex-shader2", "fragment-shader2" );

    if (useVertexShading)
        program = vertexShading;
    else
        program = fragmentShading;

    gl.useProgram( program );

    ambientProduct = mult(lightAmbient, materialAmbient);
    diffuseProduct = mult(lightDiffuse, materialDiffuse);
    specularProduct = mult(lightSpecular, materialSpecular);

    points = [
        vec4(0.0, 0.0, -1.0,1),
        vec4(0.0, 0.942809, 0.333333, 1),
        vec4(-0.816497, -0.471405, 0.333333, 1),
        vec4(0.816497, -0.471405, 0.333333,1),
    ];

    //for other body parts
    tetrahedron(points[0], points[1], points[2], points[3], 4);

    //for shell
    tetrahedron(points[0], points[1], points[2], points[3], 3);

    turtle = makeTurtle();

    //makes the floor
    var m = translate(0, -1.5, 0); //translates the floor downward
    m = mult(m, scale4(10000, 0.1, 4));
    floor = makeSphere(m, materialFloor());


    var nBuffer = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, nBuffer);
    gl.bufferData( gl.ARRAY_BUFFER, flatten(normalsArray), gl.STATIC_DRAW );
    
    var vNormal = gl.getAttribLocation( program, "vNormal" );
    gl.vertexAttribPointer( vNormal, 4, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vNormal);


    var vBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(pointsArray), gl.STATIC_DRAW);
    
    var vPosition = gl.getAttribLocation( program, "vPosition");
    gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);

    //init Matricies
    projectionMatrix = mult(frustrum ,lookAt(eye, at, up)  );
    modelViewMatrix = rotate(0, 1, 0, 0);
    modelViewMatrix = mult( modelViewMatrix, rotate(direction,0, 1, 0));
    lastDirection = direction;
    
    modelViewMatrixLoc = gl.getUniformLocation( program, "modelViewMatrix" );
    projectionMatrixLoc = gl.getUniformLocation( program, "projectionMatrix" );
    normalMatrixLoc = gl.getUniformLocation( program, "normalMatrix" );

    document.getElementById("radioVertex" ).onclick = function() 
    {
        useVertexShading = true;
        program = vertexShading;
        numOfVert = 0;
        pointsArray = [];
        normalsArray = []; 
        //resets the turtle
        modelViewMatrix = rotate(direction,0, 1, 0);
        modelViewMatrix =  mult( modelViewMatrix, translate(0, 0, 0));
        frames = maxFrames / 2.0;
        headAnimation = .01;
        quadAnimation = .00;
        shinAnimation = 0.0;
        init();
    }
  
    /*Event handler for "No" radio button*/
    document.getElementById("radioFragment" ).onclick = function() 
    {
        useVertexShading = false;
        program = fragmentShading;
        numOfVert = 0;
        pointsArray = [];
        normalsArray = []; 
        //resets the turtle
        modelViewMatrix = rotate(direction,0, 1, 0);
        modelViewMatrix =  mult( modelViewMatrix, translate(0, 0, 0));
        frames = maxFrames / 2.0;
        headAnimation = .01;
        quadAnimation = .00;
        shinAnimation = 0.0;
        init();
    }

    /*Event handler for "Yes" radio button*/
    document.getElementById("radioFollow" ).onclick = function() 
    {
        lightFollow = true;
    }
  
    /*Event handler for "No" radio button*/
    document.getElementById("radioFixed" ).onclick = function() 
    {
        lightFollow = false;
    }

    document.getElementById("speedSlider").onchange = function() 
    {
        if (speed < 0)
            speed = 0;

        if (speed > 0.20)
            speed = 0.20;
        speed = event.srcElement.value;
        document.getElementById("speedSliderValue").innerHTML = speed;
    };

    document.getElementById("directionSlider").onchange = function() 
    {
        if (direction < 0)
            direction = 0;

        if (direction > 360)
            direction = 360;

        direction = event.srcElement.value;
        document.getElementById("directionSliderValue").innerHTML = direction;
    };

    document.getElementById("eyeXSlider").onchange = function() 
    {
        eyeX = event.srcElement.value;
        document.getElementById("eyeXSliderValue").innerHTML = eyeX;
    };

    document.getElementById("eyeYSlider").onchange = function() 
    {

        eyeY = event.srcElement.value;
        document.getElementById("eyeYSliderValue").innerHTML = eyeY;
    };

    document.getElementById("eyeZSlider").onchange = function() 
    {
        eyeZ = event.srcElement.value;
        document.getElementById("eyeZSliderValue").innerHTML = eyeZ;
    };

    document.getElementById("atXSlider").onchange = function() 
    {
        atX = event.srcElement.value;
        document.getElementById("atXSliderValue").innerHTML = atX;
    };

    document.getElementById("atYSlider").onchange = function() 
    {
        atY = event.srcElement.value;
        document.getElementById("atYSliderValue").innerHTML = atY;
    };

    document.getElementById("atZSlider").onchange = function() 
    {
        atZ = event.srcElement.value;
        document.getElementById("atZSliderValue").innerHTML = atZ;
    };

    gl.uniform4fv( gl.getUniformLocation(program, 
       "ambientProduct"),flatten(ambientProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "diffuseProduct"),flatten(diffuseProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "specularProduct"),flatten(specularProduct) );	
    gl.uniform4fv( gl.getUniformLocation(program, 
       "lightPosition"),flatten(lightPosition) );
    gl.uniform1f( gl.getUniformLocation(program, 
       "shininess"),materialShininess );
    gl.uniform4fv( gl.getUniformLocation(program, 
        "emissive"),flatten(emissive) );

    render();
}

/*
 * render
 * This function sends the vertex and color data to the
 * GPU to display on the monitor. Calls itself after a delay.
 * Handle updating vertices, shape, number of subdivision, and 
 * uniform variables.
 * 
 * Params:
 * 
 * 
 * Returns:
 * 
 *
 * Modified from shadedSphere3.js
 */
function render() {
    
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    //for animations
    modelViewMatrix = mult( modelViewMatrix, translate(speed, 0, 0));     

    //updates the movement of head
    frames -= 1;

    if (frames <= 0)
    {
        //resets count
        frames = maxFrames;

        //flips the direction
        headAnimationSpeed = -headAnimationSpeed;
        quadAnimationSpeed = -quadAnimationSpeed;
        shinAnimationSpeed = -shinAnimationSpeed;
    }

    headAnimation += headAnimationSpeed;
    quadAnimation += quadAnimationSpeed;
    shinAnimation += shinAnimationSpeed;

    at = vec3(atX, atY, atZ);
    eye = vec3(eyeX, eyeY, eyeZ);

    //Camera: Follow = position relative to camera
    //        Fixed = Camera stays at last position
    if (lightFollow)
        lightPosition = vec4(eyeX + 15, eyeY + 30, eyeZ, 1.0);

    projectionMatrix = mult(frustrum ,lookAt(eye, at, up));

    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix) );
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix) );

    if (lastDirection != direction)
    {
        modelViewMatrix = mult( modelViewMatrix, rotate(-lastDirection,0, 1, 0));
        modelViewMatrix = mult( modelViewMatrix, rotate(direction,0, 1, 0));
        lastDirection = direction;
    } 

    stack.push(modelViewMatrix);

    drawEverything();

    window.requestAnimFrame(render);
}

