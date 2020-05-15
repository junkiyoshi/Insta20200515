#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto radius = 150;
	auto noise_seed = ofRandom(1000);

	this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 40, 20, 0, 360, ofColor(39), ofColor(239));
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofDrawSphere(100);

	ofRotateZ(ofRandom(1000) + ofGetFrameNum() * ofRandom(1, 3));
	ofRotateY(ofRandom(1000) + ofGetFrameNum() * ofRandom(1, 3));
	ofRotateX(ofRandom(1000) + ofGetFrameNum() * ofRandom(1, 3));

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end, ofColor face_color, ofColor frame_color) {

	if (deg_start == deg_end) { return; }

	int index = face_target.getNumVertices();

	float deg_span = 0.1;
	for (float deg = deg_start; deg <= deg_end; deg += deg_span) {

		auto noise_location = location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);
		auto next_noise_location = location + glm::vec3(radius * cos((deg + deg_span) * DEG_TO_RAD), radius * sin((deg + deg_span) * DEG_TO_RAD), 0);

		int out_radius = radius + width * ofMap(ofNoise(glm::vec4(noise_location * 0.05, ofGetFrameNum() * 0.0065)), 0, 1, 0, 1.75);
		int next_out_radius = radius + width * ofMap(ofNoise(glm::vec4(next_noise_location * 0.05, ofGetFrameNum() * 0.0065)), 0, 1, 0, 1.75);

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(out_radius * cos(deg * DEG_TO_RAD), out_radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(next_out_radius * cos((deg + deg_span) * DEG_TO_RAD), next_out_radius * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(next_out_radius * cos((deg + deg_span) * DEG_TO_RAD), next_out_radius * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(out_radius * cos(deg * DEG_TO_RAD), out_radius * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);

		for (int i = 0; i < 8; i++) {

			this->face.addColor(face_color);
			this->frame.addColor(frame_color);
		}
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}