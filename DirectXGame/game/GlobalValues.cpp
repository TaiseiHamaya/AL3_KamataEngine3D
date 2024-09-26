#include "GlobalValues.h"

#include <ranges>

GlobalValues& GlobalValues::GetInstance() {
	static GlobalValues instance;
	return instance;
}

void GlobalValues::create_group(const std::string& groupName) {
	datas[groupName];
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
				size_t itemIndex = item.second.index();
				if (itemIndex == 0) {
					int32_t& value = std::get<int32_t>(item.second);
					ImGui::DragInt(item.first.c_str(), &value);
				}
				else if (itemIndex == 1) {
					float& value = std::get<float>(item.second);
					ImGui::DragFloat(item.first.c_str(), &value);
				}
				else if (itemIndex == 2) {
					Vector3& value = std::get<Vector3>(item.second);
					ImGui::DragFloat3(item.first.c_str(), &value.x);
				}
			}
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}
#endif // _DEBUG
