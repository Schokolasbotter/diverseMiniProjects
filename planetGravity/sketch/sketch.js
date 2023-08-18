var cameraAutoTimer;
var id;
var checkBox1;
var checkBox2;
var checkBox3;
var checkBox4;
var checkBox5;
var slider1X;
var slider1Y;
var slider1Z;
var slider1Mass;
var planet1Texture;
var slider2X;
var slider2Y;
var slider2Z;
var slider2Mass;
var planet2Texture;
var slider3X;
var slider3Y;
var slider3Z;
var slider3Mass;
var planet3Texture;
var slider4X;
var slider4Y;
var slider4Z;
var slider4Mass;
var planet4Texture;
var slider5X;
var slider5Y;
var slider5Z;
var slider5Mass;
var planet5Texture;
var asteroidTexture

function preload() {
	//Load Textures
	planet1Texture = loadImage('assets/planet1.jpg');
	planet2Texture = loadImage('assets/planet2.jpg');
	planet3Texture = loadImage('assets/planet3.jpg');
	planet4Texture = loadImage('assets/planet4.jpg');
	planet5Texture = loadImage('assets/planet5.jpg');
	asteroidTexture = loadImage('assets/asteroid.jpg');
}

function setup() {
	//Canvas
	frameRate(30);
	createCanvas(1280, 720, WEBGL);
	background(0);
	//Camera
	camera = createCamera();
	cameraAutoTimer = 90;
	//UI
	UISetup();
	//Variables
	id = 0;
	//Asteroids
	asteroids = [];
	//Planets
	planets = [];
	planets.push(new Planet(slider1X.value(), slider1Y.value(), slider1Z.value(), slider1Mass.value(), planet1Texture));
	planets.push(new Planet(slider2X.value(), slider2Y.value(), slider2Z.value(), slider2Mass.value(), planet2Texture));
	planets.push(new Planet(slider3X.value(), slider3Y.value(), slider3Z.value(), slider3Mass.value(), planet3Texture));
	planets.push(new Planet(slider4X.value(), slider4Y.value(), slider4Z.value(), slider4Mass.value(), planet4Texture));
	planets.push(new Planet(slider5X.value(), slider5Y.value(), slider5Z.value(), slider5Mass.value(), planet5Texture));
}

function draw() {
	//Redraw Background
	background(0);
	//Camera
	camera.lookAt(0, 0, 0); //Look always at origin
	if (cameraAutoTimer >= 90) { //Move Camera after 3 seconds of not using ortho
		camera.move(1, 0, 0);
	}
	cameraAutoTimer++;

	//Origin (Optional)
	//push();
	//normalMaterial();
	//fill(255);
	//sphere(2);
	//pop();

	//Draw Objects
	drawPlanets();
	for (var i = 0; i < asteroids.length; i++) {
		asteroids[i]._draw();
		asteroids[i]._update();
	}
}

function mouseDragged() {
	if (mouseButton === LEFT) { //Left click to control camera
		cameraAutoTimer = 0;
		orbitControl();
	}
}

function mousePressed() {
	if (mouseButton === RIGHT) { // right click to spawn an asteroid
		asteroids.push(new Asteroid());
	}
}

function drawPlanets() {
	//Draw planets and update with slider Values
	planets[0]._draw();
	planets[0]._update(slider1X.value(), slider1Y.value(), slider1Z.value(), slider1Mass.value());
	planets[1]._draw();
	planets[1]._update(slider2X.value(), slider2Y.value(), slider2Z.value(), slider2Mass.value());
	planets[2]._draw();
	planets[2]._update(slider3X.value(), slider3Y.value(), slider3Z.value(), slider3Mass.value());
	planets[3]._draw();
	planets[3]._update(slider4X.value(), slider4Y.value(), slider4Z.value(), slider4Mass.value());
	planets[4]._draw();
	planets[4]._update(slider5X.value(), slider5Y.value(), slider5Z.value(), slider5Mass.value());
}



