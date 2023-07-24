#define NOMINMAX

#include "Manifold.h"
#include "Body.h"
#include "Collider.h"
#include "../Math/Math.h"

#include <algorithm>

namespace GameDev2D
{
    namespace Physics
    {
        Manifold::Manifold(Body* aBodyA, Body* aBodyB) :
            m_BodyA(aBodyA),
            m_BodyB(aBodyB),
            m_Overlap(0.0f),
            m_Normal(0.0f, 0.0f)
        {

        }

        void Manifold::CorrectOverlap()
        {
            Vector2 correction = m_Normal * (std::max<float>(m_Overlap - OVERLAP_ALLOWANCE, 0.0f) / (m_BodyA->GetInverseMass() + m_BodyB->GetInverseMass())) * OVERLAP_PCT_TO_CORRECT;
            m_BodyA->SetPosition(m_BodyA->GetPosition() - correction * m_BodyA->GetInverseMass());
            m_BodyB->SetPosition(m_BodyB->GetPosition() + correction * m_BodyB->GetInverseMass());
        }

        void Manifold::FlipNormal()
        {
            m_Normal = -m_Normal;
        }

        void Manifold::SetContact(float aOverlap, Vector2 aNormal)
        {
            m_Overlap = aOverlap;
            m_Normal = aNormal;
        }
    }
}
