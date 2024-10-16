#pragma once

#include <concepts>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <map>
#include <variant>

#include <Vector3.h>

// Variantが代入可能であるconcept
template<typename Variant, typename T>
concept VariantAssignable = requires(Variant v, T t) {
	{ v = t } -> std::same_as<Variant&>;
};

class GlobalValues {
private: // constractor/destractor
	GlobalValues() = default;
	~GlobalValues() = default;
	GlobalValues(const GlobalValues&) = delete;
	GlobalValues& operator=(const GlobalValues&) = delete;

public: // singleton instance
	static GlobalValues& GetInstance();

public: // using
	using Item = std::variant<int32_t, float, Vector3>;
#ifdef _DEBUG
	using Group = std::map<std::string, Item>;
#else
	using Group = std::unordered_map<std::string, Item>;
#endif // _DEBUG

public: // member functions
	void create_group(const std::string& groupName);

	template<class T>
		requires VariantAssignable<GlobalValues::Item, T>
	T get_value(const std::string& groupName, const std::string& key);

	template<class T>
		requires VariantAssignable<GlobalValues::Item, T>
	void set_value(const std::string& groupName, const std::string& key, const T& value);

	template<class T>
		requires VariantAssignable<GlobalValues::Item, T>
	void add_value(const std::string& groupName, const std::string& key, const T& value = {});

	void export_json(const std::string& exportGroupName);
	void inport_json_all();

private:
	void inport_json(const std::filesystem::path& groupName);

#ifdef _DEBUG
public: // for debug functions
	void debug_gui();
#endif // _DEBUG

private: // private value
	std::unordered_map<std::string, Group> datas;

	inline static const std::string DIRECTORY{ "./Resources/GlobalValues/" };
};

template<class T>
	requires VariantAssignable<GlobalValues::Item, T>
inline T GlobalValues::get_value(const std::string& groupName, const std::string& key) {
	if (!datas.contains(groupName)) {
		return {};
	}
	if (!datas[groupName].contains(key)) {
		return {};
	}
	return std::get<T>(datas[groupName][key]);
}

template<class T>
	requires VariantAssignable<GlobalValues::Item, T>
inline void GlobalValues::set_value(const std::string& groupName, const std::string& key, const T& value) {
	datas[groupName][key] = value;
}

template<class T>
	requires VariantAssignable<GlobalValues::Item, T>
inline void GlobalValues::add_value(const std::string& groupName, const std::string& key, const T& value) {
	datas[groupName].emplace(key, value);
}
