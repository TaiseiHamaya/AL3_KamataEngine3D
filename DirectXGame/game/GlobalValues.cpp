#include "GlobalValues.h"

#define NOMINMAX

#include <filesystem>
#include <fstream>

#include <json.hpp>
#include <Windows.h>

#include <Utility.h>

using json = nlohmann::json;

GlobalValues& GlobalValues::GetInstance() {
	static GlobalValues instance;
	return instance;
}

void GlobalValues::create_group(const std::string& groupName) {
	datas[groupName];
}

void GlobalValues::export_json(const std::string& exportGroupName) {
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

	std::filesystem::path outputDirectory{ DIRECTORY };
	if (!std::filesystem::exists(outputDirectory)) {
		std::filesystem::create_directory(outputDirectory);
	}

	std::filesystem::path outputFilePath{ DIRECTORY + exportGroupName + ".json" };
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

void GlobalValues::inport_json_all() {
	if (!std::filesystem::exists(DIRECTORY)) {
		return;
	}
	std::filesystem::directory_iterator directoryIterator{ DIRECTORY };
	for (const std::filesystem::directory_entry& entry : directoryIterator) {
		if (entry.path().extension().string() != ".json") {
			continue;
		}
		inport_json(entry.path());
	}
}

void GlobalValues::inport_json(const std::filesystem::path& file) {
	std::ifstream ifstream{ file };
	if (ifstream.fail()) {
		return;
	}
	json root;
	ifstream >> root;
	ifstream.close();

	std::string fileName = file.stem().string();
	root.contains(fileName);

	json::iterator itrGroup = root.find(fileName);

	for (json::iterator itrItem = itrGroup->begin(); itrItem != itrGroup->end(); ++itrItem) {
		const std::string& key = itrItem.key();
		if (itrItem->is_number_integer()) {
			int32_t value = itrItem->get<int32_t>();
			this->set_value(fileName, key, value);
		}
		else if (itrItem->is_number_float()) {
			float value = itrItem->get<float>();
			this->set_value(fileName, key, value);
		}
		else if (itrItem->is_array() && itrItem->size() == 3) {
			Vector3 value{ itrItem->at(0),itrItem->at(1),itrItem->at(2) };
			this->set_value(fileName, key, value);
		}
	}
}

#ifdef _DEBUG
#include <imgui.h>
void GlobalValues::debug_gui() {
	ImGui::Begin("GlobalValues");
	static std::string selectGroupp;
	auto flag = ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;
	ImGui::BeginTabBar("GlobalValuesBar", flag);
	for (std::pair<const std::string, Group>& group : datas) {
		if (ImGui::BeginTabItem(group.first.c_str())) {
			for (std::pair<const std::string, Item>& item : group.second) {
				auto& itemName = item.first;
				auto& itemValue = item.second;
				size_t itemIndex = itemValue.index();
				ImGui::SetNextItemWidth(150);
				if (itemIndex == 0) {
					int32_t& value = std::get<int32_t>(itemValue);
					ImGui::DragInt(itemName.c_str(), &value);
				}
				else if (itemIndex == 1) {
					float& value = std::get<float>(itemValue);
					ImGui::DragFloat(itemName.c_str(), &value, 0.01f);
				}
				else if (itemIndex == 2) {
					Vector3& value = std::get<Vector3>(itemValue);
					ImGui::DragFloat3(itemName.c_str(), &value.x, 0.01f);
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
