#version 440 core

#ifdef GL_ES
precision mediump float;
#endif

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec4[128] lights; // First 2 are xy of the light pt, second is the light radius, 3rd is the distance until disipate
uniform int lightSize;
uniform vec4[128] walls; // First 2 are the xy of pt1, second 2 are the xy of pt2
uniform int wallSize;
uniform float darknessPercent;
uniform float sizeMultiplier;
uniform sampler2D[128] buildings; // Textures of buildings, for info on darkening them
uniform vec2[128] buildingPositions; // Position offset of buildings
uniform int numBuildings;

vec4[128] bldgWalls;
bool wallsSet = false;

float approx_distance(float dx, float dy);

void main() {

	gl_FragCoord * vec4(1.0,-1.0,1.0,1.0);

	if (!wallsSet) {

		for (int i = 0; i < numBuildings; i++){
			vec2 bldgSize = vec2(textureSize(buildings[i], 0));

				int xStartOffest = 0;
				int xEndOffset = 0;


				bool firstSolidFound = false;
				for (int texX = 0; texX < bldgSize.x; texX++) {
				
					vec4 color = texture(buildings[i], vec2(texX, bldgSize.y - 1) / bldgSize.xy);
					if (color.a > 0 && !firstSolidFound) {
						firstSolidFound = true;
						xStartOffest = texX;
					} else if (color.a == 0 && firstSolidFound) {
						xEndOffset = int(bldgSize.x - texX);
						break;
					}

				}

				vec4 ln = vec4(
					buildingPositions[i].x + xStartOffest * 3 * sizeMultiplier,
					buildingPositions[i].y + bldgSize.y * 3 * sizeMultiplier, 
					buildingPositions[i].x + (bldgSize.x - xEndOffset) * 3 * sizeMultiplier, 
					buildingPositions[i].y + bldgSize.y * 3 * sizeMultiplier
				);

				bldgWalls[i] = ln;

		}

		wallsSet = true;

	} else {
		return;
	}

	bool insideBuilding = false;
	float maxBuildingY = 0;

	for (int i = 0; i < numBuildings; i++) {

		vec2 curPos = gl_FragCoord.xy;
		vec2 bldgSize = vec2(textureSize(buildings[i], 0));
		curPos.x -= buildingPositions[i].x;
		curPos.y -= buildingPositions[i].y;

		curPos.x /= 3 * sizeMultiplier;
		curPos.y /= 3 * sizeMultiplier;

		if (!(curPos.x < 0 || curPos.x >= bldgSize.x || curPos.y < 0 || curPos.y >= bldgSize.y)) {

			vec4 color = texture(buildings[i], curPos.xy / bldgSize.xy);
			if (color.a > 0){
				insideBuilding = true;
				float newMaxBuildingY = buildingPositions[i].y + bldgSize.y * 3 * sizeMultiplier;
				if (maxBuildingY < newMaxBuildingY) maxBuildingY = newMaxBuildingY;
				
			}

		}
	}

	float maxBrightness = 0;
	
	for (int i = 0; i < lightSize; i++) {

		vec4 cur = lights[i];
		
		// Check that the point doesnt cross a line
		bool crosses = false;
		float maxCrossY = 0;

		for (int j = 0; j < numBuildings; j++) {

			vec4 lnAB = bldgWalls[j];
			vec4 lnCD = vec4(cur.x,cur.y,gl_FragCoord.x,gl_FragCoord.y);

			if ((abs(lnAB.x - gl_FragCoord.x) + abs(lnAB.y - gl_FragCoord.y)) > 10000) {
				continue;
			}

			float maxY = lnAB.y;
			if(maxY < lnAB.w) maxY = lnAB.w;

			if(maxY < lnCD.y)
				continue;

			if (lnAB.z == lnAB.x){
				lnAB.z -= 1;
			}

			if (lnCD.z == lnCD.x){
				lnCD.z -= 1;
			}

			float m1 = (lnAB.w - lnAB.y) / (lnAB.z - lnAB.x);
			float m2 = (lnCD.w - lnCD.y) / (lnCD.z - lnCD.x);

			if (m1 != m2){

				float xCross = (-m2 * lnCD.x + lnCD.y + m1*lnAB.x - lnAB.y) / (m1 - m2);
				if (
					((xCross >= lnAB.x && xCross <= lnAB.z) || (xCross >= lnAB.z && xCross <= lnAB.x)) &&
					((xCross >= lnCD.x && xCross <= lnCD.z) || (xCross >= lnCD.z && xCross <= lnCD.x))
				) {
					// Lines cross
					crosses = true;
					maxCrossY = maxY;
					break;
				}

			}



		}

		float brightnessMultiplier = 1;
		if (insideBuilding) {
			if (cur.y < maxBuildingY) {
				brightnessMultiplier = 1 - abs(maxBuildingY - cur.y) / (60 * sizeMultiplier);
			}
		} else if (crosses) {
			brightnessMultiplier = 1 - abs(maxCrossY - cur.y) / (60 * sizeMultiplier);
		}

		if (brightnessMultiplier < 0) brightnessMultiplier = 0;

		float distanceDelta = distance(gl_FragCoord.xy, cur.xy) - cur.w * sizeMultiplier;

		if (distanceDelta < 0)
			distanceDelta = 0;

		float brightnessDelta = ((cur.z)*sizeMultiplier - distanceDelta)/((cur.z) * sizeMultiplier);
		if (brightnessDelta > 0) {
			maxBrightness += brightnessDelta * brightnessMultiplier;
		}
		
		
	}


	gl_FragColor = vec4(0.0, 0.0, 25.0 / 255.0, (225.0 / 255.0) * darknessPercent - (225.0 / 255.0) * darknessPercent * maxBrightness);
	
}