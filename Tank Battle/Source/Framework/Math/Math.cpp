#include "Math.h"
#include "../Physics/World.h"

#include <stdlib.h>
#include <complex>
#include <random>

namespace GameDev2D
{
    namespace Math
    {
        float DegreesToRadians(float aDegrees)
        {
            return aDegrees * 0.01745329251f;
        }

        float RadiansToDegrees(float aRadians)
        {
            return aRadians * 57.2957795131f;
        }

		Vector2 CalculateLinearVelocity(Vector2 aDisplacement, float aDuration)
		{
			return Vector2(aDisplacement.x / aDuration, aDisplacement.y / aDuration);
		}

		Vector2 CalculateLinearVelocity(float aSpeed, float aAngleInDegrees)
		{
			Vector2 direction = CalculateDirection(aAngleInDegrees);
			Vector2 linearVelocity = direction * aSpeed;
			return linearVelocity;
		}

		float CalculateSpeed(Vector2 aVelocity)
		{
			float speed = sqrtf(aVelocity.x * aVelocity.x + aVelocity.y * aVelocity.y);
			return speed;
		}

		Vector2 CalculateDisplacement(float aDistance, float aAngleInDegrees)
		{
			Vector2 direction = CalculateDirection(aAngleInDegrees);
			Vector2 displacement = Vector2(direction.x * aDistance, direction.y * aDistance);
			return displacement;
		}

		Vector2 CalculateDisplacement(Vector2 aVelocity, float aDuration)
		{
			return Vector2(aVelocity.x * aDuration, aVelocity.y * aDuration);
		}

		float CalculateDistance(Vector2 aPointA, Vector2 aPointB)
		{
			float xSquared = (aPointB.x - aPointA.x) * (aPointB.x - aPointA.x);
			float ySquared = (aPointB.y - aPointA.y) * (aPointB.y - aPointA.y);
			float distance = std::sqrtf(xSquared + ySquared);
			return distance;
		}

		float CalculateDistance(Vector2 aDisplacement)
		{
			float distance = sqrtf(aDisplacement.x * aDisplacement.x + aDisplacement.y * aDisplacement.y);
			return distance;
		}

		float CalculateDistanceSquared(Vector2 aPointA, Vector2 aPointB)
		{
			float xSquared = (aPointB.x - aPointA.x) * (aPointB.x - aPointA.x);
			float ySquared = (aPointB.y - aPointA.y) * (aPointB.y - aPointA.y);
			return xSquared + ySquared;
		}

		Vector2 CalculateDirection(float aAngleInDegrees)
		{
			float angleInRadians = DegreesToRadians(aAngleInDegrees);
			return Vector2(cosf(angleInRadians), sinf(angleInRadians));
		}

		float CalculateSineAmplitude(float aValue, float aWaveLength, float aAmplitude, float aFrequency)
		{
			float wavelength = aWaveLength / (M_PI * 2.0f);
			float frequency = aFrequency * (M_PI * 2.0f);
			return sinf(frequency + (aValue / wavelength)) * aAmplitude;
		}

		float CalculateAngle(float aX, float aY)
		{
			float angleInRadians = atan2f(aY, aX);
			return RadiansToDegrees(angleInRadians);
		}

		float CalculateAngle(Vector2 aVector)
		{
			return CalculateAngle(aVector.x, aVector.y);
		}

		Vector2 CalculateOrbit(Vector2 aOrigin, float aAngleInDegrees, float aDistance)
		{
			Vector2 offset = CalculateDisplacement(aDistance, aAngleInDegrees);
			return aOrigin + offset;
		}

		float PixelsToMeters(float aPixels)
		{
			return aPixels / PTM_RATIO;
		}

		Vector2 PixelsToMeters(Vector2 aPixels)
		{
			return aPixels / PTM_RATIO;
		}

		float MetersToPixels(float aMeters)
		{
			return aMeters * PTM_RATIO;
		}

		Vector2 MetersToPixels(Vector2 aMeters)
		{
			return aMeters * PTM_RATIO;
		}

        Vector2 Normalize(Vector2 vector)
        {
            float magnitude = CalculateDistance(Vector2(0.0f, 0.0f), vector);

            if (magnitude > EPSILON)
                vector /= magnitude;

            return vector;
        }

        float Dot(Vector2 a, Vector2 b)
        {
            return a.x * b.x + a.y * b.y;
        }

        Vector2 CalculateClosestPoint(Vector2 circle, float radius, Vector2 lineStart, Vector2 lineEnd)
        {
			Vector2 circleV = (circle - lineStart);     //Declares the circle using the circle and lineStart parameters
			Vector2 lineV = lineEnd - lineStart;        //determines the location of the line
			Vector2 normalV = Normalize(lineV);         //normalizes the line
			float magnituteProjectionV = Dot(circleV, normalV);     //determines the magnitude of the projection using the dot product of circleV and normalV
			Vector2 projectionV = normalV * magnituteProjectionV;   //makes the projection from the product of the normalV and magnitudeProjectionV
			//Vector2 D = lineStart + projectionV;
			//float Distance = ((circle.x - D.x) - (circle.y - D.y));
            Vector2 ClosestPoint = projectionV + lineStart;

            return ClosestPoint;     //returns projectionV
        }

        float Clamp(float value, float min, float max)
        {
            if (value < min)
                return min;
			else if (value > max)
                return max;

            return value;
        }

        float Random()
        {
			std::random_device rd;
			std::uniform_real_distribution<float> num(0.0f, 1.0f);
			return num(rd);
        }

        float Random(float max)
        {
			std::random_device rd;
			std::uniform_real_distribution<float> num(0.0f, max);
			return num(rd);
        }

        float Random(float min, float max)
        {
			std::random_device rd;
			std::uniform_real_distribution<float> num(min, max);
			return num(rd);
        }
    }
}