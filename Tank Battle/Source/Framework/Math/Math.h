#pragma once

#include "Vector2.h"

namespace GameDev2D
{
	namespace Math
	{
		const float PTM_RATIO = 32.0f; // Pixel to meter ratio
		const float EPSILON = 0.0001f;

		float DegreesToRadians(float degrees);
		float RadiansToDegrees(float radians);
		float CalculateSpeed(Vector2 aVelocity);
		float CalculateDistance(Vector2 aDisplacement);
		float CalculateDistance(Vector2 pointA, Vector2 pointB);
		float CalculateDistanceSquared(Vector2 aPointA, Vector2 aPointB);
		float CalculateSineAmplitude(float value, float wavelength, float amplitude, float frequency);
		float CalculateAngle(float x, float y);
		float CalculateAngle(Vector2 vector);
		float PixelsToMeters(float pixels);
		float MetersToPixels(float meters);
		float Dot(Vector2 a, Vector2 b);
		float Clamp(float value, float min, float max);
		float Random();
		float Random(float max);
		float Random(float min, float max);

		Vector2 CalculateLinearVelocity(Vector2 displacement, float duration);
		Vector2 CalculateLinearVelocity(float speed, float angleInDegrees);
		Vector2 CalculateDisplacement(Vector2 velocity, float duration);
		Vector2 CalculateDisplacement(float aDistance, float aAngleInDegrees);
		Vector2 CalculateDirection(float angleInDegrees);	
		Vector2 CalculateOrbit(Vector2 aOrigin, float aAngleInDegrees, float aDistance);
		Vector2 CalculateClosestPoint(Vector2 circle, float radius, Vector2 lineStart, Vector2 lineEnd);
		Vector2 PixelsToMeters(Vector2 pixels);
		Vector2 MetersToPixels(Vector2 meters);
		Vector2 Normalize(Vector2 vector);
	}
}