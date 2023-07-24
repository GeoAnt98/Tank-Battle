#pragma once

namespace ObjectPools
{
	template <typename T>
	class IPoolTemplate
	{
	public:
		virtual void ReturnToPool(T* object) = 0;
		virtual void ReturnAllToPool() = 0;

		virtual T* GetObjectW() = 0;
		virtual vector<T*> GetActiveObjects() = 0;
		virtual vector<T*> GetInactiveObjects() = 0;

	protected:
		vector<T*> m_ActiveObjects;
		vector<T*> m_InactiveObjects;
	};
}