function Planet(x, y, z, mass, textureImage) {
	//Enabled
	this._enabled = true;
	//Position
	this._position = createVector(x, y, z);
	//Visual Variables
	this._radius = map(mass, 0, 10, 0, 100);
	this._texture = textureImage;
	//Space Variables
	this._mass = mass;
	this._rotationVector = createVector(random(-1, 1), random(-1, 1), random(-1, 1));
	this._draw = function () {
		if (this._enabled) {
			push();
			//Position
			translate(this._position.x, this._position.y, this._position.z);
			//Rotate
			rotate(millis() / 10000, this._rotationVector);
			//Draw Planet
			normalMaterial();
			texture(this._texture);
			sphere(this._radius);
			pop();
		}
	}
	this._update = function (x, y, z, mass) {
		this._position.x = x;
		this._position.y = y;
		this._position.z = z;
		this._mass = mass;
		this._radius = map(mass, 0, 10, 0, 100);
	}
}

function Asteroid() {
	//Calculate position
	let cameraPosition = createVector(camera.eyeX, camera.eyeY, camera.eyeZ);
	let originDirection = createVector(0, 0, 0).sub(cameraPosition).normalize();
	this._position = cameraPosition.add(originDirection.mult(100)); //Spawn from the camerapostion towards the origin
	//Physics
	this._acceleration = createVector(0, 0, 0);
	this._velocity = createVector(0, 0, 0);
	this._mass = 10;
	this._radius = 5;
	//Id
	this._id = id;
	id++;
	this._update = function () {
		//Check Collision
		this._checkCollision();
		this._checkBoundary();
		//Update Acceleration depending on gravity
		this._acceleration = createVector(0, 0, 0);
		for (var i = 0; i < planets.length; i++) {
			this._applyGravity(planets[i]);
		}
		//Update Asteroid Movement
		this._velocity.add(this._acceleration);
		this._position.add(this._velocity);
	}

	this._draw = function () {
		push();
		normalMaterial();
		texture(asteroidTexture);
		translate(this._position.x, this._position.y, this._position.z);
		sphere(this._radius);
		pop();
	}
	this._applyGravity = function (planet) {
		if (planet._enabled) {
			//Gravity Direction
			let gravityDirection = planet._position.copy().sub(this._position);
			let distance = gravityDirection.mag();
			gravityDirection.normalize();
			//Gravity Force
			let gravityForce = (6, 67 * planet._mass) / (pow(distance, 2));
			this._acceleration.add(gravityDirection.mult(gravityForce));
		}
	}
	this._checkCollision = function () {
		for (var i = 0; i < planets.length; i++) {
			let dist = planets[i]._position.copy().sub(this._position).mag(); // distance to planets
			if (dist < this._radius + planets[i]._radius) { // when touching a planet
				for (var j = asteroids.length - 1; j >= 0; j--) { // splice asteroid with the fitting id
					if (this._id == asteroids[j]._id) {
						asteroids.splice(j, 1);
					}
				}
			}
		}
	}
	this._checkBoundary = function () {
		let dist = createVector(0, 0, 0).sub(this._position).mag();
		if (dist > 1000) { //if distance to origin is too big 
			for (var j = asteroids.length - 1; i >= 0; i--) { // splice asteroid with fitting id
				if (this._id == asteroids[j]._id) {
					asteroids.splice(j, 1);
				}
			}
		}
	}
}

//Checkbox functions
function changeEnabledPlanet1() {
	if (this.checked()) {
		planets[0]._enabled = true;
	} else {
		planets[0]._enabled = false;
	}
}

function changeEnabledPlanet2() {
	if (this.checked()) {
		planets[1]._enabled = true;
	} else {
		planets[1]._enabled = false;
	}
}

function changeEnabledPlanet3() {
	if (this.checked()) {
		planets[2]._enabled = true;
	} else {
		planets[2]._enabled = false;
	}
}

function changeEnabledPlanet4() {
	if (this.checked()) {
		planets[3]._enabled = true;
	} else {
		planets[3]._enabled = false;
	}
}

function changeEnabledPlanet5() {
	if (this.checked()) {
		planets[4]._enabled = true;
	} else {
		planets[4]._enabled = false;
	}
}

