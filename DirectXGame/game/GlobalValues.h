#pragma once

#include <variant>
#include <unordered_map>
#include <string>
#include <concepts>

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
	using Group = std::unordered_map<std::string, Item>;

public: // member functions
	void create_group(const std::string& groupName);

	template<class T>
		requires VariantAssignable<GlobalValues::Item, T>
	void set_value(const std::string& groupName, const std::string& key, const T& value);

	void export_json(const std::string& exportGroupName);

#ifdef _DEBUG
public: // for debug functions
	void debug_gui();
#endif // _DEBUG

private: // private value
	std::unordered_map<std::string, Group> datas;
};

template<class T>
	requires VariantAssignable<GlobalValues::Item, T>
inline void GlobalValues::set_value(const std::string& groupName, const std::string& key, const T& value) {
	datas[groupName][key] = value;
}
