#include "GlobalValues.h"

#define NOMINMAX

#include <filesystem>
#include <fstream>

#include <json.hpp>
#include <Windows.h>

#include <Utility.h>


GlobalValues& GlobalValues::GetInstance() {
	static GlobalValues instance;
	return instance;
}

void GlobalValues::create_group(const std::string& groupName) {
	datas[groupName];
}

void GlobalValues::export_json(const std::string& exportGroupName) {
	using json = nlohmann::json;
	if (!datas.contains(exportGroupName)) {
		std::string message = std::format("[GlobalValues] Can't find group name. \'{}\'", exportGroupName);
		Log(message);
		MessageBoxA(nullptr, message.c_str(), "GlobalValues", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	json root;
	root = json::object();
	root[exportGroupName] = json::object();

	for (std::pair<const std::string, Item> item : datas[exportGroupName]) {
		auto& itemName = item.first;
		auto& itemValue = item.second;
		size_t itemIndex = itemValue.index();
		if (itemIndex == 0) {
			int32_t& value = std::get<int32_t>(itemValue);
			root[exportGroupName][itemName] = value;
		}
		else if (itemIndex == 1) {
			float& value = std::get<float>(itemValue);
			root[exportGroupName][itemName] = value;
		}
		else if (itemIndex == 2) {
			Vector3& value = std::get<Vector3>(itemValue);
			root[exportGroupName][itemName] = json::array({ value.x, value.y, value.z });
		}
	}

	const std::string Directory{ "Resources/GlobalValues/" };

	std::filesystem::path outputDirectory{ Directory };
	if (!std::filesystem::exists(outputDirectory)) {
		std::filesystem::create_directory(outputDirectory);
	}

	std::filesystem::path outputFilePath{ Directory + exportGroupName + ".json" };
	std::ofstream ofstream{ outputFilePath };

	if(ofstream.fail()){
		std::string message = std::format("[GlobalValues] Failed open export file. \'{}\'", exportGroupName);
		Log(message);
		MessageBoxA(nullptr, message.c_str(), "GlobalValues", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	ofstream << std::setw(4) << root << std::endl;
	ofstream.close();

	std::string message = std::format("Export is successed.\nGroup - \'{}\'", exportGroupName);
	MessageBoxA(nullptr, message.c_str(), "GlobalValues", MB_OK);
}

#ifdef _DEBUG
#include <imgui.h>
void GlobalValues::debug_gui() {
	ImGui::Begin("GlobalValues");
	static std::string selectGroupp;
	ImGui::BeginTabBar("GlobalValuesBar");
	for (std::pair<const std::string, Group>& group : datas) {
		if (ImGui::BeginTabItem(group.first.c_str())) {
			for (std::pair<const std::string, Item>& item : group.second) {
				auto& itemName = item.first;
				auto& itemValue = item.second;
				size_t itemIndex = itemValue.index();
				if (itemIndex == 0) {
					int32_t& value = std::get<int32_t>(itemValue);
					ImGui::DragInt(itemName.c_str(), &value);
				}
				else if (itemIndex == 1) {
					float& value = std::get<float>(itemValue);
					ImGui::DragFloat(itemName.c_str(), &value);
				}
				else if (itemIndex == 2) {
					Vector3& value = std::get<Vector3>(itemValue);
					ImGui::DragFloat3(itemName.c_str(), &value.x);
				}
			}
			if (ImGui::Button("Export")) {
				export_json(group.first);
			}
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}
#endif // _DEBUG