function UISetup() {
	//Planet 1 Sliders
	checkBox1 = createCheckbox('Planet 1', true);
	checkBox1.position(100, 100);
	checkBox1.changed(changeEnabledPlanet1);
	planet1x = createP('X: ');
	planet1x.position(70, 111);
	slider1X = createSlider(-500, 500, 100);
	slider1X.position(100, 130);
	planet1y = createP('Y: ');
	planet1y.position(70, 141);
	slider1Y = createSlider(-500, 500, 0);
	slider1Y.position(100, 160);
	planet1z = createP('Z: ');
	planet1z.position(70, 171);
	slider1Z = createSlider(-500, 500, 200);
	slider1Z.position(100, 190);
	planet1z = createP('Mass: ');
	planet1z.position(60, 201);
	slider1Mass = createSlider(0, 10, 2);
	slider1Mass.position(100, 220);

	//Planet 2 Sliders
	checkBox2 = createCheckbox('Planet 2', true);
	checkBox2.position(300, 100);
	checkBox2.changed(changeEnabledPlanet2);
	planet2x = createP('X: ');
	planet2x.position(270, 111);
	slider2X = createSlider(-500, 500, -50);
	slider2X.position(300, 130);
	planet2y = createP('Y: ');
	planet2y.position(270, 141);
	slider2Y = createSlider(-500, 500, 150);
	slider2Y.position(300, 160);
	planet2z = createP('Z: ');
	planet2z.position(270, 171);
	slider2Z = createSlider(-500, 500, 0);
	slider2Z.position(300, 190);
	planet2z = createP('Mass: ');
	planet2z.position(260, 201);
	slider2Mass = createSlider(0, 10, 4);
	slider2Mass.position(300, 220);

	//Planet 3 Sliders
	checkBox3 = createCheckbox('Planet 3', true);
	checkBox3.position(100, 250);
	checkBox3.changed(changeEnabledPlanet3);
	planet3x = createP('X: ');
	planet3x.position(70, 261);
	slider3X = createSlider(-500, 500, -350);
	slider3X.position(100, 280);
	planet3y = createP('Y: ');
	planet3y.position(70, 291);
	slider3Y = createSlider(-500, 500, 90);
	slider3Y.position(100, 310);
	planet3z = createP('Z: ');
	planet3z.position(70, 321);
	slider3Z = createSlider(-500, 500, 400);
	slider3Z.position(100, 340);
	planet3z = createP('Mass: ');
	planet3z.position(60, 351);
	slider3Mass = createSlider(0, 10, 9);
	slider3Mass.position(100, 370);

	//Planet 4 Sliders
	checkBox4 = createCheckbox('Planet 4', true);
	checkBox4.position(300, 250);
	checkBox4.changed(changeEnabledPlanet4);
	planet4x = createP('X: ');
	planet4x.position(270, 261);
	slider4X = createSlider(-500, 500, 200);
	slider4X.position(300, 280);
	planet4y = createP('Y: ');
	planet4y.position(270, 2911);
	slider4Y = createSlider(-500, 500, 0);
	slider4Y.position(300, 310);
	planet4z = createP('Z: ');
	planet4z.position(270, 321);
	slider4Z = createSlider(-500, 500, -350);
	slider4Z.position(300, 340);
	planet4z = createP('Mass: ');
	planet4z.position(260, 351);
	slider4Mass = createSlider(0, 10, 2);
	slider4Mass.position(300, 370);

	//Planet 5 Sliders
	checkBox5 = createCheckbox('Planet 5', true);
	checkBox5.position(100, 400);
	checkBox5.changed(changeEnabledPlanet5);
	planet5x = createP('X: ');
	planet5x.position(70, 411);
	slider5X = createSlider(-500, 500, 50);
	slider5X.position(100, 430);
	planet5y = createP('Y: ');
	planet5y.position(70, 441);
	slider5Y = createSlider(-500, 500, 400);
	slider5Y.position(100, 460);
	planet5z = createP('Z: ');
	planet5z.position(70, 471);
	slider5Z = createSlider(-500, 500, -300);
	slider5Z.position(100, 490);
	planet5z = createP('Mass: ');
	planet5z.position(60, 501);
	slider5Mass = createSlider(0, 10, 9);
	slider5Mass.position(100, 520);
}