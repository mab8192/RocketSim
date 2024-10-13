#include "raylib-cpp.hpp"
#include "rlgl.h"

int main() {
	raylib::Window window(1200, 900, "RocketSim", FLAG_MSAA_4X_HINT);
	SetTargetFPS(120);
	DisableCursor();

	raylib::Camera camera(Vector3{200, 100, 200});
	camera.SetFovy(45.0f);

	camera.GetProjection();

	rlSetBlendMode(1);

	raylib::Model rocket(GenMeshCylinder(4.5f, 71.0f, 18));
	raylib::Vector3 pos(0, 0, 0), vel(0, 0, 0), acc(0, 0, 0);

	float thrust = 74400000; // newtons
	float weight = 275000; // kg
	float fuelWeight = 3400000; // kg
	float burnRate = 20481.92; // kg/s for 166s of burn time

	double throttle = 1.0f;

	while (!window.ShouldClose()) {
		// Update
		float dt = GetFrameTime();

		// Consume fuel
		if (fuelWeight > 0) {
			fuelWeight -= (throttle * burnRate) * dt;
		} else {
			fuelWeight = 0;
			thrust = 0;
		}

		// compute forces
		float Fg = -9.8 * (weight + fuelWeight);
		float force = (throttle * thrust) + Fg;

		acc = { 0, force / (weight + fuelWeight), 0};

		pos += vel * dt;
		vel += acc * dt;

		if (pos.y <= 0) {
			pos.y = 0;
		}

		camera.position.y = pos.y + 100;
		camera.target = pos;

		BeginDrawing();
		ClearBackground(BLACK);
		
		camera.BeginMode();

		DrawPlane({}, { 1000, 1000 }, BROWN);
		rocket.Draw(pos);

		camera.EndMode();

		DrawFPS(10, 10);
		DrawText(TextFormat("Fuel: %f", fuelWeight), 10, 30, 24, WHITE);
		DrawText(TextFormat("Acceleration: (%f, %f, %f)", acc.x, acc.y, acc.z), 10, 60, 24, WHITE);
		DrawText(TextFormat("Velocity: (%f, %f, %f)", vel.x, vel.y, vel.z), 10, 90, 24, WHITE);
		DrawText(TextFormat("Position: (%f, %f, %f)", pos.x, pos.y, pos.z), 10, 120, 24, WHITE);
		EndDrawing();
	}

	rocket.Unload();
	window.Close();
}
