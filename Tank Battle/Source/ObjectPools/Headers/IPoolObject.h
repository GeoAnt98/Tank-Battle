#pragma once

namespace ObjectPools
{
	class IPoolObject
	{
	public:
		virtual bool IsEnabled() = 0;

		virtual void Update(double deltatime) = 0;
		virtual void Draw() = 0;

		virtual void SetEnabled(bool) = 0;
	protected:
		bool m_IsEnabled;
	};
}