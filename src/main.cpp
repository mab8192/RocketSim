#include "raylib-cpp.hpp"
#include "rlgl.h"

int main() {
	raylib::Window window(1200, 900, "RocketSim", FLAG_MSAA_4X_HINT);
	SetTargetFPS(120);
	DisableCursor();

	float offset = 100.0f;
	raylib::Camera camera(Vector3{200, offset, 200});
	Matrix proj = MatrixPerspective(DEG2RAD * 80.0f, GetScreenWidth() / GetScreenHeight(), 0.1f, 10000000.0f);

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

		if (IsKeyDown(KEY_DOWN)) {
			offset -= 10.0f * dt;
		}
		else if (IsKeyDown(KEY_UP)) {
			offset += 10.0f * dt;
		}

		camera.position.y = pos.y + offset;
		camera.target = pos;

		BeginDrawing();
		ClearBackground(BLACK);
		
		camera.BeginMode();
		rlSetMatrixProjection(proj);

		DrawSphereEx({ 0, -6378000, 0 }, 6378000, 100, 100, DARKGREEN);
		rocket.Draw(pos, 1.0f, DARKGRAY);

		camera.EndMode();

		DrawFPS(10, 10);
		DrawText(TextFormat("Fuel (thousand kg): %f", fuelWeight / 1000.0f), 10, 30, 18, WHITE);
		DrawText(TextFormat("Velocity (m/s): %0.2f", vel.Length()), 10, 50, 18, WHITE);
		DrawText(TextFormat("Altitude (km): %0.2f", pos.y / 1000.0f), 10, 70, 18, WHITE);
		EndDrawing();
	}

	rocket.Unload();
	window.Close();
}
