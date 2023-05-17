#include "SolarSystem.h"

SolarSystem::SolarSystem(cg::GLSLProgram& program, glm::mat4x4& view, glm::mat4x4& projection): program(program), view(view), projection(projection) {
	this->sun = new Octahedron(program, view, projection, this->sun_detail, this->sun_radius, this->sun_position, glm::vec3(1.0f, 1.0f, 0.0f));
	this->planet = new Octahedron(program, view, projection, this->planet_detail, this->planet_radius, this->planet_position, glm::vec3(0.0f, 1.0f, 0.0f));
	this->planet_orbit = new Orbit(program, view, projection, this->planet_radius);
	
}

SolarSystem::~SolarSystem() {
	delete sun;
	delete planet;
	delete planet_orbit;
}

void SolarSystem::init() {
	this->sun->init();
	this->planet->init();
	this->planet_orbit->init();
	this->planet->rotateZLocal(this->planet_z_tilt);
}

void SolarSystem::draw() {
	this->sun->draw();
	this->planet->draw();
	this->planet_orbit->draw();

	if (!this->stopAnimation) {
		this->planet->rotateYGlobal(this->planet_rotation_speed);
	}
}


void SolarSystem::increasePlanetRotationSpeed() {
	if (this->planet_rotation_speed < 2.0f) {
		this->planet_rotation_speed += 0.1f;
	}
}

void SolarSystem::decreasePlanetRotationSpeed() {
	if (this->planet_rotation_speed > 0.0f) {
		this->planet_rotation_speed -= 0.1f;
	}
}

void SolarSystem::upY() {
	glm::vec3 translationVector(0.0f, 0.1f, 0.0f);
	this->sun->model = glm::translate(this->sun->model, translationVector);
	this->planet->model = glm::translate(this->planet->model, translationVector);
}

void SolarSystem::downY() {

	glm::vec3 translationVector(0.0f, -0.1f, 0.0f);
	this->sun->model = glm::translate(this->sun->model, translationVector);
	this->planet->model = glm::translate(this->planet->model, translationVector);
}


void SolarSystem::setStopAnimation(bool stop) {
	this->stopAnimation = stop;
}

bool SolarSystem::getStopAnimation() {
	return this->stopAnimation;
}
