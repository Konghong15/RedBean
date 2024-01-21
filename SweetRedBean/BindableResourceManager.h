#pragma once

#include "IBindable.h"

class Graphics;

namespace Bind
{
	class ResourceManager
	{
	public:
		static ResourceManager& Get();

		template <class T, typename... Params>
		static shared_ptr<T> Create(Graphics& graphics, Params&&... params);

	private:
		ResourceManager() = default;;
		~ResourceManager() = default;

	private:
		template <class T, typename... Params>
		shared_ptr<T> create(Graphics& graphics, Params&&... params);

	private:
		unordered_map<string, shared_ptr<IBindable>> mBindables;
	};

	template <class T, typename... Params>
	std::shared_ptr<T> ResourceManager::Create(Graphics& graphics, Params&&... params)
	{
		static_assert(std::is_base_of<IBindable, T>::value, "T must be IBindable");
		return Get().create<T>(graphics, std::forward<Params>(params)...);
	}

	template <class T, typename... Params>
	shared_ptr<T> ResourceManager::create(Graphics& graphics, Params&&... params)
	{
		const auto key = T::GenerateUID(forward<Params>(params)...);
		const auto find = mBindables.find(key);

		if (find == mBindables.end())
		{
			auto bindable = make_shared<T>(graphics, forward<Params>(params)...);
			mBindables.insert({ key, bindable });

			return bindable;
		}
		else
		{
			return static_pointer_cast<T>(find->second);
		}
	}

}