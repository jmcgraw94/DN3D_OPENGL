#include "Camera.h"
#include "Main.h"
#include "Helper.h"

using namespace std;

bool init = false;
float bobTimer = 0;
float bobSpeed = 1.5f;

Camera::Camera()
{
	view = glm::mat4();
	projection = glm::mat4();

	//Position = glm::vec3(3, 6, 8);
	Position = glm::vec3(6, .5f, 5);
	Rotation = glm::vec3(-1, 0, 0);

	ForwardVec = glm::vec3(0, 0, -1);
	UpVec = glm::vec3(0, 1, 0);

	cout << "Camera Created" << endl;
}

Camera::~Camera()
{
}

void Camera::Update() {
	view = glm::mat4();
	projection = glm::mat4();

	Rotation.y += Main::DeltaMousePos.x / 60;
	Rotation.x += Main::DeltaMousePos.y / 60;

	{
		float rotSpeed = .075;
		GLfloat MaxSpeed = 0.5f;
		GLfloat curSpeed = 0;
		vec3 velocity = vec3(0, 0, 0);
		bool moved = false;

		if (Main::HeldKeys[GLFW_KEY_W]) {
			//cameraPos += vec3(dx, 0, dy) * speed;
			curSpeed = MaxSpeed;// *ForwardVec;
			moved = true;
		}

		if (Main::HeldKeys[GLFW_KEY_S]) {
			//cameraPos -= vec3(dx, 0, dy) * speed;
			curSpeed = -MaxSpeed;// *ForwardVec;
			moved = true;
		}

		//if (Main::PressKeys[GLFW_KEY_A]) {
		//	//rot -= rotSpeed;
		//	//Rotation.y -= 4.0f;
		//	velocity.x -= curSpeed;// glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		//}

		//if (Main::PressKeys[GLFW_KEY_D]) {
		//	//rot += rotSpeed;
		//	//Rotation.y += 4.0f;
		//	velocity.x += curSpeed;// glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		//}

		if (Main::HeldKeys[GLFW_KEY_D]) {
			//rot -= rotSpeed;
			//Rotation.y += rotSpeed;
			//Rotation.y += 4.0f;
			//Position += glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}

		if (Main::HeldKeys[GLFW_KEY_A]) {
			//rot += rotSpeed;
			//Rotation.y -= rotSpeed;
			//Rotation.y -= 4.0f;
			//Position -= glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}

		vec3 INT_POS = vec3(
			(int)Position.x,
			(int)Position.y,
			(int)Position.z
		);


		//vec3 NextPos = Position + vec3(0, 0, -1);
		//Helper::PrintVec3(Main::Map.GetPixel(INT_POS.x, INT_POS.z));
		if (Main::HeldKeys[GLFW_KEY_LEFT_SHIFT]) 
			curSpeed *= 2;

		velocity.x = (cos(Rotation.y) / 10) * curSpeed;
		velocity.z = (sin(Rotation.y) / 10) * curSpeed;

		if (moved)
			bobSpeed = 10.0f;
		else
			bobSpeed = 2.5f;

		bobTimer += Main::DeltaTime * bobSpeed;

		if (Main::Map.GetPixel(
			(int)(Position.x + velocity.x * 5.0f),
			(int)Position.z)
			!= vec4(1, 1, 1, 1)) {
			velocity.x = 0;
		}
		//Position.x += velocity.x;

		if (Main::Map.GetPixel(
			(int)Position.x,
			(int)(Position.z + velocity.z * 5.0f))
			!= vec4(1, 1, 1, 1)) {
			velocity.z = 0;
		}
		//Position.z += velocity.z;
		Rotation.x = clamp(Rotation.x, -PI / 2, PI / 2);
		Position += velocity;
	}

	//ForwardVec = Rotation;

	//Rotation.y += Main::DeltaMousePos.x / 2;
	//Rotation.x += Main::DeltaMousePos.y / 2;

	//ForwardVec.y = atan2(Rotation.z, Rotation.x);

	//Helper::PrintVec3(Rotation);

	//	Helper::PrintVec3(cameraPos, "Camera Pos");
	//cout << cameraPos.z << endl;
	//view = glm::rotate(view, glm::radians(Rotation.z), vec3(0, 0, 1));
	//view = glm::rotate(view, glm::radians(45.0f), vec3(1, 0, 0));
	view = glm::rotate(view, Rotation.x, vec3(1, 0, 0));
	view = glm::rotate(view, Rotation.y + glm::radians(90.0f), vec3(0, 1, 0));

	view = glm::translate(view, -Position);
	view = glm::translate(view, vec3(0, sin(bobTimer * 1.25f) / (1 / bobSpeed * 70), 0));
	projection = glm::perspective(45.0f, (float)WIN_W / (float)WIN_H, 0.1f, 100.0f);
	//projection *= glm::lookAt(vec3(0,0,0), cameraPos, cameraUp);
}
