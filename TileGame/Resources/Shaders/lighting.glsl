#version 440 core

#ifdef GL_ES
precision mediump float;
#endif

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec4[1024] lights; // First 2 are xy of the light pt, second is the light radius, 3rd is the distance until disipate
uniform int lightSize;
uniform vec4[2048] walls; // First 2 are the xy of pt1, second 2 are the xy of pt2
uniform int wallSize;
uniform float darknessPercent;
uniform float sizeMultiplier;
uniform vec2[2048] buildingVerticies; // Points that define the outline of buildings. If the index 
									  // is inside of ending indexes, then that is the final vertex in the building
uniform int numBuildingVerticies;
uniform float[2048] endingIndexes; // Defines the ending vertex indexes for each building
uniform float[2048] buildingMaxY;

void main() {

	gl_FragCoord * vec4(1.0,-1.0,1.0,1.0);

	bool insideBuilding = false;
	int buildingId = -1;

	if (numBuildingVerticies > 0){
		// Has not crossed a wall line, so check for crossing a building line
		int buildingNumber = 0;
		bool oddNodes = false;
		int j = int(endingIndexes[buildingNumber]);
		for (int i = 0; i < numBuildingVerticies; i++){
			if (
				(buildingVerticies[i].y < gl_FragCoord.y && buildingVerticies[j].y >= gl_FragCoord.y ||
				buildingVerticies[j].y < gl_FragCoord.y && buildingVerticies[i].y >= gl_FragCoord.y) &&
				(buildingVerticies[i].x <= gl_FragCoord.x ||  buildingVerticies[j].x <= gl_FragCoord.x)
			) {
				if (
					buildingVerticies[i].x + 
					(gl_FragCoord.y - buildingVerticies[i].y) / ( -buildingVerticies[i].y + buildingVerticies[j].y) * 
					(buildingVerticies[j].x - buildingVerticies[i].x) < gl_FragCoord.x
				) {
					oddNodes = !oddNodes;
				}

			}

			j = i;

			if (i == int(endingIndexes[buildingNumber])) {
				// Finished current building

				if (oddNodes){
					insideBuilding = true;
					buildingId = buildingNumber;
					break;
				}

				buildingNumber++;
				j = int(endingIndexes[buildingNumber]);
				oddNodes = false;

			}

		}


	}

	float maxBrightness = 0;
	
	for (int i = 0; i < lightSize; i++) {

		vec4 cur = lights[i];
		
		// Check that the point doesnt cross a line
		bool crosses = false;
		for(int j = 0; j < wallSize; j++){

			vec4 lnAB = walls[j];
			vec4 lnCD = vec4(cur.x,cur.y,gl_FragCoord.x,gl_FragCoord.y);

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
					break;
				}

			}



		}

		if (!crosses) {
			if (insideBuilding) {
				if (buildingMaxY[buildingId] > cur.y) {
					continue;
				}
			}

			float distanceDelta = distance(gl_FragCoord.xy, cur.xy)  - cur.w * sizeMultiplier;
			if (distanceDelta < 0) 
				distanceDelta = 0;

			float brightnessDelta = ((cur.z)*sizeMultiplier - distanceDelta)/((cur.z) * sizeMultiplier);
			if (brightnessDelta > 0) {
				maxBrightness += brightnessDelta;
			}
		}
		
	}


	gl_FragColor = vec4(0.0, 0.0, 25.0 / 255.0, (225.0 / 255.0) * darknessPercent - (225.0 / 255.0) * darknessPercent * maxBrightness);
	
}