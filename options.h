#pragma once
class options {
public:
	options(float uutility, float wweight, bool iisSelected) {
		utility = uutility, weight = wweight, isSelected = iisSelected;
	}
	float utility;
	float weight;
	bool isSelected;
	
};