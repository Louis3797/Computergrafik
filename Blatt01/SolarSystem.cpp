#include "SolarSystem.h"


SolarSystem::SolarSystem(cg::GLSLProgram& program, glm::mat4x4& view, glm::mat4x4& projection): program(program), view(view), projection(projection) {

	this->sun = new Octahedron(program, view, projection, this->sun_detail, this->sun_radius, this->sun_position, glm::vec3(1.0f, 1.0f, 0.0f));
	this->planet = new Octahedron(program, view, projection, this->planet_detail, this->planet_radius, this->planet_position, glm::vec3(0.0f, 1.0f, 0.0f));
	this->moon = new Octahedron(program, view, projection, this->moon_detail, this->moon_radius, this->moon_position, glm::vec3(1.0f, 1.0f, 1.0f));


	this->planet_orbit = new Orbit(program, view, projection, this->planet_position.x, glm::vec3(0.0f,0.0f,0.0f), 360);
	this->moon_orbit = new Orbit(program, view, projection, this->planet_radius + 1.0f, this->planet_position, 360);


	this->sun_axis = new CelestialBodyAxis(program, view, projection, - (this->sun_radius + 1.0f), (this->sun_radius + 1.0f));
	this->planet_axis = new CelestialBodyAxis(program, view, projection, - (this->planet_radius + 1.0f), this->planet_radius + 1.0f);
	
}

SolarSystem::~SolarSystem() {
	delete sun;
	delete planet;
	delete moon;
	delete planet_orbit;
	delete moon_orbit;
	delete sun_axis;
	delete planet_axis;
}

void SolarSystem::init() {

	this->sun_axis->init();
	this->planet_axis->init();
	
	this->planet_orbit->init();	
	this->moon_orbit->init();

	this->moon->init();
	this->planet->init();
	this->sun->init();
	this->planet->rotateZLocal(this->planet_z_tilt);
}

void SolarSystem::draw() {

	this->planet_orbit->draw();



	// set new position
	this->sun_axis->model = this->sun->model;
	this->planet_axis->model = this->planet->model;
	this->sun_axis->draw();
	this->planet_axis->draw();

	
	this->moon->draw();
	this->moon->model = planet->model;
	this->moon->rotateXGlobal(15.0f);

	this->moon_orbit->model = this->planet->model;
	this->moon_orbit->draw();

	this->planet->draw();
	this->sun->draw();	

	if (!this->stopAnimation) {
		this->planet->rotateYGlobal(this->planet_rotation_speed);
		//this->planet->rotateYLocal(0.3f);
	}




}


void SolarSystem::increasePlanetRotationSpeed() {
	if (this->planet_rotation_speed < 2.0f) {
		this->planet_rotation_speed += 0.1f;
	}
}

void SolarSystem::decreasePlanetRotationSpeed() {
	if (this->planet_rotation_speed > 0.1f) {
		this->planet_rotation_speed -= 0.1f;
	}
}

void SolarSystem::upY() {
	glm::vec3 translationVector(0.0f, 0.1f, 0.0f);
	this->sun_axis->model = glm::translate(this->sun_axis->model, translationVector);
	this->planet_axis->model = glm::translate(this->planet_axis->model, translationVector);
	this->sun->model = glm::translate(this->sun->model, translationVector);
	this->planet_orbit->model = glm::translate(this->planet_orbit->model, translationVector);
	this->planet->model = glm::translate(this->planet->model, translationVector);
}

void SolarSystem::downY() {

	glm::vec3 translationVector(0.0f, -0.1f, 0.0f);
	this->sun_axis->model = glm::translate(this->sun_axis->model, translationVector);
	this->planet_axis->model = glm::translate(this->planet_axis->model, translationVector);
	this->sun->model = glm::translate(this->sun->model, translationVector);
	this->planet_orbit->model = glm::translate(this->planet_orbit->model, translationVector);
	this->planet->model = glm::translate(this->planet->model, translationVector);
}


void SolarSystem::setStopAnimation(bool stop) {
	this->stopAnimation = stop;
}

bool SolarSystem::getStopAnimation() {
	return this->stopAnimation;
}
