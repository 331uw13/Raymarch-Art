




vec3 color_id(vec3 p, int id) {
	vec3 color = vec3(0.0, 0.0, 0.0);

	vec3 light = ADD_LIGHT(p, vec3(3.0, 5.0, -5.0), vec3(1.0, 1.0, 1.0), 0.5);

	switch(id) {

		case 0:
			color = vec3(1.0);
			break;

		case 1:
			color = vec3(0.7, 1.0, 0.9);
			break;
		
		case 2:
			color = vec3(0.3, 0.3, 1.0);
			break;

	
		default:break;
	}

	return color*light;
}



vec2 map(vec3 p) {
	float r = 1e32;


	vec2 a = vec2(SPHERE_SDF(p, 1.0), 0.0);

	//vec2 a = vec2(BOX_SDF(p+vec3(0.0, 1.0, 0.0), vec3(10.0, 0.2, 10.0), 0.0), 0.0);


	return a;
}



void shader_main() {

	CAMERA_POS = vec3(0.0, 0.0, -10.0);

	vec3 dir = RAYDIR(60.0);
	vec4 r = RAYMARCH(CAMERA_POS, dir, 0.001, 100.0);

	if(r.w >= 0.0) {

		vec3 c0 = color_id(r.xyz, int(round(r.w)));

		for(int i = 0; i < 1; i++) {
		
			r = RAYMARCH(r.xyz, NORMAL(r.xyz), 0.001, 10.0);
			if(r.w >= 0.0) {
				
				c0 *= 0.65;
				c0 += color_id(r.xyz, int(round(r.w)));
			}
		}



		RAY_COLOR = c0;

	}
}

